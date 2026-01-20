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
#include "TextCrossReportPanelChildDlg.h"
#include "GLockDocument.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CTextCrossReportPanelChildDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CTextCrossReportPanelChildDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TEXTCROSSREPORT, &CTextCrossReportPanelChildDlg::OnNMDblclkListTextcrossreport)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TEXTCROSSREPORT, &CTextCrossReportPanelChildDlg::OnNMCustomdrawListTextcrossreport)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CTextCrossReportPanelChildDlg::CTextCrossReportPanelChildDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CTextCrossReportPanelChildDlg::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CTextCrossReportPanelChildDlg::DoDataExchange (CDataExchange *pDX) 
{
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_LIST_TEXTCROSSREPORT,m_ListReport);
}
BOOL CTextCrossReportPanelChildDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_ListReport.SetExtendedStyle(m_ListReport.GetExtendedStyle()| LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListReport.InsertColumn(0,_T("ID"),LVCFMT_LEFT,100);
	m_ListReport.InsertColumn(1,_T("文字1"),LVCFMT_LEFT,350);
	m_ListReport.InsertColumn(2,_T("文字2"),LVCFMT_LEFT,350);
	m_ListReport.InsertColumn(3,_T("重叠比例"),LVCFMT_LEFT,100);

	return TRUE;
}
//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CTextCrossReportPanelChildDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
void CTextCrossReportPanelChildDlg::OnSizeChanged()
{
	CRect rc,rc1;
	GetClientRect(rc);
	rc1.top = rc.top+5; 
	rc1.left = rc.left+5; 
	rc1.right = rc.right-5; 
	rc1.bottom = rc.bottom -5;
	m_ListReport.MoveWindow(&rc1);
}
void CTextCrossReportPanelChildDlg::ShowTextCrossResult(CArray<TextCrossRecord>& result)
{
	m_ListReport.DeleteAllItems();
	m_result.RemoveAll();
	m_result.Append(result);
	for(int i=0;i<m_result.GetCount();i++)
	{
		TextCrossRecord& tcr=m_result.GetAt(i);
		CString strId;
		strId.Format(_T("%d"),i+1);
		int nIndex=m_ListReport.InsertItem(i,strId);
		m_ListReport.SetItemText(nIndex,1,tcr.strText1);
		m_ListReport.SetItemText(nIndex,2,tcr.strText2);
		CString strScale;
		strScale.Format(_T("%.1f%%"),100*tcr.dScale);
		m_ListReport.SetItemText(nIndex,3,strScale);
	}
}

void CTextCrossReportPanelChildDlg::OnNMDblclkListTextcrossreport(NMHDR *pNMHDR, LRESULT *pResult)
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
	TextCrossRecord& tcr=m_result.GetAt(nItem);
	AcDbObjectIdArray ids;
	ids.append(tcr.textId1);
	ids.append(tcr.textId2);
	GSelectHelper::SetEntitySelected(ids,true);
	GZoom::DyZoom(ids,1.1);
}

void CTextCrossReportPanelChildDlg::OnNMCustomdrawListTextcrossreport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLVCUSTOMDRAW*   pLVCD   =   reinterpret_cast <NMLVCUSTOMDRAW*> (   pNMHDR   ); 

	//   Take   the   default   processing   unless   we   set   this   to   something   else   below. 
	*pResult   =   CDRF_DODEFAULT; 

	//   First   thing   -   check   the   draw   stage.   If   it 's   the   control 's   prepaint 
	//   stage,   then   tell   Windows   we   want   messages   for   every   item. 

	if   (   CDDS_PREPAINT   ==   pLVCD-> nmcd.dwDrawStage   ) 
	{ 
		*pResult   =   CDRF_NOTIFYITEMDRAW; 
	} 
	else   if   (   CDDS_ITEMPREPAINT   ==   pLVCD-> nmcd.dwDrawStage   ) 
	{ 
		//   This   is   the   notification   message   for   an   item.     We 'll   request 
		//   notifications   before   each   subitem 's   prepaint   stage. 

		*pResult   =   CDRF_NOTIFYSUBITEMDRAW; 
	} 
	else   if   (   (CDDS_ITEMPREPAINT   |   CDDS_SUBITEM)   ==   pLVCD-> nmcd.dwDrawStage   ) 
	{ 
		//   This   is   the   prepaint   stage   for   a   subitem.   Here 's   where   we   set   the 
		//   item 's   text   and   background   colors.   Our   return   value   will   tell   
		//   Windows   to   draw   the   subitem   itself,   but   it   will   use   the   new   colors 
		//   we   set   here. 
		//   The   text   color   will   cycle   through   red,   green,   and   light   blue. 
		//   The   background   color   will   be   light   blue   for   column   0,   red   for 
		//   column   1,   and   black   for   column   2. 
		if(pLVCD->nmcd.dwItemSpec>=0&&pLVCD->nmcd.dwItemSpec<m_result.GetCount())
		{
			TextCrossRecord& tcr=m_result.GetAt(pLVCD->nmcd.dwItemSpec);

			COLORREF   crText;
			//COLORREF   crBkgnd;
			if(tcr.dScale>=0.7)
			{
				//严重警告
				crText=RGB(255,   0,   0);
				pLVCD-> clrText   =   crText; 
			}
			else if(tcr.dScale<0.7&&tcr.dScale>=0.3)
			{
				//警告
				crText=RGB(255,127,0);
				pLVCD-> clrText   =   crText;
			}
			else if(tcr.dScale<0.3&&tcr.dScale>=0.1)
			{
				//提示
				crText=RGB(0,   255,   0);
				pLVCD-> clrText   =   crText;
			}
			else
			{
				//可忽略
				crText=RGB(192,   192,   192);//灰色
				pLVCD-> clrText   =   crText;

			}
		}
		

		//   Store   the   colors   back   in   the   NMLVCUSTOMDRAW   struct. 
		
		//pLVCD-> clrTextBk   =   crBkgnd; 

		//   Tell   Windows   to   paint   the   control   itself. 
		*pResult   =   CDRF_DODEFAULT; 
	}
}
