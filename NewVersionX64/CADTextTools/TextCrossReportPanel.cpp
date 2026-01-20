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
//----- TextCrossReportPanel.cpp : Implementation of CTextCrossReportPanel
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "TextCrossReportPanel.h"
CTextCrossReportPanel* m_pTextCrossReportPanel=NULL;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CTextCrossReportPanel, CAcUiDockControlBar)

BEGIN_MESSAGE_MAP(CTextCrossReportPanel, CAcUiDockControlBar)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//----- CTextCrossReportPanel *pInstance = new CTextCrossReportPanel;
//----- pInstance->Create (acedGetAcadFrame (), "My title bar") ;
//----- pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
//----- pInstance->RestoreControlBar () ;

//-----------------------------------------------------------------------------
static CLSID clsCTextCrossReportPanel = {0xeab78c04, 0x2194, 0x47ad, {0xa4, 0xf2, 0xad, 0xca, 0x3e, 0x3b, 0xb6, 0x3c}} ;


//-----------------------------------------------------------------------------
CTextCrossReportPanel::CTextCrossReportPanel () : CAcUiDockControlBar() {
}

//-----------------------------------------------------------------------------
CTextCrossReportPanel::~CTextCrossReportPanel () {

}

//-----------------------------------------------------------------------------
#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CAcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

//-----------------------------------------------------------------------------
BOOL CTextCrossReportPanel::Create (CWnd *pParent, LPCSTR lpszTitle) {
	CString strWndClass ;
	strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
	CRect rect (0, 0, 250, 200) ;
	if (!CAcUiDockControlBar::Create (
			strWndClass, (LPCTSTR)lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
			rect, pParent, 0
		)
	)
		return (FALSE) ;

	SetToolID (&clsCTextCrossReportPanel) ;
	EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP/*| CBRS_ALIGN_RIGHT*/);// | CBRS_ALIGN_TOP
	DockControlBar(CBRS_ALIGN_BOTTOM,CRect(0, 0, 800, 100));
	RestoreControlBar(AFX_IDW_DOCKBAR_BOTTOM);
	// TODO: Add your code here
	
	return (TRUE) ;
}

//-----------------------------------------------------------------------------
//----- This member function is called when an application requests the window be 
//----- created by calling the Create or CreateEx member function
int CTextCrossReportPanel::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if ( CAcUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
		return (-1) ;

	//----- Point to our resource
	CAcModuleResourceOverride resourceOverride; 	
	//----- Create it and set the parent as the dockctrl bar
	mChildDlg.Create (IDD_DIALOG_TEXTCROSS, this) ;
	//----- Move the window over so we can see the control lines
	mChildDlg.MoveWindow (0, 0, 100, 100, TRUE) ;
	mChildDlg.ShowWindow(SW_NORMAL);
	CRect cRect;
	GetClientRect(cRect); 
	cRect.top = cRect.top + 16;
	mChildDlg.MoveWindow(&cRect,TRUE);
	mChildDlg.OnSizeChanged();
	return (0) ;
}

//-----------------------------------------------------------------------------
void CTextCrossReportPanel::SizeChanged (CRect *lpRect, BOOL bFloating, int flags) {
	// If valid
	if (::IsWindow (mChildDlg.GetSafeHwnd ())) 
	{
		//----- Always point to our resource to be safe
		CAcModuleResourceOverride resourceOverride ;
		//----- Then update its window size relatively
		CRect cRect;
		GetClientRect(cRect); cRect.top = cRect.top + 16;
		mChildDlg.MoveWindow(&cRect,TRUE);
		mChildDlg.OnSizeChanged();
	}
}

//-----------------------------------------------------------------------------
//-----  Function called when user selects a command from Control menu or when user 
//----- selects the Maximize or the Minimize button.
void CTextCrossReportPanel::OnSysCommand (UINT nID, LPARAM lParam) {
	CAcUiDockControlBar::OnSysCommand (nID, lParam) ;
}

//-----------------------------------------------------------------------------
//----- The framework calls this member function after the window's size has changed
void CTextCrossReportPanel::OnSize (UINT nType, int cx, int cy) {
	CAcUiDockControlBar::OnSize (nType, cx, cy) ;
	// If valid
	if (::IsWindow (mChildDlg.GetSafeHwnd ())) 
	{
		//----- Always point to our resource to be safe
		CAcModuleResourceOverride resourceOverride ;
		//----- then update its window position relatively
		mChildDlg.MoveWindow (0, 0, cx, cy) ;
	}
}
BOOL CTextCrossReportPanel::ShowDockPane(BOOL bShow)
{
	CMDIFrameWnd *pAcadFrame = acedGetAcadFrame();

	if (pAcadFrame)
	{
		pAcadFrame->ShowControlBar(this, bShow, FALSE);
		return TRUE;
	}

	return FALSE;
}
void CTextCrossReportPanel::ShowTextCrossResult(CArray<TextCrossRecord>& result)
{
	mChildDlg.ShowTextCrossResult(result);
}
