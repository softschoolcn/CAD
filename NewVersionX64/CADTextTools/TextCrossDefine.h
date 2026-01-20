#pragma once
//文字重叠检测结果
struct TextCrossRecord
{
	AcDbObjectId textId1;
	AcDbObjectId textId2;
	AcGePoint3dArray pts1;
	AcGePoint3dArray pts2;
	double dScale;
	CString strText1;
	CString strText2;
};