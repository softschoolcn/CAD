#pragma once
#include "GColorRecord.h"
//设置统一颜色的辅助类
class GLOBALTOOLS_EXT GSameColorHelper
{
public:
	GSameColorHelper(void);
public:
	~GSameColorHelper(void);
	//设置当前整个数据库中呈现统一颜色
	Acad::ErrorStatus SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,const int nColorIndex);
	//设置整个数据库中呈现统一颜色
	Acad::ErrorStatus SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbDatabase* pDb,const int nColorIndex);
	//设置整个块定义中呈现统一颜色
	Acad::ErrorStatus SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbBlockTableRecord* pBTR,const int nColorIndex);
	//设置单个实体中呈现统一颜色
	Acad::ErrorStatus SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectId objId,const int nColorIndex);
	//设置实体中呈现统一颜色
	Acad::ErrorStatus SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectIdArray& ids,const int nColorIndex);
private:
	//设置实体颜色
	Acad::ErrorStatus SetEntityColor(AcDbEntity* pEnt,const int nColorIndex);
	//设置实体颜色
	Acad::ErrorStatus SetEntityColor(const AcDbObjectId id,const int nColorIndex);
	//打开所有的图层并返回原有的状态
	Acad::ErrorStatus OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
	//重新设置图层回原有的状态
	Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
	//设置整个块定义中呈现统一颜色
	Acad::ErrorStatus _SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbBlockTableRecord* pBTR,const int nColorIndex);
	Acad::ErrorStatus _SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectIdArray& blockIds,AcDbObjectId objId,const int nColorIndex);
	
};
