#include "StdAfx.h"
#include "CommCommand.h"
#include "AcCommFunction.h"
#include "GLockDocument.h"
#include "SortArray.h"
#include "IEHelper.h"
#include "GHighlightHelper.h"
#include "GSimpleSelDialog.h"
#include "GMousePreviewDialog.h"
#include "GRegDialog.h"
#include "GxAfxFunction.h"
#include "GAcGroupFunction.h"
Acad::ErrorStatus SPlineToPLine(AcDbSpline *spline)
{
	AcGePoint3dArray pnts;
	bool rc=getSplineSamplePoints(spline,pnts);
	if(!rc)
		return Acad::eInvalidInput;
	RemovePointBetweenLine(pnts);
	AcDbPolyline* pLine=new AcDbPolyline(pnts.logicalLength());
	for(int i=0;i<pnts.logicalLength();i++)
	{
		pLine->addVertexAt(i,convert3dPointTo2d(pnts.at(i)));
	}
	SameEntityAttr(pLine,spline);
	AppendEntity(pLine,spline->blockId());
	pLine->close();
	return Acad::eOk;
}
Acad::ErrorStatus SPlineToPLine(AcDbObjectId id)
{
	AcDbObjectPointer<AcDbSpline> spLine(id,AcDb::kForRead);
	if(spLine.openStatus()!=Acad::eOk)
		return spLine.openStatus();
	Acad::ErrorStatus es=SPlineToPLine(spLine.object());
	if(es!=Acad::eOk)
	{
		return es;
	}
	es=spLine->upgradeOpen();
	if(es!=Acad::eOk)
	{
		return es;
	}
	spLine->erase();
	return Acad::eOk;

}
void SPlineToPLine()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("SPLINE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择样条曲线:"),_T("\n移除样条曲线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	AcDbObjectId id;
	for(int i=0;i<ids.logicalLength();i++)
	{
		id=ids.at(i);
		SPlineToPLine(id);
	}
}

bool calcArea(double& dArea,AcDbObjectId ObjId)
{
	AcDbEntity* pEnt = NULL;
	if(Acad::eOk != acdbOpenObject(pEnt,ObjId,AcDb::kForRead))
	{
		return false;
	}

	// 过滤实体类型，只处理从AcDbCurve类继承的子类               
	if( !(pEnt->isKindOf(AcDb2dPolyline::desc())) && 
		!(pEnt->isKindOf(AcDb3dPolyline::desc())) && 
		!(pEnt->isKindOf(AcDbArc::desc())) && 
		!(pEnt->isKindOf(AcDbCircle::desc())) && 
		!(pEnt->isKindOf(AcDbEllipse::desc())) && 
		!(pEnt->isKindOf(AcDbLeader::desc())) && 
		!(pEnt->isKindOf(AcDbLine::desc())) && 
		!(pEnt->isKindOf(AcDbPolyline::desc())) && 
		!(pEnt->isKindOf(AcDbRay::desc())) && 
		!(pEnt->isKindOf(AcDbSpline::desc())) && 
		!(pEnt->isKindOf(AcDbXline::desc())) )
	{
		pEnt->close();
		return false;
	}

	AcDbCurve* pCurve = NULL;
	if(pEnt->isKindOf(AcDb2dPolyline::desc()) || pEnt->isKindOf(AcDb3dPolyline::desc()))
	{
		AcDbSpline* spline = NULL;
		pEnt->handOverTo(spline);
		pCurve = (AcDbCurve*) spline;
	}
	else
		pCurve = (AcDbCurve*)pEnt; 

	if (NULL == pCurve)
	{
		pEnt->close();
		return false;
	}
	if(!pCurve->isClosed())
	{
		pEnt->close();
		return false;
	}

	// 计算面积
	double dTemp = 0.0;
	pCurve->getArea(dTemp);
	dArea=dTemp;
	pCurve->close();
	return true;
}
bool calcLength(double& dLen,AcDbObjectId ObjId)
{
	AcDbEntity* pEnt = NULL;
	if(Acad::eOk != acdbOpenObject(pEnt,ObjId,AcDb::kForRead))
	{
		return false;
	}

	// 过滤实体类型，只处理从AcDbCurve类继承的子类               
	if( !(pEnt->isKindOf(AcDb2dPolyline::desc())) && 
		!(pEnt->isKindOf(AcDb3dPolyline::desc())) && 
		!(pEnt->isKindOf(AcDbArc::desc())) && 
		!(pEnt->isKindOf(AcDbCircle::desc())) && 
		!(pEnt->isKindOf(AcDbEllipse::desc())) && 
		!(pEnt->isKindOf(AcDbLeader::desc())) && 
		!(pEnt->isKindOf(AcDbLine::desc())) && 
		!(pEnt->isKindOf(AcDbPolyline::desc())) && 
		!(pEnt->isKindOf(AcDbRay::desc())) && 
		!(pEnt->isKindOf(AcDbSpline::desc())) && 
		!(pEnt->isKindOf(AcDbXline::desc())) )
	{
		pEnt->close();
		return false;
	}

	AcDbCurve* pCurve = NULL;
	if(pEnt->isKindOf(AcDb2dPolyline::desc()) || pEnt->isKindOf(AcDb3dPolyline::desc()))
	{
		AcDbSpline* spline = NULL;
		pEnt->handOverTo(spline);
		pCurve = (AcDbCurve*) spline;
	}
	else
		pCurve = (AcDbCurve*)pEnt; 

	if (NULL == pCurve)
	{
		pEnt->close();
		return false;
	}

	// 计算面积
	double dTemp = 0.0;
	double dEndParam;
	pCurve->getEndParam(dEndParam);
	pCurve->getDistAtParam(dEndParam,dTemp);
	dLen=dTemp;
	pCurve->close();
	return true;
}
//bClipboard:是否将结果拷贝到剪贴板中
void _calcArea(bool bClipboard)
{
	AcDbObjectId objId;
	AcDbObjectIdArray objIds;
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择统计面积的闭合曲线:"),_T("\n移除统计面积的闭合曲线:")};
	while (RTNORM == acedSSGet(_T(":$:S"),promptPtrs,NULL,NULL,ssName))//每次只选取一个，如果之前已有选择集包含多个实体，此处将得到多个实体
	{
		int nCount1=objIds.logicalLength();
		AcDbObjectIdArray objIdsSwap;
		SSToIds(objIdsSwap,ssName);
		acedSSFree(ssName);
		int nCount2=objIdsSwap.logicalLength();
		for (int i = 0; i < objIdsSwap.logicalLength(); i++)
		{
			int nFind = -1;
			objIds.find(objIdsSwap.at(i), nFind);
			if (nFind < 0)
			{
				objIds.append(objIdsSwap.at(i));
				AcDbEntity *pEnt = NULL;
				if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIdsSwap.at(i), AcDb::kForWrite))
				{
					pEnt->highlight();
				}
				if (pEnt != NULL)
				{
					pEnt->close();
					pEnt = NULL;
				}
			}
		}
		int nCount3=objIds.logicalLength();
		if(nCount3>nCount1)
		{
			if(nCount3==nCount1+nCount2)
			{
				//全部都是新选择的对象
				acutPrintf(_T("\n选择了%d个对象，总共选择了%d个对象"),nCount2,nCount3);
			}
			else
			{
				//有不是新选择的对象
				acutPrintf(_T("\n选择了%d个对象(%d个重复)，总共选择了%d个对象"),nCount2,nCount1+nCount2-nCount3,nCount3);
			}
		}
	}


	if(objIds.logicalLength()<=0)
		return;//不创建图层
	double dArea=0.0;
	int nCount = 0;
	for (int i = 0; i < objIds.logicalLength(); i++)
	{
		double dTmp=0.0;
		if(!calcArea(dTmp,objIds.at(i)))
			continue;
		dArea+=dTmp;
		nCount++;
	}
	if(bClipboard)
	{
		acutPrintf(_T("\n闭合曲线共%d条,共计面积%.3f平方米(约%.1f亩),已将\"%.3f\"复制到剪贴板"),nCount,dArea,dArea*3.0/2000.0,dArea);
		CString strText;
		strText.Format(_T("%.3f"),dArea);
		CopyTextToClipboard(strText);
	}
	else
	{
		acutPrintf(_T("\n闭合曲线共%d条,共计面积%.3f平方米(约%.1f亩)"),nCount,dArea,dArea*3.0/2000.0);
	}

	//使所有高亮的对象不高亮显示
	for (int i = 0; i < objIds.logicalLength(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIds.at(i), AcDb::kForWrite))
		{
			pEnt->unhighlight();
		}
		if (pEnt != NULL)
		{
			pEnt->close();
			pEnt = NULL;
		}
	}
}
void calcArea()
{
	_calcArea(false);
}
void calcArea1()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	_calcArea(true);
}
//长度统计
void _calcLength(bool bClipboard)
{
	AcDbObjectId objId;
	AcDbObjectIdArray objIds;
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择统计长度的曲线:"),_T("\n移除统计长度的曲线:")};
	while (RTNORM == acedSSGet(_T(":$:S"),promptPtrs,NULL,NULL,ssName))//每次只选取一个，如果之前已有选择集包含多个实体，此处将得到多个实体
	{
		int nCount1=objIds.logicalLength();
		AcDbObjectIdArray objIdsSwap;
		SSToIds(objIdsSwap,ssName);
		acedSSFree(ssName);
		int nCount2=objIdsSwap.logicalLength();
		for (int i = 0; i < objIdsSwap.logicalLength(); i++)
		{
			int nFind = -1;
			objIds.find(objIdsSwap.at(i), nFind);
			if (nFind < 0)
			{
				objIds.append(objIdsSwap.at(i));
				AcDbEntity *pEnt = NULL;
				if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIdsSwap.at(i), AcDb::kForWrite))
				{
					pEnt->highlight();
				}
				if (pEnt != NULL)
				{
					pEnt->close();
					pEnt = NULL;
				}
			}
		}
		int nCount3=objIds.logicalLength();
		if(nCount3>nCount1)
		{
			if(nCount3==nCount1+nCount2)
			{
				//全部都是新选择的对象
				acutPrintf(_T("\n选择了%d个对象，总共选择了%d个对象"),nCount2,nCount3);
			}
			else
			{
				//有不是新选择的对象
				acutPrintf(_T("\n选择了%d个对象(%d个重复)，总共选择了%d个对象"),nCount2,nCount1+nCount2-nCount3,nCount3);
			}
		}
	}


	if(objIds.logicalLength()<=0)
		return;//不创建图层
	double dLen=0.0;
	int nCount = 0;
	for (int i = 0; i < objIds.logicalLength(); i++)
	{
		double dTmp=0.0;
		if(!calcLength(dTmp,objIds.at(i)))
			continue;
		dLen+=dTmp;
		nCount++;
	}
	if(bClipboard)
	{
		acutPrintf(_T("\n曲线共%d条,共计长度%.3f米,已将\"%.3f\"复制到剪贴板"),nCount,dLen,dLen,dLen/10.0,dLen/1000.0);
		CString strText;
		strText.Format(_T("%.3f"),dLen);
		CopyTextToClipboard(strText);
	}
	else
	{
		acutPrintf(_T("\n曲线共%d条,共计长度%.3f米"),nCount,dLen,dLen/10.0,dLen/1000.0);
	}

	//使所有高亮的对象不高亮显示
	for (int i = 0; i < objIds.logicalLength(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIds.at(i), AcDb::kForWrite))
		{
			pEnt->unhighlight();
		}
		if (pEnt != NULL)
		{
			pEnt->close();
			pEnt = NULL;
		}
	}
}
//长度统计
void calcLength()
{
	_calcLength(false);
}
void calcLength1()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	_calcLength(true);
}

CArray<GColorRecord> colorRecordArr;
//恢复原来颜色
void SameColorRollback()
{
	GColorRecord::Rollback(colorRecordArr);
}
//统一为黑白色
void AllBlack()
{
	colorRecordArr.RemoveAll();
	GSameColorHelper helper;
	helper.SetAllEntityInSameColor(colorRecordArr,7);

}
//统一为选择的颜色
void AllSelColor()
{
	int nColorIndex=7;
	if(acedSetColorDialog(nColorIndex,true,256)!=Adesk::kTrue)
	{
		return;
	}
	colorRecordArr.RemoveAll();
	GSameColorHelper helper;

	helper.SetAllEntityInSameColor(colorRecordArr,nColorIndex);

}
//选择对象变为黑白色
void SelectBlack()
{
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择变为黑白的对象:"),_T("\n移除变为黑白的对象:")};
	int rc=acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName);
	if(rc!=RTNORM)
		return;
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	colorRecordArr.RemoveAll();
	GSameColorHelper helper;

	helper.SetEntityInSameColor(colorRecordArr,ids,7);
}
//选择对象转换为选择的颜色
void SelSelColor()
{
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择改变颜色的对象:"),_T("\n移除改变颜色的对象:")};
	int rc=acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName);
	if(rc!=RTNORM)
		return;
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);

	int nColorIndex=7;
	if(acedSetColorDialog(nColorIndex,true,256)!=Adesk::kTrue)
	{
		return;
	}
	colorRecordArr.RemoveAll();
	GSameColorHelper helper;

	helper.SetEntityInSameColor(colorRecordArr,ids,nColorIndex);
}
void RemoveSamePtArr(AcGePoint3dArray &ptArr,AcGeDoubleArray &dBulges)
{
	if(ptArr.logicalLength()<2)
		return;
	for(int nLen=1;nLen<ptArr.logicalLength();nLen++)
	{
		if(ptArr[nLen].distanceTo(ptArr[nLen-1])<0.001)
		{
			ptArr.removeAt(nLen);
			dBulges.removeAt(nLen);
			nLen--;
			continue;
		}
	}
}
//自相交检测
void SelfIntersect()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多段线:"),_T("\n移除多段线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	AcDbObjectId id;
	for(int i=0;i<ids.logicalLength();i++)
	{
		id=ids.at(i);
		AcGePoint3dArray VertPnts;
		AcGeDoubleArray VertBlgs;
		AcGePoint2dArray ptBacks;

		GetEntVertPntsAndBlgs(id,VertPnts,VertBlgs);
		RemoveSamePtArr(VertPnts,VertBlgs);
		if(!JudgeCureIsSelInterSect(VertPnts,VertBlgs, ptBacks)) //自相交
		{
			continue;
		}
		AcGePoint2d ptCen = ptBacks.at(0);

		AcDbEntity* pEnt = NULL;
		es=acdbOpenObject(pEnt,id,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		AcDbExtents exten;
		pEnt->getGeomExtents(exten);
		pEnt->close();

		double dWidth = (exten.maxPoint().x-exten.minPoint().x);
		double dHeight = (exten.maxPoint().y-exten.minPoint().y);

		if ((dWidth >= 0.001) && (dHeight >= 0.001))
		{
			dWidth /= 20;
			dHeight /= 20;
		}
		if ((dWidth >= 20) || (dHeight >= 20))
		{
			dWidth = dHeight = 20;
		}

		CString strErr;
		strErr.Format(_T("该多段线在点%.4f, %.4f处有自相交。"), ptCen.x, ptCen.y);
		acutPrintf(_T("\n%s\n"), strErr);

		DyZoom(ptCen,dHeight,dWidth);
	}
}

//选中同类对象
void SelSameEntity()
{
	// 支持多选
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择对象:"),_T("\n移除对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	AcDbObjectIdArray ids;
	EnumAllEntityInCurrentSpace(ids);
	AcDbObjectIdArray sel_ids;
	int i=0;
	AcArray<AcRxClass*> class_array;
	for(i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(EntIds.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!class_array.contains(spEnt->isA()))
		{
			class_array.append(spEnt->isA());
		}

	}
	for(i=0;i<ids.logicalLength();i++)
	{
		if(EntIds.contains(ids.at(i)))
		{
			sel_ids.append(ids.at(i));
			continue;
		}
		AcDbEntityPointer spEnt1(ids.at(i),AcDb::kForRead);
		if(spEnt1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!class_array.contains(spEnt1->isA()))
		{
			continue;
		}
		AcDbLayerTableRecordPointer spLayer(spEnt1->layerId(),AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spLayer->isOff()||spLayer->isFrozen())
		{
			continue;
		}
		sel_ids.append(ids.at(i));


	}
	SetEntitySelected(sel_ids,true);
}
//删除同类对象
void DelSameEntity()
{
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择对象:"),_T("\n移除对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	TCHAR lpResult[256+1]={0};
	int rc=acedGetString(0,_T("\n是否删除块定义中的同类实体,Y/N:"),lpResult);
	if(rc!=RTNORM)
		return;
	bool bInBlock=false;
	if(lstrcmpi(lpResult,_T("Y"))==0)
	{
		bInBlock=true;
	}
	AcDbObjectIdArray ids;
	if(!bInBlock)
	{
		EnumAllEntityInCurrentSpace(ids);
	}
	else
	{
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

		if (NULL == pDb)
			return;
		AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
		Acad::ErrorStatus es=spBTR.openStatus();
		if(es!=eOk)
			return;
		AcDbBlockTableIterator* pIterator=NULL;
		spBTR->newIterator(pIterator);
		if(pIterator==NULL)
		{
			return;
		}
		for(pIterator->start();(!pIterator->done());pIterator->step())
		{
			AcDbBlockTableRecord* pBTR=NULL;
			if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
				continue;
			EnumAllEntityInBTR(ids,pBTR);
			pBTR->close();
			pBTR=NULL;

		}
		delete pIterator;
		pIterator=NULL;
	}
	AcDbObjectIdArray sel_ids;
	int i=0;
	AcArray<AcRxClass*> class_array;
	for(i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(EntIds.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!class_array.contains(spEnt->isA()))
		{
			class_array.append(spEnt->isA());
		}

	}
	for(i=0;i<ids.logicalLength();i++)
	{
		if(EntIds.contains(ids.at(i)))
		{
			sel_ids.append(ids.at(i));
			continue;
		}
		AcDbEntityPointer spEnt1(ids.at(i),AcDb::kForRead);
		if(spEnt1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!class_array.contains(spEnt1->isA()))
		{
			continue;
		}
		AcDbLayerTableRecordPointer spLayer(spEnt1->layerId(),AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spLayer->isOff()||spLayer->isFrozen())
		{
			continue;
		}
		sel_ids.append(ids.at(i));
	}
	int nCount=0;
	for(i=0;i<sel_ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(sel_ids.at(i),AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->erase();
		nCount++;
	}
	acutPrintf(_T("\n删除同类对象%d个"),nCount);
}
//官网CAD工具之家
void AssistantWebSite()
{
	GotoURL(_T("http://www.cadgj.com/"));
}
//论坛求助
void BBSSeekhelp()
{
	GotoURL(_T("http://www.cadgj.com/GoodAssistant/forum.php?mod=forumdisplay&fid=41"));
}
//报告错误
void AssistantError()
{
	GotoURL(_T("http://www.cadgj.com/GoodAssistant/forum.php?mod=forumdisplay&fid=43"));
}
//功能建议
void AssistantSuggest()
{
	GotoURL(_T("http://www.cadgj.com/GoodAssistant/forum.php?mod=forumdisplay&fid=42"));
}
//使用说明
void AssistantDoc()
{
	//打开pdf
	TCHAR lpFilePdf[MAX_PATH+1];
	GetModuleFileName(_hdllInstance, lpFilePdf, MAX_PATH);
	PathRemoveFileSpec(lpFilePdf);
	PathRemoveFileSpec(lpFilePdf);
	PathAppend(lpFilePdf,_T("Doc\\CAD好帮手.pdf"));
	if(PathFileExists(lpFilePdf))
	{
		HINSTANCE hi=ShellOpenFile(_T(".pdf"),lpFilePdf);
		if((UINT)hi>HINSTANCE_ERROR)
		{
			return;
		}
	}
	//打开WORD
	TCHAR lpFileDoc[MAX_PATH+1];
	GetModuleFileName(_hdllInstance, lpFileDoc, MAX_PATH);
	PathRemoveFileSpec(lpFileDoc);
	PathRemoveFileSpec(lpFileDoc);
	PathAppend(lpFileDoc,_T("Doc\\CAD好帮手.doc"));
	if(PathFileExists(lpFileDoc))
	{
		HINSTANCE hi=ShellOpenFile(_T(".doc"),lpFileDoc);
		if((UINT)hi>HINSTANCE_ERROR)
		{
			return;
		}
		hi=ShellOpenFile(_T(".docx"),lpFileDoc);
		if((UINT)hi>HINSTANCE_ERROR)
		{
			return;
		}
	}
	if(AfxMessageBox(_T("未找到任何pdf或者doc阅读器，无法打开本地帮助，是否打开在线帮助?"),MB_YESNO)!=IDYES)
	{
		return;
	}
	GotoURL(_T("http://www.cadgj.com/?page_id=186"));
}
//最新版本
void AssistantUpdate()
{
	CallUpdate(false);
}
//块统计
void CountBlock(int& nCount,AcDbBlockReference* pRef,AcDbObjectId blockId)
{
	AcDbBlockTableRecordPointer spRec(pRef->blockTableRecord(),AcDb::kForRead);
	if(spRec.openStatus()!=Acad::eOk)
		return;
	AcDbBlockTableRecordIterator* pIterator=NULL;
	Acad::ErrorStatus es=spRec->newIterator(pIterator);
	if(es!=Acad::eOk)
		return;
	if(pIterator==NULL)
		return;
	for(pIterator->start();!pIterator->done();pIterator->step())
	{
		AcDbEntity* pEnt=NULL;
		es=pIterator->getEntity(pEnt,AcDb::kForRead);
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef1=AcDbBlockReference::cast(pEnt);
			if(pRef1->blockTableRecord()==blockId)
			{
				nCount++;
			}
			else
			{
				CountBlock(nCount,pRef1,blockId);
			}
		}
		pEnt->close();
		pEnt=NULL;
	}
	delete pIterator;
	pIterator=NULL;
}
void FlashEntity(AcDbObjectId ObjId) 
{ 
	AcDbEntity *pEnt=NULL; 
	AcTransaction* pTrans=actrTransactionManager->startTransaction();
	Acad::ErrorStatus es;
	if((es = acdbOpenAcDbEntity(pEnt,ObjId,AcDb::kForWrite)) != Acad::eOk )
	{
		acutPrintf(_T("\n Error:%s!"),acadErrorStatusText(es));
		actrTransactionManager->endTransaction();
		return; 
	}
	pEnt->highlight(); 
	acedUpdateDisplay();


	DWORD dwCount=GetTickCount(); 
	while(1)
	{
		if(GetTickCount()-dwCount>=100) 
			break;
		Sleep(10);
	}
	pEnt->unhighlight(); 
	acedUpdateDisplay();

	dwCount=GetTickCount(); 
	while(1)
	{
		if(GetTickCount()-dwCount>=100) 
			break;  
		Sleep(10);
	}
	pEnt->highlight(); 
	acedUpdateDisplay();
	dwCount=GetTickCount(); 
	while(1)
	{
		if(GetTickCount()-dwCount>=100) 
			break;  
		Sleep(10);
	}
	pEnt->unhighlight(); 
	acedUpdateDisplay();

	dwCount=GetTickCount(); 
	while(1)
	{
		if(GetTickCount()-dwCount>=100) 
			break;    
		Sleep(10);
	}
	pEnt->highlight(); 
	acedUpdateDisplay();
	dwCount=GetTickCount(); 
	while(1)
	{
		if(GetTickCount()-dwCount>=100) 
			break;    
		Sleep(10);
	}
	pEnt->unhighlight(); 
	acedUpdateDisplay();

	pEnt->close();

	actrTransactionManager->endTransaction();
}
void FlashEntityByColor(AcDbObjectIdArray& ids,int nStartColor=1,int nEndColor=7,int nStep=1,DWORD dwMilliseconds=400)
{
	CArray<GColorRecord> colorRecordArr;
	for(int i=nStartColor;i<nEndColor;i+=nStep)
	{
		GSameColorHelper helper;
		if(i==nStartColor)
		{
			helper.SetEntityInSameColor(colorRecordArr,ids,i);
		}
		else
		{
			CArray<GColorRecord> tmp;
			helper.SetEntityInSameColor(tmp,ids,i);
		}
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		Sleep(dwMilliseconds);
	}
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	Sleep(dwMilliseconds);
	GColorRecord::Rollback(colorRecordArr);
}
void FlashEntityByColor(AcDbObjectId objId,int nStartColor=1,int nEndColor=7,int nStep=1,DWORD dwMilliseconds=400)
{
	AcDbObjectIdArray ids;
	ids.append(objId);
	FlashEntityByColor(ids,nStartColor,nEndColor,nStep,dwMilliseconds);
}
//块统计
void CountBlock()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("INSERT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择统计块:"),_T("\n移除统计块:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()!=1)
	{
		return;
	}
	GHighlightHelper highlightHelper;
	AcDbObjectId id1=ids1.at(0);
	FlashEntityByColor(id1,1,7,1,100);
	highlightHelper.Highlight(id1);
	AcDbObjectPointer<AcDbBlockReference> spRef1(id1,AcDb::kForRead);
	if(spRef1.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId blockId=spRef1->blockTableRecord();
	spRef1->close();
	AcDbBlockTableRecordPointer spRec(blockId,AcDb::kForRead);
	if(spRec.openStatus()!=Acad::eOk)
	{
		return;
	}
	const TCHAR* lpName=NULL;
	spRec->getName(lpName);
	spRec->close();
	acutPrintf(_T("\n您选中的是块\"%s\""),lpName);
	ads_point p1, p2;  //交互矩形点坐标获取
	if (acedGetPoint(NULL, _T("\n>>请框选统计范围,指定第一个角点: "), p1) != RTNORM)
	{
		acutRelRb(rb);
		return;
	}

	if (acedGetCorner(p1, _T("\n>>指定第二个角点: "), p2) != RTNORM)
	{
		acutRelRb(rb);
		return;
	}

	if (p1[0] == p2[0] || p1[1] == p2[1]) //矩形框合法性检查
		return;

	ads_name ss;
	int result = acedSSGet(_T("C"), asDblArray(AcGePoint3d(p1[0], p1[1], p1[2])), asDblArray(AcGePoint3d(p2[0], p2[1], p2[2])), rb, ss);
	acutRelRb(rb);
	AcDbObjectIdArray ids2;
	SSToIds(ids2,ss);
	acedSSFree(ss);
	int nCount=0;
	int nCountInBlock=0;
	AcDbObjectIdArray refIds;
	for(int i=0;i<ids2.logicalLength();i++)
	{
		AcDbObjectPointer<AcDbBlockReference> spRef(ids2.at(i),AcDb::kForRead);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spRef->blockTableRecord()==blockId)
		{
			nCount++;
			refIds.append(ids2.at(i));
		}
		else
		{
			CountBlock(nCountInBlock,spRef.object(),blockId);
		}
	}
	if(nCountInBlock>0)
	{
		acutPrintf(_T("\n共发现块\"%s\"%d个,其中普通块%d个,块中块%d个"),lpName,nCount+nCountInBlock,nCount,nCountInBlock);
	}
	else
	{
		acutPrintf(_T("\n共发现块\"%s\"%d个"),lpName,nCount);
	}
	highlightHelper.UnHighlight(id1);
	DyZoom(refIds,1.1);
	FlashEntityByColor(refIds);
	SetEntitySelected(refIds,true);

}
//删除超短线
void DelShortLine()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE,LINE,ARC"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多段线/直线/圆弧:"),_T("\n移除多段线/直线/圆弧:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()==0)
	{
		return;
	}
	double dShortLine=0.1;
	dShortLine=GUserDataRecord::getInstance()->GetProfileDouble(_T("DelShortLine"),_T("ShortLine"),dShortLine);
	CString strInfo;
	strInfo.Format(_T("\n请输入最小长度<%f>:"),dShortLine);
	double dValue=dShortLine;
	rc=acedGetReal(strInfo,&dValue);
	if(rc!=RTNORM)
	{
		if(rc==RTNONE)
		{
			//用户直接按下Enter键
		}
		else
		{
			return;
		}
	}
	else
	{
		dShortLine=dValue;
	}
	GUserDataRecord::getInstance()->WriteProfileDouble(_T("DelShortLine"),_T("ShortLine"),dShortLine);
	int nCount = 0;
	int nTotalCount=0;
	for (int i = 0; i < ids1.logicalLength(); i++)
	{
		double dTmp=0.0;
		if(!calcLength(dTmp,ids1.at(i)))
			continue;
		if(dTmp>=dShortLine)
		{
			continue;
		}
		nTotalCount++;
		AcDbEntityPointer spEnt(ids1.at(i),AcDb::kForWrite);
		Acad::ErrorStatus es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开实体失败:%s"),acadErrorStatusText(es));
			continue;
		}
		es=spEnt->erase();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n删除实体失败:%s"),acadErrorStatusText(es));
			continue;
		}
		nCount++;
	}
	if(nTotalCount==0)
	{
		acutPrintf(_T("\n没有找到长度小于%f的超短线"),dShortLine);
		return;
	}
	if(nCount==nTotalCount)
	{
		acutPrintf(_T("\n成功删除超短线%d条"),nCount);
	}
	else
	{
		acutPrintf(_T("\n成功删除超短线%d条,失败%d条"),nCount,nTotalCount-nCount);
	}
}
//多段线抽稀
void ThinLine()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	ads_name ssName;
	acutPrintf(_T("\n请选择多段线:\n"));
	TCHAR* promptPtrs[2]={_T("\n选择多段线:"),_T("\n移除多段线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()==0)
	{
		return;
	}
	double dShortLine=0.1;
	dShortLine=GUserDataRecord::getInstance()->GetProfileDouble(_T("ThinLine"),_T("ShortLine"),dShortLine);
	CString strInfo;
	strInfo.Format(_T("\n请输入最小长度<%f>:"),dShortLine);
	double dValue=dShortLine;
	rc=acedGetReal(strInfo,&dValue);
	if(rc!=RTNORM)
	{
		if(rc==RTNONE)
		{
			//用户直接按下Enter键
		}
		else
		{
			return;
		}
	}
	else
	{
		dShortLine=dValue;
	}
	GUserDataRecord::getInstance()->WriteProfileDouble(_T("ThinLine"),_T("ShortLine"),dShortLine);
	double dMinAngle=179.0;
	dMinAngle=GUserDataRecord::getInstance()->GetProfileDouble(_T("ThinLine"),_T("MinAngle"),dMinAngle);
	strInfo.Format(_T("\n请输入最小角度(单位度，在0~180度之间，一般要求接近180度)<%f>:"),dMinAngle);
	dValue=dMinAngle;
	rc=acedGetReal(strInfo,&dValue);
	if(rc!=RTNORM)
	{
		if(rc==RTNONE)
		{
			//用户直接按下Enter键
		}
		else
		{
			return;
		}
	}
	else
	{
		dMinAngle=dValue;
	}
	GUserDataRecord::getInstance()->WriteProfileDouble(_T("ThinLine"),_T("MinAngle"),dMinAngle);
	dMinAngle=dMinAngle*PI/180.0;
	int nCount = 0;
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId id=ids1.at(i);
		AcDbObjectPointer<AcDbPolyline> spLine(ids1.at(i),AcDb::kForWrite);
		Acad::ErrorStatus es=spLine.openStatus();
		if(es!=Acad::eOk)
			continue;
		int nNum=spLine->numVerts();
		for(int j=1;j<nNum-1;j++)
		{
			double bulge=0.0;
			spLine->getBulgeAt(j,bulge);
			if(fabs(bulge)>1E-8)
				continue;//圆弧，不考虑
			AcGePoint3d pt1,pt2,pt3;
			spLine->getPointAt(j-1,pt1);
			spLine->getPointAt(j,pt2);
			spLine->getPointAt(j+1,pt3);
			double dAngle=GetIntersectionAngle(pt1,pt2,pt3);
			double dLen=pt1.distanceTo(pt3);
			if(dAngle>=dMinAngle&&dLen<dShortLine)
			{
				//可以去除该点
				spLine->removeVertexAt(j);
				nNum-=1;
				j-=1;
				nCount++;
			}
		}
	}
	acutPrintf(_T("\n成功多余点%d个"),nCount);


}
//多段线删点
void RemovePLinePoint()
{
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	ads_point p1, p2;  //交互矩形点坐标获取
	if (acedGetPoint(NULL, _T("\n>>请框选删除点的范围,指定第一个角点: "), p1) != RTNORM)
	{
		acutRelRb(rb);
		return;
	}

	if (acedGetCorner(p1, _T("\n>>指定第二个角点: "), p2) != RTNORM)
	{
		acutRelRb(rb);
		return;
	}

	if (p1[0] == p2[0] || p1[1] == p2[1]) //矩形框合法性检查
		return;

	ads_name ss;
	int result = acedSSGet(_T("C"), asDblArray(AcGePoint3d(p1[0], p1[1], p1[2])), asDblArray(AcGePoint3d(p2[0], p2[1], p2[2])), rb, ss);
	acutRelRb(rb);
	AcDbObjectIdArray ids2;
	SSToIds(ids2,ss);
	acedSSFree(ss);
	AcGePoint3d pt1(p1[X],p1[Y],p1[Z]);
	AcGePoint3d pt2(p2[X],p2[Y],p2[Z]);
	AcDbExtents ext;
	ext.addPoint(pt1);
	ext.addPoint(pt2);
	int nCount = 0;
	int nDelLine=0;
	for(int i=0;i<ids2.logicalLength();i++)
	{
		BOOL bDel=FALSE;
		AcDbObjectId id=ids2.at(i);
		AcDbObjectPointer<AcDbPolyline> spLine(id,AcDb::kForWrite);
		Acad::ErrorStatus es=spLine.openStatus();
		if(es!=Acad::eOk)
			continue;
		int nNum=spLine->numVerts();
		for(int j=0;j<=nNum-1;j++)
		{
			AcGePoint3d pt3;
			spLine->getPointAt(j,pt3);
			if(!IsPointInExtents(pt3,ext))
				continue;
			if(nNum<=2)
				continue;//多段线不建议小于两个点
			//可以去除该点
			spLine->removeVertexAt(j);
			nNum-=1;
			j-=1;
			nCount++;
			bDel=TRUE;
		}
		if(bDel)
		{
			nDelLine++;
		}
	}
	acutPrintf(_T("\n成功%d条多段线上的点%d个"),nDelLine,nCount);
}
//Z轴规零
void ZToZero()
{
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择对象:"),_T("\n移除对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	for(int i=0;i<EntIds.logicalLength();i++)
	{
		AcDbObjectId id=EntIds.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
			continue;
		AcGeMatrix3d mat;
		mat.setToTranslation(AcGeVector3d(0,0,-1E99));
		spEnt->transformBy(mat);
		mat.setToTranslation(AcGeVector3d(0,0,1E99));
		spEnt->transformBy(mat);
	}
}
/*
extern void ReRegCmds();
//30:命令别名
void AssistantCommandAlias()
{
	CAcModuleResourceOverride res;
	GCommandAliasDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
	acedRegCmds->removeGroup(_T("CADCommTools"));
	ReRegCmds();
	}
}
*/
void ReVerse(AcGePoint3dArray &VertPnts,AcGeDoubleArray &VertBlgs)
{
	AcGePoint3dArray ptArr;
	AcGeDoubleArray ptBlgs;
	if(VertPnts.logicalLength()==2)
	{
		AcGePoint3d ptAt=VertPnts[0];
		VertPnts[0]=VertPnts[1];
		VertPnts[1]=ptAt;
		VertBlgs[0]=-VertBlgs[0];
		return;
	}
	for(int nLen=VertPnts.logicalLength()-1;nLen>0;nLen--)
	{
		ptArr.append(VertPnts[nLen]);
		ptBlgs.append(-VertBlgs[nLen-1]);
	}
	ptArr.append(VertPnts[0]);
	ptBlgs.append(-VertBlgs[VertPnts.logicalLength()-1]);
	VertPnts.setLogicalLength(0);
	VertBlgs.setLogicalLength(0);
	VertPnts.append(ptArr);
	VertBlgs.append(ptBlgs);
}
BOOL AddPoints(AcGePoint3dArray &pointarr,AcGeDoubleArray &blgsarr,
			   AcGePoint3dArray &lastpointarr,AcGeDoubleArray &lastblgsarr)
{
	double dTool=0.00;
	if(lastpointarr.logicalLength()<2)
	{
		lastpointarr.append(pointarr);
		lastblgsarr.append(blgsarr);
		return TRUE;
	}
	else 
	{
		AcGePoint3d ptStart=pointarr[0];
		AcGePoint3d ptEnd=pointarr[1];
		AcGePoint3d ptLastStart=lastpointarr[0];
		AcGePoint3d ptLastEnd=lastpointarr[lastpointarr.logicalLength()-1];
		int nType=0;//0 是尾-手 1：尾-尾 2：首-尾 3：首-首
		double dMinDist=ptLastEnd.distanceTo(ptStart);
		if(ptLastEnd.distanceTo(ptEnd)<dMinDist)  //
		{
			dMinDist=ptLastEnd.distanceTo(ptEnd); 
			nType=1;
		}
		if(ptLastStart.distanceTo(ptEnd)<dMinDist)
		{
			dMinDist=ptLastStart.distanceTo(ptEnd);
			nType=2;
		}
		if(ptLastStart.distanceTo(ptStart)<dMinDist)
		{
			dMinDist=ptLastStart.distanceTo(ptStart);
			nType=3;
		}
		if(dMinDist>1E-6)
		{
			return FALSE;
		}
		if(nType==0) // 尾-手 
		{
			lastpointarr.removeLast();
			lastblgsarr.removeLast();
			lastpointarr.append(pointarr);
			lastblgsarr.append(blgsarr);
		}
		else if(nType==1)//尾-尾
		{
			ReVerse(pointarr,blgsarr); 
			lastpointarr.removeLast();
			lastblgsarr.removeLast();
			lastpointarr.append(pointarr);
			lastblgsarr.append(blgsarr);
		}
		else if(nType==2) //首-尾
		{
			pointarr.removeLast();
			blgsarr.removeLast();
			lastpointarr.insertAt(0,pointarr[0]);
			lastblgsarr.insertAt(0,blgsarr[0]);
		}
		else if(nType==3) //首-首
		{
			ReVerse(pointarr,blgsarr); 
			pointarr.removeLast();
			blgsarr.removeLast();
			lastpointarr.insertAt(0,pointarr[0]);
			lastblgsarr.insertAt(0,blgsarr[0]);
		}
		return TRUE;

	}
}
BOOL AddPoints(AcGePoint3d ptStart,AcGePoint3d ptEnd,double dBulge,
			   AcGePoint3dArray &lastpointarr,AcGeDoubleArray &lastblgsarr)
{
	AcGePoint3dArray ptArr;
	AcGeDoubleArray ptBlgs;
	ptArr.append(ptStart);
	ptArr.append(ptEnd);
	ptBlgs.append(dBulge);
	ptBlgs.append(0.0);
	return AddPoints(ptArr,ptBlgs,lastpointarr,lastblgsarr);
}
void Exp_JoinLines(AcDbObjectIdArray& ids1)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcGePoint3dArray* ptsArray=new AcGePoint3dArray[ids1.logicalLength()];
	AcArray<double>* bulgesArray=new AcArray<double>[ids1.logicalLength()];
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId objId=ids1.at(i);
		AcDbEntityPointer spEnt(objId,AcDb::kForRead);
		es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine=AcDbPolyline::cast(spEnt.object());
			if(pLine->isClosed())
			{
				//已经闭合的不再连接
				continue;
			}
			for(int j=0;j<pLine->numVerts();j++)
			{
				AcGePoint3d pt;
				pLine->getPointAt(j,pt);
				double bulge=0.0;
				pLine->getBulgeAt(j,bulge);
				ptsArray[i].append(pt);
				bulgesArray[i].append(bulge);
			}
		}
		else if(spEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine=AcDbLine::cast(spEnt.object());
			AcGePoint3d ptStart;
			pLine->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pLine->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(0.0);
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);
		}
		else if(spEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc* pArc=AcDbArc::cast(spEnt.object());
			AcGePoint3d ptStart;
			pArc->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pArc->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(tan((pArc->endAngle()-pArc->startAngle())/4.0));
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);

		}

	}
	AcGeIntArray indexArray;//已经处理过的对象列表
	AcGeIntArray invalidIndexArray;
	AcGeIntArray oldLengthArray;
	for(int i=0;i<ids1.logicalLength();i++)
	{
		if(ptsArray[i].logicalLength()<2)
		{
			invalidIndexArray.append(i);
		}
		oldLengthArray.append(ptsArray[i].logicalLength());
	}
	while(indexArray.logicalLength()!=ids1.logicalLength())
	{
		//还有没有处理完的，继续处理
		int nFirstIndex=-1;
		for(nFirstIndex=0;nFirstIndex<ids1.logicalLength();nFirstIndex++)
		{
			if(indexArray.contains(nFirstIndex))
			{
				continue;
			}
			break;
		}
		if(ptsArray[nFirstIndex].logicalLength()<2)
		{
			indexArray.append(nFirstIndex);
			continue;
		}
		BOOL bFind=FALSE;
		for(int i=0;i<ids1.logicalLength();i++)
		{
			if(indexArray.contains(i))
				continue;
			if(ptsArray[i].logicalLength()<2)
			{
				indexArray.append(i);
				continue;
			}
			if(nFirstIndex==i)
				continue;
			if(AddPoints(ptsArray[i],bulgesArray[i],ptsArray[nFirstIndex],bulgesArray[nFirstIndex]))
			{
				bFind=TRUE;
				ptsArray[i].setLogicalLength(0);
				bulgesArray[i].setLogicalLength(0);
				indexArray.append(i);
			}
		}
		if(!bFind)
		{
			indexArray.append(nFirstIndex);
		}
	}
	AcDbObjectIdArray rc_ids;
	for(int i=0;i<ids1.logicalLength();i++)
	{
		if(ptsArray[i].logicalLength()>=2&&ptsArray[i].logicalLength()!=oldLengthArray.at(i))
		{
			AcDbPolyline* pLine=new AcDbPolyline(ptsArray[i].logicalLength());
			for(int j=0;j<ptsArray[i].logicalLength();j++)
			{
				AcGePoint2d pt2d=convert3dPointTo2d(ptsArray[i].at(j));
				pLine->addVertexAt(j,pt2d,bulgesArray[i].at(j));
			}
			AcDbObjectId objId=ids1.at(i);
			AcDbEntityPointer spEnt(objId,AcDb::kForRead);
			es=spEnt.openStatus();
			if(es==Acad::eOk)
			{
				SameEntityAttr(pLine,spEnt.object());
				rc_ids.append(AppendEntity(pLine,spEnt->blockId()));
				pLine->close();
			}
			else
			{
				delete pLine;
				pLine=NULL;
				invalidIndexArray.append(i);
			}
		}
	}
	for(int i=0;i<ids1.logicalLength();i++)
	{
		if(!invalidIndexArray.contains(i)&&
			ptsArray[i].logicalLength()!=oldLengthArray.at(i))
		{
			EraseEntity(ids1.at(i));
		}
		else
		{
			rc_ids.append(ids1.at(i));
		}
	}

	ids1.setLogicalLength(0);
	ids1.append(rc_ids);

	delete[] ptsArray;
	delete[] bulgesArray;
}
//将直线/圆弧/多段线连接起来
void JoinLines()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE,LINE,ARC"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择连接对象:"),_T("\n移除连接对象:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()==0)
	{
		return;
	}
	Exp_JoinLines(ids1);
}
BOOL DelRedundancyPointFromPolyline(AcDbObjectId ObjId)
{
	AcDbPolyline* pLine = NULL;
	ErrorStatus es = acdbOpenObject(pLine, ObjId, AcDb::kForWrite, Adesk::kFalse);
	if(Acad::eOk != es) 
	{
		acutPrintf(_T("\nERROR: %s\n"), acadErrorStatusText(es));
		return FALSE;
	}
	/*
	if(!pLine->isClosed())
	{
		acutPrintf(_T("\n对象不闭合!")); pLine->close(); return FALSE;
	}
	*/
	int nNumPt = pLine->numVerts();
	if(nNumPt < 3)
	{
		pLine->close(); 
		return TRUE;
	}

	AcGePoint3d ptA,ptB;
	double dA=0.0,dB=0.0;

	for(int i=nNumPt-1; i>0; i--)
	{
		pLine->getPointAt(i,ptA); 
		pLine->getPointAt(i-1,ptB);

		if((fabs(ptA.x-ptB.x) < 0.0001) && (fabs(ptA.y-ptB.y)<0.0001)) //两个点完全重合
		{
			pLine->removeVertexAt(i--);continue;
		}

		if (ptA.distanceTo(ptB) < 0.001)
		{
			pLine->getBulgeAt(i,dA); 
			pLine->getBulgeAt(i-1,dB);

			if(fabs(dA)>0.001 && fabs(dB)>0.001) continue;  //两点不重合，且都含有凸度

			if(fabs(dA) < 0.0001) 
				pLine->removeVertexAt(i);
			else if(fabs(dB) < 0.0001)
				pLine->removeVertexAt(--i);
		}
	}
	if(pLine->isClosed())
	{
		nNumPt = pLine->numVerts();
		pLine->getPointAt(0,ptA);
		pLine->getPointAt(nNumPt-1,ptB);

		if((fabs(ptA.x-ptB.x) < 0.0001) && (fabs(ptA.y-ptB.y)<0.0001)) //两个点完全重合
		{
			pLine->removeVertexAt(nNumPt-1);
		}
		else if (ptA.distanceTo(ptB) < 0.0001 )
		{	
			pLine->getBulgeAt(0,dA); 
			pLine->getBulgeAt(nNumPt-1,dB);

			if(fabs(dA) > fabs(dB)) 
				pLine->removeVertexAt(nNumPt-1);
			else
				pLine->removeVertexAt(0);
		}
	}

	while(true)
	{
		//检查自身线段之间的重叠
		nNumPt = pLine->numVerts();
		if(nNumPt<3)
		{
			break;
		}
		AcGePoint3d ptC,ptD;
		double dDC;
		pLine->getPointAt(nNumPt-1,ptC);
		pLine->getPointAt(nNumPt-2,ptD);
		pLine->getBulgeAt(nNumPt-2,dDC);
		BOOL bFind=FALSE;
		for(int i=0;i<=nNumPt-4;i++)
		{
			AcGePoint3d ptE,ptF;
			double dE,dF;
			pLine->getPointAt(i,ptE);
			pLine->getBulgeAt(i,dE);
			pLine->getPointAt(i+1,ptF);
			pLine->getBulgeAt(i+1,dF);
			if(fabs(dE-dDC)<1E-8&&
				fabs(ptC.x-ptE.x)<1E-6&&
				fabs(ptC.y-ptE.y)<1E-6&&
				fabs(ptD.x-ptF.x)<1E-6&&
				fabs(ptD.y-ptF.y)<1E-6)
			{
				bFind=TRUE;
			}
			//反向一下
			AcGePoint3d ptTmp=ptE;
			ptE=ptF;
			ptF=ptTmp;
			dE=-dE;
			if(fabs(dE-dDC)<1E-8&&
				fabs(ptC.x-ptE.x)<1E-6&&
				fabs(ptC.y-ptE.y)<1E-6&&
				fabs(ptD.x-ptF.x)<1E-6&&
				fabs(ptD.y-ptF.y)<1E-6)
			{
				bFind=TRUE;
			}
		}
		if(!bFind)
		{
			break;
		}
		pLine->setPointAt(nNumPt-1,convert3dPointTo2d(ptD));
		pLine->removeVertexAt(nNumPt-1-1);

	}
	

	pLine->close();

	return TRUE;
}
//删除重复点
void RemoveSamePts()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多段线:"),_T("\n移除多段线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()==0)
	{
		return;
	}
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId objId=ids1.at(i);
		DelRedundancyPointFromPolyline(objId);
	}

}
//简单选择，根据图层和实体类型选择对象
void SimpleSel()
{
	AcDbObjectIdArray ids;
	EnumAllEntityInCurrentSpace(ids);
	AcDbObjectIdArray layerIdArray;
	CStringArray      layerNameArray;
	AcArray<AcRxClass*> entTypeArray;
	Acad::ErrorStatus es=Acad::eOk;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,ids.at(i),AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			if(!entTypeArray.contains(pEnt->isA()))
			{
				entTypeArray.append(pEnt->isA());
			}
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				CString strLayerName;
				es=GetLayerName(strLayerName,layerId);
				if(es==Acad::eOk)
				{
					layerIdArray.append(layerId);
					layerNameArray.Add(strLayerName);
				}
			}
		}
	}
	CAcModuleResourceOverride res;
	GSimpleSelDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.layerIdArray.append(layerIdArray);
	dlg.layerNameArray.Append(layerNameArray);
	dlg.entTypeArray.append(entTypeArray);
	if(dlg.DoModal()!=IDOK)
		return;
	int Err;
	ads_name ssname;
	Err=acedSSGet(_T("X"),NULL,NULL,dlg.selFilter,ssname);
	acutRelRb(dlg.selFilter);
	dlg.selFilter=NULL;
	if(Err!=RTNORM)
	{
		AfxMessageBox(_T("执行选择失败,未找到任何符合您设定条件的实体"));
		return;
	}
	acedSSSetFirst(ssname,ssname);
	acedSSFree(ssname);
	acedUpdateDisplay();
}

#include "ParcelViewInputPoint.h"
#include "GMousePreviewDocReactor.h"
extern GMousePreviewDocReactor* Glb_MousePreviewDocReactor;
bool m_bXDataDynViewOpen=false;
CStringArray Glb_MousePreviewKeyArr;
CStringArray Glb_MousePreviewValueArr;
CArray<bool> Glb_MousePreviewOpenArr;
bool         Glb_MousePreviewAutoOpen;
void InitXDataDynView()
{
	CString strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("MousePreview"),_T("AutoOpen"),2048,_T("False"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		Glb_MousePreviewAutoOpen=true;
	}
	else
	{
		Glb_MousePreviewAutoOpen=false;
	}
	Glb_MousePreviewKeyArr.Add(_T("EntType"));
	Glb_MousePreviewValueArr.Add(_T("实体类型"));
	Glb_MousePreviewKeyArr.Add(_T("Layer"));
	Glb_MousePreviewValueArr.Add(_T("图层"));
	Glb_MousePreviewKeyArr.Add(_T("Length"));
	Glb_MousePreviewValueArr.Add(_T("长度"));
	Glb_MousePreviewKeyArr.Add(_T("Area"));
	Glb_MousePreviewValueArr.Add(_T("面积"));
	Glb_MousePreviewKeyArr.Add(_T("BlockName"));
	Glb_MousePreviewValueArr.Add(_T("块名称"));
	Glb_MousePreviewKeyArr.Add(_T("HatchName"));
	Glb_MousePreviewValueArr.Add(_T("填充名称"));
	Glb_MousePreviewKeyArr.Add(_T("FontStyle"));
	Glb_MousePreviewValueArr.Add(_T("字体样式"));
	for(int i=0;i<Glb_MousePreviewKeyArr.GetCount();i++)
	{
		strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("MousePreview"),Glb_MousePreviewKeyArr.GetAt(i),2048,_T("False"));
		if(strTmp.CompareNoCase(_T("True"))==0)
		{
			Glb_MousePreviewOpenArr.Add(true);
		}
		else
		{
			Glb_MousePreviewOpenArr.Add(false);
		}
	}
	if(Glb_MousePreviewAutoOpen)
	{
		m_bXDataDynViewOpen=true;
		Glb_MousePreviewDocReactor->SetXDataDynViewState(curDoc(),1);
	}
}
//鼠标预览
void MousePreview()
{
	CAcModuleResourceOverride res;
	GMousePreviewDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
		return;
	m_bXDataDynViewOpen=dlg.m_bOpenPreview;
	if(dlg.m_bOpenPreview)
	Glb_MousePreviewDocReactor->SetXDataDynViewState(curDoc(),1);
	else
	{
		Glb_MousePreviewDocReactor->SetXDataDynViewState(curDoc(),0);
	}
	GUserDataRecord::getInstance()->WriteProfileString(_T("MousePreview"),_T("AutoOpen"),Glb_MousePreviewAutoOpen?_T("True"):_T("False"));
	for(int i=0;i<Glb_MousePreviewKeyArr.GetCount();i++)
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("MousePreview"),Glb_MousePreviewKeyArr.GetAt(i),Glb_MousePreviewOpenArr.GetAt(i)?_T("True"):_T("False"));
	}
}
//软件注册
void AssReg()
{
	CAcModuleResourceOverride res;
	GRegDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();
}
//图纸防修改
void ProtectEnts()
{
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择防止修改的对象:"),_T("\n移除防止修改的对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pTable = NULL;
	Acad::ErrorStatus es = pDb->getBlockTable(pTable, AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return;
	}
	AcDbObjectPointer<AcDbBlockTable> spTable;
	spTable.acquire(pTable);
	AcDbObjectId recordId;
	AcDbBlockTableRecord* pRec = new AcDbBlockTableRecord();
	es=pRec->setName(_T("*U"));
	if(es!=eOk)
	{
		delete pRec;
		pRec=NULL;
		return;
	}
	es=spTable->add(recordId,pRec);
	if(es!=eOk)
	{
		delete pRec;
		pRec=NULL;
		return;
	}
	pRec->close();
	AcDbIdMapping idMap;
	es=pDb->deepCloneObjects(EntIds,recordId,idMap);
	//插入一个块参照
	AcDbMInsertBlock *pBlkRef=new AcDbMInsertBlock;
	pBlkRef->setBlockTableRecord(recordId);//指定所引用的图块表记录的对象ID
	resbuf to,from;
	from.restype=RTSHORT;//插入图块要进行用户坐标与世界坐标的转换
	from.resval.rint=1;//UCS
	to.restype=RTSHORT;
	to.resval.rint=0;//WCS
	AcGeVector3d normal(0.0,0.0,1.0);
	acedTrans(&(normal.x),&from,&to,Adesk::kTrue,&(normal.x));//转换函数
	pBlkRef->setPosition(AcGePoint3d(0,0,0)); //指定基点
	pBlkRef->setRotation(0.0);//旋转角度
	pBlkRef->setNormal(normal);
	pBlkRef->setDatabaseDefaults(pDb);

	AcGeScale3d dGeScale(1.0,1.0,1.0);
	pBlkRef->setScaleFactors(dGeScale);//放大比例

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk == acdbOpenObject(pBlockTableRecord, pDb->currentSpaceId(), AcDb::kForWrite))
	{
		AcDbObjectId resultId;
		pBlockTableRecord->appendAcDbEntity(resultId,pBlkRef);
		pBlkRef->close();
		pBlockTableRecord->close();
	}
	else
	{
		delete pBlkRef;
		pBlkRef=NULL;
	}
	//删除旧的实体对象
	for(long i=0;i<EntIds.logicalLength();i++)
	{
		ForceEraseEntity(EntIds.at(i));
	}
}
#include "GBarcodeMgr.h"
#include "GBarCodeJig.h"
#include "GBarCodeDialog.h"
//插入条形码
void BarCode()
{
	CAcModuleResourceOverride res;
	GBarCodeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	CString strBarCode=dlg.m_strBarCode;
	ads_point result;
	bool bText=dlg.m_bText;
	int rc=acedGetPoint(NULL,_T("\n>>请选择插入点\n"),result);
	if(rc!=RTNORM)
		return;
	AcGePoint3d position(result[0],result[1],result[2]);
	position=GCADUtils::UCS2WCS(position);
	GBarCodeJig jig;
	jig.ptBase=position;
	jig.strBarCode=strBarCode;
	jig.bText=bText;
	jig.InitEntity();
	int ret=jig.Do();
	if(ret==RTNORM)
	{
		CString strName=_T("BarCode");
		makeGroup(acdbHostApplicationServices()->workingDatabase(),strName,true,jig.m_ids);
	}
}
#include "GEntAlignDialog.h"
GEntAlignDialog * pDlgEntAlign = NULL;
Adesk::Boolean createEntAlignDialog(AcDbObjectIdArray& ids)
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) {
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}
	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( pDlgEntAlign == NULL ) 
	{
		if ( (pDlgEntAlign = new GEntAlignDialog ( ids,pWnd )) == NULL ) {
			//AfxMessageBox ( _T("Unable to allocate a CMDITestDialog.") );
			return Adesk::kFalse;
		}

		BOOL succeeded = pDlgEntAlign->Create ( GEntAlignDialog::IDD,pWnd );
		if ( !succeeded ) {
			//AfxMessageBox ( _T("Unable to create the dialog.") );
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
void showDlgEntAlign(AcDbObjectIdArray& ids)
{
	//if( pDlgReadInfo ) {
	//	// dialog was already created, just display it
	//	// (the dialog only gets deallocated when docman.arx is unloaded)
	//	pDlgReadInfo->ShowWindow( SW_SHOWNORMAL );
	//	return;
	//}
	if( pDlgEntAlign ) 
	{
		pDlgEntAlign->MoveBack();
		pDlgEntAlign->DestroyWindow();
		delete pDlgEntAlign;
		pDlgEntAlign = NULL;
	}
	if( !pDlgEntAlign && !createEntAlignDialog(ids) ) 
	{
		//acutPrintf( _T("Failed to create modeless dialog box.\n") );
	}
	else
	{
		pDlgEntAlign->ShowWindow(SW_NORMAL);
		pDlgEntAlign->CenterWindow();
	}
}
void destroyDlgEntAlign()
{
	if( pDlgEntAlign )
	{
		pDlgEntAlign->MoveBack();
		pDlgEntAlign->DestroyWindow();
		delete pDlgEntAlign;
		pDlgEntAlign = NULL;
	}
}
//实体对齐
void EntityAlign()
{
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择需要对齐的对象:"),_T("\n移除需要对齐的对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()<2)
	{
		acutPrintf(_T("\n需要对齐的数量必须大于等于2\n"));
		return;
	}
	showDlgEntAlign(EntIds);

}
#ifdef ACAD_2004
void TableAlign()
{
	acutPrintf(_T("\nAutoCAD2004不支持表格功能，如果您需要使用表格对齐功能，请升级至AutoCAD2005或以上版本"));
}
#else
#include "GTableAlignDialog.h"
GTableAlignDialog * pDlgTableAlign = NULL;
Adesk::Boolean createTableAlignDialog(AcDbObjectIdArray& ids)
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) {
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}
	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( pDlgEntAlign == NULL ) 
	{
		if ( (pDlgTableAlign = new GTableAlignDialog ( ids,pWnd )) == NULL ) {
			//AfxMessageBox ( _T("Unable to allocate a CMDITestDialog.") );
			return Adesk::kFalse;
		}

		BOOL succeeded = pDlgTableAlign->Create ( GTableAlignDialog::IDD,pWnd );
		if ( !succeeded ) {
			//AfxMessageBox ( _T("Unable to create the dialog.") );
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
void showDlgTableAlign(AcDbObjectIdArray& ids)
{
	//if( pDlgReadInfo ) {
	//	// dialog was already created, just display it
	//	// (the dialog only gets deallocated when docman.arx is unloaded)
	//	pDlgReadInfo->ShowWindow( SW_SHOWNORMAL );
	//	return;
	//}
	if( pDlgTableAlign ) 
	{
		pDlgTableAlign->MoveBack();
		pDlgTableAlign->DestroyWindow();
		delete pDlgTableAlign;
		pDlgTableAlign = NULL;
	}
	if( !pDlgTableAlign && !createTableAlignDialog(ids) ) 
	{
		//acutPrintf( _T("Failed to create modeless dialog box.\n") );
	}
	else
	{
		pDlgTableAlign->ShowWindow(SW_NORMAL);
		pDlgTableAlign->CenterWindow();
	}
}
void destroyDlgTableAlign()
{
	if( pDlgTableAlign )
	{
		pDlgTableAlign->MoveBack();
		pDlgTableAlign->DestroyWindow();
		delete pDlgTableAlign;
		pDlgTableAlign = NULL;
	}
}


//表格对齐
void TableAlign()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	resbuf* rb = acutBuildList(RTDXF0, _T("ACAD_TABLE"), 0);
	TCHAR* promptPtrs[2]={_T("\n选择需要对齐的表格:"),_T("\n移除需要对齐的表格:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,rb,ssName)!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()<=0)
	{
		return;
	}
	showDlgTableAlign(EntIds);

}
#endif
#include "GEntAverPosDialog.h"
GEntAverPosDialog * pDlgEntAverPos = NULL;
Adesk::Boolean createEntAverPosDialog(AcDbObjectIdArray& ids)
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) {
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}
	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( pDlgEntAverPos == NULL ) 
	{
		if ( (pDlgEntAverPos = new GEntAverPosDialog ( ids,pWnd )) == NULL ) {
			//AfxMessageBox ( _T("Unable to allocate a CMDITestDialog.") );
			return Adesk::kFalse;
		}

		BOOL succeeded = pDlgEntAverPos->Create ( GEntAverPosDialog::IDD,pWnd );
		if ( !succeeded ) {
			//AfxMessageBox ( _T("Unable to create the dialog.") );
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
void showDlgEntAverPos(AcDbObjectIdArray& ids)
{
	//if( pDlgReadInfo ) {
	//	// dialog was already created, just display it
	//	// (the dialog only gets deallocated when docman.arx is unloaded)
	//	pDlgReadInfo->ShowWindow( SW_SHOWNORMAL );
	//	return;
	//}
	if( pDlgEntAverPos ) 
	{
		pDlgEntAverPos->MoveBack();
		pDlgEntAverPos->DestroyWindow();
		delete pDlgEntAverPos;
		pDlgEntAverPos = NULL;
	}
	if( !pDlgEntAverPos && !createEntAverPosDialog(ids) ) 
	{
		//acutPrintf( _T("Failed to create modeless dialog box.\n") );
	}
	else
	{
		pDlgEntAverPos->ShowWindow(SW_NORMAL);
		pDlgEntAverPos->CenterWindow();
	}
}
void destroyDlgEntAverPos()
{
	if( pDlgEntAverPos )
	{
		pDlgEntAverPos->MoveBack();
		pDlgEntAverPos->DestroyWindow();
		delete pDlgEntAverPos;
		pDlgEntAverPos = NULL;
	}
}
//对象均布
void EntAverPos()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择需要均匀分布的对象:"),_T("\n移除需要均匀分布的对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()<3)
	{
		acutPrintf(_T("\n需要对齐的数量必须大于等于3\n"));
		return;
	}
	showDlgEntAverPos(EntIds);
}
//按颜色选择
void SelectByColor()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	// 只支持单选
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择颜色模板对象:"),_T("\n移除颜色模板对象:")};
	if(acedSSGet(_T(":$:S"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()!=1)
	{
		return;
	}
	AcDbEntityPointer spEnt(EntIds.at(0),AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	int nColorIndex=GetAbsColorIndex(spEnt.object());
	spEnt->close();
	AcDbObjectIdArray ids;
	EnumAllEntityInCurrentSpace(ids);
	AcDbObjectIdArray needSelectIds;
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(ids.at(i)==EntIds.at(0))
		{
			needSelectIds.append(ids.at(i));
			continue;
		}
		AcDbEntityPointer spEnt1(ids.at(i),AcDb::kForRead);
		if(spEnt1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(GetAbsColorIndex(spEnt1.object())!=nColorIndex)
		{
			continue;
		}
		needSelectIds.append(ids.at(i));
	}
	SetEntitySelected(needSelectIds,true);
}
//按颜色选择-多色
void SelectByMultColor()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	// 只支持单选
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择颜色模板对象:"),_T("\n移除颜色模板对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()==0)
	{
		return;
	}
	AcArray<int> nColorIndexArr;
	for(int j=0;j<EntIds.logicalLength();j++)
	{
		AcDbEntityPointer spEnt(EntIds.at(0),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			return;
		}
		int nColorIndex=GetAbsColorIndex(spEnt.object());
		spEnt->close();
		if(nColorIndexArr.contains(nColorIndex))
		{
			continue;
		}
		nColorIndexArr.append(nColorIndex);
	}
	
	AcDbObjectIdArray ids;
	EnumAllEntityInCurrentSpace(ids);
	AcDbObjectIdArray needSelectIds;
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(EntIds.contains(ids.at(i)))
		{
			needSelectIds.append(ids.at(i));
			continue;
		}
		AcDbEntityPointer spEnt1(ids.at(i),AcDb::kForRead);
		if(spEnt1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!nColorIndexArr.contains(GetAbsColorIndex(spEnt1.object())))
		{
			continue;
		}
		needSelectIds.append(ids.at(i));
	}
	SetEntitySelected(needSelectIds,true);
}
//选中当前文件
void SelCurFile()
{
	CString strDwgFileName;
	if(!GCADUtils::GetCurDwgFileName(strDwgFileName))
	{
		AfxMessageBox(_T("当前文件未保存，无法定位"));
		return;
	}
	CString strArg;
	strArg.Format(_T("/select,%s"),strDwgFileName);
	::ShellExecute(NULL,_T("open"),_T("explorer.exe"),strArg.GetBuffer(),NULL,SW_SHOW);
	strArg.ReleaseBuffer();
}
//临时隐藏
void TempHide()
{
	// 支持多选
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择临时隐藏对象:"),_T("\n移除临时隐藏对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	for(int i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointerEx<AcDbEntity> spEnt(EntIds.at(i),AcDb::kForWrite,false,true);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->setVisibility(AcDb::kInvisible);
		GCADUtils::MarkObject(spEnt.object(),_T("cadgjcomTempHide"),NULL,true);
	}
}
//临时隐藏其它
void TempHideOther()
{
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
	{
		return;
	}
	AcDbObjectIdArray ids;
	EnumAllEntityInBTR(ids,pDb->currentSpaceId());
	// 支持多选
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择临时隐藏例外对象:"),_T("\n移除临时隐藏例外对象:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(GCADUtils::FindObjectId(EntIds,ids.at(i))>=0)
		{
			continue;
		}
		AcDbEntityPointerEx<AcDbEntity> spEnt(ids.at(i),AcDb::kForWrite,false,true);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->setVisibility(AcDb::kInvisible);
		GCADUtils::MarkObject(spEnt.object(),_T("cadgjcomTempHide"),NULL,true);
	}
}
//79:恢复显示
void RoolbackShow()
{
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
	{
		return;
	}
	AcDbObjectIdArray ids;
	EnumAllEntityInBTR(ids,pDb->currentSpaceId());
	Acad::ErrorStatus es=Acad::eOk;
	int nCount=0;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForRead);
		es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			continue;
		}
		bool bMark=false;
		es=GCADUtils::IsMarkedObject(spEnt.object(),_T("cadgjcomTempHide"),bMark);
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(!bMark)
		{
			continue;
		}
		es=spEnt->upgradeOpen();
		if(es!=Acad::eOk)
		{
			continue;
		}
		spEnt->setVisibility(AcDb::kVisible);
		es=GCADUtils::MarkObject(spEnt.object(),_T("cadgjcomTempHide"),NULL,false);
		nCount++;
	}
	if(nCount>0)
	{
	acutPrintf(_T("\n成功恢复显示%d个实体\n"),nCount);
	}
	else
	{
		acutPrintf(_T("\n未找到隐藏实体或恢复失败\n"));
	}
}
//82:导出坐标点
void ExportPoints()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多段线:"),_T("\n移除多段线:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()!=1)
	{
		return;
	}
	AcDbObjectPointer<AcDbPolyline> spEnt(ids1.at(0),AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		acutPrintf(_T("\r\n读取多段线节点失败:%s\r\n"),::acadErrorStatusTextChs(spEnt.openStatus()));
		return;
	}
	AcGePoint2dArray pts;
	for(int i=0;i<spEnt->numVerts();i++)
	{
		AcGePoint2d pt;
		spEnt->getPointAt(i,pt);
		pts.append(pt);
	}
	if(pts.logicalLength()<=1)
	{
		acutPrintf(_T("\r\n多段线顶点数少于2\r\n"));
	}
	if(spEnt->isClosed())
	{
		AcGePoint2d ptS,ptE;
		ptS=pts.at(0);
		ptE=pts.at(pts.logicalLength()-1);
		if(fabs(ptS.x-ptE.x)>1E-8||fabs(ptS.y-ptE.y)>1E-8)
		{
			pts.append(ptS);
		}
	}
	CString lpDwgFile;
	CFileDialog fileDlg(FALSE, _T(".xls"), NULL, OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		_T("Excel文件 (*.xls)|*.xls||"), acedGetAcadFrame());
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	lpDwgFile=fileDlg.GetPathName();
	try
	{ 
		Excel::_ApplicationPtr pExcelApp; 
		HRESULT hr = pExcelApp.CreateInstance(L"Excel.Application"); 
		ATLASSERT(SUCCEEDED(hr)); 
		pExcelApp->Visible = true;   // make Excel’s main window visible 
		Excel::_WorkbookPtr pWorkbook=pExcelApp->Workbooks->Add();

		Excel::_WorksheetPtr pWorksheet = pWorkbook->ActiveSheet; 
		Excel::RangePtr pRange = pWorksheet->Cells;  
		pRange->Item[1][1] = L"ID";  // read/write cell’s data 
		pRange->Item[1][2] = L"X";
		pRange->Item[1][3] = L"Y";
		AcGePoint2d pt;
		for(int i=0;i<pts.logicalLength();i++)
		{
			pt=pts.at(i);
			CString strId;
			strId.Format(_T("%d"),i+1);
			pRange->Item[1+i+1][1]=_bstr_t(strId);
			strId.Format(_T("%7.3f"),pt.x);
			pRange->Item[1+i+1][2]=_bstr_t(strId);
			strId.Format(_T("%7.3f"),pt.y);
			pRange->Item[1+i+1][3]=_bstr_t(strId);

		}
		Excel::XlFileFormat;
		_variant_t FileFormat=Excel::xlExcel8;
		pWorkbook->SaveAs(_bstr_t(lpDwgFile), 
			&FileFormat,   // FileFormat 
         &vtMissing,    // Password
         &vtMissing,    // WriteResPassword
         &vtMissing,    // ReadOnlyRecommended
         &vtMissing,    // CreateBackup
         Excel::xlNoChange);   // AccessMode

		pWorkbook->Close(VARIANT_FALSE);  // save changes 
		pExcelApp->Quit(); 
	} 
	catch (_com_error& error) 
	{ 
		ATLASSERT(FALSE); 
		ATLTRACE2(error.ErrorMessage()); 
	} 
}
CString GetCellValue(Excel::RangePtr pCell)
{
	CString result;
	COleVariant vResult;
	SYSTEMTIME st ;
	CString stry ,strm ,strd ;
	///////////////////////
	vResult = pCell->GetValue2();
	switch ( vResult.vt )
	{
	case VT_BSTR:    //OLE Automation string
		{
			result = vResult.bstrVal;
			break;
		}
	case VT_R8 : // 8 byte real
		{
			double dValue=vResult.dblVal;
			int nValue=(int)dValue;
			if(fabs(dValue-(double)nValue)<1E-5)
			{
				result.Format(_T("%d"),nValue);
			}
			else
			result.Format(_T("%f"),dValue);
			break;
		}
	case VT_DATE: //date
		{
			VariantTimeToSystemTime(vResult.date, &st);
			stry.Format(_T("%d"),st.wYear);
			strm.Format(_T("%d"),st.wMonth);
			strd.Format(_T("%d"),st.wDay);
			result = stry+"-"+strm+"-"+strd;
			break ;
		}
	case VT_EMPTY: //empty
		{
			result.Empty();
			break ;
		}
	default:
		{
			result.Empty();
			break;
		}
	}

	return result;

}
//暂时技术解决不过
//83:导入坐标点
void ImportPoints()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	
	CString lpDwgFile;
	CFileDialog fileDlg(true, NULL, NULL, OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		_T("Excel文件 (*.xls;*.xlsx)|*.xls;*.xlsx||"), acedGetAcadFrame());
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	lpDwgFile=fileDlg.GetPathName();
	try
	{ 
		Excel::_ApplicationPtr pExcelApp; 
		HRESULT hr = pExcelApp.CreateInstance(L"Excel.Application"); 
		ATLASSERT(SUCCEEDED(hr)); 
		pExcelApp->Visible = true;   // make Excel’s main window visible 
		Excel::_WorkbookPtr pWorkbook=pExcelApp->Workbooks->Open(_bstr_t(lpDwgFile));
		HWND Glb_ExcelHwnd=(HWND)pExcelApp->Hwnd;
		pExcelApp->Caption=L"请选择你需要导入的数据......";
		long oldRow=0;
		long oldRowArray[10]={-1};
		while(true)
		{
			if(!IsWindow(Glb_ExcelHwnd))
			{
				break;
			}
			pWorkbook=pExcelApp->ActiveWorkbook;
			Excel::_WorksheetPtr pWorksheet = pWorkbook->ActiveSheet;
			
			Excel::RangePtr pSelection=pExcelApp->Selection;
			int nRow=pSelection->Rows->Count;
			int nColumn=pSelection->Columns->Count;
			if(GetAsyncKeyState(VK_LBUTTON)<0||
				GetAsyncKeyState(VK_RBUTTON)<0||
				GetAsyncKeyState(VK_SHIFT)<0||
				GetAsyncKeyState(VK_CONTROL)<0||
				GetAsyncKeyState(VK_MBUTTON)<0)
			{
				Sleep(100);
				continue;
			}
			if(nRow>1&&(nColumn==2||nColumn==3))
			{
				bool bKeep=true;
				for(int i=0;i<10;i++)
				{
					if(oldRowArray[i]<0)
					{
						oldRowArray[i]=nRow;
					}
					else if(oldRowArray[i]==nRow)
					{
					}
					else
					{
						bKeep=false;
						break;
					}
				}
				if(!bKeep)
				{
					for(int i=0;i<10;i++)
					{
						oldRowArray[i]=-1;
					}
					continue;
				}
				CString strInfo;
				if(oldRow==nRow)
				{
					continue;
				}
				oldRow=nRow;
				strInfo.Format(_T("共选择了%d行数据，是否确认?"),oldRow);
				if(MessageBox(Glb_ExcelHwnd,strInfo,_T("提示"),MB_YESNO)==IDYES)
				{
					AcDbPolyline* pLine=new AcDbPolyline;
					int nCount=0;
					for(int i=1;i<=nRow;i++)
					{
						Excel::RangePtr pCell=pSelection->GetItem(i,nColumn==3?2:1);
						double x;
						if(!GStringHelper::IsValidDouble(GetCellValue(pCell),x))
						{
							continue;
						}
						pCell=pSelection->GetItem(i,nColumn==3?3:2);
						double y;
						if(!GStringHelper::IsValidDouble(GetCellValue(pCell),y))
						{
							continue;
						}
						nCount++;
						if(i==nRow)
						{
							if(nRow==1)
							{
								pLine->addVertexAt(nCount-1,AcGePoint2d(x,y));
							}
							else
							{
								AcGePoint2d pt0;
								pLine->getPointAt(0,pt0);
								if(fabs(pt0.x-x)<1E-8&&fabs(pt0.y-y)<1E-8)
								{
									pLine->setClosed(Adesk::kTrue);
								}
								else
								{
									pLine->addVertexAt(nCount-1,AcGePoint2d(x,y));
								}
							}
						}
						else
						{
							pLine->addVertexAt(nCount-1,AcGePoint2d(x,y));
						}
					}
					AcDbObjectId id=AddToCurrentSpace(pLine);
					pLine->close();
					if(id.isValid())
					{
						AcDbObjectIdArray ids;
						ids.append(id);
					SetEntitySelected(ids,true);
	DyZoom(ids,1.1);
					}
					break;
				}
			}
			Sleep(500);
		}
		if(IsWindow(Glb_ExcelHwnd))
		{
		pWorkbook->Close(VARIANT_FALSE);  // save changes 
		pExcelApp->Quit();
		}
	} 
	catch (_com_error& error) 
	{ 
		//ATLASSERT(FALSE); 
		ATLTRACE2(error.ErrorMessage()); 
	}
}
//获取标注中的文字
Acad::ErrorStatus GetDimText(CString& strText,AcDbDimension* pDim)
{
	AcDbObjectIdArray ids;
	Acad::ErrorStatus es=EnumAllEntityInBTR(ids,pDim->dimBlockId());
	if(es!=Acad::eOk)
	{
		return es;
	}
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());
			TCHAR* pTextStr=pText->textString();
			strText=pTextStr;
			acutDelString(pTextStr);
			return Acad::eOk;
		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			TCHAR* pTextStr=pText->contents();
			strText=pTextStr;
			acutDelString(pTextStr);
			MTextItemArray itemArr;
			itemArr.item.str=strText;
		itemArr.item.nType=-1;
		itemArr.updateChildren();
		strText=itemArr.getText();
			return Acad::eOk;
		}
	}
	return Acad::eNotApplicable;

}
bool IsAllTrue(CArray<bool>& arr)
{
	for(int i=0;i<arr.GetCount();i++)
	{
		if(!arr.GetAt(i))
		{
			return false;
		}
	}
	return true;
}
//84:导出标注
#include "GExportDimensionDialog.h"
void ExportDims()
{
	{
	if(!CHECK_REG_BOOL())
		return;
	}
	CAcModuleResourceOverride res;
	GExportDimensionDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	AcDbObjectIdArray ids;
	if(dlg.m_bAllEnt)
	{
		AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
	{
		return;
	}
	EnumAllEntityInBTR(ids,pDb->currentSpaceId());
	}
	else
	{
		ads_name ssName;
		resbuf* rb = acutBuildList(RTDXF0, _T("DIMENSION"), 0);
		TCHAR* promptPtrs[2]={_T("\n选择标注:"),_T("\n移除标注:")};
		int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	}
	CStringArray strTextArray;
	CStringArray strTypeArray;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(!spEnt->isKindOf(AcDbDimension::desc()))
		{
			continue;
		}
		AcDbDimension* pDim=AcDbDimension::cast(spEnt.object());
		CString strType=_T("");
		for(int j=0;j<dlg.m_classArray.GetCount();j++)
		{
			if(pDim->isKindOf(dlg.m_classArray.GetAt(j)))
			{
				strType=dlg.m_strNameArray.GetAt(j);
				break;
			}
		}
		if(strType.IsEmpty())
		{
			GLog::WriteLine(_T("忽略不需要导出的标注:%s，需要导出的标注类型共有:%d个"),pDim->isA()->name(),dlg.m_classArray.GetCount());
			continue;
		}
		CString strText;
		if(GetDimText(strText,pDim)!=Acad::eOk)
		{
			continue;
		}
		strTextArray.Add(strText);
		strTypeArray.Add(strType);
	}
	if(strTextArray.GetCount()==0)
	{
		AfxMessageBox(_T("未找到任何距离标注"));
		return;
	}
	GLog::WriteLine(_T("找到需要导出的标注:%d个"),strTextArray.GetCount());
	CString lpDwgFile;
	CFileDialog fileDlg(FALSE, _T(".xls"), NULL, OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		_T("Excel文件 (*.xls)|*.xls||"), acedGetAcadFrame());
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	lpDwgFile=fileDlg.GetPathName();
	try
	{ 
		Excel::_ApplicationPtr pExcelApp; 
		HRESULT hr = pExcelApp.CreateInstance(L"Excel.Application"); 
		ATLASSERT(SUCCEEDED(hr)); 
		pExcelApp->Visible = true;   // make Excel’s main window visible 
		Excel::_WorkbookPtr pWorkbook=pExcelApp->Workbooks->Add();

		Excel::_WorksheetPtr pWorksheet = pWorkbook->ActiveSheet; 
		Excel::RangePtr pRange = pWorksheet->Cells;  
		pRange->Item[1][1] = L"ID";  // read/write cell’s data 
		pRange->Item[1][2] = L"标注类型";
		pRange->Item[1][3] = L"标注内容";
		CArray<bool> m_bOutArray;//已经输出过的了
		for(int i=0;i<strTextArray.GetCount();i++)
		{
			m_bOutArray.Add(false);
		}
		int nIndex=0;
		while(!IsAllTrue(m_bOutArray))
		{
			CString strDimType=_T("");
		for(int i=0;i<strTextArray.GetCount();i++)
		{
			if(m_bOutArray.GetAt(i))
			{
				continue;
			}
			if(strDimType.IsEmpty())
			{
			 strDimType=strTypeArray.GetAt(i);
			}
			else
			{
				if(strDimType.CompareNoCase(strTypeArray.GetAt(i))!=0)
				{
					continue;
				}
			}
			CString strId;
			strId.Format(_T("%d"),i+1);
			pRange->Item[1+nIndex+1][1]=_bstr_t(strId);
			pRange->Item[1+nIndex+1][2]=_bstr_t(strTypeArray.GetAt(i));
			pRange->Item[1+nIndex+1][3]=_bstr_t(strTextArray.GetAt(i));
			m_bOutArray.SetAt(i,true);
			nIndex++;
		}
		}
		
		_variant_t FileFormat=Excel::xlExcel8;
		pWorkbook->SaveAs(_bstr_t(lpDwgFile), 
			&FileFormat,   // FileFormat 
         &vtMissing,    // Password
         &vtMissing,    // WriteResPassword
         &vtMissing,    // ReadOnlyRecommended
         &vtMissing,    // CreateBackup
         Excel::xlNoChange);   // AccessMode

		pWorkbook->Close(VARIANT_FALSE);  // save changes 
		pExcelApp->Quit(); 
	} 
	catch (_com_error& error) 
	{ 
		ATLASSERT(FALSE); 
		ATLTRACE2(error.ErrorMessage()); 
	}
}
#include "GJoinDwgDialog.h"
#include "GJoinDwgHelper.h"
//110:图纸合并
void tzhb()
{
	{
		if(!CHECK_REG_BOOL())
		return;
	}
	CAcModuleResourceOverride res;
	GJoinDwgDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	GJoinDwgParam param=dlg.m_param;
	GJoinDwgHelper helper;
	helper.m_param=param;
	helper.Join();
}

