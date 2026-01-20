#pragma once

//全局函数
//文字递增
//bIncrease:表示是否递增
//bIncrease=true,0->9->10
//bIncrease=false,0->9->0
BOOL NumPlus(CString& strText,int nNum,bool bIncrease);
void NumCanPlus(CString& strText,CArray<int>& startIndexArray,CArray<int>& endIndexArray);
bool IsXor(long nState,const long nPropState);
//点是否在面域中(暂时只建议计算四个点构成的面域)
bool IsPointInRegion(AcGePoint3d pt,AcGePoint3dArray& pts);
bool RgnIntersect(double& dScale,AcGePoint3dArray& pts1,AcGePoint3dArray& pts2);
//拷贝文字到剪贴板中
BOOL CopyTextToClipboard(CString strText,HWND hWnd=NULL);
