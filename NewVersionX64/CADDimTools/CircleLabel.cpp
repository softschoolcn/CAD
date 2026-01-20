#include "stdafx.h"
#include "CircleLabel.h"
//////////////////////////////////////////////////////////////////////////
CCircleLabel::CCircleLabel(void)
{
}

CCircleLabel::~CCircleLabel(void)
{
}
BOOL CCircleLabel::CreatCircleLabel(ads_point StartPt, ads_point EndPt, double Bulge)
{
	AcDbObjectId   lineLayerId;
	Acad::ErrorStatus es=UpdateLayerByConfig(lineLayerId,_T("坐标标注_线"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置[坐标标注_线]图层失败:%s"),acadErrorStatusTextChs(es));
		return FALSE;
	}
	AcDbObjectId   textLayerId;
	es=UpdateLayerByConfig(textLayerId,_T("坐标标注_文字"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置[坐标标注_文字]图层失败:%s"),acadErrorStatusTextChs(es));
		return FALSE;
	}
	int nHTBL=1;
	int nCTBL=1;
	GCADUtils::GetScale(nHTBL,nCTBL);
	//获得圆心坐标和弧线中点坐标
	ads_point ptCentre, ptMid;

	double dDist = acutDistance(StartPt, EndPt);

	double dAngle = acutAngle(EndPt, StartPt);

	double dRadius = dDist / (2.0 * sin(2.0 * atan(Bulge)));

	ads_point ptMidTmp;
	ptMidTmp[0] = (StartPt[0] + EndPt[0]) / 2.0;
	ptMidTmp[1] = (StartPt[1] + EndPt[1]) / 2.0;
	ptMidTmp[2] = (StartPt[2] + EndPt[2]) / 2.0;

	acutPolar(ptMidTmp, dAngle + PI / 2.0, dDist * Bulge / 2.0, ptMid);

	acutPolar(ptMid, dAngle - PI / 2.0, dRadius, ptCentre);

	//得到输入参数
	ads_point pIn;

	if (acedGetPoint(ptCentre, _T("\n请选择引出线端点: "), pIn) != RTNORM)
	{
		return FALSE;
	}
	GCADUtils::UCS2WCS(pIn);

	acutPolar(ptCentre, acutAngle(ptCentre, ptMid), acutDistance(pIn, ptCentre), pIn);

	//画引线
	AcDbBlockTable *pBlockTable;
	acdbCurDwg()->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();
	AcDbObjectId TextID;

	AcDbObjectId ObjID;
	AcDbLine *pLine;
	AcGePoint3d startPt;
	AcGePoint3d endPt;

	startPt.x = ptCentre[0]; startPt.y = ptCentre[1]; startPt.z = ptCentre[2];
	endPt.x = pIn[0]; endPt.y = pIn[1]; endPt.z = pIn[2];
	pLine = new AcDbLine(startPt, endPt);
	pLine->setLayer(lineLayerId);
	pBlockTableRecord->appendAcDbEntity(ObjID, pLine);
	pLine->close();

	float fSpace = float(FontHeight * 0.5*nHTBL);

	//添加新字体类型
	//if (AddBigFontToCurDwg(_T(""), USER_TextStyle) == false)
	TextID = GetTextStyleObjID(_T("Standard"));
	//else TextID = GetTextStyleObjID(USER_TextStyle);

	dRadius = fabs(dRadius);
	CString str;
	//str.Format(_T("R%f"), dRadius);
	str.Format(_T(" R%.f"), dRadius*m_dScale);// 输出到小数点
	ads_real dLabelAngle = acutAngle(pIn, ptCentre);//计算标注坐标用的角度
	if (dLabelAngle >= PI / 2.0 && dLabelAngle < PI * 3.0 / 2.0)
	{
		AcGePoint3d InsertPt;
		AcDbText *pText;
		double distance = GetTextWidth(str, FontHeight*nHTBL);
		InsertPt.x = pIn[0] + fSpace * sin(dLabelAngle) + distance * cos(dLabelAngle);
		InsertPt.y = pIn[1] - fSpace * cos(dLabelAngle) + distance * sin(dLabelAngle);
		InsertPt.z = pIn[2];
		pText = new AcDbText(
			InsertPt,
			str,
			TextID,
			FontHeight*nHTBL,
			PI + dLabelAngle);
		pText->setLayer(textLayerId);
		pBlockTableRecord->appendAcDbEntity(ObjID, pText);
		pText->close();
	}
	else
	{
		AcGePoint3d InsertPt;
		AcDbText *pText;
		InsertPt.x = pIn[0] - fSpace * sin(dLabelAngle);
		InsertPt.y = pIn[1] + fSpace * cos(dLabelAngle);
		InsertPt.z = pIn[2];
		pText = new AcDbText(
			InsertPt,
			str,
			TextID,
			FontHeight*nHTBL,
			dLabelAngle);
		pText->setLayer(textLayerId);
		pBlockTableRecord->appendAcDbEntity(ObjID, pText);
		pText->close();
	}

	pBlockTableRecord->close();

	return TRUE;
}