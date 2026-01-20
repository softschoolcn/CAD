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
//----- GBatchCopyDialog.cpp : Implementation of GBatchCopyDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GBatchCopyDialog.h"
#include "GLockDocument.h"
#include "GTextJig.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (GBatchCopyDialog, CAdUiDialog)

BEGIN_MESSAGE_MAP(GBatchCopyDialog, CAdUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &GBatchCopyDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_DIS, &GBatchCopyDialog::OnBnClickedCheckDis)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &GBatchCopyDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &GBatchCopyDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO_LOOP, &GBatchCopyDialog::OnBnClickedButtonLoop)
	ON_BN_CLICKED(IDC_RADIO_INC, &GBatchCopyDialog::OnBnClickedButtonInc)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
GBatchCopyDialog::GBatchCopyDialog (AcDbObjectId id,CString strLeft,CString strMid,CString strRight,CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiDialog (GBatchCopyDialog::IDD, pParent, hInstance),
m_pLastJig(NULL),m_bWaitDis(FALSE),m_id(id),m_strLeft(strLeft),m_strMid(strMid),m_strRight(strRight),m_bLoop(false)
{
}

//-----------------------------------------------------------------------------
void GBatchCopyDialog::DoDataExchange (CDataExchange *pDX) 
{
	CAdUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_CHECK_DIS,m_btnDis);
	DDX_Control(pDX,IDC_EDIT_DIS,m_editDis);
	DDX_Control(pDX,IDC_EDIT_NUM,m_editNum);
	DDX_Control(pDX,IDC_EDIT_ADD,m_editAdd);

}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT GBatchCopyDialog::OnAcadKeepFocus (WPARAM, LPARAM) 
{
	return (FALSE) ;
}
AcDbEntity* CreateNewText(AcDbEntity* pEnt)
{
	if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pNewMText=(AcDbMText*)pEnt->clone();
		pNewMText->setLocation(pNewMText->location());
		return pNewMText;
	}
	if(pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=(AcDbText*)pEnt->clone();
		pText->setPosition(pText->position());
		return pText;
	}
	else
	{
		return NULL;
	}
}
BOOL GBatchCopyDialog::OnInitDialog()
{
	CAdUiDialog::OnInitDialog();
	m_editAdd.SetWindowText(_T("1"));
	m_editNum.SetWindowText(_T("1"));
	bool bLoop=GUserDataRecord::getInstance()->GetProfileBool(_T("GBatchCopyDialog"),_T("Loop"),false);
	if(bLoop)
	{
		SetItemCheck(IDC_RADIO_LOOP,TRUE);
	}
	else
	{
		SetItemCheck(IDC_RADIO_INC,TRUE);
	}

	if(bLoop)
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("循环模式下0-->9-->0"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("递增模式下0-->9-->10"));
	}
	m_bLoop=bLoop;
	GLockDocument _Lock;
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	OnBegin();
	return TRUE;
}
void GBatchCopyDialog::OnBegin()
{
	ShowWindow(SW_HIDE);
	GLockDocument _Lock;
	//交互选择实体
	BeginEditorCommand();
new_begin:
	if(m_bWaitDis)
	{

		ads_point a_pt;
		int rc=acedGetPoint(NULL,_T("请输入任意点"),a_pt);
		if(rc==RTNORM)
		{
		ads_real result=0.0;
		rc=acedGetDist(a_pt,_T("\n请输入间距"),&result);
		if(rc==RTNORM)
		{
			CString strTmp;
			strTmp.Format(_T("%5.2f"),result);
			m_editDis.SetWindowText(strTmp);
		}
		m_bWaitDis=FALSE;
		}
	}
	ShowWindow(SW_SHOW);
	AcDbEntityPointer spEnt(m_id,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		CompleteEditorCommand();
		return;
	}
	AcDbEntity* pNewText=CreateNewText(spEnt.object());
	if(pNewText==NULL)
	{
		CompleteEditorCommand();
		return;
	}
	spEnt->close();
	GBatchCopyTextJig jig;
	jig.m_pDlgBatchCopy=this;
	jig.SetEntity(pNewText);
	m_pLastJig=&jig;
	int ret = jig.Do();
	if(ret==RTNORM)
	{
		ShowWindow(SW_HIDE);
		m_pLastJig=NULL;
		goto new_begin;
	}
	else if(m_bWaitDis)
	{
		ShowWindow(SW_HIDE);
		m_pLastJig=NULL;
		goto new_begin;
	}
	m_pLastJig=NULL;
	

	CompleteEditorCommand();
	OnOK();
}

void GBatchCopyDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//输入间距

	if(m_pLastJig!=NULL)
	{
		m_pLastJig->m_bStop=TRUE;
		m_bWaitDis=TRUE;
		//模拟按下ESC键
		keybd_event(VK_ESCAPE,0,0,0);
		keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_ESCAPE,0,0,0);
		keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
		//delete m_pLastJig;
		//m_pLastJig=NULL;
	}
	else
	{
		BeginEditorCommand();
		ads_point a_pt;
		a_pt[X]=pt.x;
		a_pt[Y]=pt.y;
		a_pt[Z]=pt.z;
		ads_real result=0.0;
		int rc=acedGetDist(a_pt,_T("\n请输入间距"),&result);
		if(rc==RTNORM)
		{
			CString strTmp;
			strTmp.Format(_T("%5.2f"),result);
			m_editDis.SetWindowText(strTmp);
		}
		CompleteEditorCommand();
	}

}

void GBatchCopyDialog::OnBnClickedCheckDis()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_btnDis.GetCheck())
	{
		m_editDis.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	}
	else
	{
		m_editDis.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
}

void GBatchCopyDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pLastJig!=NULL)
	{
	CAdUiDialog::OnClose();
	keybd_event(VK_ESCAPE,0,0,0);
		keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_ESCAPE,0,0,0);
		keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
	}
	OnOK();
}

void GBatchCopyDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}

void GBatchCopyDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
void GBatchCopyDialog::OnBnClickedButtonLoop()
{
	bool bLoop=GetItemCheck(IDC_RADIO_LOOP);
	if(bLoop)
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("循环模式下0-->9-->0"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("递增模式下0-->9-->10"));
	}
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GBatchCopyDialog"),_T("Loop"),bLoop);
	m_bLoop=bLoop;

}
void GBatchCopyDialog::OnBnClickedButtonInc()
{
	bool bLoop=GetItemCheck(IDC_RADIO_LOOP);
	if(bLoop)
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("循环模式下0-->9-->0"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TIP,_T("递增模式下0-->9-->10"));
	}
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GBatchCopyDialog"),_T("Loop"),bLoop);
	m_bLoop=bLoop;
}
