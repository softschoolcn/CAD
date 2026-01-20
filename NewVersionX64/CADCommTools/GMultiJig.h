#pragma once
#include "GMultiJigEntity.h"
class GMultiJig:public AcEdJig
{
public:
	GMultiJig(void);
public:
	~GMultiJig(void);
	virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbEntity* entity() const;

    int Do();
	void Add(AcDbEntity* pEnt)
	{
		mEntity->Add(pEnt);
	}
	void GetIds(AcDbObjectIdArray& ids);
	private:
    GMultiJigEntity* mEntity;  //jig辅助实体
    AcGePoint3d mPoint;  //鼠标位置点
	AcDbObjectIdArray m_ids;
};
