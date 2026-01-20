#include "StdAfx.h"
#include "TextStatistics.h"
#if _MSC_VER<=1300
#ifndef ACAD_2004
#include "dbtable.h"
#include "dbTableStyle.h"
#endif
#endif
double GetMTextWidth(const CString content, const AcDbObjectId idTextStyle, double height)
{
	AcDbMText* pText = new AcDbMText;
	AcDbObjectId textStyleId;
	pText->setTextStyle(idTextStyle);
	pText->setContents(content);
	pText->setTextHeight(height);
	pText->setLocation(AcGePoint3d::kOrigin);
	pText->setAttachment(AcDbMText::kMiddleCenter);
	AcDbExtents extents;
	pText->getGeomExtents(extents);
	delete pText;
	pText = NULL;
	return (extents.maxPoint().x - extents.minPoint().x);
}
void TextStatistics(CTextStatisticsResult& result,const AcDbEntity* pEnt)
{
	if(pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=AcDbText::cast(pEnt);
		if(pText!=NULL)
		{
			result.Add(pText->textString());
		}
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pText=AcDbMText::cast(pEnt);
		if(pText!=NULL)
		{
			result.Add(GCADUtils::GetMText(pText));
		}
	}
	else if(pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		//统计属性
		AcDbBlockReference* pRef=AcDbBlockReference::cast(pEnt);
		AcDbObjectIterator* pObjIter=pRef->attributeIterator();
		if(pObjIter!=NULL)
		{
			pObjIter->start();
			while(!pObjIter->done())
			{
				AcDbEntity* pSubEnt=pObjIter->entity();
				if(pSubEnt!=NULL)
				{
					TextStatistics(result,pSubEnt);
				}
				pObjIter->step();
			}
			delete pObjIter;
			pObjIter=NULL;
		}
		//统计块的定义
		AcDbObjectPointer<AcDbBlockTableRecord> spRec(pRef->objectId(),AcDb::kForRead);
		if(spRec.openStatus()==Acad::eOk)
		{
			AcDbBlockTableRecordIterator* pIter = NULL;
			spRec->newIterator(pIter);

			std::auto_ptr<AcDbBlockTableRecordIterator> spIter(pIter);

			for(pIter->start(); !pIter->done(); pIter->step() )
			{
				AcDbEntity* pEnt = NULL;
				Acad::ErrorStatus es=pIter->getEntity(pEnt, AcDb::kForRead);
				if(es!=eOk)
					continue;
				TextStatistics(result,pEnt);
				pEnt->close();
			}
		}
	}
}
void TextStatistics(CTextStatisticsResult& result,const AcDbObjectId objId)
{
	AcDbEntityPointer spEnt(objId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
		return;
	TextStatistics(result,spEnt.object());
}
#ifdef ACAD_2004
//文字统计
void TextStatistics()
{
	acutPrintf(_T("\nAutoCAD2004不支持表格功能，如果您需要使用文字统计功能，请升级至AutoCAD2005或以上版本"));
}
#else
//文字统计
void TextStatistics()
{
	ads_name ssName;
	int rc=acedSSGet(NULL, NULL,NULL,NULL,ssName);
	if(rc!=RTNORM)
	{
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	CTextStatisticsResult result;
	for(long i=0;i<ids.logicalLength();i++)
	{
		TextStatistics(result,ids.at(i));
	}
	if(result.m_ResultItems.GetCount()==0)
	{
		return;
	}
	result.Sort();
	ads_point ptResult;
	rc=acedGetPoint(NULL,_T("\r\n请输入结果表格插入点"),ptResult);
	if(rc!=RTNORM)
	{
		return;
	}


	AcDbTable *pTable=new AcDbTable();
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	pTable->setTableStyle(pDb->tablestyle());
	pTable->setDatabaseDefaults(acdbHostApplicationServices()->workingDatabase());
	pTable->setNumColumns(3);
	pTable->setNumRows(result.m_ResultItems.GetCount()+1);
	pTable->setPosition(GCADUtils::UCS2WCS(AcGePoint3d(ptResult[X],ptResult[Y],ptResult[Z])));
	pTable->setTextString(0,0,_T("序号"));
	pTable->setTextString(0,1,_T("内容"));
	pTable->setTextString(0,2,_T("数量"));
	pTable->setAlignment(0,0,AcDb::kMiddleCenter);
	pTable->setAlignment(0,1,AcDb::kMiddleCenter);
	pTable->setAlignment(0,2,AcDb::kMiddleCenter);

	AcDbObjectPointer<AcDbTableStyle> spStyle(pDb->tablestyle(),AcDb::kForRead);
	if(spStyle.openStatus()==Acad::eOk)
	{
		AcDbObjectId idStyle=spStyle->textStyle();
		double       dHeight=spStyle->textHeight();
		double dTableTextWidth=-1.0;//内容列的宽度
		for(long j=0;j<result.m_ResultItems.GetCount();j++)
		{
			CTextStatisticsResultItem& item=result.m_ResultItems.GetAt(j);
			double dWidht=GetMTextWidth(item.strText,idStyle,dHeight);
			if(dWidht>dTableTextWidth)
			{
				dTableTextWidth=dWidht;
			}
		}
		if(dTableTextWidth>0)
		{
			pTable->setColumnWidth(1,min(dTableTextWidth+20,dTableTextWidth*1.1));
		}
	}
	pTable->setColumnWidth(0,20);
	pTable->setColumnWidth(2,20);

	for(long j=0;j<result.m_ResultItems.GetCount();j++)
	{
		CTextStatisticsResultItem& item=result.m_ResultItems.GetAt(j);
		CString strId;
		strId.Format(_T("%d"),j+1);
		pTable->setTextString(j+1,0,strId);
		pTable->setTextString(j+1,1,item.strText);
		CString strNum;
		strNum.Format(_T("%d"),item.nNum);
		pTable->setTextString(j+1,2,strNum);
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
	/*
	double dTableTextWidth=-1.0;//内容列的宽度
	for(long j=0;j<result.m_ResultItems.GetCount();j++)
	{
		CTextStatisticsResultItem& item=result.m_ResultItems.GetAt(j);
		AcDbObjectId idStyle=pTable->textStyle();
		double       dHeight=pTable->textHeight();
		double dWidht=GetMTextWidth(item.strText,idStyle,dHeight);
		if(dWidht>dTableTextWidth)
		{
			dTableTextWidth=dWidht;
		}
	}
	if(dTableTextWidth>0)
	{
		pTable->setColumnWidth(1,min(dTableTextWidth+20,dTableTextWidth*1.1));
	}
	*/
	pTable->generateLayout();
	pTable->close();


}
#endif
void CTextStatisticsResult::Add(const CString strText,const long nNum)
{
	if(nNum<=0)
		return;
	for(long i=0;i<m_ResultItems.GetCount();i++)
	{
		CTextStatisticsResultItem& item=m_ResultItems.GetAt(i);
		if(item.strText.CompareNoCase(strText)==0)
		{
			item.nNum+=nNum;
			m_ResultItems.SetAt(i,item);
			return;
		}
	}
	CTextStatisticsResultItem item;
	item.strText=strText;
	item.nNum=nNum;
	m_ResultItems.Add(item);

}
void CTextStatisticsResult::Add(const CTextStatisticsResultItem item)
{
	Add(item.strText,item.nNum);
}
CTextStatisticsResult::CTextStatisticsResult(const CTextStatisticsResult& result)
{
	m_ResultItems.RemoveAll();
	m_ResultItems.Append(result.m_ResultItems);
}
CTextStatisticsResult& CTextStatisticsResult::operator=(const CTextStatisticsResult& result)
{
	if(this==&result)
		return *this;
	m_ResultItems.RemoveAll();
	m_ResultItems.Append(result.m_ResultItems);
	return *this;
}
void CTextStatisticsResult::Sort()
{
	for(long i=0;i<m_ResultItems.GetCount()-1;i++)
	{
		CTextStatisticsResultItem one_value=m_ResultItems.GetAt(i);
		for(long j=i+1;j<m_ResultItems.GetCount();j++)
		{
			CTextStatisticsResultItem another_value=m_ResultItems.GetAt(j);
			if(another_value.strText.CompareNoCase(one_value.strText)<0)//升序排列
			{
				CTextStatisticsResultItem value_swap=m_ResultItems.GetAt(i);
				m_ResultItems.SetAt(i,m_ResultItems.GetAt(j));
				m_ResultItems.SetAt(j,value_swap);
				one_value=m_ResultItems.GetAt(i);


			}
		}
	}
}
