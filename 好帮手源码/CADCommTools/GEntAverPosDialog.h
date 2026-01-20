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
//----- GEntAverPosDialog.h : Declaration of the GEntAverPosDialog
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "GWndEx.h"
//-----------------------------------------------------------------------------
class GEntAverPosDialog : public CAcUiDialog {
	DECLARE_DYNAMIC (GEntAverPosDialog)

public:
	GEntAverPosDialog (AcDbObjectIdArray& m_ids,CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_AVERPOS} ;
	virtual BOOL OnInitDialog();
	void MoveBack();
	void Apply();
	void ApplyXSimple(CArray<double>& xMinArr,CArray<double>& xMaxArr,CArray<AcDbObjectId>& xIdArr);
	void ApplyXNotSimple(CArray<double>& xMinArr,CArray<double>& xMaxArr,CArray<AcDbObjectId>& xIdArr);
	void ApplyX();
	void ApplyY();
	void ApplyYSimple(CArray<double>& yMinArr,CArray<double>& yMaxArr,CArray<AcDbObjectId>& xIdArr);
	void ApplyYNotSimple(CArray<double>& yMinArr,CArray<double>& yMaxArr,CArray<AcDbObjectId>& xIdArr);
	//记录偏移
	void AddMoveRec(AcDbObjectId id,double dx,double dy);

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	AcDbObjectIdArray   ids;//实体数组
	AcGePoint3dArray    pts;//偏移数组
	CAcUiSelectButton	m_btnPos;
	CAcUiSelectButton	m_btnExt;
	AcGePoint3d ptMin;
	AcGePoint3d ptMax;

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedCheck3();
public:
	afx_msg void OnBnClickedCheck4();
public:
	afx_msg void OnBnClickedCheck5();
public:
	afx_msg void OnBnClickedCheck6();
public:
	afx_msg void OnBnClickedCheck7();
public:
	afx_msg void OnBnClickedCheck8();
public:
	afx_msg void OnBnClickedCheck9();
public:
	afx_msg void OnBnClickedRadio4();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
} ;
