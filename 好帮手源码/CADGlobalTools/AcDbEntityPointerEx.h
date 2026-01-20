#pragma once
#include "dbobjptr.h"
template<class T_OBJECT>
class AcDbEntityPointerEx:public AcDbObjectPointer<T_OBJECT>
{
public:
	AcDbEntityPointerEx(AcDbObjectId   objId,
		AcDb::OpenMode mode,
		bool           openErased = false,
		bool           openOnLockLayer=false);
public:
	~AcDbEntityPointerEx(void);
protected:
	bool m_bLock;//图层是否锁定
	bool m_bFrozen;//图层是否冻结
	AcDbObjectId m_layerId;//图层ID
};
template<class T_OBJECT>
AcDbEntityPointerEx<T_OBJECT>::AcDbEntityPointerEx(AcDbObjectId   objId,
												   AcDb::OpenMode mode,
												   bool           openErased = false,
												   bool           openOnLockLayer=false):AcDbObjectPointer<T_OBJECT>(objId,mode,openErased)
{
	if(m_status==Acad::eOnLockedLayer&&mode==AcDb::kForWrite)
	{
		m_status=acdbOpenObject(m_ptr, objId, AcDb::kForRead, openErased);
		if(m_status!=Acad::eOk)
		{
			return;
		}
		m_layerId=m_ptr->layerId();
		AcDbLayerTableRecordPointer spLayer(m_layerId,AcDb::kForWrite);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			//如果解锁图层失败
			//获取到了实体也无法操作
			m_ptr->close();
			m_ptr=NULL;
			m_status=spLayer.openStatus();
			return;
		}
		m_bLock=spLayer->isLocked();
		m_bFrozen=spLayer->isFrozen();
		spLayer->setIsLocked(false);
		spLayer->setIsFrozen(false);
		m_status=m_ptr->upgradeOpen();
		if(m_status!=Acad::eOk)
		{
			//如果解锁图层失败
			//获取到了实体也无法操作
			m_ptr->close();
			m_ptr=NULL;
			return;
		}
	}
	else
	{
		m_bLock=false;
		m_bFrozen=false;
		m_layerId.setNull();
	}
}
template<class T_OBJECT>
AcDbEntityPointerEx<T_OBJECT>::~AcDbEntityPointerEx()
{
	if(!m_layerId.isValid())
		return;
	AcDbLayerTableRecordPointer spLayer(m_layerId,AcDb::kForWrite);
	if(spLayer.openStatus()!=Acad::eOk)
	{
		return;
	}
	spLayer->setIsLocked(m_bLock);
	spLayer->setIsFrozen(m_bFrozen);

}
