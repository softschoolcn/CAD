#pragma once
#include "GPlotDefine.h"
//图框边框管理类
class GFrameBorderHelper
{
public:
	GFrameBorderHelper(void);
public:
	~GFrameBorderHelper(void);
	GFrameDefine m_frame;
	CString m_strFileName;
	Acad::ErrorStatus GetPlotItems(CArray<GPlotItem>& plotItems,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
	//打印对象排序
	void SortOrder(AcArray<int>& indexArray,AcGePoint3dArray& ptMinArray,AcGePoint3dArray& ptMaxArray);
	static bool GetRects(AcGePoint3d& ptMin,AcGePoint3d& ptMax,AcDbPolyline* pLine,double dMin=1E-4);
private:
	Acad::ErrorStatus GetPlotItems(CArray<GPlotItem>& plotItems,AcDbBlockTableRecord* pBTR);
	//获取矩形
	void GetRects(CStringArray& strNameArray,AcGePoint3dArray& ptMinArray,AcGePoint3dArray& ptMaxArray,AcDbObjectIdArray& ids);
	static void ComparePoint(bool& bXSame,bool& bYSame,AcGePoint2d pt1,AcGePoint2d pt2);
	//能构成矩形的分组在一起
	void TwoLines(AcArray<AcGePoint3dArray*>& ptArrayGroup1,AcArray<AcGePoint3dArray*>& ptArrayGroup2,AcGePoint3dArray& ptTmpArray1,AcGePoint3dArray& ptTmpArray2,bool bHor);
	//释放
	void Release(AcArray<AcGePoint3dArray*>& ptArrayGroup);
	//是否包含
	bool IsContain(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1);

	//是否是内边框
	bool IsInnerBorder(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1);

	//是否包含
	bool IsSame(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1);
	//点是否在范围内
	bool IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext);
	bool NeedSwap(AcGePoint3d ptMin1,AcGePoint3d ptMax1,AcGePoint3d ptMin2,AcGePoint3d ptMax2);
};
