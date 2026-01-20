#include "StdAfx.h"
#include "GOpenCmdDocHelper.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
GOpenCmdDocHelper::GOpenCmdDocHelper(void)
{
}

GOpenCmdDocHelper::~GOpenCmdDocHelper(void)
{
}
//获取文档对应的窗口
HWND GOpenCmdDocHelper::GetDocWindow(AcApDocument* pDoc)
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
			return hWnd;
		}
	}
	return NULL;
}
//交换两个文档的窗体位置
void GOpenCmdDocHelper::SwapDocPosition(AcApDocument* pDoc1,AcApDocument* pDoc2)
{
	HWND hWnd1=GetDocWindow(pDoc1);
	CRect rc1;
	GetWindowRect(hWnd1,&rc1);


	HWND hWnd2=GetDocWindow(pDoc2);
	CRect rc2;
	GetWindowRect(hWnd2,&rc2);

	CWnd* pWnd1=CWnd::FromHandle(hWnd1);
	CWnd* pWnd2=CWnd::FromHandle(hWnd2);
	CRect rc1_par;
	pWnd1->GetParent()->GetWindowRect(&rc1_par);

	MoveWindow(hWnd1,rc2.left-rc1_par.left,rc2.top-rc1_par.top,rc2.Width(),rc2.Height(),TRUE);
	MoveWindow(hWnd2,rc1.left-rc1_par.left,rc1.top-rc1_par.top,rc1.Width(),rc1.Height(),TRUE);
}
//确保两个文档的左右位置
void GOpenCmdDocHelper::MakeSureDocLeftRight(AcApDocument* pDoc1,AcApDocument* pDoc2)
{
	HWND hWnd1=GetDocWindow(pDoc1);
	CRect rc1;
	GetWindowRect(hWnd1,&rc1);


	HWND hWnd2=GetDocWindow(pDoc2);
	CRect rc2;
	GetWindowRect(hWnd2,&rc2);

	if(rc1.left>rc2.left)
	{
		CWnd* pWnd1=CWnd::FromHandle(hWnd1);
		CWnd* pWnd2=CWnd::FromHandle(hWnd2);
		CRect rc1_par;
		pWnd1->GetParent()->GetWindowRect(&rc1_par);

		MoveWindow(hWnd1,rc2.left-rc1_par.left,rc2.top-rc1_par.top,rc2.Width(),rc2.Height(),TRUE);
		MoveWindow(hWnd2,rc1.left-rc1_par.left,rc1.top-rc1_par.top,rc1.Width(),rc1.Height(),TRUE);
	}
}
//设置文档的显示方式
void GOpenCmdDocHelper::ShowDocWindow(AcApDocument* pDoc,int nCmdShow)
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
}
//打开对比文档
Acad::ErrorStatus GOpenCmdDocHelper::OpenCmpDoc(CmpResult* pRc)
{
	if(pRc->nType==0)
	{
		return Acad::eInvalidInput;
	}
	Acad::ErrorStatus es=Acad::eOk;
	if(pRc->nType!=2&&pRc->m_pDoc1==NULL)
	{
		es=OpenDoc(pRc->m_pDoc1,pRc->m_strFile1);
		if(es!=Acad::eOk)
		{
			return es;
		}
	}
	if(pRc->nType!=3&&pRc->m_pDoc2==NULL)
	{
		es=OpenDoc(pRc->m_pDoc2,pRc->m_strFile2);
		if(es!=Acad::eOk)
		{
			return es;
		}
	}
	if(pRc->nType==2||pRc->nType==3)
	{
		return Acad::eOk;
	}
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step())
	{
		if(pIter->document()!=pRc->m_pDoc2&&pIter->document()!=pRc->m_pDoc1)
		{
			ShowDocWindow(pIter->document(),SW_MINIMIZE);
		}
		else
		{
			ShowDocWindow(pIter->document(),SW_MAXIMIZE);
		}
	}
	delete pIter;
	pIter=NULL;
	SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_COMMAND,ID_WINDOW_TILE_VERT,2);
	MakeSureDocLeftRight(pRc->m_pDoc1,pRc->m_pDoc2);
	return Acad::eOk;
}
//打开对比文档
Acad::ErrorStatus GOpenCmdDocHelper::OpenDoc(AcApDocument*& pDoc,const CString strFile)
{
	//GLockDocument _lock;
	CString strCommand;
	strCommand.Format(_T("_ZOOM E "));
#if _MSC_VER>=1900
	acDocManager->pushAcadResourceHandle();
#else
	acDocManager->pushResourceHandle(acedGetAcadResourceInstance());
#endif
	Acad::ErrorStatus es=acDocManager->appContextOpenDocument(strFile);
	acDocManager->popResourceHandle();
	if(es==Acad::eOk)
	{
		pDoc=acDocManager->mdiActiveDocument();
		pDoc->pushDbmod();
		acDocManager->lockDocument(pDoc);
		acDocManager->sendStringToExecute(pDoc, strCommand);
		acDocManager->unlockDocument(pDoc);
		return es;
	}
	else
	{
		acutPrintf(_T("\n打开文档失败"));
		return es;
	}
}
