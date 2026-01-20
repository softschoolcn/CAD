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
//----- GCommandAliasDlg.cpp : Implementation of GCommandAliasDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GCommandAliasDlg.h"
BOOL ReadCommands(CArray<GCommandDefine>& cmds,LPCTSTR lpFile,BOOL ingoreNoName)
{
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpFile))
	{
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMNodeListPtr lpCmdNodeList=lpDocElement->getElementsByTagName(_T("Command"));
	MSXML2::IXMLDOMNodePtr lpCmdNode = NULL;

	for ( ; (lpCmdNode = lpCmdNodeList->nextNode()) != NULL ; )
	{
		CString strName=GetNodeAttrValue(lpCmdNode,_T("Name"));
		CString strCmd=GetNodeAttrValue(lpCmdNode,_T("Cmd"));
		CString strAlias=GetNodeAttrValue(lpCmdNode,_T("Alias"));
		if(strName.IsEmpty()&&(!ingoreNoName))
			continue;
		if(strCmd.IsEmpty())
			continue;
		if(strAlias.IsEmpty())
		{
			strAlias=strCmd;
		}
		GCommandDefine cmd;
		cmd.strName=strName;
		cmd.strCmd=strCmd;
		cmd.strAlias=strAlias;
		cmds.Add(cmd);

	}
	return TRUE;
}
BOOL ResetUserCommands(CArray<GCommandDefine>& sys_cmds,CArray<GCommandDefine>& user_cmds)
{
	for(int i=0;i<sys_cmds.GetCount();i++)
	{
		GCommandDefine& cmd=sys_cmds.GetAt(i);
		BOOL bFind=FALSE;
		for(int j=0;j<user_cmds.GetCount();j++)
		{
			GCommandDefine& cmd1=user_cmds.GetAt(j);
			if(cmd.strCmd.CompareNoCase(cmd1.strCmd)==0)
			{
				cmd.strAlias=cmd1.strAlias;
				bFind=TRUE;
				break;
			}
		}
		if(bFind)
		{
			sys_cmds.SetAt(i,cmd);
		}
	}
	return TRUE;
}
CString GetCommandAlias(CArray<GCommandDefine>& sys_cmds,CString strCmd)
{
for(int i=0;i<sys_cmds.GetCount();i++)
	{
		GCommandDefine& cmd=sys_cmds.GetAt(i);
		if(cmd.strCmd.CompareNoCase(strCmd)==0)
		{
			return cmd.strAlias;
		}
	}
    return strCmd;
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (GCommandAliasDlg, CDialog)

BEGIN_MESSAGE_MAP(GCommandAliasDlg, CDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
GCommandAliasDlg::GCommandAliasDlg (CWnd *pParent /*=NULL*/) : CDialog (GCommandAliasDlg::IDD, pParent) 
{
}

//-----------------------------------------------------------------------------
void GCommandAliasDlg::DoDataExchange (CDataExchange *pDX) 
{
	CDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_GRID, mGrid);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT GCommandAliasDlg::OnAcadKeepFocus (WPARAM, LPARAM) 
{
	return (TRUE) ;
}
BOOL GCommandAliasDlg::OnInitDialog()
{
	BOOL rc=CDialog::OnInitDialog();
	CGridCell* pCell = new CGridCell();
	mGrid.SetDefaultCellType(pCell->GetRuntimeClass());
	int nCol=0;
	nCol=mGrid.InsertColumn(_T("     命令名称     "));
	mGrid.SetColumnWidth(nCol,120);
	nCol=mGrid.InsertColumn(_T("命令"));
	mGrid.SetColumnWidth(nCol,150);
	nCol=mGrid.InsertColumn(_T("命令别名"));
	mGrid.SetColumnWidth(nCol,150);
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetFixedColumnCount(1);
	mGrid.AutoSizeColumn(0);
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("config\\CommandAlias.xml"));
	CArray<GCommandDefine> sys_cmds;
	CArray<GCommandDefine> user_cmds;
	if(PathFileExists(lpPath1))
	{
		ReadCommands(sys_cmds,lpPath1,FALSE);
	}

	TCHAR lpPath2[MAX_PATH+1]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("UserConfig\\CommandAlias.xml"));
	if(PathFileExists(lpPath2))
	{
		ReadCommands(user_cmds,lpPath2,TRUE);
	}
	ResetUserCommands(sys_cmds,user_cmds);
	for(int i=0;i<sys_cmds.GetCount();i++)
	{
		GCommandDefine& cmd=sys_cmds.GetAt(i);
		int row = mGrid.InsertRow(cmd.strName);  //插入命令名称
		mGrid.SetItemState(row,1,mGrid.GetItemState(row,1)|GVIS_READONLY);
		mGrid.SetItemText(row,1,cmd.strCmd);
		mGrid.SetItemText(row,2,cmd.strAlias);
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return rc;
}
//执行设置
BOOL GCommandAliasDlg::ExcSet()
{
	int len = mGrid.GetRowCount();
	CArray<GCommandDefine> cmds;
	for (int i = 1; i < len; i++)
	{
		GCommandDefine cmd;
		cmd.strCmd=mGrid.GetItemText(i,1);
		cmd.strAlias=mGrid.GetItemText(i,2);
		if(cmd.strAlias.IsEmpty())
		{
			mGrid.SetSelectedRange(i,1,i,2);
			mGrid.SetFocusCell(i,2);
			AfxMessageBox(_T("命令别名不可以设置为空"));
			return FALSE;
		}
		cmds.Add(cmd);
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("config\\CommandAlias.xml"));
	CArray<GCommandDefine> sys_cmds;
	if(PathFileExists(lpPath1))
	{
		ReadCommands(sys_cmds,lpPath1,FALSE);
	}
	CArray<GCommandDefine> save_cmds;
	for(int j=0;j<sys_cmds.GetCount();j++)
	{
		GCommandDefine& cmd1=sys_cmds.GetAt(j);
		for(int k=0;k<cmds.GetCount();k++)
		{
			GCommandDefine& cmd2=cmds.GetAt(k);
			if(cmd1.strCmd.CompareNoCase(cmd2.strCmd)==0&&cmd1.strAlias.CompareNoCase(cmd2.strAlias)!=0)
			{
				save_cmds.Add(cmd2);
				break;
			}
		}
	}
	TCHAR lpPath2[MAX_PATH+1]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("UserConfig\\CommandAlias.xml"));
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);

	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='GB2312'"));
	lpDocument->appendChild(lpInstruction);

	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("Root")));

	lpDocument->appendChild(lpRoot);

	// 参数列表
	for (int l=0; l<save_cmds.GetCount(); l++)
	{
		GCommandDefine& save_cmd=save_cmds.GetAt(l);

		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Command"));
		lpParam->setAttribute(_T("Cmd"), _variant_t(save_cmd.strCmd));
		lpParam->setAttribute(_T("Alias"), _variant_t(save_cmd.strAlias));

		lpRoot->appendChild(lpParam);
	}
	hr=lpDocument->save(lpPath2);
	if(SUCCEEDED(hr))
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("保存失败"));
		return FALSE;
	}
}
