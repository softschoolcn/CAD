//////////////////////////////////////////////////////////////////////////////
// AcEdInputPointMonitor, created by  [2004-11-10], , 

#include "StdAfx.h"
#include "dbpl.h"
#include "dbobjptr.h "
#include "GxAfxFunction.h"
#include "ParcelViewInputPoint.h"
extern CStringArray Glb_MousePreviewKeyArr;
extern CStringArray Glb_MousePreviewValueArr;
extern CArray<bool> Glb_MousePreviewOpenArr;
BOOL bMonitorInputPointFirstTime=TRUE;
bool IsPropOpen(CString strKey)
{
	for(int i=0;i<Glb_MousePreviewKeyArr.GetCount();i++)
	{
		if(strKey.CompareNoCase(Glb_MousePreviewKeyArr.GetAt(i))==0)
		{
			return Glb_MousePreviewOpenArr.GetAt(i);
		}
	}
	return false;
}

AcDbHatch *g_hatch=NULL;
void UnHightEnt(AcDbObjectId EntId)
{
	AcDbEntity *pEnt=NULL;
	if(acdbOpenObject(pEnt,EntId,AcDb::kForRead)==Acad::eOk)
	{
		pEnt->unhighlight();
		pEnt->close();
		pEnt=NULL;
	}
}
void UnHightEnt(AcDbObjectIdArray Ids)
{
	for(int nLen=0;nLen<Ids.logicalLength();nLen++)
		UnHightEnt(Ids[nLen]);
}
CString GetEntLayerName(AcDbObjectId EntId)
{
	AcDbEntity *pEnt=NULL;
	CString strLayerName;
	if(acdbOpenObject(pEnt,EntId,AcDb::kForRead)==Acad::eOk)
	{
		strLayerName=pEnt->layer();
		pEnt->close();
		pEnt=NULL;
	}
	return strLayerName;
}
void HightLightEnt(AcDbObjectId EntId)
{
	AcDbEntity *pEnt=NULL;
	if(acdbOpenObject(pEnt,EntId,AcDb::kForRead)==Acad::eOk)
	{
		pEnt->highlight();
		pEnt->close();
		pEnt=NULL;
	}

}
void HightLightEnt(AcDbObjectIdArray Ids)
{
	for(int nLen=0;nLen<Ids.logicalLength();nLen++)
		HightLightEnt(Ids[nLen]);
}
AsdkDynViewInputPoint::AsdkDynViewInputPoint()
:AcEdInputPointMonitor()
{
	ReadEntTypeAlias(entTypeNameArray,entTypeAliasArray);
}


AsdkDynViewInputPoint::~AsdkDynViewInputPoint()
{
	if(g_hatch!=NULL)
	{
		delete g_hatch;
		g_hatch=NULL;
	}
} 
#if _MSC_VER<1500
Acad::ErrorStatus AsdkDynViewInputPoint::monitorInputPoint(bool& appendToTooltipStr,
														   TCHAR*& additionalTooltipString,
														   AcGiViewportDraw* drawContext,
														   AcApDocument* document,
														   bool pointComputed,
														   int history,
														   const AcGePoint3d& lastPoint,
														   const AcGePoint3d& rawPoint,
														   const AcGePoint3d& grippedPoint,
														   const AcGePoint3d& cartesianSnappedPoint,
														   const AcGePoint3d& osnappedPoint,
														   AcDb::OsnapMask osnapMask,
														   const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
														   AcDb::OsnapMask osnapOverrides,
														   const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
														   const AcArray<AcDbObjectId>& apertureEntities,
														   const AcArray< AcDbObjectIdArray,
														   AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedApertureEntities,
														   const AcArray<int>& gsSelectionMark,
														   const AcArray<AcDbObjectId>& keyPointEntities,
														   const AcArray< AcDbObjectIdArray,
														   AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedKeyPointEntities,
														   const AcArray<int>& keyPointGsSelectionMark,
														   const AcArray<AcGeCurve3d*>& alignmentPaths,
														   const AcGePoint3d& computedPoint,
														   const TCHAR* tooltipString)
{   
	if(bMonitorInputPointFirstTime)
	{
		GLog::WriteLine(_T("AsdkDynViewInputPoint::monitorInputPoint"));
		bMonitorInputPointFirstTime=FALSE;
	}
	//acutPrintf(_T("(%f,%f)\n"),rawPoint.x,rawPoint.y);
	//return Acad::eOk;
	AcDbEntity *pEnt=NULL; 
	//	if(g_nSoftType!=-1)
	//		return Acad::eOk;
	UnHightEnt(g_HighObjIds);
	AcDbDatabase *pCurDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBlockTale=NULL;
	AcDbObjectId recordId;
	AcDbObjectId currentSpaceId;
	currentSpaceId=pCurDb->currentSpaceId();
	if(pCurDb->getBlockTable(pBlockTale,AcDb::kForRead)!=Acad::eOk)
	{ 
		return Acad::eOk;
	}
	pBlockTale->getAt(ACDB_MODEL_SPACE,recordId);
	pBlockTale->close();
	pBlockTale=NULL;
	if(recordId!=currentSpaceId)
	{ 
		return Acad::eOk;
	}
	TCHAR *mtooltipStr;
	//mtooltipStr[0] = '\0';
	int length = apertureEntities.length();
	if(length<1)
	{ 
		return Acad::eOk; 
	}
	acDocManager->lockDocument(curDoc());
	AcDbObjectId DimId=apertureEntities.at(0);
	CString strLayerName=GetEntLayerName(DimId); 
	g_HighObjIds.setLogicalLength(0);
	CString strValue; 
	//CDistLinkDict dict;
	//AcDbObjectIdArray subIds;
	//AcDbObjectIdArray outIds;

	g_HighObjIds.append(DimId); 
	HightLightEnt(g_HighObjIds);
	if(g_hatch!=NULL)
	{
		drawContext->geometry().draw(g_hatch);
		delete g_hatch;
		g_hatch=NULL;
	}  
	acDocManager->unlockDocument(curDoc()); 
	CString strNew;
	strNew.AppendFormat(_T("\nCAD好帮手属性预览"),strLayerName);
	strNew.AppendFormat(_T("\nCAD工具之家(www.cadgj.com)"),strLayerName);
	AcDbEntityPointer spEnt(DimId,AcDb::kForRead);
	if(spEnt.openStatus()==Acad::eOk)
	{
		if(IsPropOpen(_T("EntType")))
	{
		//实体类型
		CString strAlias=GetTypeAlias(spEnt->isA()->name());
		strNew.AppendFormat(_T("\n类型: %s"),strAlias);

	}
	if(IsPropOpen(_T("Layer")))
	{
		strNew.AppendFormat(_T("\n图层: %s"),strLayerName);
	}
		if(spEnt->isKindOf(AcDbCurve::desc()))
		{
			AcDbCurve* pCurve=AcDbCurve::cast(spEnt.object());
			if(IsPropOpen(_T("Length")))
			{
				double dStart=0.0;
				double dEnd=0.0;
				pCurve->getStartParam(dStart);
				pCurve->getEndParam(dEnd);
				double dist=0.0;
				pCurve->getDistAtParam(dEnd,dist);
				strNew.AppendFormat(_T("\n长度: %5.2f"),dist);
			}
			if(IsPropOpen(_T("Area")))
			{
				double dArea=0.0;
				pCurve->getArea(dArea);
				strNew.AppendFormat(_T("\n面积: %5.2f"),dArea);
			}
			
		}
		else if(spEnt->isKindOf(AcDbHatch::desc()))
		{
			if(IsPropOpen(_T("HatchName")))
			{
				AcDbHatch* pHatch=AcDbHatch::cast(spEnt.object());
				strNew.AppendFormat(_T("\n填充名称: %s"),pHatch->patternName());
			}
		}
		else if(spEnt->isKindOf(AcDbBlockReference::desc()))
		{
			if(IsPropOpen(_T("BlockName")))
			{
				CString strBlockName=GetBlockName(AcDbBlockReference::cast(spEnt.object()));
				strNew.AppendFormat(_T("\n块名称: %s"),strBlockName);
			}
			
		}
		else if(spEnt->isKindOf(AcDbText::desc())||
			spEnt->isKindOf(AcDbMText::desc()))
		{
			if(IsPropOpen(_T("FontStyle")))
			{
				AcDbObjectId styleId;
				if(spEnt->isKindOf(AcDbText::desc()))
				{
					styleId=AcDbText::cast(spEnt.object())->textStyle();
				}
				else
				{
					styleId=AcDbMText::cast(spEnt.object())->textStyle();
				}
				CString strName=_T("未知");
				AcDbTextStyleTableRecordPointer spRec(styleId,AcDb::kForRead);
				if(spRec.openStatus()==Acad::eOk)
				{
					const TCHAR* lpName=NULL;
					spRec->getName(lpName);
					strName=lpName;
				}
				strNew.AppendFormat(_T("\n文字样式: %s"),strName);
			}
			

		}
		
	}

	
	strNew.TrimRight(); 
	strTip=strNew;
	appendToTooltipStr = true;
	acutNewString(strTip,additionalTooltipString);
	return Acad::eOk;
}
#else
Acad::ErrorStatus AsdkDynViewInputPoint::monitorInputPoint(const AcEdInputPoint& input, AcEdInputPointMonitorResult& output)
{
	if(bMonitorInputPointFirstTime)
	{
		GLog::WriteLine(_T("AsdkDynViewInputPoint::monitorInputPoint"));
		bMonitorInputPointFirstTime=FALSE;
	}
	//acutPrintf(_T("(%f,%f)\n"),rawPoint.x,rawPoint.y);
	//return Acad::eOk;
	AcDbEntity *pEnt=NULL; 
	//	if(g_nSoftType!=-1)
	//		return Acad::eOk;
	UnHightEnt(g_HighObjIds);
	AcDbDatabase *pCurDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBlockTale=NULL;
	AcDbObjectId recordId;
	AcDbObjectId currentSpaceId;
	currentSpaceId=pCurDb->currentSpaceId();
	if(pCurDb->getBlockTable(pBlockTale,AcDb::kForRead)!=Acad::eOk)
	{ 
		return Acad::eOk;
	}
	pBlockTale->getAt(ACDB_MODEL_SPACE,recordId);
	pBlockTale->close();
	pBlockTale=NULL;
	if(recordId!=currentSpaceId)
	{ 
		return Acad::eOk;
	}
	TCHAR *mtooltipStr;
	//mtooltipStr[0] = '\0';
	int length = input.pickedEntities().length();
	if(length<1)
	{ 
		return Acad::eOk; 
	}
	acDocManager->lockDocument(curDoc());
	AcDbObjectId DimId=input.pickedEntities().at(0);
	CString strLayerName=GetEntLayerName(DimId); 
	g_HighObjIds.setLogicalLength(0);
	CString strValue; 
	//CDistLinkDict dict;
	//AcDbObjectIdArray subIds;
	//AcDbObjectIdArray outIds;

	g_HighObjIds.append(DimId); 
	HightLightEnt(g_HighObjIds);
	if(g_hatch!=NULL)
	{
		input.drawContext()->geometry().draw(g_hatch);
		delete g_hatch;
		g_hatch=NULL;
	}  
	acDocManager->unlockDocument(curDoc()); 
	CString strNew;
	strNew.AppendFormat(_T("\nCAD好帮手属性预览"),strLayerName);
	strNew.AppendFormat(_T("\nCAD工具之家(www.cadgj.com)"),strLayerName);
	AcDbEntityPointer spEnt(DimId,AcDb::kForRead);
	if(spEnt.openStatus()==Acad::eOk)
	{
		if(IsPropOpen(_T("EntType")))
	{
		//实体类型
		CString strAlias=GetTypeAlias(spEnt->isA()->name());
		strNew.AppendFormat(_T("\n类型: %s"),strAlias);

	}
	if(IsPropOpen(_T("Layer")))
	{
		strNew.AppendFormat(_T("\n图层: %s"),strLayerName);
	}
		if(spEnt->isKindOf(AcDbCurve::desc()))
		{
			AcDbCurve* pCurve=AcDbCurve::cast(spEnt.object());
			if(IsPropOpen(_T("Length")))
			{
				double dStart=0.0;
				double dEnd=0.0;
				pCurve->getStartParam(dStart);
				pCurve->getEndParam(dEnd);
				double dist=0.0;
				pCurve->getDistAtParam(dEnd,dist);
				strNew.AppendFormat(_T("\n长度: %5.2f"),dist);
			}
			if(IsPropOpen(_T("Area")))
			{
				double dArea=0.0;
				pCurve->getArea(dArea);
				strNew.AppendFormat(_T("\n面积: %5.2f"),dArea);
			}
			
		}
		else if(spEnt->isKindOf(AcDbHatch::desc()))
		{
			if(IsPropOpen(_T("HatchName")))
			{
				AcDbHatch* pHatch=AcDbHatch::cast(spEnt.object());
				strNew.AppendFormat(_T("\n填充名称: %s"),pHatch->patternName());
			}
		}
		else if(spEnt->isKindOf(AcDbBlockReference::desc()))
		{
			if(IsPropOpen(_T("BlockName")))
			{
				CString strBlockName=GetBlockName(AcDbBlockReference::cast(spEnt.object()));
				strNew.AppendFormat(_T("\n块名称: %s"),strBlockName);
			}
			
		}
		else if(spEnt->isKindOf(AcDbText::desc())||
			spEnt->isKindOf(AcDbMText::desc()))
		{
			if(IsPropOpen(_T("FontStyle")))
			{
				AcDbObjectId styleId;
				if(spEnt->isKindOf(AcDbText::desc()))
				{
					styleId=AcDbText::cast(spEnt.object())->textStyle();
				}
				else
				{
					styleId=AcDbMText::cast(spEnt.object())->textStyle();
				}
				CString strName=_T("未知");
				AcDbTextStyleTableRecordPointer spRec(styleId,AcDb::kForRead);
				if(spRec.openStatus()==Acad::eOk)
				{
					const TCHAR* lpName=NULL;
					spRec->getName(lpName);
					strName=lpName;
				}
				strNew.AppendFormat(_T("\n文字样式: %s"),strName);
			}
			

		}
		
	}

	
	strNew.TrimRight(); 
	strTip=strNew;
	output.appendToTooltipStr();
	output.setAdditionalTooltipString(strTip);
	return Acad::eOk;
}
#endif

bool AsdkDynViewInputPoint::excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity,
														int gsSelectionMark)
{
	return false;
}
CString AsdkDynViewInputPoint::GetTypeAlias(const CString strName) const
{
	for(int i=0;i<entTypeNameArray.GetCount();i++)
	{
		if(strName.CompareNoCase(entTypeNameArray.GetAt(i))==0)
		{
			return entTypeAliasArray.GetAt(i);
		}
	}
	return strName;
}
