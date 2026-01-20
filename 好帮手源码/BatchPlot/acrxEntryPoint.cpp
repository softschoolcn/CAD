// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GBatchPlotDialog.h"
#include "GFrameBorderHelper.h"
#include "GPlotHelper.h"
#include "GTextToPlineCommand.h"
#include "CommFunction.h"
#include "GTransConfigDialog.h"
#include "acadi.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

#include "GFrameBorderHelper.h"
void AddToGroup(CArray<CArray<GPlotItemEx>*>& groups,GPlotItemEx& item)
{
	for(int i=0;i<groups.GetCount();i++)
	{
		CArray<GPlotItemEx>* group=groups.GetAt(i);
		if(group->GetCount()==0)
		{
			continue;
		}
		if(group->GetAt(0).m_strFile.CompareNoCase(item.m_strFile)==0)
		{
			group->Add(item);
			return;
		}
	}
	CArray<GPlotItemEx>* group1=new CArray<GPlotItemEx>;
	group1->Add(item);
	groups.Add(group1);
}
void MakeGroup(CArray<CArray<GPlotItemEx>*>& groups,CArray<GPlotItemEx>& items)
{
	for(int i=0;i<items.GetCount();i++)
	{
		AddToGroup(groups,items.GetAt(i));
	}
}
CString GetFileName(CArray<GPlotItemEx>* group,int i,GBatchPlotDialog* dlg)
{
	GPlotItemEx item=group->GetAt(i);
	TCHAR m_lpfile[MAX_PATH+1]={0};
	if(item.m_strFile.IsEmpty())
	{
		CString strDwgFileName;
		GetCurDwgFileName(strDwgFileName);
		lstrcpy(m_lpfile,strDwgFileName);
	}
	else
	{
		lstrcpy(m_lpfile,item.m_strFile);
	}
	CString strTmp=PathFindFileName(m_lpfile);
	lstrcpy(m_lpfile,strTmp);
	PathRemoveExtension(m_lpfile);
	int nCount=group->GetCount();
	CString strFile;
	if(nCount>=0&&nCount<=9)
	{
		strFile.Format(_T("%s_%d.%s"),m_lpfile,i+1,dlg->m_strExt);
	}
	else if(nCount>=10&&nCount<=99)
	{
		strFile.Format(_T("%s_%.2d.%s"),m_lpfile,i+1,dlg->m_strExt);
	}
	else if(nCount>=100&&nCount<=999)
	{
		strFile.Format(_T("%s_%.3d.%s"),m_lpfile,i+1,dlg->m_strExt);
	}
	else
	{
		strFile.Format(_T("%s_%d.%s"),m_lpfile,i+1,dlg->m_strExt);
	}
	return strFile;
}
void BatchPlot(CStringArray& resultFileNameArray,CArray<GPlotItemEx>* group,GBatchPlotDialog* dlg)
{
	GPlotHelper helper;
	helper.m_strDevice=dlg->m_strDevice;
	for(int i=0;i<group->GetCount();i++)
	{
		GPlotItemEx item=group->GetAt(i);
		helper.SetupPlotSetting(item,true,dlg->m_bAllow90);
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
		if(dlg->m_bPlotToFile)
		{
			CString mResultFile=GetFileName(group,i,dlg);
			TCHAR lpFile[MAX_PATH+1]={0};
			lstrcpy(lpFile,dlg->m_strPath);
			PathAppend(lpFile,mResultFile);
			mResultFile=lpFile;
			resultFileNameArray.Add(mResultFile);
			BSTR name = mResultFile.AllocSysString();
			plotRlt= pPlot->PlotToFile(name, plotConfig, &succeed);
			SysFreeString(name);
		}
		else
		{
			for(int j=0;j<item.m_copies;j++)
			{
				plotRlt= pPlot->PlotToDevice(plotConfig, &succeed);
			}
		}

	}
}
//合并PDF文件
void HBPDF(CStringArray& strFileArray)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("bin\\pdftk.exe"));//支持路径
	CString strExe=lpPath;
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("bin"));//支持路径
	CString strArg;
	TCHAR lpPath2[MAX_PATH]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("合并结果.pdf"));
	DeleteFile(lpPath2);
	for(int i=0;i<strFileArray.GetCount();i++)
	{
		TCHAR lpPath1[MAX_PATH]={0};
		lstrcpy(lpPath1,lpPath);
		PathAppend(lpPath1,PathFindFileName(strFileArray.GetAt(i)));
		CopyFile(strFileArray.GetAt(i),lpPath1,FALSE);
		strArg.AppendFormat(_T("\"%s\" "),PathFindFileName(strFileArray.GetAt(i)));
	}
	strArg.AppendFormat(_T("output \"合并结果.pdf\""));
	CString strRun;
	strRun.Format(_T("\"%s\" %s"),strExe,strArg);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if(!CreateProcess(NULL,   // No module name (use command line). 
		strRun.GetBuffer(), // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		lpPath,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 

	{
		//todo log
		//
		DeleteFile(lpPath2);
		for(int i=0;i<strFileArray.GetCount();i++)
		{
			TCHAR lpPath1[MAX_PATH]={0};
			lstrcpy(lpPath1,lpPath);
			PathAppend(lpPath1,PathFindFileName(strFileArray.GetAt(i)));
			DeleteFile(lpPath1);
		}
		return;
	}
	strRun.ReleaseBuffer();
	WaitForSingleObject(pi.hProcess,INFINITE);
	TCHAR lpPath3[MAX_PATH+1]={0};
	lstrcpy(lpPath3,strFileArray.GetAt(0));
	PathRemoveFileSpec(lpPath3);
	PathAppend(lpPath3,_T("合并结果.pdf"));
	CopyFile(lpPath2,lpPath3,FALSE);
	
	DeleteFile(lpPath2);
	for(int i=0;i<strFileArray.GetCount();i++)
	{
		TCHAR lpPath1[MAX_PATH]={0};
		lstrcpy(lpPath1,lpPath);
		PathAppend(lpPath1,PathFindFileName(strFileArray.GetAt(i)));
		DeleteFile(lpPath1);
	}

}
//90:批量打印
void BatchPlotEx()
{
	if(!CheckReg(_T("20140223")))
	{
		return;
	}
	CAcModuleResourceOverride res;
	GBatchPlotDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	int nOld=0;
	GCADUtils::GetCADIntVar(_T("backgroundplot"),nOld);
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),0);
	}
	CStringArray resultFileNameArray;
	CArray<CArray<GPlotItemEx>*> groups;
	MakeGroup(groups,dlg.m_plotItemExs);
	for(int i=0;i<groups.GetCount();i++)
	{
		CArray<GPlotItemEx>* group=groups.GetAt(i);
		BatchPlot(resultFileNameArray,group,&dlg);
		delete group;
		group=NULL;
		groups.SetAt(i,NULL);
	}
	groups.RemoveAll();
	if(dlg.m_strExt.CompareNoCase(_T("pdf"))==0)
	{
		if(dlg.m_bJoinPDF)
		{
			HBPDF(resultFileNameArray);
			if(dlg.m_bDelMidPdf)
			{
				for(int i=0;i<resultFileNameArray.GetCount();i++)
				{
					DeleteFile(resultFileNameArray.GetAt(i));
				}
			}
		}
	}
	if(nOld!=0)
	{
		GCADUtils::SetCADIntVar(_T("backgroundplot"),nOld);
	}
}
///////////////////////////////////////////////////////////////////////////////
//格式转换
GTransConfigDialog * pGTransConfigDialog = NULL;
Adesk::Boolean createGTransConfigDialog()
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) 
	{
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}
	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( pGTransConfigDialog == NULL ) 
	{
		if ( (pGTransConfigDialog = new GTransConfigDialog ( pWnd )) == NULL ) 
		{
			return Adesk::kFalse;
		}

		BOOL succeeded = pGTransConfigDialog->Create ( GTransConfigDialog::IDD,pWnd );
		if ( !succeeded ) 
		{
			return Adesk::kFalse;
		}
	}
	pGTransConfigDialog->CenterWindow();
	return Adesk::kTrue;
}
void showGTransConfigDialog()
{
	//if( pDlgReadInfo ) {
	//	// dialog was already created, just display it
	//	// (the dialog only gets deallocated when docman.arx is unloaded)
	//	pDlgReadInfo->ShowWindow( SW_SHOWNORMAL );
	//	return;
	//}
	if( pGTransConfigDialog ) 
	{
		pGTransConfigDialog->DestroyWindow();
		delete pGTransConfigDialog;
		pGTransConfigDialog = NULL;
	}
	if( !pGTransConfigDialog && !createGTransConfigDialog() ) 
	{
		ads_printf( _T("Failed to create modeless dialog box.\n") );
	}
	else
	{
		pGTransConfigDialog->ShowWindow(SW_SHOWNORMAL);		
	}
}
void destroyGTransConfigDialog()
{
	if( pGTransConfigDialog )
	{
		pGTransConfigDialog->DestroyWindow();
		delete pGTransConfigDialog;
		pGTransConfigDialog = NULL;
	}
}
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CBatchPlotApp : public AcRxArxApp {

public:
	CBatchPlotApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		//90:批量打印
		acedRegCmds->addCommand(_T("BatchPlot") ,
			_T("cadgj_BatchPlot") , 
			_T("cadgj_BatchPlot") ,
			ACRX_CMD_MODAL|ACRX_CMD_SESSION ,
			BatchPlotEx);
		//29:文字转线
	acedRegCmds->addCommand(_T("BatchPlot") ,
		_T("TextToPline") , 
		_T("TextToPline") ,
		ACRX_CMD_MODAL,
		TextToPline);

	//格式转换
	acedRegCmds->addCommand(_T("BatchPlot"),
			_T("DwgTrans"),
			_T("GSZH"),
			ACRX_CMD_MODAL,
			showGTransConfigDialog);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here

		acedRegCmds->removeGroup(_T("BatchPlot"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CBatchPlotApp)

