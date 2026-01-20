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
//----- CmpReportDialog.h : Declaration of the CmpReportDialog
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
#include "acui.h"
#include "resource.h"
#include "CmpExcute.h"
#include "afxmt.h"
#include "GViewTableRecord.h"
#include "CmpReportHelper.h"
//-----------------------------------------------------------------------------
class CmpReportDialog : public CAcUiDialog 
{
	DECLARE_DYNAMIC (CmpReportDialog)

public:
	CmpReportDialog (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CmpReportDialog();

	enum { IDD = IDD_DLG_REPORT} ;
	void OnSizeChanged();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	virtual BOOL OnInitDialog();
	void ShowCmpResult(int nMode,bool bReg);
	void ShowCmpResultEx(int nMode,bool bReg);
	void SetDiffResult(int nIndex,CmpDiffResult& rc);
    bool       m_bEx;//文件夹对比
	int        m_nMode;
	bool       m_bReg;
	GViewTableRecordData m_selfSetView1;
	GViewTableRecordData m_selfSetView2;
protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ShowCmpResult(CmpResult* pRC);
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	CListCtrl      m_ListSub;
	CListCtrl      m_ListLeft;
	CListCtrl      m_ListRight;
	CListCtrl      m_listFiles;
	CStatic        m_staticMldbck;
	CStatic        m_staticDbwj1;
	CStatic        m_staticDbwj2;
	CStatic m_staticSourceText;
	CButton        m_btnDxfLToR;
	CButton        m_btnDxfRToL;
	CButton        m_btnEntLToR;
	CButton        m_btnEntRToL;
	CButton        m_btnColor;
	void ChangeColor(int leftStart, int leftEnd, int rightStart, int rightEnd,bool bSame);
	CRichEditCtrl richTextBox1;
	CRichEditCtrl richTextBox2;
	CToolTipCtrl* m_pTip;
	CArray<GDocViewTableRecordData> m_viewDatas;
	GDocViewTableRecordData m_lastSameData;//最后一次同步视图的数据
	CMutex m_mutex1;
	AcDbExtents m_ext;
	bool m_bInChangeEnt;//是否正在切换实体
	void ShowEntity(AcApDocument* pDoc,const CString strHandle,const int nMode);
	void AddExt(AcDbExtents& ext,AcApDocument* pDoc,const CString strHandle);
	void CompareText();
	bool CheckEditDocument();
	void ChangeResultColor(CArray<CmpDiffResult>&   diffResults,bool bBlock,bool bFile1,bool bRollback);
	void OnTimer_OpenDoc(UINT_PTR nIDEvent);
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickListLeft(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblclkListLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListRight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnNMDblclkListSub(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
} ;
