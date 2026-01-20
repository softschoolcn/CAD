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
//----- GBatchCopyDialog.h : Declaration of the GBatchCopyDialog
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "Resource.h"
#include "GTextJig.h"
#include "GWndEx.h"
//-----------------------------------------------------------------------------
class GBatchCopyDialog : public CAdUiDialog {
	DECLARE_DYNAMIC (GBatchCopyDialog)

public:
	GBatchCopyDialog (AcDbObjectId id,CString strLeft,CString strMid,CString strRight,CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_BATCHCOPY} ;
	AcDbObjectId m_id;
	AcGePoint3d pt;
	CEdit       m_editDis;
	CEdit       m_editNum;
	CEdit       m_editAdd;
	CButton     m_btnDis;
	BOOL        m_bWaitDis;//等待输入距离
	CString m_strLeft;
	CString m_strMid;
	CString m_strRight;
	bool    m_bLoop;//是否循环模式
	void OnBegin();
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	GBatchCopyTextJig*   m_pLastJig;

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheckDis();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonLoop();
	afx_msg void OnBnClickedButtonInc();
} ;
