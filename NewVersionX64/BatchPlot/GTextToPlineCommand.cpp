#include "StdAfx.h"
#include "GFrameBorderHelper.h"
#include "GPlotHelper.h"
#include "GTextToPlineCommand.h"
#include "acadi.h"
#include "GTextToPlineCommand.h"
#if _MSC_VER>=1700
#include "acedCmdNF.h"
#endif
void _TextToPline(AcDbObjectIdArray& textIds)
{
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbObjectIdArray ids;
	ids.append(textIds);
	AcGePoint3d ptMin;
	AcGePoint3d ptMax;
	GCADUtils::GetMaxViewPoint(ids,ptMin,ptMax);

	AcDbPolyline* pLine=new AcDbPolyline(4);
	pLine->addVertexAt(0,AcGePoint2d(ptMin.x,ptMin.y));
	pLine->addVertexAt(1,AcGePoint2d(ptMax.x,ptMin.y));
	pLine->addVertexAt(2,AcGePoint2d(ptMax.x,ptMax.y));
	pLine->addVertexAt(3,AcGePoint2d(ptMin.x,ptMax.y));
	pLine->setClosed(Adesk::kTrue);
	AcDbObjectId plId=GCADUtils::AddToCurrentSpace(pLine);
	pLine->close();

	GPlotItemEx item;
	item.m_copies=1;
	item.m_ownerHandle=pDb->currentSpaceId().handle();
	item.m_ptMax=ptMax;
	item.m_ptMin=ptMin;
	double dWidth=ptMax.x-ptMin.x;
	double dHeight=ptMax.y-ptMin.y;
	item.m_ptMax.x+=dWidth*0.05;
	item.m_ptMin.x-=dWidth*0.05;
	item.m_ptMax.y+=dHeight*0.05;
	item.m_ptMin.y-=dHeight*0.05;
	item.m_size=_T("ISO A0 (1189.00 x 841.00 毫米)");
	item.m_strBlockName=_T("");
	item.m_style=_T("无");
	GPlotHelper helper;
	helper.m_strDevice=_T("DXBTrans.cadgj.com.pc3");
	if(!helper.SetupPlotSetting(item,true,false,AcDbPlotSettings::ShadePlotType::kHidden))
	{
		acutPrintf(_T("\n设置DXB文件打印失败"));
		return;
	}
	//开始打印
	IAcadApplication* pAcad = NULL;
	IAcadDocument* pDoc = NULL;
	IAcadPlot* pPlot = NULL;

	HRESULT hr = NOERROR;
	LPUNKNOWN pUnk = NULL;
	LPDISPATCH pAcadDisp = NULL;
	pAcadDisp = acedGetAcadWinApp()->GetIDispatch(TRUE);
	pAcad = (IAcadApplication*)pAcadDisp;
	pAcad->get_ActiveDocument(&pDoc);
	pDoc->get_Plot(&pPlot);

	VARIANT_BOOL succeed = FALSE;
	_variant_t plotConfig = helper.m_strDevice;
	HRESULT plotRlt;
	TCHAR lpFile[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpFile);
	PathAppend(lpFile,_T("DxbTrans.dxb"));
	CString mResultFile=lpFile;
	BSTR name = mResultFile.AllocSysString();
	plotRlt= pPlot->PlotToFile(name, plotConfig, &succeed);
	SysFreeString(name);
	if(plotRlt!=S_OK||(!PathFileExists(lpFile)))
	{
		acutPrintf(_T("\n打印DXB文件失败"));
		return;
	}
	ads_name first;
	ads_name last;
	AcDbObjectId firstId;
	AcDbObjectId lastId; 
	acdbEntLast(first);
	acdbGetObjectId(firstId,first);
#if _MSC_VER>=1700
	int rc=acedCommandS(RTSTR,_T("_.dxbin"),RTSTR,lpFile,RTNONE);
#else
	int rc=acedCommand(RTSTR,_T("_.dxbin"),RTSTR,lpFile,RTNONE);
#endif
	if(rc==RTNORM)
	{
		AcDbObjectIdArray ObjIdArray;
		acdbEntLast(last);
		acdbGetObjectId(lastId,last);
		while(firstId!=lastId)
		{
			if(firstId!=NULL)
				acdbEntNext(first,first);
			else
				acdbEntNext(NULL,first);
			acdbGetObjectId(firstId,first);  
			ObjIdArray.append(firstId);
		}
		double dArea=(ptMax.x-ptMin.x)*(ptMax.y-ptMin.y);
		AcGePoint3d ptMin1;
		AcGePoint3d ptMax1;
		GCADUtils::GetMaxViewPoint(ObjIdArray,ptMin1,ptMax1);
		for(int i=ObjIdArray.logicalLength()-1;i>=0;i--)
		{
			AcDbEntityPointer spEnt(ObjIdArray.at(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			double dScale=min((ptMax.x-ptMin.x)/(ptMax1.x-ptMin1.x),(ptMax.y-ptMin.y)/(ptMax1.y-ptMin1.y));
			AcGePoint3d ptMid=GCADUtils::MidPoint(ptMin,ptMax);
			AcGePoint3d ptMid1=GCADUtils::MidPoint(ptMin1,ptMax1);
			AcGeMatrix3d xform;
			xform.setToTranslation(ptMid-ptMid1);
			spEnt->transformBy(xform);
			xform.setToScaling(dScale,ptMid);
			spEnt->transformBy(xform);
		}

		Exp_JoinLines(ObjIdArray);

		for(int i=ObjIdArray.logicalLength()-1;i>=0;i--)
		{
			AcDbEntityPointer spEnt(ObjIdArray.at(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			if(spEnt->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline* pLine=AcDbPolyline::cast(spEnt.object());
				AcGePoint3d ptMin2;
				AcGePoint3d ptMax2;
				double dArea3=0.0;
				pLine->getArea(dArea3);
				if(dArea3>=0.95*dArea||GFrameBorderHelper::GetRects(ptMin2,ptMax2,pLine,1E-2))
				{
					double dArea2=(ptMax2.x-ptMin2.x)*(ptMax2.y-ptMin2.y);
					double dScale=dArea2/dArea;
					if(dArea3>=0.95*dArea||dScale>=0.9)
					{
						pLine->erase();
						ObjIdArray.removeAt(i);
					}
				}
			}
		}

	}

	for(int i=0;i<ids.logicalLength();i++)
	{
		GCADUtils::EraseEntity(ids.at(i));
	}
	GCADUtils::EraseEntity(plId);
}
//设置实体为不可见的
void ChangeVisible(AcDbObjectIdArray& ids,AcDb::Visibility vi)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->setVisibility(vi);
	}
}
void ChangeVisible(AcDbObjectId& id,AcDb::Visibility vi)
{
	AcDbObjectIdArray ids;
	ids.append(id);
	ChangeVisible(ids,vi);
}
//设置实体为不可见的
void ChangeToUnVisible(AcDbObjectIdArray& ids,AcDbObjectIdArray& changedIds)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->visibility()!=AcDb::kVisible)
		{
			continue;
		}
		spEnt->setVisibility(AcDb::kInvisible);
		changedIds.append(ids.at(i));
	}
}
//文字转线
void TextToPline()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
		return;
	const TCHAR* lpFileName=NULL;
	pDb->getFilename(lpFileName);
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	AcDbObjectIdArray all_ids;
	EnumAllEntityInCurrentSpace(all_ids,pDb);
	for(int i=all_ids.logicalLength()-1;i>=0;i--)
	{
		if(GCADUtils::FindObjectId(ids,all_ids.at(i))>=0)
		{
			all_ids.removeAt(i);
		}
	}
	AcArray<bool> locks;
	AcArray<bool> frozens;
	AcDbObjectIdArray layerIds;
	es=GCADUtils::OpenAllLayer(pDb,layerIds,locks,frozens);
	if(es!=Acad::eOk)
		return;
	AcDbObjectIdArray changedIds;
	ChangeToUnVisible(all_ids,changedIds);
	int nOld=0;
	GCADUtils::GetCADIntVar(_T("backgroundplot"),nOld);
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),0);
	}

	AcDbObjectIdArray changedIds1;
	ChangeToUnVisible(ids,changedIds);
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectIdArray textIds;
		textIds.append(ids.at(i));
		ChangeVisible(textIds,AcDb::kVisible);
		_TextToPline(textIds);
		ChangeVisible(textIds,AcDb::kInvisible);
	}
	ChangeVisible(changedIds1,AcDb::kVisible);
	ChangeVisible(changedIds,AcDb::kVisible);
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),nOld);
	}
	GCADUtils::ResetAllLayer(layerIds,locks,frozens);
}
