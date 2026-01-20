#include "Stdafx.h"
#include "CmpReportHelper.h"
#ifdef _WIN64
void CALLBACK TimerProc_StartPlot(HWND hWnd, UINT nMsg, UINT_PTR nTimerId, DWORD dwTime)
#else
void CALLBACK TimerProc_StartPlot(HWND hWnd, UINT nMsg, UINT nTimerId, DWORD dwTime)
#endif
{
	CmpReportHelper::instance.OnTimer(hWnd,nTimerId);
}
CmpReportHelper CmpReportHelper::instance;
CmpReportHelper::CmpReportHelper()
{
	m_bReg=false;
}
CmpReportHelper::~CmpReportHelper()
{
}
void CmpReportHelper::ShowCmpResult(int nMode,bool bReg)
{
	Stop();
	m_nMode=nMode;
	m_bReg=bReg;
	SetTimer(acedGetAcadFrame()->GetSafeHwnd(),282,100,TimerProc_StartPlot);
	SetTimer(acedGetAcadFrame()->GetSafeHwnd(),284,1000,TimerProc_StartPlot);
	ShowCmpResult();
}
void CmpReportHelper::ShowCmpResult()
{
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	if(pRC->nType==4)
	{
		OpenCmpDoc(pRC);
	}
}
void CmpReportHelper::Stop()
{
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	KillTimer(acedGetAcadFrame()->GetSafeHwnd(),276);
	KillTimer(acedGetAcadFrame()->GetSafeHwnd(),282);
	KillTimer(acedGetAcadFrame()->GetSafeHwnd(),284);
}
//打开对比文档
void CmpReportHelper::OpenCmpDoc(CmpResult* pRc)
{
	m_selfSetView1.m_dWidth=-1;
	m_selfSetView1.m_dHeight=-1;
	m_selfSetView2.m_dWidth=-1;
	m_selfSetView2.m_dHeight=-1;
	if(pRc->nType==1||pRc->nType==4)
	{
		if(pRc->m_pDoc1==NULL&&pRc->m_pDoc2==NULL)
		{
			AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
			for (; !pIter->done(); pIter->step())
			{
				ShowDocWindow(pIter->document(),SW_MINIMIZE);
			}
			delete pIter;
			pIter=NULL;
			CString strCommand;
			strCommand.Format(_T("_ZOOM E "));
#if _MSC_VER>=1900
			acDocManager->pushAcadResourceHandle();
#else
			acDocManager->pushResourceHandle(acedGetAcadResourceInstance());
#endif
			Acad::ErrorStatus es=acDocManager->appContextOpenDocument(pRc->m_strFile2);
			acDocManager->popResourceHandle();
			if(es==Acad::eOk)
			{
				pRc->m_pDoc2=acDocManager->mdiActiveDocument();
				acDocManager->sendStringToExecute(pRc->m_pDoc2, strCommand);
			}
			else
			{
				acutPrintf(_T("\n打开文档2失败"));
				return;
			}
			m_bInChangeDwg=true;
			SetTimer(acedGetAcadFrame()->GetSafeHwnd(),276,500,TimerProc_StartPlot);
		}
		else if(pRc->nType==1)
		{
			AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
			for (; !pIter->done(); pIter->step())
			{
				if(pIter->document()!=pRc->m_pDoc2&&pIter->document()!=pRc->m_pDoc1)
					ShowDocWindow(pIter->document(),SW_MINIMIZE);
			}
			delete pIter;
			pIter=NULL;
			AcApDocumentIterator* pIter1 = acDocManager->newAcApDocumentIterator();
			for (; !pIter1->done(); pIter1->step())
			{
				if(pIter1->document()==pRc->m_pDoc2)
					ShowDocWindow(pIter1->document(),SW_NORMAL);
			}
			delete pIter1;
			pIter1=NULL;
			SetTimer(acedGetAcadFrame()->GetSafeHwnd(),276,500,TimerProc_StartPlot);

		}
	}
}
void CmpReportHelper::OnTimer(HWND hWnd,UINT_PTR nIDEvent)
{
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	if(pRC==NULL)
	{
		return;
	}
	if(nIDEvent==274)
	{
		KillTimer(hWnd,nIDEvent);
		if(pRC->nType==1||pRC->nType==4)
		{
			if(pRC->m_pDoc1==NULL&&pRC->m_pDoc2==NULL)
			{
				AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
				for (; !pIter->done(); pIter->step())
				{
					ShowDocWindow(pIter->document(),SW_MINIMIZE);
				}
				delete pIter;
				pIter=NULL;
				CString strCommand;
				strCommand.Format(_T("_ZOOM E "));
#if _MSC_VER>=1900
				acDocManager->pushAcadResourceHandle();
#else
				acDocManager->pushResourceHandle(acedGetAcadResourceInstance());
#endif
				Acad::ErrorStatus es=acDocManager->appContextOpenDocument(pRC->m_strFile2);
				acDocManager->popResourceHandle();
				if(es==Acad::eOk)
				{
					pRC->m_pDoc2=acDocManager->mdiActiveDocument();
					acDocManager->sendStringToExecute(pRC->m_pDoc2, strCommand);
				}
				else
				{
					acutPrintf(_T("\n打开文档2失败"));
					return;
				}
				CmpReportHelper::instance.m_bInChangeDwg=true;
				::SetTimer(acedGetAcadFrame()->GetSafeHwnd(),276,500,TimerProc_StartPlot);
			}
			else if(pRC->nType==1)
			{
				AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
				for (; !pIter->done(); pIter->step())
				{
					if(pIter->document()!=pRC->m_pDoc2&&pIter->document()!=pRC->m_pDoc1)
						ShowDocWindow(pIter->document(),SW_MINIMIZE);
				}
				delete pIter;
				pIter=NULL;
				AcApDocumentIterator* pIter1 = acDocManager->newAcApDocumentIterator();
				for (; !pIter1->done(); pIter1->step())
				{
					if(pIter1->document()==pRC->m_pDoc2)
						ShowDocWindow(pIter1->document(),SW_NORMAL);
				}
				delete pIter1;
				pIter1=NULL;
				::SetTimer(acedGetAcadFrame()->GetSafeHwnd(),276,500,TimerProc_StartPlot);

			}
		}
	}
	else if(nIDEvent==276)
	{
		KillTimer(hWnd,nIDEvent);
		if(pRC->m_pDoc1==NULL||pRC->m_pDoc2==NULL)
		{
			CString strCommand;
			strCommand.Format(_T("_ZOOM E "));
#if _MSC_VER>=1900
			acDocManager->pushAcadResourceHandle();
#else
			acDocManager->pushResourceHandle(acedGetAcadResourceInstance());
#endif
			Acad::ErrorStatus es=acDocManager->appContextOpenDocument(pRC->m_strFile1);
			acDocManager->popResourceHandle();
			if(es==Acad::eOk)
			{
				pRC->m_pDoc1=acDocManager->mdiActiveDocument();
				acDocManager->sendStringToExecute(acDocManager->mdiActiveDocument(), strCommand);
			}
			else
			{
				acutPrintf(_T("\n打开文档1失败"));
				m_bInChangeDwg=false;
				return;
			}
			::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_COMMAND,ID_WINDOW_TILE_VERT,2);
			m_bInChangeDwg=false;
		}
		else
		{

			AcApDocumentIterator* pIter1 = acDocManager->newAcApDocumentIterator();
			for (; !pIter1->done(); pIter1->step())
			{
				if(pIter1->document()==pRC->m_pDoc1)
					ShowDocWindow(pIter1->document(),SW_MAXIMIZE);
			}
			delete pIter1;
			pIter1=NULL;
			::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd,WM_COMMAND,ID_WINDOW_TILE_VERT,2);
			m_bInChangeDwg=false;
		}
	}
	else if(nIDEvent==282)
	{
		if(!GCmpResultInstance::instance.CheckDocument())
		{
			return;
		}
		m_mutex1.Lock();
		GDocViewTableRecordData dat;
		dat.m_pDoc=acDocManager->mdiActiveDocument();
		AcDbViewTableRecord view;
		getCurrentView(view);
		dat.m_CenterPt=view.centerPoint();
		dat.m_dWidth=view.width();
		dat.m_dHeight=view.height();
		m_viewDatas.Add(dat);
		m_mutex1.Unlock();
	}
	else if(nIDEvent==284)
	{
		if(!m_bReg)
		{
			return;
		}
		if(!GCmpResultInstance::instance.CheckDocument())
		{
			return;
		}
		m_mutex1.Lock();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		if(pDoc1==NULL)
		{
			KillTimer(hWnd,nIDEvent);
			return;
		}
		if(pDoc1==pRC->m_pDoc1||pDoc1==pRC->m_pDoc2)
		{
			GDocViewTableRecordData dat;
			bool bFind=false;
			for(int i=0;i<m_viewDatas.GetCount();i++)
			{
				GDocViewTableRecordData& tmp=m_viewDatas.GetAt(i);
				if(tmp.m_pDoc==pRC->m_pDoc1||tmp.m_pDoc==pRC->m_pDoc2)
				{
					dat=tmp;
					bFind=true;
				}
			}
			if(bFind)
			{
				if(m_lastSameData.m_pDoc==NULL||(!m_lastSameData.IsSame(dat)))
				{
					if(dat.m_pDoc==pRC->m_pDoc1)
					{
						//是自己设置的忽略
						if(m_selfSetView1.IsSame(dat))
						{
							m_viewDatas.RemoveAll();
							return;
						}
						else
						{
							m_selfSetView1.m_dWidth=-1;
							m_selfSetView1.m_dHeight=-1;
							m_selfSetView2.m_dWidth=-1;
							m_selfSetView2.m_dHeight=-1;
						}
					}
					else if(dat.m_pDoc==pRC->m_pDoc2)
					{
						//是自己设置的忽略
						if(m_selfSetView2.IsSame(dat))
						{
							m_viewDatas.RemoveAll();
							return;
						}
						else
						{
							m_selfSetView1.m_dWidth=-1;
							m_selfSetView1.m_dHeight=-1;
							m_selfSetView2.m_dWidth=-1;
							m_selfSetView2.m_dHeight=-1;
						}
					}
					else
					{
						m_selfSetView1.m_dWidth=-1;
						m_selfSetView1.m_dHeight=-1;
						m_selfSetView2.m_dWidth=-1;
						m_selfSetView2.m_dHeight=-1;
					}
					AcApDocument* pDoc2=dat.m_pDoc==pRC->m_pDoc1?pRC->m_pDoc2:pRC->m_pDoc1;
					if(pDoc2!=pDoc1)
						acDocManager->activateDocument(pDoc2);
					AcDbViewTableRecord pVwRec;
					pVwRec.setCenterPoint(dat.m_CenterPt);
					pVwRec.setWidth(dat.m_dWidth);
					pVwRec.setHeight(dat.m_dHeight);
					m_lastSameData=dat;
					CString strInfo;
					strInfo.Format(_T("同步视图[%s]:中心点(%.2f,%.2f),Width=%.2f,Height=%.2f"),dat.m_pDoc==pRC->m_pDoc1?_T("原始文件->比较文件"):_T("比较文件->原始文件"),dat.m_CenterPt.x,dat.m_CenterPt.y,dat.m_dWidth,dat.m_dHeight);
					OutputDebugString(strInfo);
					GLog::WriteLine(strInfo);

					Acad::ErrorStatus es=acedSetCurrentView(&pVwRec,NULL);
					actrTransactionManager->flushGraphics();
					acedUpdateDisplay();
					if(pDoc2!=pDoc1)
						acDocManager->activateDocument(pDoc1);
				}
				m_viewDatas.RemoveAll();
			}
		}
		else
		{

		}
		m_mutex1.Unlock();
	}
}