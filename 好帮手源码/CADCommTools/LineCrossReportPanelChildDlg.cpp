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
//----- TextCrossReportPanelChildDlg.cpp : Implementation of CTextCrossReportPanelChildDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "LineCrossReportPanelChildDlg.h"
#include "AcCommFunction.h"
#include "GLockDocument.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CLineCrossReportPanelChildDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CLineCrossReportPanelChildDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TEXTCROSSREPORT, &CLineCrossReportPanelChildDlg::OnNMDblclkListTextcrossreport)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CLineCrossReportPanelChildDlg::CLineCrossReportPanelChildDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CLineCrossReportPanelChildDlg::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CLineCrossReportPanelChildDlg::DoDataExchange (CDataExchange *pDX) 
{
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_LIST_TEXTCROSSREPORT,m_ListReport);
}
BOOL CLineCrossReportPanelChildDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_ListReport.SetExtendedStyle(m_ListReport.GetExtendedStyle()| LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListReport.InsertColumn(0,_T("ID"),LVCFMT_LEFT,100);
	m_ListReport.InsertColumn(1,_T("曲线1"),LVCFMT_LEFT,300);
	m_ListReport.InsertColumn(2,_T("曲线2"),LVCFMT_LEFT,300);
	m_ListReport.InsertColumn(3,_T("曲线关系"),LVCFMT_LEFT,300);

	return TRUE;
}
//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CLineCrossReportPanelChildDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
void CLineCrossReportPanelChildDlg::OnSizeChanged()
{
	CRect rc,rc1;
	GetClientRect(rc);
	rc1.top = rc.top+5; 
	rc1.left = rc.left+5; 
	rc1.right = rc.right-5; 
	rc1.bottom = rc.bottom -5;
	m_ListReport.MoveWindow(&rc1);
}
void CLineCrossReportPanelChildDlg::ShowEntity(int nItem,int nSubItem,AcDbObjectId id)
{
	CString strText;
	AcDbEntityPointer spEnt(id,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		strText.Format(_T("打开实体失败:%s"),acadErrorStatusText(spEnt.openStatus()));
		m_ListReport.SetItemText(nItem,nSubItem,strText);
		return;
	}
	CString strHandle;
	objToHandleStr(spEnt.object(),strHandle);
	CString strType=_T("未知类型");
	if(spEnt->isKindOf(AcDbLine::desc()))
	{
		strType=_T("直线");
	}
	else if(spEnt->isKindOf(AcDbArc::desc()))
	{
		strType=_T("圆弧");
	}
	else if(spEnt->isKindOf(AcDbPolyline::desc()))
	{
		strType=_T("多段线");
	}
	strText.Format(_T("%s<%s>"),strType,strHandle);
	m_ListReport.SetItemText(nItem,nSubItem,strText);
}
void CLineCrossReportPanelChildDlg::ShowLineCrossResult(CArray<LineCrossResultItem>& result)
{
	m_ListReport.DeleteAllItems();
	m_result.RemoveAll();
	m_result.Append(result);
	for(int i=0;i<m_result.GetCount();i++)
	{
		LineCrossResultItem& lcr=m_result.GetAt(i);
		CString strId;
		strId.Format(_T("%d"),i+1);
		int nIndex=m_ListReport.InsertItem(i,strId);
		ShowEntity(nIndex,1,lcr.id1);
		ShowEntity(nIndex,2,lcr.id2);
		CString strRel=_T("");
		//1<<1:完全重叠
		//1<<2:AB压盖CD
		//1<<3:CD压盖AB
		//1<<4:部分重叠
		//1<<5:完全不重叠
		if((lcr.rc&(1<<1))==(1<<1))
		{
			if(!strRel.IsEmpty())
			{
				strRel+=_T("|");
			}
			strRel+=_T("完全重叠");
		}
		if((lcr.rc&(1<<2))==(1<<2))
		{
			if(!strRel.IsEmpty())
			{
				strRel+=_T("|");
			}
			strRel+=_T("压盖");
		}
		if((lcr.rc&(1<<3))==(1<<3))
		{
			if(!strRel.IsEmpty())
			{
				strRel+=_T("|");
			}
			strRel+=_T("被压盖");
		}
		if((lcr.rc&(1<<4))==(1<<4))
		{
			if(!strRel.IsEmpty())
			{
				strRel+=_T("|");
			}
			strRel+=_T("部分重叠");
		}
		m_ListReport.SetItemText(nIndex,3,strRel);
	}
}

void CLineCrossReportPanelChildDlg::OnNMDblclkListTextcrossreport(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	if (m_ListReport.GetSelectedCount() <= 0)
		return;

	GLockDocument lock;
	// 获取选中行
	POSITION pos = m_ListReport.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	//获取ObjectId
	int nItem = m_ListReport.GetNextSelectedItem(pos);
	if(nItem<0||nItem>=m_result.GetCount())
		return;
	LineCrossResultItem& lcr=m_result.GetAt(nItem);
	AcDbObjectIdArray ids;
	ids.append(lcr.id1);
	ids.append(lcr.id2);
	SetEntitySelected(ids,true);
	DyZoom(ids,1.1);
}
