// (C) Copyright 2002-2005 by Autodesk, Inc. 
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
//----- GTransConfigDialog.cpp : Implementation of GTransConfigDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GTransConfigDialog.h"
#include "CommFunction.h"
#include "GsFunction.h"
#include "GLockDocument.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (GTransConfigDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(GTransConfigDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &GTransConfigDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &GTransConfigDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &GTransConfigDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &GTransConfigDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &GTransConfigDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &GTransConfigDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK6, &GTransConfigDialog::OnBnClickedCheck6)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
GTransConfigDialog::GTransConfigDialog (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (GTransConfigDialog::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void GTransConfigDialog::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT GTransConfigDialog::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
BOOL GTransConfigDialog::OnInitDialog()
{
	BOOL rc=CAcUiDialog::OnInitDialog();
	m_config.Read();
	ShowConfig(&m_config);

	int nId=IDC_RADIO_A0;
	if(m_config.m_PaperMode==kISOAOPaper)
	{
		nId=IDC_RADIO_A0;
	}
	else if(m_config.m_PaperMode==kISOA1Paper)
	{
		nId=IDC_RADIO_A1;
	}
	else if(m_config.m_PaperMode==kISOA2Paper)
	{
		nId=IDC_RADIO_A2;
	}
	else if(m_config.m_PaperMode==kISOA3Paper)
	{
		nId=IDC_RADIO_A3;
	}
	else if(m_config.m_PaperMode==kISOA4Paper)
	{
		nId=IDC_RADIO_A4;
	}
	((CButton*)GetDlgItem(nId))->SetCheck(TRUE);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());

	return rc;
}
//显示配置
void GTransConfigDialog::ShowConfig(GTransConfig* pConfig)
{
	//转换格式
	CArray<int> mItemIdArray;
	CStringArray mTypeArray;
	InitTypeItems(mItemIdArray,mTypeArray);
	for(int i=0;i<mItemIdArray.GetCount();i++)
	{
		CString strType=mTypeArray.GetAt(i);
		CButton* pBtn=(CButton*)GetDlgItem(mItemIdArray.GetAt(i));
		if(pBtn==NULL)
			continue;
		if(IsStringInArray(pConfig->m_TypeArray,strType,true))
		{
			pBtn->SetCheck(TRUE);
		}
		else
		{
			pBtn->SetCheck(FALSE);
		}
	}
	//颜色设置
	CButton* pBtn_BlackWhite=(CButton*)GetDlgItem(IDC_CHECK6);
	pBtn_BlackWhite->SetCheck(pConfig->m_bBlackWhite);

	//输出对象
	ShowEntityMode(pConfig->m_EntitysMode);
	SetDlgItemText(IDC_EDIT4,pConfig->m_strFloder);
	SetDlgItemText(IDC_EDIT5,pConfig->m_strFileName);
}
void GTransConfigDialog::EnableItems(CArray<int>& enableIds,BOOL bEnable)
{
	for(long i=0;i<enableIds.GetCount();i++)
	{
		int nId=enableIds.GetAt(i);
		CWnd* pWnd=GetDlgItem(nId);
		if(pWnd==NULL)
			continue;
		pWnd->EnableWindow(bEnable);
	}
}
void GTransConfigDialog::ShowEntityMode(GTransEntitysMode setMode)
{
	CArray<int> mItemIdArray2;
	CArray<GTransEntitysMode> mModeArray2;
	InitEntitysModeItems(mItemIdArray2,mModeArray2);
	for(int i=0;i<mItemIdArray2.GetCount();i++)
	{
		GTransEntitysMode mode=mModeArray2.GetAt(i);
		CButton* pBtn=(CButton*)GetDlgItem(mItemIdArray2.GetAt(i));
		if(pBtn==NULL)
			continue;
		if(mode==setMode)
		{
			CString strText;
			pBtn->GetWindowText(strText);
			strText=_T("当前模式:")+strText;
			GetDlgItem(IDC_STATIC_ENTMODE)->SetWindowText(strText); 
		}

	}
}
void GTransConfigDialog::InitTypeItems(CArray<int>& mItemIdArray,CStringArray& mTypeArray)
{
	mItemIdArray.Add(IDC_CHECK1);
	mTypeArray.Add(_T("jpg"));
	mItemIdArray.Add(IDC_CHECK3);
	mTypeArray.Add(_T("tif"));
	mItemIdArray.Add(IDC_CHECK5);
	mTypeArray.Add(_T("pdf"));
	mItemIdArray.Add(IDC_CHECK7);
	mTypeArray.Add(_T("eps"));
}
void GTransConfigDialog::InitEntitysModeItems(CArray<int>& mItemIdArray,CArray<GTransEntitysMode>& mModeArray)
{
	mItemIdArray.Add(IDC_BUTTON1);
	mModeArray.Add(kTransEntitysScreen);
	mItemIdArray.Add(IDC_BUTTON2);
	mModeArray.Add(kTransEntitysExtents);
	mItemIdArray.Add(IDC_BUTTON3);
	mModeArray.Add(kTransEntitysEnts);
	mItemIdArray.Add(IDC_BUTTON4);
	mModeArray.Add(kTransEntitysAll);
}

void GTransConfigDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CAD屏幕范围
	m_config.m_EntitysMode=kTransEntitysScreen;
	ShowEntityMode(m_config.m_EntitysMode);
	m_config.m_bGetExt=false;
}

void GTransConfigDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//所有对象
	m_config.m_EntitysMode=kTransEntitysAll;
	ShowEntityMode(m_config.m_EntitysMode);
	m_config.m_bGetExt=false;
}

void GTransConfigDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择对象
	m_config.m_EntitysMode=kTransEntitysEnts;
	ShowEntityMode(m_config.m_EntitysMode);
	BeginEditorCommand();
	ads_name ssName;
	if (acedSSGet(NULL, NULL, NULL, NULL, ssName) != RTNORM)
	{
		CompleteEditorCommand();
		return;
	}
	CompleteEditorCommand();
	m_config.m_ids.setLogicalLength(0);
	SSToIds(ssName,m_config.m_ids);
	acedSSFree(ssName);
	m_config.m_bGetExt=false;
}

void GTransConfigDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取两点坐标
	m_config.m_bGetExt=false;
	m_config.m_EntitysMode=kTransEntitysExtents;
	m_config.m_ext.set(AcGePoint3d(0,0,0),AcGePoint3d(0,0,0));
	ShowEntityMode(m_config.m_EntitysMode);
	BeginEditorCommand();
	ads_point p1, p2;  
	if (acedGetPoint(NULL, _T("\n>>指定第一个角点: \n"), p1) != RTNORM)
	{
		CompleteEditorCommand();
		return;
	}

	if (acedGetCorner(p1, _T("\n>>指定第二个角点: \n"), p2) != RTNORM) 
	{
		CompleteEditorCommand();
		return;
	}

	// 矩形框合法性检查
	if (p1[0] == p2[0] || p1[1] == p2[1]) 
	{
		acutPrintf(_T("\n>>矩形框范围不合理"));
		CompleteEditorCommand();
		return;
	}
	double dMinX=min(p1[X],p2[X]);
	double dMaxX=max(p1[X],p2[X]);
	double dMinY=min(p1[Y],p2[Y]);
	double dMaxY=max(p1[Y],p2[Y]);
	m_config.m_ext.set(AcGePoint3d(dMinX,dMinY,0),AcGePoint3d(dMaxX,dMaxY,0));
	m_config.m_bGetExt=true;
	CompleteEditorCommand();
}

void GTransConfigDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CArray<int> mItemIdArray;
	CStringArray mTypeArray;
	InitTypeItems(mItemIdArray,mTypeArray);
	m_config.m_TypeArray.RemoveAll();
	for(int i=0;i<mItemIdArray.GetCount();i++)
	{
		CString strType=mTypeArray.GetAt(i);
		CButton* pBtn=(CButton*)GetDlgItem(mItemIdArray.GetAt(i));
		if(pBtn==NULL)
			continue;
		if(!pBtn->GetCheck())
			continue;
		if(!pBtn->IsWindowEnabled())
			continue;
		m_config.m_TypeArray.Add(strType);
	}
	if(m_config.m_TypeArray.GetCount()==0)
	{
		AfxMessageBox(_T("请至少设置一种转换格式"),MB_OK);
		return;
	}
	CButton* pBtn_BlackWhite=(CButton*)GetDlgItem(IDC_CHECK6);
	if(pBtn_BlackWhite->GetCheck())
	{
		m_config.m_bBlackWhite=true;
	}
	else
	{
		m_config.m_bBlackWhite=false;
	}
	if(m_config.m_EntitysMode==kTransEntitysEnts&&m_config.m_ids.logicalLength()==0)
	{
		OnBnClickedButton3();
		return;
	}
	if(m_config.m_EntitysMode==kTransEntitysExtents&&
		(!m_config.m_bGetExt))
	{
		OnBnClickedButton2();
		return;
	}
	GetDlgItemText(IDC_EDIT4,m_config.m_strFloder);
	CString strDwgFileName;
	BOOL rc=GetCurDwgFileName(strDwgFileName);
	if(m_config.m_strFloder.IsEmpty())
	{
		if(!rc)
		{
			AfxMessageBox(_T("请输入输出目录"),MB_OK);
			return;
		}
		else
		{
			TCHAR lpPath[MAX_PATH+1];
			lstrcpy(lpPath,strDwgFileName);
			PathRemoveFileSpec(lpPath);
			m_config.m_strFloder=lpPath;
		}
	}
	else
	{
		if(!PathFileExists(m_config.m_strFloder))
		{
			if(AfxMessageBox(_T("输出目录不存在，是否创建"),MB_YESNO)!=IDYES)
			{
				return;
			}
			if(!CreateDirectory(m_config.m_strFloder,NULL))
			{
				AfxMessageBox(_T("创建目录失败，请检查输出目录配置是否正确"));
				return;
			}
		}
	}
	GetDlgItemText(IDC_EDIT5,m_config.m_strFileName);
	if(m_config.m_strFileName.IsEmpty())
	{
		TCHAR lpPath[MAX_PATH+1];
		if(rc)
		{
			lstrcpy(lpPath,strDwgFileName);
		}
		else
		{
			lstrcpy(lpPath,curDoc()->docTitle());
		}
		lstrcpy(lpPath,PathFindFileName(lpPath));
		PathRenameExtension(lpPath,_T(""));
		m_config.m_strFileName=lpPath;
	}
	else
	{
		CArray<TCHAR> invalidChars;
		invalidChars.Add(_T('\\'));
		invalidChars.Add(_T('/'));
		invalidChars.Add(_T(':'));
		invalidChars.Add(_T('*'));
		invalidChars.Add(_T('?'));
		invalidChars.Add(_T('\r'));
		invalidChars.Add(_T('\n'));
		invalidChars.Add(_T('"'));
		invalidChars.Add(_T('<'));
		invalidChars.Add(_T('>'));
		invalidChars.Add(_T('|'));
		for(int i=0;i<m_config.m_strFileName.GetLength();i++)
		{
			TCHAR c=m_config.m_strFileName.GetAt(i);
			for(int j=0;j<invalidChars.GetCount();j++)
			{
				if(invalidChars.GetAt(j)==c)
				{
					AfxMessageBox(_T("文件名不能包含下列任何字符之一\r\n\\/:*?\\r\\n\"<>|\r\n请检查"),MB_OK);
					return;
				}
			}
		}
	}
	if(((CButton*)GetDlgItem(IDC_RADIO_A0))->GetCheck())
	{
		m_config.m_PaperMode=kISOAOPaper;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_A1))->GetCheck())
	{
		m_config.m_PaperMode=kISOA1Paper;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_A2))->GetCheck())
	{
		m_config.m_PaperMode=kISOA2Paper;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_A3))->GetCheck())
	{
		m_config.m_PaperMode=kISOA3Paper;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_A4))->GetCheck())
	{
		m_config.m_PaperMode=kISOA4Paper;
	}
	GLockDocument _Lock;
	CString strErrorInfo;
	BOOL rc1=GsOutput(&m_config,strErrorInfo);
	if(rc1)
	{
		OnOK();
		CString strFloder=m_config.m_strFloder;
		GetDlgItemText(IDC_EDIT4,m_config.m_strFloder);
		GetDlgItemText(IDC_EDIT5,m_config.m_strFileName);
		m_config.Write();
		if(AfxMessageBox(_T("转换成功，是否去生成目录下查看结果?"),MB_YESNO)==IDYES)
		{
			::ShellExecute(NULL,_T("open"),_T("explorer.exe"),strFloder,NULL,SW_SHOW);
		}
	}
	else
	{
		CString strError;
		strError.Format(_T("转换失败\r\n%s"),strErrorInfo);
		AfxMessageBox(strError);
	}
}
static CString Glb_strPath;
static int CALLBACK BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	::ZeroMemory(&szDir, sizeof(TCHAR));

	switch (uMsg)
	{
		// Set the selected directory at startup.
	case BFFM_INITIALIZED:
		{
			TCHAR path[MAX_PATH]={0};
			CString strDwgFileName;
			lstrcpy(path,Glb_strPath);
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)path);
		}
		break;

	case BFFM_VALIDATEFAILED:
		break;

	default:
		break;
	}
	return 0;
}
void GTransConfigDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//浏览目录
	GetDlgItemText(IDC_EDIT4,m_config.m_strFloder);
	CString strDwgFileName;
	BOOL rc=GetCurDwgFileName(strDwgFileName);
	TCHAR lpPath[MAX_PATH+1];
	if(m_config.m_strFloder.IsEmpty())
	{
		lstrcpy(lpPath,strDwgFileName);
		PathRemoveFileSpec(lpPath);
	}
	else
	{
		lstrcpy(lpPath,m_config.m_strFloder);
	}
	Glb_strPath=lpPath;
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	TCHAR path[MAX_PATH]={0};
	lstrcpy(path,lpPath);
	bi.pszDisplayName = path;
	bi.lpszTitle = _T("请选择目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCtrlCallback;
	bi.iImage = 0;
	bi.lParam = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, path);
		SetDlgItemText(IDC_EDIT4,path);
	}
}

void GTransConfigDialog::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	//转换成黑白图
	CButton* pBtn_BlackWhite=(CButton*)GetDlgItem(IDC_CHECK6);
	if(pBtn_BlackWhite->GetCheck())
	{
		m_config.m_bBlackWhite=true;
	}
	else
	{
		m_config.m_bBlackWhite=false;
	}



}
