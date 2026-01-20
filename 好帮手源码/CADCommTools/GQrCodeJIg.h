#pragma once
#include "GQrCodeMgr.h"
#include "GMultJigEnt.h"
class GQrCodeEnt:public GMultJigEnt
{
public:
	AcGePoint3d ptBase;//基点
	AcGePoint3d pt;//另外一个点
	int maxY;
	int maxX;
	bool* s;
	//bPriview=true,是预览
	void Update(AcGePoint3d newPt,bool bPriview=true);
	bool IsFill(bool* s,int X,int Y);
	bool IsAllFill(bool* s,int X,int YStart,int YEnd);

};
class GQrCodeJIg: public AcEdJig
{
public:
	GQrCodeJIg(void);
	~GQrCodeJIg(void);
public:
	virtual AcDbEntity* entity() const{return pEnt;}
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual int Do();
	AcGePoint3d m_point;             //插入点
	AcGePoint3d m_pointTopLast;//插入点对应的上面的点(上一次用户输入)
	AcGePoint3d ptBase;//基点
	int maxY;
	int maxX;
	bool* s;
	GQrCodeEnt* pEnt;
	void InitEnt();
	AcDbObjectIdArray m_ids;
};
