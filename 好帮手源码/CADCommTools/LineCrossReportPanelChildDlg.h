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
//----- TextCrossReportPanelChildDlg.h : Declaration of the CTextCrossReportPanelChildDlg
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "resource.h"
#include "CurveCommCommand.h"
//-----------------------------------------------------------------------------
class CLineCrossReportPanelChildDlg : public CAcUiDialog {
	DECLARE_DYNAMIC (CLineCrossReportPanelChildDlg)

public:
	CLineCrossReportPanelChildDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_LINECROSS} ;
	void OnSizeChanged();
	BOOL OnInitDialog();
	void ShowLineCrossResult(CArray<LineCrossResultItem>& result);

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	CListCtrl      m_ListReport;
	CArray<LineCrossResultItem> m_result;
	void ShowEntity(int nItem,int nSubItem,AcDbObjectId id);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkListTextcrossreport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListTextcrossreport(NMHDR *pNMHDR, LRESULT *pResult);
} ;
