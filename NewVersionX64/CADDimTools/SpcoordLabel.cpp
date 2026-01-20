#include "StdAfx.h"
#include "SpcoordLabel.h"
//#include "GxAfxFunction.h"
#ifndef ACAD_2004
#if _MSC_VER<1400
#include "dbtable.h"
#endif
#endif
//#include "DistXData.h"
//#include "DataDefine.h"
//#include "CommonFunc.h"

//extern IDistConnection * Glb_pConnection;
//extern ST_BIZZINFO Glb_BizzInfo; // 业务信息
//extern ST_LAYERINFO Glb_LayerInfo; // 图层信息

double CSpcoordLabel::FontHeight = 3.0;


//////////////////////////////////////////////////////////////////////////
CSpcoordLabel::CSpcoordLabel(void)
{
	/*	// 获取捕捉参数
	m_OldRbf = acutNewRb(RTREAL);
	acedGetVar(_T("osmode"), m_OldRbf);
	// 设置新捕捉参数（点捕捉）
	struct resbuf *NewRbf = acutBuildList(RTSHORT, 9, 0);
	acedSetVar(_T("osmode"), NewRbf);
	acutRelRb(NewRbf);
	*/
	m_bExchangeXY=false;
	m_dScale=1.0;
}

CSpcoordLabel::~CSpcoordLabel(void)
{
	// 还原捕捉参数
	//	acedSetVar("osmode", m_OldRbf);
	//	acutRelRb(m_OldRbf);
}

void CSpcoordLabel::CoverAngelP2(ads_real &dAngle,ads_real &dLabelAngle)
{
	if(dAngle>=0 && dAngle<PI*0.5)
	{
		dLabelAngle = dAngle + PI*0.5;
	}
	else if(dAngle>=PI*0.5 && dAngle<PI*2)
	{
		dLabelAngle = dAngle - PI*1.5;
	}
}

void CSpcoordLabel::CoverAngelP3(ads_real &dAngle,ads_real &dLabelAngle)
{
	if (dAngle >= PI*0.5 && dAngle < PI)
	{
		dLabelAngle = dAngle - PI*0.5;
		dAngle = PI + dAngle;
	}
	else if (dAngle >= PI && dAngle <= PI*1.5)
	{
		dLabelAngle = dAngle - PI*0.5;
		dAngle = dAngle - PI;
	}
}

double CSpcoordLabel::GetTextWidth(LPCTSTR sText, double dHigh)
{
	if (lstrlen(sText)>1023 ) return 0;//这个地方容错一下 
	//下面求每列的最大列宽
	ads_point ptLB;
	ads_point ptRT;
	struct resbuf *rb=acutBuildList(1,sText,40,dHigh,0);
	double dReturn = 0;
	if (acedTextBox(rb, ptLB, ptRT) == RTNORM)
	{
		dReturn = ptRT[X];
	}
	acutRelRb(rb);
	return dReturn;
}

BOOL CSpcoordLabel::AddBigFontToCurDwg(CString BigFontFileName, CString TextStyleName)
{
	AcDbTextStyleTable *pTextStyleTable;
	acdbHostApplicationServices()->workingDatabase()
		->getTextStyleTable(pTextStyleTable, AcDb::kForWrite);

	AcDbTextStyleTableRecord *pTextStyleTableRecord;
	AcDbObjectId TextID;

	pTextStyleTableRecord = new AcDbTextStyleTableRecord;
	pTextStyleTableRecord->setBigFontFileName(BigFontFileName);
	pTextStyleTableRecord->setFileName(_T("宋体"));
	pTextStyleTableRecord->setName(TextStyleName); //属性标注专用字体
	if(pTextStyleTable->add(TextID, pTextStyleTableRecord)==Acad::eOk)
	{
		pTextStyleTableRecord->close();
	}
	else
	{
		delete pTextStyleTableRecord;
	}
	if(pTextStyleTable->getAt(TextStyleName, TextID)!=Acad::eOk)
	{
		CString strMSG;
		strMSG.Format(_T("找不到\"%s\"文件或属性标注专用字体对像"),BigFontFileName);
		MessageBox(NULL, strMSG, _T("数慧提示"), MB_OK|MB_ICONINFORMATION);
		pTextStyleTableRecord->close();
		pTextStyleTable->close();
		return false;
	}

	pTextStyleTableRecord->close();
	pTextStyleTable->close();

	return true;
}

AcDbObjectId CSpcoordLabel::GetTextStyleObjID(CString TextStyleName)
{
	AcDbTextStyleTable *pTextStyleTable;
	acdbHostApplicationServices()->workingDatabase()
		->getTextStyleTable(pTextStyleTable, AcDb::kForRead);

	AcDbObjectId TextID;
	TextID.setNull();

	if(pTextStyleTable->getAt(TextStyleName, TextID)!=Acad::eOk)
	{
		CString strMSG;
		strMSG.Format(_T("找不到\"%s\"字体风格"),TextStyleName);
		MessageBox(NULL, strMSG, _T("CAD工具之家提示"), MB_OK|MB_ICONINFORMATION);
		pTextStyleTable->close();
		TextID.setNull();
		return TextID;
	}

	pTextStyleTable->close();

	return TextID;
}
// 创建带引线的文字标注
BOOL CSpcoordLabel::CreateDimText(CString strText, ads_point startPt /* = NULL*/)
{
	AcDbObjectId   lineLayerId;
	Acad::ErrorStatus es=UpdateLayerByConfig(lineLayerId,_T("文字标注_线"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置[文字标注_线]图层失败:%s"),acadErrorStatusTextChs(es));
		return FALSE;
	}
	AcDbObjectId   textLayerId;
	es=UpdateLayerByConfig(textLayerId,_T("文字标注_文字"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置[文字标注_文字]图层失败:%s"),acadErrorStatusTextChs(es));
		return FALSE;
	}
	int nHTBL=1;
	int nCTBL=1;
	GCADUtils::GetScale(nHTBL,nCTBL);
	//得到输入参数
	ads_point p1, p2 ,p3;
	if (startPt != NULL)
	{
		p1[0] = startPt[0];
		p1[1] = startPt[1];
		p1[2] = startPt[2];
	}
	else
	{
		//得到输入参数
		if (acedGetPoint(NULL, _T("\n指定起点: "), p1) != RTNORM)
			return FALSE;
	}
	// 输入第二点
	CString strPrompt;
	strPrompt.Format(_T("\n 请指定下一个点或输入字高 <%.1f>: "), FontHeight);

	acedInitGet(INP_NNEG | RSG_OTHER, _T("1 2 3 4 5 6 7 8 9 ."));

	switch (acedGetPoint(p1, strPrompt, p2))
	{
	case RTNORM:
		break;
	case RTKWORD:
		{
			TCHAR szInput[128] = {0};
			acedGetInput(szInput);
			CString strInput = szInput;
			if (strInput.Find(_T(',')) < 0)
				FontHeight=_tstof(szInput);

			strPrompt = _T("\n 请输入下一个点");
			acedGetPoint(p1, strPrompt, p2);
		}
		break;
	default:
		return FALSE;
	}
	GCADUtils::UCS2WCS(p1);
	GCADUtils::UCS2WCS(p2);

	// 第三个点
	p3[0] = 2 * p2[0] - p1[0];
	p3[1] = p2[1];
	p3[2] = p2[2];

	AcDbBlockTable *pBlockTable;
	acdbCurDwg()->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();

	float fSpace = float(FontHeight * nHTBL*0.5);
	//计算p2,p3的角度
	ads_real dAngle;
	dAngle = acutAngle(p2, p3);

	AcDbObjectId objId;

	//当前字体
	AcDbObjectId TextID = acdbHostApplicationServices()->workingDatabase()->textstyle();
	//确保是中文字体，如果不是则修改为宋体

	double textLengh = 0.0;

	AcDbText *pText = new AcDbText(AcGePoint3d(0,0,0), strText, TextID, FontHeight*nHTBL, 0);

	if (p1[0] <= p2[0])	//右指向生成标注
	{
		//从p2点开始计算坐标标注起点
		//AcGePoint3d InsertPt = asPnt3d(p2);
		//从p2点开始计算坐标标注起点
		ads_real dLabelAngle;  //计算标注坐标用的角度
		CoverAngelP2(dAngle, dLabelAngle);
		AcGePoint3d InsertPt;
		InsertPt.x = p2[0] + fSpace * cos(dLabelAngle);
		InsertPt.y = p2[1] + fSpace * sin(dLabelAngle);
		InsertPt.z = p2[2];

		pText->setPosition(InsertPt);
		pText->setHorizontalMode(AcDb::kTextLeft);	
		pText->setVerticalMode(AcDb::kTextBase);
		pText->setAlignmentPoint(InsertPt);
		pText->setLayer(textLayerId);

		pBlockTableRecord->appendAcDbEntity(objId, pText);
		pText->close();
	}
	else 
	{
		//AcGePoint3d InsertPt = asPnt3d(p2);
		//从p3点开始计算坐标标注起点
		ads_real dLabelAngle;  //计算标注坐标用的角度
		CoverAngelP3(dAngle, dLabelAngle);
		AcGePoint3d InsertPt;
		InsertPt.x = p3[0] + fSpace * cos(dLabelAngle);
		InsertPt.y = p3[1] + fSpace * sin(dLabelAngle);
		InsertPt.z = p3[2];

		pText->setPosition(InsertPt);
		pText->setHorizontalMode(AcDb::kTextRight);
		pText->setVerticalMode(AcDb::kTextBase);
		pText->setAlignmentPoint(InsertPt);
		pText->setLayer(textLayerId);
		pBlockTableRecord->appendAcDbEntity(objId, pText);
		pText->close();
	}

	AcDbExtents Ext;
	pText->getGeomExtents(Ext);

	AcGePoint3d minPt,maxPt;
	minPt = Ext.minPoint();
	maxPt = Ext.maxPoint();
	textLengh = acutDistance(asDblArray(minPt),asDblArray(maxPt));

	p3[0] = p2[0] + textLengh * cos(dAngle);
	p3[1] = p2[1] + textLengh * sin(dAngle);
	p3[2] = p2[2];

	AcGePoint2d pt1;
	AcGePoint2d pt2;
	AcGePoint2d pt3;

	pt1.x = p1[0]; pt1.y = p1[1]; 
	pt2.x = p2[0]; pt2.y = p2[1]; 
	pt3.x = p3[0]; pt3.y = p3[1]; 

	AcDbPolyline * pLine = new AcDbPolyline(3);
	pLine->addVertexAt(0, pt1);
	pLine->addVertexAt(1, pt2);
	pLine->addVertexAt(2, pt3);
	pLine->setLayer(lineLayerId);

	pBlockTableRecord->appendAcDbEntity(objId, pLine);
	pLine->close();

	pBlockTableRecord->close();

	return TRUE;
}
double CSpcoordLabel::GetTextWidth(CString sText, double dHigh,AcDbObjectId& TextID)
{
	AcDbText *pText = new AcDbText(AcGePoint3d(0,0,0), sText, TextID, dHigh, 0);
	AcDbExtents Ext;
	pText->getGeomExtents(Ext);
	AcGePoint3d minPt,maxPt;
	minPt = Ext.minPoint();
	maxPt = Ext.maxPoint();
	double textLength = acutDistance(asDblArray(minPt),asDblArray(maxPt));
	pText->close();
	pText=NULL;
	return textLength;
}
double CSpcoordLabel::GetTextMaxWidth(CStringArray& strTextArray,double dHigh,AcDbObjectId& TextID)
{
	double textLength = 0.0;
	for(int i=0;i<strTextArray.GetCount();i++)
	{
		CString strText=strTextArray.GetAt(i);
		double dTmp=GetTextWidth(strText,dHigh,TextID);
		if(dTmp>textLength)
		{
			textLength=dTmp;
		}
	}
	return textLength;
}
BOOL CSpcoordLabel::CreateCoordLabel(ads_point startPt)
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
	AcDbObjectIdArray objIdArray;

	//得到输入参数
	ads_point p1, p2 ,p3;
	if (startPt != NULL)
	{
		p1[0] = startPt[0];
		p1[1] = startPt[1];
		p1[2] = startPt[2];
	}
	else
	{
		//得到输入参数
		if (acedGetPoint(NULL, _T("\n>>指定点坐标: "), p1) != RTNORM)
			return FALSE;
	}
	// 输入第二点
	CString strPrompt;
	strPrompt.Format(_T("\n>>请指定下一个点或输入字高 <%.1f>: "), FontHeight);

	acedInitGet(INP_NNEG | RSG_OTHER, _T("1 2 3 4 5 6 7 8 9 ."));

	switch (acedGetPoint(p1, strPrompt, p2))
	{
	case RTNORM:
		break;
	case RTKWORD:
		{
			TCHAR szInput[128] = {0};
			acedGetInput(szInput);
			CString strInput = szInput;
			if (strInput.Find(_T(',')) < 0)
				FontHeight=_tstof(szInput);

			strPrompt = _T("\n>>请输入下一个点: ");
			acedGetPoint(p1, strPrompt, p2);
		}
		break;
	default:
		return FALSE;
	}

	GCADUtils::UCS2WCS(p1);
	GCADUtils::UCS2WCS(p2);

	// 第三个点
	p3[0] = 2 * p2[0] - p1[0];
	p3[1] = p2[1];
	p3[2] = p2[2];

	AcDbBlockTable *pBlockTable;
	acdbCurDwg()->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();

	float fSpace = float(FontHeight * 0.5*nHTBL);
	//计算p2,p3的角度
	ads_real dAngle;
	dAngle = acutAngle(p2, p3);

	AcDbObjectId objId;

	// 字体
	AcDbObjectId TextID = GetTextStyleObjID(_T("Standard"));

	double textLength = 0.0;

	AcGePoint3d cadPt(p1[0],p1[1],0.0);
	AcGePoint3d xyPt=cadPt;

	CString strX;
	strX.Format(_T("X=%.3f"), m_bExchangeXY?xyPt.y*m_dScale:xyPt.x*m_dScale);// 交换XY坐标，输出小数点后三位
	AcDbText *pTextX = new AcDbText(AcGePoint3d(0,0,0), strX, objId, FontHeight*nHTBL, 0);
	pTextX->setLayer(textLayerId);

	AcDbExtents Ext;
	pTextX->getGeomExtents(Ext);

	AcGePoint3d minPt,maxPt;
	minPt = Ext.minPoint();
	maxPt = Ext.maxPoint();
	double textLengthX = acutDistance(asDblArray(minPt),asDblArray(maxPt));

	CString strY;
	strY.Format(_T("Y=%.3f"), m_bExchangeXY?xyPt.x*m_dScale:xyPt.y*m_dScale);// 交换XY坐标，输出小数点后三位
	AcDbText *pTextY = new AcDbText(AcGePoint3d(0,0,0), strY, objId, FontHeight*nHTBL, 0);
	pTextY->setLayer(textLayerId);

	pTextY->getGeomExtents(Ext);

	minPt = Ext.minPoint();
	maxPt = Ext.maxPoint();
	double textLengthY = acutDistance(asDblArray(minPt),asDblArray(maxPt));

	textLength = textLengthX > textLengthY ? textLengthX : textLengthY;

	p3[0] = p2[0] + textLength * cos(dAngle);
	p3[1] = p2[1] + textLength * sin(dAngle);
	p3[2] = p2[2];

	if (p1[0] <= p2[0])
	{
		//从p2点开始计算坐标标注起点
		ads_real dLabelAngle;  //计算标注坐标用的角度
		CoverAngelP2(dAngle, dLabelAngle);

		AcGePoint3d InsertPt;

		InsertPt.x = p2[0] + fSpace * cos(dLabelAngle);
		InsertPt.y = p2[1] + fSpace * sin(dLabelAngle);
		InsertPt.z = p2[2];

		pTextX->setPosition(InsertPt);

		pBlockTableRecord->appendAcDbEntity(objId, pTextX);
		objIdArray.append(objId);
		pTextX->close();

		InsertPt.x = p2[0] - (fSpace + FontHeight*nHTBL) * cos(dLabelAngle);
		InsertPt.y = p2[1] - (fSpace + FontHeight*nHTBL) * sin(dLabelAngle);
		InsertPt.z = p2[2];

		pTextY->setPosition(InsertPt);

		pBlockTableRecord->appendAcDbEntity(objId, pTextY);
		objIdArray.append(objId);
		pTextY->close();
	}
	else 
	{
		//从p3点开始计算坐标标注起点
		ads_real dLabelAngle;  //计算标注坐标用的角度
		CoverAngelP3(dAngle, dLabelAngle);

		AcGePoint3d InsertPt;

		InsertPt.x = p3[0] + fSpace * cos(dLabelAngle);
		InsertPt.y = p3[1] + fSpace * sin(dLabelAngle);
		InsertPt.z = p3[2];

		pTextX->setPosition(InsertPt);

		pBlockTableRecord->appendAcDbEntity(objId, pTextX);
		objIdArray.append(objId);
		pTextX->close();

		InsertPt.x = p3[0] - (fSpace + FontHeight*nHTBL) * cos(dLabelAngle);
		InsertPt.y = p3[1] - (fSpace + FontHeight*nHTBL) * sin(dLabelAngle);
		InsertPt.z = p3[2];

		pTextY->setPosition(InsertPt);

		pBlockTableRecord->appendAcDbEntity(objId, pTextY);
		objIdArray.append(objId);
		pTextY->close();
	}

	//画引线
	AcDbPolyline *pLine;
	AcGePoint2d pt1;
	AcGePoint2d pt2;
	AcGePoint2d pt3;

	pt1.x = p1[0]; pt1.y = p1[1]; 
	pt2.x = p2[0]; pt2.y = p2[1]; 
	pt3.x = p3[0]; pt3.y = p3[1]; 

	pLine = new AcDbPolyline(3);
	pLine->addVertexAt(0, pt1);
	pLine->addVertexAt(1, pt2);
	pLine->addVertexAt(2, pt3);
	pLine->setLayer(lineLayerId);

	pBlockTableRecord->appendAcDbEntity(objId, pLine);
	objIdArray.append(objId);
	pLine->close();

	pBlockTableRecord->close();

	//	CString strId;
	//	strId.Format("%d",objIdArray.at(0).asOldId());

	//	createGroup(objIdArray, strId);

	return TRUE;
}

