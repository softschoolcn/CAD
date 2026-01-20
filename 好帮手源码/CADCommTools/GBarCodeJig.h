#pragma once
#include "GMultJigEnt.h"
class GBarCodeEnt:public GMultJigEnt
{
public:
	AcGePoint3d ptBase;
	AcGePoint3d pt;
	CString strBarCode;
	bool bText;
	void Update(AcGePoint3d newPt);
};
class GBarCodeJig: public AcEdJig
{
public:
	GBarCodeJig(void);
public:
	~GBarCodeJig(void);
	virtual AcDbEntity* entity() const{return pEnt;}
	AcGePoint3d ptBase;
	CString strBarCode;
	bool bText;
	AcDbObjectIdArray m_ids;
	void InitEntity();
	virtual AcEdJig::DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual int Do();
	AcGePoint3d m_point;             //插入点
	AcGePoint3d m_pointTopLast;//插入点对应的上面的点(上一次用户输入)
private:
	GBarCodeEnt* pEnt;
};
