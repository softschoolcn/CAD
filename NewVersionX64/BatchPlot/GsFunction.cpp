#include "StdAfx.h"
#include "GsFunction.h"
#include "acgi.h"
#include <math.h>
#include "gs.h"
#include "AcGsManager.h"
#include "imgDef.h"
#include "CommFunction.h"
#include "GPlotHelper.h"
#include "GPlotDpiConfig.h"
#include "acadi.h"
// get the current vp

int getCVPort()

{

	struct resbuf rb;

	ads_getvar(_T("CVPORT"), &rb);

	return rb.resval.rint;

}



void BackColor(AcDbObjectIdArray& changeColorIds,AcArray<int>& changeColorArray)
{
	for (int i=0;i<changeColorIds.logicalLength();i++)
	{
		AcDbObjectId id=changeColorIds.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()==Acad::eOk)
		{
			spEnt->setColorIndex(changeColorArray.at(i));
		}
	}
}
CString GetMText(AcDbMText* pText)
{
	CString text("");
	AcDbVoidPtrArray ents;
	Acad::ErrorStatus es = pText->explode(ents);
	if (es == Acad::eOk)
	{
		int len = ents.length();
		for (int i = 0; i < len; i++)
		{
			AcDbText* pText = AcDbText::cast((AcDbEntity*)ents.at(i));
			if (pText != NULL)
			{
				if(i!=0)
				{
					text += _T("\n");
				}
				text += pText->textString();
				delete pText;
			}
		}
	}
	return text;
}
bool IsEmptyEntity(AcDbEntity *pEnt)
{
	if(pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=AcDbText::cast(pEnt);
		CString strText=pText->textString();
		if(strText.IsEmpty())
		{
			return true;
		}
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText=AcDbMText::cast(pEnt);
		CString strText=GetMText(pMText);
		if(strText.IsEmpty())
		{
			return true;
		}

	}
	return false;
}
int RemoveEmptyEntitys(AcDbObjectIdArray& ids)
{
	AcDbObjectIdArray rc_ids;
	for(int nLen=0;nLen<ids.logicalLength();nLen++)
	{
		AcDbEntity *pEnt=NULL;
		AcDbExtents ex;
		if(acdbOpenObject(pEnt,ids[nLen],AcDb::kForRead)==Acad::eOk)
		{
			if(!IsEmptyEntity(pEnt))
			{
				rc_ids.append(pEnt->objectId());
			}
			pEnt->close();				 
			pEnt=NULL;
		}
	}
	int nCount=ids.logicalLength()-rc_ids.logicalLength();
	ids.setLogicalLength(0);
	ids.append(rc_ids);
	return nCount;

}
Acad::ErrorStatus GetExtents(AcDbExtents& exAll,AcDbObjectIdArray& ids)
{
	if(ids.logicalLength()==0)
	{
		return eNullExtents;
	}
	Acad::ErrorStatus es=eNullExtents;
	for(int nLen=0;nLen<ids.logicalLength();nLen++)
	{
		AcDbEntity *pEnt=NULL;
		AcDbExtents ex;
		if(acdbOpenObject(pEnt,ids[nLen],AcDb::kForRead)==Acad::eOk)
		{
			if(pEnt->getGeomExtents(ex)!=Acad::eOk)
			{
				pEnt->close();
				continue;
			}
			es=Acad::eOk;
			pEnt->close();				 
			pEnt=NULL;
			exAll.addExt(ex);
		}
	}
	return es;
}
Acad::ErrorStatus GetExtents(AcDbObjectIdArray& ids,AcDbExtents& exts,GTransConfig* pConfig)
{
	if(pConfig->m_EntitysMode==kTransEntitysAll)
	{
		AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTableRecordPointer spRec(pDb->currentSpaceId(),AcDb::kForRead);
		if(spRec.openStatus()!=Acad::eOk)
			return spRec.openStatus();
		AcDbBlockTableRecordIterator* pIterator=NULL;
		Acad::ErrorStatus es=spRec->newIterator(pIterator);
		if(es!=Acad::eOk)
		{
			return Acad::eOk;
		}
		for(;!pIterator->done();pIterator->step())
		{
			AcDbObjectId id;
			if(pIterator->getEntityId(id)!=Acad::eOk)
			{
				continue;
			}
			ids.append(id);
		}
		delete pIterator;
		pIterator=NULL;
		RemoveEmptyEntitys(ids);
		return GetExtents(exts,ids);
	}
	else if(pConfig->m_EntitysMode==kTransEntitysEnts)
	{
		ids.append(pConfig->m_ids);
		RemoveEmptyEntitys(ids);
		return GetExtents(exts,ids);
	}
	else if(pConfig->m_EntitysMode==kTransEntitysExtents)
	{
		exts=pConfig->m_ext;
		/*
		ads_name ss;
		AcGePoint3d p1=exts.minPoint();
		AcGePoint3d p2=exts.maxPoint();
		int result = acedSSGet(_T("C"), asDblArray(AcGePoint3d(p1[0], p1[1], p1[2])), asDblArray(AcGePoint3d(p2[0], p2[1], p2[2])), NULL, ss);
		SSToIds(ss,ids);
		acedSSFree(ss);
		RemoveEmptyEntitys(ids);
		*/
	}
	else if(pConfig->m_EntitysMode==kTransEntitysScreen)
	{
		AcGePoint3d leftBottom;
		AcGePoint3d rightTop;
		CRect Region;
		getViewExtent(leftBottom,rightTop,Region);
		double dMinX=min(leftBottom.x,rightTop.x);
		double dMaxX=max(leftBottom.x,rightTop.x);
		double dMinY=min(leftBottom.y,rightTop.y);
		double dMaxY=max(leftBottom.y,rightTop.y);
		exts.set(AcGePoint3d(dMinX,dMinY,0.0),AcGePoint3d(dMaxX,dMaxY,0.0));
		ads_name ss;
		AcGePoint3d p1=exts.minPoint();
		AcGePoint3d p2=exts.maxPoint();
		int result = acedSSGet(_T("C"), asDblArray(AcGePoint3d(p1[0], p1[1], p1[2])), asDblArray(AcGePoint3d(p2[0], p2[1], p2[2])), NULL, ss);
		SSToIds(ss,ids);
		acedSSFree(ss);
		RemoveEmptyEntitys(ids);

	}
	return Acad::eOk;
}
CString GetPaper(int& nPaperWidth,int& nPaperHeight,GTransPaperMode mode,double dWidth,double dHeight)
{
	CString strPaperSizeArray[10]={
		_T("ISO A0 (1189.00 x 841.00 毫米)"),
		_T("ISO A0 (841.00 x 1189.00 毫米)"),
		_T("ISO A1 (841.00 x 594.00 毫米)"),
		_T("ISO A1 (594.00 x 841.00 毫米)"),
		_T("ISO A2 (594.00 x 420.00 毫米)"),
		_T("ISO A2 (420.00 x 594.00 毫米)"),
		_T("ISO A3 (420.00 x 297.00 毫米)"),
		_T("ISO A3 (297.00 x 420.00 毫米)"),
		_T("ISO A4 (297.00 x 210.00 毫米)"),
		_T("ISO A4 (210.00 x 297.00 毫米)")};
		int nPaperWidthArray[10]={1189,841,841,594,594,420,420,297,297,210};
		int nPaperHeigthArray[10]={841,1189,594,841,420,594,297,420,210,297};
		int index=2*mode+(dHeight>dWidth?1:0);
		nPaperWidth=nPaperWidthArray[index];
		nPaperHeight=nPaperHeigthArray[index];
		return strPaperSizeArray[index];
}
BOOL TransEpsTypes(CString resultFile,CString newType,CString newFile,int nPaperWidth,int nPaperHeight)
{
	GPlotDpiConfig dpiConfig;
	dpiConfig.Read();
	TCHAR tmpNewFileEPS[MAX_PATH]={0};
	lstrcpy(tmpNewFileEPS,resultFile);

	TCHAR _tmpNewFile[MAX_PATH]={0};
	lstrcpy(_tmpNewFile,resultFile);
	PathRenameExtension(_tmpNewFile,_T(".")+newType);
	CString strTmp=tmpNewFileEPS;
	CString tmpNewFile=_tmpNewFile;

	CString title;
	title.Format(_T("转换%s文件"), newType);
	CString content = newFile;
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("3rd\\gs"));//支持路径
	CString strSupport=lpPath;
	PathAppend(lpPath,_T("gs.exe"));

	CString cmd;
	if (newType.CompareNoCase(_T("pdf")) == 0)
	{
		float print_xdpi=dpiConfig.PdfDpiConfig.print_xdpi;
		float print_ydpi=dpiConfig.PdfDpiConfig.print_ydpi;
		int width, height;
		int widthpt, heightpt;
		const TCHAR *device=_T("pdfwrite");
		double widthmm=nPaperWidth;//毫米
		double heightmm=nPaperHeight;//毫米
		widthpt =(unsigned int)(widthmm*72/25.4+0.5);
		heightpt = (unsigned int)(heightmm*72/25.4+0.5);
		width  = (unsigned int)(widthmm  / 25.4 * print_xdpi + 0.5);
		height = (unsigned int)(heightmm / 25.4 * print_ydpi + 0.5);
		CString strTmp=_T("");
		strTmp.AppendFormat(_T("-I\"%s\" "),strSupport);
		strTmp+=_T("-q -DQUIET -dBATCH -dNOPAUSE -dNOPROMPT ");
		strTmp.AppendFormat(_T("-sDEVICE=%s "),device);
		strTmp.AppendFormat(_T("-dDEVICEXRESOLUTION=%g "), (double)print_xdpi);
		strTmp.AppendFormat(_T("-dDEVICEYRESOLUTION=%g "), (double)print_ydpi);

		strTmp.AppendFormat(_T("-dDEVICEWIDTH=%u "), width);
		strTmp.AppendFormat(_T("-dDEVICEHEIGHT=%u "), height);
		strTmp+=_T("-dCompatibilityLevel=1.2 ");
		strTmp.AppendFormat(_T("-sOutputFile=%s "), tmpNewFile);
		int fixed_media=0;
		strTmp.AppendFormat(_T("-c \"<< /Policies << /PageSize %d >> /PageSize [%d %d] "),(fixed_media == 1) ? 5 : 3, widthpt, heightpt);
		strTmp.AppendFormat(_T("/InputAttributes currentpagedevice /InputAttributes get mark exch {1 index /Priority eq not {pop << /PageSize [%d %d] >>} if } forall >> >> setpagedevice\" -f "),widthpt, heightpt);
		strTmp+=_T("-c .setpdfwrite -f ");
		strTmp.Append(tmpNewFileEPS);
		cmd=strTmp;
		// -dDEVICEWIDTHPOINTS=297 -dDEVICEHEIGHTPOINTS=841 
		//cmd.Format(_T("-I\"%s\" -dBATCH -dSAFER -DNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=%s -c -dEPSFitPage -dPDFFitPage -f %s"), strSupport,tmpNewFile, tmpNewFileEPS);
	}
	else if (newType.CompareNoCase(_T("tif")) == 0)
	{
		float print_xdpi=dpiConfig.TifDpiConfig.print_xdpi;
		float print_ydpi=dpiConfig.TifDpiConfig.print_ydpi;
		int width, height;
		int widthpt, heightpt;
		const TCHAR *device=_T("tifflzw");
		double widthmm=nPaperWidth;//毫米
		double heightmm=nPaperHeight;//毫米
		widthpt =(unsigned int)(widthmm*72/25.4+0.5);
		heightpt = (unsigned int)(heightmm*72/25.4+0.5);
		width  = (unsigned int)(widthmm  / 25.4 * print_xdpi + 0.5);
		height = (unsigned int)(heightmm / 25.4 * print_ydpi + 0.5);


		CString strTmp=_T("");
		strTmp.AppendFormat(_T("-I\"%s\" "),strSupport);
		strTmp+=_T("-q -DQUIET -dBATCH -dNOPAUSE -dNOPROMPT ");
		strTmp.AppendFormat(_T("-sDEVICE=%s "),device);
		strTmp.AppendFormat(_T("-dDEVICEXRESOLUTION=%g "), (double)print_xdpi);
		strTmp.AppendFormat(_T("-dDEVICEYRESOLUTION=%g "), (double)print_ydpi);

		strTmp.AppendFormat(_T("-dDEVICEWIDTH=%u "), width);
		strTmp.AppendFormat(_T("-dDEVICEHEIGHT=%u "), height);
		strTmp+=_T("-dAdjustWidth=0 ");
		strTmp+=_T("-dMaxStripSize=65536 ");
		strTmp.AppendFormat(_T("-sOutputFile=%s "), tmpNewFile);

		int fixed_media=0;
		strTmp.AppendFormat(_T("-c \"<< /Policies << /PageSize %d >> /PageSize [%d %d] "),(fixed_media == 1) ? 5 : 3, widthpt, heightpt);
		strTmp.AppendFormat(_T("/InputAttributes currentpagedevice /InputAttributes get mark exch {1 index /Priority eq not {pop << /PageSize [%d %d] >>} if } forall >> >> setpagedevice\" -f "),widthpt, heightpt);
		strTmp.Append(tmpNewFileEPS);
		cmd=strTmp;
		//cmd.Format(_T("-dDEVICEWIDTHPOINTS=%d -dDEVICEHEIGHTPOINTS=%d  -dEPSFitPage -sDEVICE=tiffg32d -sOutputFile=%s %s"), (int)GPlot::GetInstance()->m_dPaperWidth,(int)GPlot::GetInstance()->m_dPaperHeight,tmpNewFile, tmpNewFileEPS);
	}
	else if (newType.CompareNoCase(_T("jpg")) == 0)
	{
		float print_xdpi=dpiConfig.JpgDpiConfig.print_xdpi;
		float print_ydpi=dpiConfig.JpgDpiConfig.print_ydpi;
		int width, height;
		int widthpt, heightpt;
		const TCHAR *device=_T("jpeg");
		double widthmm=nPaperWidth;//毫米
		double heightmm=nPaperHeight;//毫米
		widthpt =(unsigned int)(widthmm*72/25.4+0.5);
		heightpt = (unsigned int)(heightmm*72/25.4+0.5);
		width  = (unsigned int)(widthmm  / 25.4 * print_xdpi + 0.5);
		height = (unsigned int)(heightmm / 25.4 * print_ydpi + 0.5);
		CString strTmp=_T("");
		strTmp.AppendFormat(_T("-I\"%s\" "),strSupport);
		strTmp+=_T("-q -DQUIET -dBATCH -dNOPAUSE -dNOPROMPT ");
		strTmp.AppendFormat(_T("-sDEVICE=%s "),device);
		strTmp.AppendFormat(_T("-dDEVICEXRESOLUTION=%g "), (double)print_xdpi);
		strTmp.AppendFormat(_T("-dDEVICEYRESOLUTION=%g "), (double)print_ydpi);

		strTmp.AppendFormat(_T("-dDEVICEWIDTH=%u "), width);
		strTmp.AppendFormat(_T("-dDEVICEHEIGHT=%u "), height);
		strTmp.AppendFormat(_T("-sOutputFile=%s "), tmpNewFile);
		int fixed_media=0;
		strTmp.AppendFormat(_T("-c \"<< /Policies << /PageSize %d >> /PageSize [%d %d] "),(fixed_media == 1) ? 5 : 3, widthpt, heightpt);
		strTmp.AppendFormat(_T("/InputAttributes currentpagedevice /InputAttributes get mark exch {1 index /Priority eq not {pop << /PageSize [%d %d] >>} if } forall >> >> setpagedevice\" -f "),widthpt, heightpt);
		strTmp.Append(tmpNewFileEPS);
		cmd=strTmp;
		//cmd.Format(_T("-I\"%s\" -dSAFER -dBATCH -dNOPAUSE  -dFirstPage=1 -dLastPage=1 -sDEVICE=jpeg -dGraphicsAlphaBits=4 -sOutputFile=%s %s"), strSupport,tmpNewFile, tmpNewFileEPS);
	}
	else
	{
		return FALSE;
	}
	CString strTmp1;
	strTmp1.Format(_T("\"%s\" %s"),lpPath,cmd);
	GLog::WriteLine(_T("执行类型转换，命令行参数:%s"), strTmp1);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if(!CreateProcess(NULL,   // No module name (use command line). 
		strTmp1.GetBuffer(), // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 

	{
		//todo log
		//
		GLog::WriteLine(_T("调用gs.exe执行类型转换失败:%d"), ::GetLastError());
		return FALSE;
	}
	strTmp1.ReleaseBuffer();
	WaitForSingleObject(pi.hProcess,INFINITE);
	title += _T("完成");
	CopyFile(tmpNewFile,newFile,FALSE);
	DeleteFile(tmpNewFile);

	GLog::WriteLine(_T("完成类型转换，类型:%s"), newType);
	return TRUE;
}
void SetVisibility(AcDbObjectIdArray& ids,AcDb::Visibility newVal)
{
	for(int i=ids.logicalLength()-1;i>=0;i--)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->setVisibility(newVal);
	}
}
//使用GS输出
BOOL _GsOutput(GTransConfig* pConfig,CString& strErrorInfo)
{

	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbObjectIdArray ids;
	AcDbExtents exts;
	Acad::ErrorStatus es=GetExtents(ids,exts,pConfig);
	if(es!=eOk)
	{
		strErrorInfo.Format(_T("获取范围失败:%s"),acadErrorStatusText(es));
		return FALSE;
	}

	AcDbObjectIdArray all_ids;
	if(pConfig->m_EntitysMode==kTransEntitysEnts)
	{
		EnumAllEntityInCurrentSpace(all_ids,pDb);
		for(int i=all_ids.logicalLength()-1;i>=0;i--)
		{
			if(ids.contains(all_ids.at(i)))
			{
				all_ids.removeAt(i);
			}
		}
		SetVisibility(all_ids,AcDb::kInvisible);
	}

	double dMinX,dMinY,dMaxX,dMaxY;
	if(pConfig->m_EntitysMode==kTransEntitysAll||pConfig->m_EntitysMode==kTransEntitysEnts)
	{
		AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
		dMinX=max(pDb->extmin().x,exts.minPoint().x);
		dMinY=max(pDb->extmin().y,exts.minPoint().y);
		dMaxX=min(pDb->extmax().x,exts.maxPoint().x);
		dMaxY=min(pDb->extmax().y,exts.maxPoint().y);
		exts.set(AcGePoint3d(dMinX,dMinY,0.0),AcGePoint3d(dMaxX,dMaxY,0.0));
	}
	else
	{
		dMinX=exts.minPoint().x;
		dMinY=exts.minPoint().y;
		dMaxX=exts.maxPoint().x;
		dMaxY=exts.maxPoint().y;
	}

	AcGePoint3d ptMin=exts.minPoint();
	AcGePoint3d ptMax=exts.maxPoint();

	GPlotItemEx item;
	item.m_copies=1;
	item.m_ownerHandle=pDb->currentSpaceId().handle();
	item.m_ptMax=ptMax;
	item.m_ptMin=ptMin;
	double dWidth=ptMax.x-ptMin.x;
	double dHeight=ptMax.y-ptMin.y;
	int nPaperWidth;
	int nPaperHeight;
	item.m_size=GetPaper(nPaperWidth,nPaperHeight,pConfig->m_PaperMode,dWidth,dHeight);
	item.m_strBlockName=_T("");
	item.m_style=_T("无");
	GPlotHelper helper;
	helper.m_strDevice=_T("EPSTrans.cadgj.com.pc3");
	if(!helper.SetupPlotSetting(item,true,false,AcDbPlotSettings::kWireframe))
	{
		acutPrintf(_T("\n设置EPS文件打印失败"));
		return FALSE;
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
	PathAppend(lpFile,_T("EpsTrans.eps"));
	CString mResultFile=lpFile;
	BSTR name = mResultFile.AllocSysString();
	plotRlt= pPlot->PlotToFile(name, plotConfig, &succeed);
	SysFreeString(name);
	SetVisibility(all_ids,AcDb::kVisible);
	if(plotRlt!=S_OK||(!PathFileExists(lpFile)))
	{
		if(plotRlt!=S_OK)
		{
			GLog::WriteLine(_T("_GsOutput::plotRlt=0x%x"),plotRlt);
		}
		if(!PathFileExists(lpFile))
		{
			GLog::WriteLine(_T("_GsOutput::未生成结果文件:%s"),lpFile);
		}
		acutPrintf(_T("\n打印EPS文件失败"));
		return FALSE;
	}
	for(int i=0;i<pConfig->m_TypeArray.GetCount();i++)
	{
		CString strType=pConfig->m_TypeArray.GetAt(i);
		CString strFileName;
		strFileName.Format(_T("%s.%s"),pConfig->m_strFileName,strType);
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,pConfig->m_strFloder);
		PathAppend(lpPath,strFileName);
		strFileName=lpPath;
		if(strType.CompareNoCase(_T("eps"))==0)
		{
			CopyFile(mResultFile,strFileName,FALSE);
		}
		else
		{
			TransEpsTypes(mResultFile,strType,strFileName,nPaperWidth,nPaperHeight);
		}
	}

	return TRUE;
}
BOOL GsOutput(GTransConfig* pConfig,CString& strErrorInfo)
{
	int nOld=0;
	GCADUtils::GetCADIntVar(_T("backgroundplot"),nOld);
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),0);
	}
	AcDbObjectIdArray ids;
	AcArray<bool> locks;
	AcArray<bool> frozens;
	OpenAllLayer(ids,locks,frozens);
	GSameColorHelper helper;
	CArray<GColorRecord> colorRecordArr;
	if(pConfig->m_bBlackWhite)
	{
		helper.SetAllEntityInSameColor(colorRecordArr,7);
	}
	BOOL rc=_GsOutput(pConfig,strErrorInfo);
	if(pConfig->m_bBlackWhite)
	{
		GColorRecord::Rollback(colorRecordArr);
	}
	ResetAllLayer(ids,locks,frozens);
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),nOld);
	}
	return rc;
}