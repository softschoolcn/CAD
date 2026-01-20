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
//----- GTableAlignDialog.h : Declaration of the GTableAlignDialog
//-----------------------------------------------------------------------------
#pragma once
#ifndef ACAD_2004
//-----------------------------------------------------------------------------
#include "acui.h"
#include "GWndEx.h"
struct GTableCellAlignRecordItem
{
	int nRow;
	int nCol;
	AcDb::CellAlignment kAlign;
};
struct GTableCellAlignRecord
{
	GTableCellAlignRecord(){};
	AcDbObjectId m_tableId;
	CArray<GTableCellAlignRecordItem> m_recordItemArray;
	Acad::ErrorStatus Record();
	Acad::ErrorStatus SetXAlign(int nMode,int nAlign);
	Acad::ErrorStatus SetYAlign(int nMode,int nAlign);
	void ToXYAlign(const AcDb::CellAlignment kAlign,int& nXAlign,int& nYAlign);
	void ToTableAlign(AcDb::CellAlignment& kAlign,const int nXAlign,const int nYAlign);
	Acad::ErrorStatus MoveBack();
	GTableCellAlignRecord(const GTableCellAlignRecord& src);
	GTableCellAlignRecord& operator=(const GTableCellAlignRecord& src);
};

//-----------------------------------------------------------------------------
class GTableAlignDialog : public CAcUiDialog {
	DECLARE_DYNAMIC (GTableAlignDialog)

public:
	GTableAlignDialog (AcDbObjectIdArray& m_ids,CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_TABLEALIGN} ;
	AcDbObjectIdArray   ids;//实体数组
	CArray<GTableCellAlignRecord> m_recordArray;
	void MoveBack();
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

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
	Acad::ErrorStatus Apply();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedButton1();
} ;

#endif
