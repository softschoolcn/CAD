#include "stdafx.h"
#include "CommCommand.h"
#if _MSC_VER<=1300
#ifndef ACAD_2004
#include "dbtable.h"
#include "dbTableStyle.h"
#endif
#endif
#if _MSC_VER<=1300
//填充统计
void HatchStatistics()
{
	acutPrintf(_T("\nAutoCAD2006以下不支持表格或者填充面积计算，如果您需要使用填充统计功能，请升级至AutoCAD2007或以上版本"));
}
#else
struct GHatchItem
{
	int nNum;
	double a;
	CString strName;
};
void AddHatchResult(CArray<GHatchItem>& items,GHatchItem item)
{
	for(int i=0;i<items.GetCount();i++)
	{
		GHatchItem& tmp=items.GetAt(i);
		if(tmp.strName.CompareNoCase(item.strName)==0)
		{
			tmp.nNum+=item.nNum;
			tmp.a+=item.a;
			items.SetAt(i,tmp);
			return;
		}
	}
	items.Add(item);
}
//输出块的统计结果
void OutputHatchResult(CArray<GHatchItem>& items)
{
	ads_point ptResult;
	int rc=acedGetPoint(NULL,_T("\n请输入结果表格插入点"),ptResult);
	if(rc!=RTNORM)
	{
		return;
	}
	AcDbTable *pTable=new AcDbTable();
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	pTable->setTableStyle(pDb->tablestyle());
	pTable->setDatabaseDefaults(acdbHostApplicationServices()->workingDatabase());
	pTable->setNumColumns(3);
	pTable->setNumRows(items.GetCount()+1);
	pTable->setPosition(GCADUtils::UCS2WCS(AcGePoint3d(ptResult[X],ptResult[Y],ptResult[Z])));
	pTable->setTextString(0,0,_T("填充名称"));
	pTable->setTextString(0,1,_T("面积"));
	pTable->setTextString(0,2,_T("数量"));
	pTable->setAlignment(0,0,AcDb::kMiddleCenter);
	pTable->setAlignment(0,1,AcDb::kMiddleCenter);
	pTable->setAlignment(0,2,AcDb::kMiddleCenter);
	pTable->setColumnWidth(0,40);
	pTable->setColumnWidth(1,40);
	pTable->setColumnWidth(2,20);

	for(long j=0;j<items.GetCount();j++)
	{
		GHatchItem& item=items.GetAt(j);
		pTable->setTextString(j+1,0,item.strName);
		CString strTmp;
		strTmp.Format(_T("%.3f"),item.a);
		pTable->setTextString(j+1,1,strTmp);
		strTmp.Format(_T("%d"),item.nNum);
		pTable->setTextString(j+1,2,strTmp);
		pTable->setAlignment(j+1,0,AcDb::kMiddleCenter);
		pTable->setAlignment(j+1,1,AcDb::kMiddleCenter);
		pTable->setAlignment(j+1,2,AcDb::kMiddleCenter);
	}
	pTable->generateLayout();
	GCADUtils::AddToCurrentSpace(pTable);
	int minRow;
	int maxRow;
	int minCol;
	int maxCol;
	if(pTable->isMergedCell(0,0,&minRow,&maxRow,&minCol,&maxCol))
	{
		pTable->unmergeCells(minRow,maxRow,minCol,maxCol);
	}
	pTable->generateLayout();
	pTable->close();
}
//121:填充统计
void HatchStatistics()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("HATCH"), 0);
	AcDbObjectIdArray ids1;
	GCADUtils::GetCurrentSelectObjIds(ids1,rb);
	if(ids1.logicalLength()==0)
	{
		ads_name ssName;
		TCHAR* promptPtrs[2]={_T("\n选择填充:"),_T("\n移除填充:")};
		int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
		if(rc!=RTNORM)
		{
			acutRelRb(rb);
			return;
		}
		acutRelRb(rb);

		SSToIds(ids1,ssName);
		acedSSFree(ssName);
	}
	else
	{
		acutRelRb(rb);
	}
	CArray<GHatchItem> items;
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId id=ids1.at(i);
		AcDbObjectPointer<AcDbHatch> spHatch(id,AcDb::kForRead);
		if(spHatch.openStatus()!=Acad::eOk)
		{
			continue;
		}
		double a;
		spHatch->getArea(a);
		CString strName=spHatch->patternName();
		GHatchItem item;
		item.nNum=1;
		item.a=a;
		item.strName=strName;
		AddHatchResult(items,item);
	}
	OutputHatchResult(items);
}
#endif