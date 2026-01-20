#include "StdAfx.h"
#include "GMultJigEnt.h"

GMultJigEnt::GMultJigEnt(void)
{
}

GMultJigEnt::~GMultJigEnt(void)
{
	Release();
}
/*!
@brief 添加到数据库
*/
void GMultJigEnt::AddToDb(AcDbObjectIdArray& ids)
{
	//获取活动空间
	AcDbLayoutManager* pLayoutMng = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId spaceId = pLayoutMng->getActiveLayoutBTRId();
	AcDbBlockTableRecord* pSpace = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)pSpace, spaceId, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		//GLOG_ERROR(_T("获取活动空间失败:") << acadErrorStatusText(es));
		return;
	}

	//添加到活动空间
	for(int i=0;i<mEntArr.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)mEntArr.at(i);
		if(pEnt!=NULL)
		{
			AcDbObjectId id;
			pSpace->appendAcDbEntity(id,pEnt);
			ids.append(id);
			pEnt->close();
		}
	}
	mEntArr.setLogicalLength(0);

	pSpace->close();
}
void GMultJigEnt::Release()
{
	for(int i=0;i<mEntArr.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)mEntArr.at(i);
		if(pEnt!=NULL)
		{
			delete pEnt;
			pEnt=NULL;
		}
	}
	mEntArr.setLogicalLength(0);
}
#if _MSC_VER<1500
Adesk::Boolean GMultJigEnt::worldDraw(AcGiWorldDraw* mode)
#else
Adesk::Boolean GMultJigEnt::subWorldDraw(AcGiWorldDraw* mode)
#endif
{
	for(int i=0;i<mEntArr.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)mEntArr.at(i);
		if(pEnt->worldDraw(mode)!=Adesk::kTrue)
		{
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
#if _MSC_VER<1500
	void      GMultJigEnt::viewportDraw(AcGiViewportDraw* mode)
#else
	
	void      GMultJigEnt::subViewportDraw(AcGiViewportDraw* mode)
#endif
	{
		for(int i=0;i<mEntArr.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)mEntArr.at(i);
		pEnt->viewportDraw(mode);
	}
	}
