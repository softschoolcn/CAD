#pragma once

class GLOBALTOOLS_EXT GZoom
{
public:
	GZoom(void);
public:
	~GZoom(void);
	//缩放到数据库对应的范围，相当于执行ZOOM/E命令
	static Acad::ErrorStatus ZoomDatabaseExtent(AcDbDatabase* pDb);
	//功能:	设置视图（相当于Zoom Window命令）
	//参数:	Pt1		视图左上角点
	//			Pt2		视图右下角点
	//			ex_ratio	扩展比率,一般为1.0
	static Acad::ErrorStatus view_set(AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio);
	static bool DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio);
	static bool DyZoom(const AcDbObjectId ObjId,double ex_ratio);
private:
	static bool GetMaxViewPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax);
	//枚举块表中的所有实体
	static Acad::ErrorStatus EnumAllEntitys(AcDbObjectIdArray& ObjIdArray,AcDbBlockTableRecord* pRec);
	static Acad::ErrorStatus EnumAllEntitysInModelSpace(AcDbObjectIdArray& ObjIdArray,AcDbDatabase* pDb);
	static Acad::ErrorStatus view_set(AcDbViewportTableRecord* pViewPortRec,AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio);
	static Acad::ErrorStatus DyZoom(AcDbViewportTableRecord* pViewPortRec,AcGePoint3d ptMin, AcGePoint3d ptMax,double ex_ratio=1.0);
};
