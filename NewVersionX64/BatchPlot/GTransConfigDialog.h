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
//----- GTransConfigDialog.h : Declaration of the GTransConfigDialog
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "GTransConfig.h"
//-----------------------------------------------------------------------------
class GTransConfigDialog : public CAcUiDialog {
	DECLARE_DYNAMIC (GTransConfigDialog)

public:
	GTransConfigDialog (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_TRANS} ;
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	//œ‘ æ≈‰÷√
	void ShowConfig(GTransConfig* pConfig);
	void ShowEntityMode(GTransEntitysMode mode);

	DECLARE_MESSAGE_MAP()
private:
	void InitTypeItems(CArray<int>& mItemIdArray,CStringArray& mTypeArray);
	void InitEntitysModeItems(CArray<int>& mItemIdArray,CArray<GTransEntitysMode>& mModeArray);
	void EnableItems(CArray<int>& enableIds,BOOL bEnable);
	GTransConfig m_config;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedCheck6();
} ;
