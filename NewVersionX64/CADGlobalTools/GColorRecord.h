#pragma once

class GLOBALTOOLS_EXT GColorRecord
{
public:
	GColorRecord(void);
	GColorRecord(const GColorRecord& src);
	GColorRecord& operator=(const GColorRecord& src);
public:
	~GColorRecord(void);
	AcDbObjectId objId;
	AcCmColor Color;
	//标注
	AcCmColor Dimclrd;
	AcCmColor Dimclre;
	AcCmColor Dimclrt;
	//填充
	Adesk::Boolean GradientOneColorMode;
	AcDbHatch::HatchObjectType hatchObjectType; 
	AcArray<AcCmColor> GradientColors;
	AcArray<float> GradientValues;
	//文字
	CString strText;
	//读取
	Acad::ErrorStatus Read(AcDbEntity* pEnt);
	Acad::ErrorStatus Write(AcDbEntity* pEnt);
	//回滚
	Acad::ErrorStatus Rollback();
	//回滚
	static Acad::ErrorStatus Rollback(CArray<GColorRecord>& colorRecordArr);
	//功能：锁定\解锁图层
	//参数：objId：图层id
	//		bFlag：锁定方式。0:强制打开；1:强制锁定；其他:根据当前状态取反。
	static Acad::ErrorStatus LockLayer(AcDbObjectId objId,int bFlag);
};
