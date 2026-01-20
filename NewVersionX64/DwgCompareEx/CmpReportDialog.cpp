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
//----- CmpReportDialog.cpp : Implementation of CmpReportDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CmpReportDialog.h"
#include "CommCommand.h"
#include "GLockDocument.h"
#include "GOpenCmdDocHelper.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CmpReportDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(CmpReportDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LEFT, &CmpReportDialog::OnNMClickListLeft)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILES, &CmpReportDialog::OnNMClickListFiles)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES, &CmpReportDialog::OnNMDblclkListFiles)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LEFT, &CmpReportDialog::OnNMDblclkListLeft)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RIGHT, OnNMClickListRight)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUB, &CmpReportDialog::OnNMDblclkListSub)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CmpReportDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CmpReportDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CmpReportDialog::CmpReportDialog (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : 
CAcUiDialog (CmpReportDialog::IDD, pParent, hInstance),m_bInChangeEnt(false),m_bEx(false)
{
	m_nMode=0;
	m_bReg=false;
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	m_pTip=NULL;
}
CmpReportDialog::~CmpReportDialog()
{
	if(m_pTip!=NULL)
	{
		m_pTip->DestroyWindow();
		delete m_pTip;
		m_pTip=NULL;
	}
}

//-----------------------------------------------------------------------------
void CmpReportDialog::DoDataExchange (CDataExchange *pDX) 
{
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_LIST_LEFT,m_ListLeft);
	DDX_Control(pDX,IDC_LIST_RIGHT,m_ListRight);
	DDX_Control(pDX,IDC_LIST_FILES,m_listFiles);
	DDX_Control(pDX,IDC_STATIC_MLDBCK,m_staticMldbck);
	DDX_Control(pDX,IDC_STATIC_DBWJ1,m_staticDbwj1);
	DDX_Control(pDX,IDC_STATIC_DBWJ2,m_staticDbwj2);
	DDX_Control(pDX,IDC_STATIC_SOURCETEXT,m_staticSourceText);
	DDX_Control(pDX,IDC_RICHEDIT_SOURCETEXT,richTextBox1);
	DDX_Control(pDX,IDC_RICHEDIT_MODIFYTEXT,richTextBox2);
	DDX_Control(pDX,IDC_LIST_SUB,m_ListSub);
	DDX_Control(pDX,IDC_BUTTON1,m_btnDxfLToR);
	DDX_Control(pDX,IDC_BUTTON3,m_btnDxfRToL);
	DDX_Control(pDX,IDC_BUTTON4,m_btnEntLToR);
	DDX_Control(pDX,IDC_BUTTON5,m_btnEntRToL);
	DDX_Control(pDX,IDC_BUTTON6,m_btnColor);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CmpReportDialog::OnAcadKeepFocus (WPARAM, LPARAM) 
{
	return (TRUE) ;
}
void CmpReportDialog::OnSize(UINT nType, int cx, int cy)
{
	CAcUiDialog::OnSize(nType, cx, cy);

	OnSizeChanged();
}
BOOL CmpReportDialog::OnCommand (WPARAM wParam, LPARAM lParam) 
{
	switch ( wParam )
	{
	case IDOK:
	case IDCANCEL:
		return (FALSE) ;
	}	
	return (CAcUiDialog::OnCommand (wParam, lParam)) ;
}
BOOL CmpReportDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_ListLeft.SetExtendedStyle(m_ListLeft.GetExtendedStyle()| LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListLeft.InsertColumn(0,_T("对象位置"),LVCFMT_LEFT,50);
	m_ListLeft.InsertColumn(1,_T("对象类型"),LVCFMT_LEFT,50);
	m_ListLeft.InsertColumn(2,_T("差异类型"),LVCFMT_LEFT,50);
	m_ListLeft.InsertColumn(3,_T("对象句柄"),LVCFMT_LEFT,50);

	m_ListRight.InsertColumn(0,_T("名称"),LVCFMT_LEFT,50);
	m_ListRight.InsertColumn(1,_T("旧值"),LVCFMT_LEFT,50);
	m_ListRight.InsertColumn(2,_T("新值"),LVCFMT_LEFT,50);

	m_listFiles.InsertColumn(0,_T("文件名称"),LVCFMT_LEFT,50);
	m_listFiles.InsertColumn(1,_T("对比文件"),LVCFMT_LEFT,0);
	m_listFiles.InsertColumn(2,_T("差异"),LVCFMT_LEFT,50);

	m_ListSub.InsertColumn(0,_T("序号"),LVCFMT_LEFT,50);
	m_ListSub.InsertColumn(1,_T("旧值"),LVCFMT_LEFT,50);
	m_ListSub.InsertColumn(2,_T("新值"),LVCFMT_LEFT,50);
	//SetTimer(282,100,NULL);
	//SetTimer(284,1000,NULL);

	richTextBox1.SetFont(m_ListRight.GetFont());
	richTextBox1.SetAutoURLDetect(FALSE);
	CHARFORMAT2 cf;
	cf.dwEffects=0;
	cf.dwMask=CFM_FACE|CFM_SIZE;
	lstrcpy(cf.szFaceName,_T("宋体"));
	cf.yHeight=14*14;
	cf.cbSize=cf.yHeight;
	richTextBox1.SetDefaultCharFormat(cf);
	richTextBox2.SetFont(m_ListRight.GetFont());
	richTextBox2.SetAutoURLDetect(FALSE);
	richTextBox2.SetDefaultCharFormat(cf);
	m_btnColor.SetWindowText(Glb_bOriginalColor?_T("显示对比结果颜色"):_T("显示原始图颜色"));
	m_pTip=NULL;
	m_pTip=new CToolTipCtrl();
	m_pTip->Create(this,TTS_ALWAYSTIP);
	m_pTip->SetMaxTipWidth(400);

	m_pTip->AddTool(&m_btnEntLToR,_T("选中实体左边同步到右边"));
	m_pTip->AddTool(&m_btnEntRToL,_T("选中实体右边同步到左边"));

	m_pTip->AddTool(&m_btnDxfLToR,_T("选中DXF子项左边同步到右边"));
	m_pTip->AddTool(&m_btnDxfRToL,_T("选中DXF子项右边同步到左边"));
	return TRUE;
}
BOOL CmpReportDialog::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message== WM_LBUTTONDOWN ||

		pMsg->message== WM_LBUTTONUP ||

		pMsg->message== WM_MOUSEMOVE)
	{
		if(m_pTip!=NULL)
			m_pTip->RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CmpReportDialog::SetDiffResult(int nIndex,CmpDiffResult& rc)
{
	m_ListLeft.SetItemText(nIndex,1,rc.strName);

	if(rc.nType==1)
	{
		m_ListLeft.SetItemText(nIndex,2,_T("修改"));
		m_ListLeft.SetItemText(nIndex,3,rc.strHandle1+_T(",")+rc.strHandle2);
	}
	else if(rc.nType==2)
	{
		m_ListLeft.SetItemText(nIndex,2,_T("新增"));
		m_ListLeft.SetItemText(nIndex,3,rc.strHandle2);
	}
	else if(rc.nType==3)
	{
		m_ListLeft.SetItemText(nIndex,2,_T("删除"));
		m_ListLeft.SetItemText(nIndex,3,rc.strHandle1);
	}
	m_ListLeft.SetItemData(nIndex,(DWORD_PTR)(&rc));
}
void CmpReportDialog::ShowCmpResultEx(int nMode,bool bReg)
{
	m_bEx=true;
	m_nMode=nMode;
	m_bReg=bReg;
	m_listFiles.DeleteAllItems();
	m_ListLeft.DeleteAllItems();
	m_ListRight.DeleteAllItems();
	
	for(int i=0;i<GCmpResultInstance::instance.GetResultCount();i++)
	{
		CmpResult* pRC=GCmpResultInstance::instance.GetResult(i);
		CString strItemName=_T("");
		if(pRC->m_strFile1.IsEmpty())
		{
			if(nMode!=2)
			{
				strItemName=PathFindFileName(pRC->m_strFile2);
			}
			else
			{
				strItemName=_T("(空)");
			}
			
		}
		else
		{
			strItemName=nMode!=2?PathFindFileName(pRC->m_strFile1):pRC->m_strFile1;
		}
		int nIndex=m_listFiles.InsertItem(m_listFiles.GetItemCount(),strItemName);
		if(nMode==2)
		{
			if(pRC->m_strFile2.IsEmpty())
			{
				m_listFiles.SetItemText(nIndex,1,_T("(空)"));
			}
			else
			{
				m_listFiles.SetItemText(nIndex,1,pRC->m_strFile2);
			}
		}
		if(pRC->nType==1)
		{
			m_listFiles.SetItemText(nIndex,2,_T("修改"));
		}
		else if(pRC->nType==2)
		{
			m_listFiles.SetItemText(nIndex,2,_T("新增"));
		}
		else if(pRC->nType==3)
		{
			m_listFiles.SetItemText(nIndex,2,_T("删除"));
		}
		m_listFiles.SetItemData(nIndex,(DWORD_PTR)(pRC));
	}
	OnSizeChanged();
}
#ifdef _WIN64
void CALLBACK TimerProc_OpenDoc(HWND hWnd, UINT nMsg, UINT_PTR nTimerId, DWORD dwTime)
#else
void CALLBACK TimerProc_OpenDoc(HWND hWnd, UINT nMsg, UINT nTimerId, DWORD dwTime)
#endif
{

}
void CmpReportDialog::ShowCmpResult(CmpResult* pRC)
{
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	richTextBox1.SetWindowText(_T(""));
	richTextBox2.SetWindowText(_T(""));
	m_ListSub.ShowWindow(SW_HIDE);
	richTextBox1.ShowWindow(SW_NORMAL);
	richTextBox2.ShowWindow(SW_NORMAL);
	m_staticSourceText.ShowWindow(SW_NORMAL);
	m_ListLeft.DeleteAllItems();
	m_ListRight.DeleteAllItems();
	int nIndex=-1;
	//模型空间的差异
	for (long i=0;i<pRC->m_diffsInModelSpace.GetCount();i++)
	{
		CmpDiffResult& rc=pRC->m_diffsInModelSpace.GetAt(i);
		if(rc.nType==0)
			continue;
		if(rc.m_bRevise)
			continue;
		nIndex=m_ListLeft.InsertItem(m_ListLeft.GetItemCount(),_T("模型空间"));
		SetDiffResult(nIndex,rc);
	}
	//图层的差异比较结果
	for (long i=0;i<pRC->m_diffsInLayerTable.GetCount();i++)
	{
		CmpDiffResult& rc=pRC->m_diffsInLayerTable.GetAt(i);
		if(rc.nType==0)
			continue;
		if(rc.m_bRevise)
			continue;
		CString strName;
		strName.Format(_T("图层表[%s]"),rc.strName);
		nIndex=m_ListLeft.InsertItem(m_ListLeft.GetItemCount(),strName);
		SetDiffResult(nIndex,rc);
	}
	for (long i=0;i<pRC->m_diffsInTextStyleTable.GetCount();i++)
	{
		CmpDiffResult& rc=pRC->m_diffsInTextStyleTable.GetAt(i);
		if(rc.nType==0)
			continue;
		if(rc.m_bRevise)
			continue;
		CString strName;
		strName.Format(_T("文字样式表[%s]"),rc.strName);
		nIndex=m_ListLeft.InsertItem(m_ListLeft.GetItemCount(),strName);
		SetDiffResult(nIndex,rc);
	}
	for (long i=0;i<pRC->m_diffsInLinetypeTable.GetCount();i++)
	{
		CmpDiffResult& rc=pRC->m_diffsInLinetypeTable.GetAt(i);
		if(rc.nType==0)
			continue;
		if(rc.m_bRevise)
			continue;
		CString strName;
		strName.Format(_T("线形表[%s]"),rc.strName);
		nIndex=m_ListLeft.InsertItem(m_ListLeft.GetItemCount(),strName);
		SetDiffResult(nIndex,rc);
	}
	//块定义的差异
	for(long j=0;j<pRC->m_diffsInBlockDefines.GetCount();j++)
	{
		CmpBlockDiffResult& rc1=pRC->m_diffsInBlockDefines.GetAt(j);
		for(long k=0;k<rc1.m_diffItems.GetCount();k++)
		{
			CmpDiffResult& rc=rc1.m_diffItems.GetAt(k);
			if(rc.nType==0)
				continue;
			if(rc.m_bRevise)
				continue;
			CString strName;
			strName.Format(_T("块定义[%s]"),rc1.strName);
			nIndex=m_ListLeft.InsertItem(m_ListLeft.GetItemCount(),strName);
			SetDiffResult(nIndex,rc);
		}
	}
	m_staticDbwj1.SetWindowText(_T("对比文件1：")+pRC->m_strFile1);
	m_staticDbwj1.Invalidate();
	m_staticDbwj1.UpdateWindow();
	m_staticDbwj2.Invalidate();
	m_staticDbwj2.SetWindowText(_T("对比文件2：")+pRC->m_strFile2);
	m_staticDbwj2.UpdateWindow();
	OnSizeChanged();
	if(pRC->nType==4)
	{
		KillTimer(282);
	    KillTimer(284);
		SetTimer(274,100,NULL);
		//SetTimer(282,100,NULL);
	    //SetTimer(284,1000,NULL);
	}
}
void CmpReportDialog::ShowCmpResult(int nMode,bool bReg)
{
	m_nMode=nMode;
	m_bReg=bReg;
	m_bEx=false;
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	ShowCmpResult(pRC);

}
void CmpReportDialog::ShowEntity(AcApDocument* pDoc,const CString strHandle,const int nMode)
{
	AcApDocument* pDocCur=acDocManager->curDocument();
	AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
	acDocManager->activateDocument(pDoc);
	acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
	AcDbObjectId retId;
	AcDbHandle objHandle=strHandle;
	if(nMode==0||nMode==2)
	{
		Acad::ErrorStatus es=pDoc->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=eOk)
		{
			acutPrintf(_T("\n转换成ObjectId失败，失败原因:%s"),acadErrorStatusText(es));
			acDocManager->activateDocument(pDoc1);
			acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
			return;
		}
		AcDbObjectPointer<AcDbEntity> spEnt(retId,AcDb::kForRead);
		es=spEnt.openStatus();
		if(es!=eOk)
		{
			//acutPrintf(_T("\n打开对象失败，失败原因:%s"),acadErrorStatusText(es));
			acDocManager->activateDocument(pDoc1);
			acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
			return;
		}
		AcDbObjectPointer<AcDbBlockTableRecord> spRec(spEnt->blockId(),AcDb::kForRead);
		es=spRec.openStatus();
		if(es!=eOk)
		{
			acutPrintf(_T("\n打开块表记录，失败原因:%s"),acadErrorStatusText(es));
			acDocManager->activateDocument(pDoc1);
			acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
			return;
		}
		if(spRec->isLayout())
		{
			//模型空间或布局空间中的对象
			SetEntitySelected(retId,true);
			if(nMode==2)
			{
				view_set(m_ext.minPoint(),m_ext.maxPoint(),1.1);
				actrTransactionManager->queueForGraphicsFlush();
				actrTransactionManager->flushGraphics();
				acedUpdateDisplay();
			}
		}
		else
		{
			//块表记录中的对象
		}
	}
	else if(nMode==1||nMode==2)
	{
		view_set(m_ext.minPoint(),m_ext.maxPoint(),1.1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
	}
	acDocManager->activateDocument(pDoc1);
	acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);


}
void CmpReportDialog::OnNMClickListLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	richTextBox1.SetWindowText(_T(""));
	richTextBox2.SetWindowText(_T(""));
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	*pResult = 0;
	if(CmpReportHelper::instance.m_bInChangeDwg)
	{
		AfxMessageBox(_T("正在切换文档，请不要操作太快，谢谢"));
		return;
	}
	if(m_bInChangeEnt)
	{
		AfxMessageBox(_T("正在切换实体，请不要操作太快，谢谢"));
		return;
	}
	m_ListRight.DeleteAllItems();
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	bool bCompare=false;
	for(long i=0;i<pRC->m_diffItems.GetCount();i++)
	{
		CmpDiffItem& item=pRC->m_diffItems.GetAt(i);
		if(item.m_bRevise)
		{
			continue;
		}
		int nIndex=m_ListRight.InsertItem(m_ListRight.GetItemCount(),item.strName);
		m_ListRight.SetItemText(nIndex,1,item.strOldValue);
		m_ListRight.SetItemText(nIndex,2,item.strNewValue);
		m_ListRight.SetItemData(nIndex,(DWORD_PTR)(pRC->m_diffItems.GetData()+i));
		if((!bCompare)&&
			(item.nDxfCode==1||(!item.m_bOnlyOne)))
		{
			//文字内容
			m_ListRight.SetSelectionMark(nIndex);
			CompareText();
			bCompare=true;
		}
	}
}

void CmpReportDialog::OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ListLeft.DeleteAllItems();
	m_ListRight.DeleteAllItems();
	int nSelect = m_listFiles.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpResult* pRc=(CmpResult*)m_listFiles.GetItemData(nSelect);
	GCmpResultInstance::instance.SetCurrentResultIndex(nSelect);
	ShowCmpResult(pRc);
}
//向AcApDocument文档对象发送消息
LRESULT SendMessageToDoc(AcApDocument* pDoc,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CDocument* pMFCDoc=pDoc->cDoc();
	if(pMFCDoc!=NULL)
	{
		POSITION pos=pMFCDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=pMFCDoc->GetNextView(pos);
			HWND hWnd=pView->GetSafeHwnd();
			hWnd=::GetParent(hWnd);
			::SendMessage(hWnd,Msg, wParam,lParam); //最大化
		}
	}
	return 0;
}
void MaxSizeDoc(AcApDocument* pDoc)
{
	SendMessageToDoc(pDoc,WM_SYSCOMMAND, SC_MAXIMIZE,0);
}
void MinSizeDoc(AcApDocument* pDoc)
{
	SendMessageToDoc(pDoc,WM_SYSCOMMAND, SC_MINIMIZE,0);
}
//设置文档的显示方式
LRESULT ShowDocWindow(AcApDocument* pDoc,int nCmdShow)
{
	CDocument* pMFCDoc=pDoc->cDoc();
	if(pMFCDoc!=NULL)
	{
		POSITION pos=pMFCDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=pMFCDoc->GetNextView(pos);
			HWND hWnd=pView->GetSafeHwnd();
			hWnd=::GetParent(hWnd);
			ShowWindow(hWnd,nCmdShow);
		}
	}
	return 0;
}
void CmpReportDialog::OnNMDblclkListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSelect = m_listFiles.GetSelectionMark();
	if(nSelect < 0) 
		return;
	if(CmpReportHelper::instance.m_bInChangeDwg)
	{
		AfxMessageBox(_T("正在切换文档，请不要操作太快，谢谢"));
		return;
	}
	GCmpResultInstance::instance.SetCurrentResultIndex(nSelect);
	SetTimer(274,100,NULL);

}
void CmpReportDialog::AddExt(AcDbExtents& ext,AcApDocument* pDoc,const CString strHandle)
{
	AcDbObjectId retId;
	AcDbHandle objHandle=strHandle;
	Acad::ErrorStatus es=pDoc->database()->getAcDbObjectId(retId,false,objHandle);
	if(es!=eOk)
	{
		return;
	}
	AcDbEntityPointer spEnt(retId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbExtents tmp_ext;
	es=spEnt->getGeomExtents(tmp_ext);
	if(es!=Acad::eOk)
	{
		return;
	}
	ext.addExt(tmp_ext);
}

void CmpReportDialog::OnNMDblclkListLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//if(!m_bReg)
	//{
	//	AfxMessageBox(_T("未注册版本不支持双击定位功能，请您先注册"));
	//	return;
	//}
	*pResult = 0;
	if(CmpReportHelper::instance.m_bInChangeDwg)
	{
		AfxMessageBox(_T("正在切换文档，请不要操作太快，谢谢"));
		return;
	}
	if(m_bInChangeEnt)
	{
		AfxMessageBox(_T("正在切换实体，请不要操作太快，谢谢"));
		return;
	}
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	SetTimer(278,500,NULL);
}
void CmpReportDialog::OnNMClickListRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CompareText();
	
}

void CmpReportDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CAcUiDialog::OnClose();
	KillTimer(276);
	KillTimer(278);
	KillTimer(280);
	KillTimer(282);
	KillTimer(284);
}
//用十字叉高亮显示一个点
void HightVert(AcGePoint3d ptCenter,int nColor)
{
	double DRAWWIDTH=20.0;
	CPoint ptPixel;
	CPoint point1;
	CPoint point2;
	CPoint point3;
	CPoint point4;
	ads_point pt1;
	ads_point pt2;
	ads_point pt3;
	ads_point pt4;
	acedCoordFromWorldToPixel(1,asDblArray(ptCenter),ptPixel);
	point1.x=ptPixel.x+DRAWWIDTH;
	point1.y=ptPixel.y+DRAWWIDTH;

	point2.x=ptPixel.x-DRAWWIDTH;
	point2.y=ptPixel.y+DRAWWIDTH;

	point3.x=ptPixel.x-DRAWWIDTH;
	point3.y=ptPixel.y-DRAWWIDTH;

	point4.x=ptPixel.x+DRAWWIDTH;
	point4.y=ptPixel.y-DRAWWIDTH;


	acedCoordFromPixelToWorld(1,point1,pt1 );
	acedCoordFromPixelToWorld(1,point2,pt2 );
	acedCoordFromPixelToWorld(1,point3,pt3 );
	acedCoordFromPixelToWorld(1,point4,pt4 );

	double dDist=asPnt3d(pt1).distanceTo(asPnt3d(pt3));

	acutPolar(asDblArray(ptCenter),PI/4,dDist/2, pt1);
	acutPolar(asDblArray(ptCenter),PI*3/4,dDist/2, pt2);
	acutPolar(asDblArray(ptCenter),PI*5/4,dDist/2, pt3);
	acutPolar(asDblArray(ptCenter),PI*7/4,dDist/2, pt4); 
	acedGrDraw(asDblArray(ptCenter),pt1,nColor,0);
	acedGrDraw(asDblArray(ptCenter),pt2,nColor,0);
	acedGrDraw(asDblArray(ptCenter),pt3,nColor,0);
	acedGrDraw(asDblArray(ptCenter),pt4,nColor,0);
}
void CmpReportDialog::OnNMDblclkListSub(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSelect = m_ListSub.GetSelectionMark();
	if(nSelect < 0) 
		return;
	int nDxfCode=m_ListSub.GetItemData(nSelect);
	if(nDxfCode<10||nDxfCode>17)
	{
		return;
	}
	CString strOldValue=m_ListSub.GetItemText(nSelect,1);
	if(strOldValue.IsEmpty())
	{
		return;
	}
	CString strNewValue=m_ListSub.GetItemText(nSelect,2);
	if(strNewValue.IsEmpty())
	{
		return;
	}
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	if(pCmpRc==NULL)
	{
		return;
	}
	//3DPoint
	CStringArray strOldValueArray;
	SplitString(strOldValue,',',strOldValueArray);
	CStringArray strNewValueArray;
	SplitString(strNewValue,',',strNewValueArray);
	if(strOldValueArray.GetCount()!=3||strNewValueArray.GetCount()!=3)
	{
		return;
	}
	AcGePoint3d ptOld(_tstof(strOldValueArray.GetAt(0)),_tstof(strOldValueArray.GetAt(1)),_tstof(strOldValueArray.GetAt(2)));
	AcGePoint3d ptNew(_tstof(strNewValueArray.GetAt(0)),_tstof(strNewValueArray.GetAt(1)),_tstof(strNewValueArray.GetAt(2)));
	double dMinX=min(ptOld.x,ptNew.x);
	double dMaxX=max(ptOld.x,ptNew.x);

	double dMinY=min(ptOld.y,ptNew.y);
	double dMaxY=max(ptOld.y,ptNew.y);
	AcDbExtents ext(AcGePoint3d(dMinX,dMinY,0.0),AcGePoint3d(dMaxX,dMaxY,0.0));
	bool bExt=dMaxX-dMinX>1E-4||dMaxY-dMinY>1E-4;
	AcApDocument* pDocCur=acDocManager->curDocument();
	AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
	if(!GCmpResultInstance::instance.CheckDocument())
	{
		return;
	}
	if(pDoc1==pCmpRc->m_pDoc1)
	{
		if(bExt)
		{
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			Sleep(100);
			AcDbViewTableRecord view;
			getCurrentView(view);
			m_selfSetView1.m_CenterPt=view.centerPoint();
			m_selfSetView1.m_dWidth=view.width();
			m_selfSetView1.m_dHeight=view.height();
		}
		HightVert(ptOld,1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->activateDocument(pCmpRc->m_pDoc2);
		if(bExt)
		{
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			Sleep(100);
			AcDbViewTableRecord view;
			getCurrentView(view);
			m_selfSetView2.m_CenterPt=view.centerPoint();
			m_selfSetView2.m_dWidth=view.width();
			m_selfSetView2.m_dHeight=view.height();
		}
		HightVert(ptNew,1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->activateDocument(pCmpRc->m_pDoc1);

	}
	else if(pDoc1==pCmpRc->m_pDoc2)
	{
		if(bExt)
		{
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			Sleep(100);
			AcDbViewTableRecord view;
			getCurrentView(view);
			m_selfSetView2.m_CenterPt=view.centerPoint();
			m_selfSetView2.m_dWidth=view.width();
			m_selfSetView2.m_dHeight=view.height();
		}
		HightVert(ptNew,1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->activateDocument(pCmpRc->m_pDoc1);
		if(bExt)
		{
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			view_set(ext.minPoint(),ext.maxPoint(),1.2);
			Sleep(100);
			AcDbViewTableRecord view;
			getCurrentView(view);
			m_selfSetView1.m_CenterPt=view.centerPoint();
			m_selfSetView1.m_dWidth=view.width();
			m_selfSetView1.m_dHeight=view.height();
		}
		HightVert(ptOld,1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->activateDocument(pCmpRc->m_pDoc2);
	}


}

void CmpReportDialog::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//原始颜色和对比颜色切换
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	if(pRC==NULL)
	{
		return;
	}
	AcArray<bool> locks[2];
	AcArray<bool> frozens[2];
	AcDbObjectIdArray ids[2];
	if(Glb_bOriginalColor)
	{
		//切换到对比颜色
		if(!CheckEditDocument())
		{
			return;
		}
		AcApDocument* pDoc=pRC->m_pDoc2;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		OpenAllLayer(pRC->m_pDoc2->database(),ids[0],locks[0],frozens[0]);
		for(long i=0;i<pRC->m_diffsInBlockDefines.GetCount();i++)
		{
			CmpBlockDiffResult& rc=pRC->m_diffsInBlockDefines.GetAt(i);
			ChangeResultColor(rc.m_diffItems,true,false,false);
		}
		ChangeResultColor(pRC->m_diffsInModelSpace,false,false,false);
		ResetAllLayer(ids[0],locks[0],frozens[0]);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);


		pDoc=pRC->m_pDoc1;
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		OpenAllLayer(pRC->m_pDoc1->database(),ids[1],locks[1],frozens[1]);
		for(long i=0;i<pRC->m_diffsInBlockDefines.GetCount();i++)
		{
			CmpBlockDiffResult& rc=pRC->m_diffsInBlockDefines.GetAt(i);
			ChangeResultColor(rc.m_diffItems,true,true,false);
		}
		ChangeResultColor(pRC->m_diffsInModelSpace,false,true,false);
		ResetAllLayer(ids[1],locks[1],frozens[1]);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);


		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
		
	}
	else
	{
		if(!CheckEditDocument())
		{
			return;
		}
		AcApDocument* pDoc=pRC->m_pDoc2;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		OpenAllLayer(pRC->m_pDoc2->database(),ids[0],locks[0],frozens[0]);
		for(long i=0;i<pRC->m_diffsInBlockDefines.GetCount();i++)
		{
			CmpBlockDiffResult& rc=pRC->m_diffsInBlockDefines.GetAt(i);
			ChangeResultColor(rc.m_diffItems,true,false,true);
		}
		ChangeResultColor(pRC->m_diffsInModelSpace,false,false,true);
		ResetAllLayer(ids[0],locks[0],frozens[0]);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);


		pDoc=pRC->m_pDoc1;
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		OpenAllLayer(pRC->m_pDoc1->database(),ids[1],locks[1],frozens[1]);
		for(long i=0;i<pRC->m_diffsInBlockDefines.GetCount();i++)
		{
			CmpBlockDiffResult& rc=pRC->m_diffsInBlockDefines.GetAt(i);
			ChangeResultColor(rc.m_diffItems,true,true,true);
		}
		ChangeResultColor(pRC->m_diffsInModelSpace,false,true,true);
		ResetAllLayer(ids[1],locks[1],frozens[1]);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);


		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
	}
	Glb_bOriginalColor=!Glb_bOriginalColor;
	m_btnColor.SetWindowText(Glb_bOriginalColor?_T("显示对比结果颜色"):_T("显示原始图颜色"));

	
}
void CmpReportDialog::ChangeResultColor(CArray<CmpDiffResult>&   diffResults,bool bBlock,bool bFile1,bool bRollback)
{
	Acad::ErrorStatus es=eOk;
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	for(long i=0;i<diffResults.GetCount();i++)
	{
		CmpDiffResult& rc=diffResults.GetAt(i);

		if(rc.nType==0||rc.nType==1)
		{
			//完全相同/改变了
			int nColor=(rc.nType==0||rc.m_bRevise)?Glb_nColorSame:Glb_nColorChange;
			if(bBlock&&(rc.nType==0||rc.m_bRevise))
			{
				nColor=0;//如果是完全相同的话，就把块设置为ByBlock
			}
			AcDbHandle handle1=rc.strHandle1;
			AcDbObjectId recId;
			if(bFile1)
			{
				es=pRC->m_pDoc1->database()->getAcDbObjectId(recId,false,handle1);
				if(es==eOk)
				{
					if(bRollback)
					{
						SetEntityColor(recId,rc.colRecord1);
					}
					else
					{
						SetEntityColor(recId,nColor,rc.colRecord1);
					}
				}
			}
			AcDbHandle handle2=rc.strHandle2;
			if(!bFile1)
			{
				es=pRC->m_pDoc2->database()->getAcDbObjectId(recId,false,handle2);
				if(es==eOk)
				{
					if(bRollback)
					{
						SetEntityColor(recId,rc.colRecord2);
					}
					else
					{
						SetEntityColor(recId,nColor,rc.colRecord2);
					}
				}
			}
		}
		else if(rc.nType==2)
		{
			AcDbHandle handle=rc.strHandle2;
			//新增加
			AcDbObjectId recId;
			es=pRC->m_pDoc2->database()->getAcDbObjectId(recId,false,handle);
			if(!bFile1)
			{
				if(es==eOk)
				{
					if(bRollback)
					{
						SetEntityColor(recId,rc.colRecord2);
					}
					else
					{
						SetEntityColor(recId,Glb_nColorNew,rc.colRecord2);
					}
				}
			}
		}
		else if(rc.nType==3)
		{
			//删除
			AcDbHandle handle=rc.strHandle1;
			AcDbObjectId recId;
			if(bFile1)
			{
				es=pRC->m_pDoc1->database()->getAcDbObjectId(recId,false,handle);
				if(es==eOk)
				{
					if(bRollback)
					{
						SetEntityColor(recId,rc.colRecord1);
					}
					else
					{
						SetEntityColor(recId,Glb_nColorDelete,rc.colRecord1);
					}
				}
			}
		}
		if(!bRollback)
		diffResults.SetAt(i,rc);
	}
}
