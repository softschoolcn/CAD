#include "StdAfx.h"
#include "resource.h"
#include "CmpReportDialog.h"
#include "CommCommand.h"
#include "GOpenCmdDocHelper.h"
AcDbObjectId lastSelectId;
void CmpReportDialog::OnTimer_OpenDoc(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	GOpenCmdDocHelper helper;
	if(helper.OpenCmpDoc(pCmpRc)!=Acad::eOk)
	{
		return;
	}
	if(pCmpRc->nType==2||pCmpRc->nType==3)
	{
		return;
	}
	SetTimer(282,100,NULL);
	SetTimer(284,1000,NULL);
}
void CmpReportDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	CAcUiDialog::OnTimer(nIDEvent);
	if(nIDEvent==274)
	{
		OnTimer_OpenDoc(nIDEvent);
	}
	else if(nIDEvent==278)
	{
		KillTimer(nIDEvent);
		int nSelect = m_ListLeft.GetSelectionMark();
		if(nSelect < 0)
		{
			m_bInChangeEnt=false;
			return;
		}
		CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
		if(!GCmpResultInstance::instance.CheckDocument())
		{
			m_bInChangeEnt=false;
			return;
		}
		AcDbExtents ext;
		if(pRC->nType==3)
		{
			m_bInChangeEnt=false;
			AddExt(ext,pCmpRc->m_pDoc1,pRC->strHandle1);
			m_ext=ext;
			ShowEntity(pCmpRc->m_pDoc1,pRC->strHandle1,2);
		}
		else if(pRC->nType==2)
		{
			m_bInChangeEnt=false;
			AddExt(ext,pCmpRc->m_pDoc2,pRC->strHandle2);
			m_ext=ext;
			ShowEntity(pCmpRc->m_pDoc2,pRC->strHandle2,2);
		}
		else if(pRC->nType==1)
		{
			AddExt(ext,pCmpRc->m_pDoc1,pRC->strHandle1);
			AddExt(ext,pCmpRc->m_pDoc2,pRC->strHandle2);
			m_ext=ext;
			ShowEntity(pCmpRc->m_pDoc1,pRC->strHandle1,2);

		}
		SetTimer(280,100,NULL);
	}
	else if(nIDEvent==280)
	{
		KillTimer(nIDEvent);
		int nSelect = m_ListLeft.GetSelectionMark();
		if(nSelect < 0)
		{
			m_bInChangeEnt=false;
			return;
		}
		CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
		if(!GCmpResultInstance::instance.CheckDocument())
		{
			m_bInChangeEnt=false;
			return;
		}
		if(pRC->nType==1)
		{
			ShowEntity(pCmpRc->m_pDoc2,pRC->strHandle2,2);
		}
		else
		{
			ShowEntity(pCmpRc->m_pDoc2,pRC->strHandle2,1);
		}
		m_bInChangeEnt=false;
		return;
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
		if(dat.m_pDoc==pCmpRc->m_pDoc1||dat.m_pDoc==pCmpRc->m_pDoc2)
		{
			AcDbObjectIdArray ids;
			GetCurrentSelectObjIds(ids);
			if(ids.logicalLength()!=1)
			{
				return;
			}
			if(lastSelectId==ids.at(0))
			{
				return;
			}
			lastSelectId=ids.at(0);
			TCHAR tmpStr[256]={0};
			lastSelectId.handle().getIntoAsciiBuffer(tmpStr);
			for(long i=0;i<m_ListLeft.GetItemCount();i++)
			{
				m_ListLeft.SetItemState(i,0,-1);
			}
			for(long i=0;i<m_ListLeft.GetItemCount();i++)
			{
				CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(i);
				if((dat.m_pDoc==pCmpRc->m_pDoc1&&pRC->strHandle1.CompareNoCase(tmpStr)==0)||
					(dat.m_pDoc==pCmpRc->m_pDoc2&&pRC->strHandle2.CompareNoCase(tmpStr)==0))
				{
					m_ListLeft.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					m_ListLeft.SetSelectionMark(i);
					m_ListLeft.EnsureVisible(i,TRUE);
					richTextBox1.SetWindowText(_T(""));
					richTextBox2.SetWindowText(_T(""));
					m_selfSetView1.m_dWidth=-1;
					m_selfSetView1.m_dHeight=-1;
					m_selfSetView2.m_dWidth=-1;
					m_selfSetView2.m_dHeight=-1;
					if(CmpReportHelper::instance.m_bInChangeDwg)
					{
						return;
					}
					if(m_bInChangeEnt)
					{
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
					break;
				}
			}

		}
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
			KillTimer(nIDEvent);
			return;
		}
		if(pDoc1==pCmpRc->m_pDoc1||pDoc1==pCmpRc->m_pDoc2)
		{
			GDocViewTableRecordData dat;
			bool bFind=false;
			for(int i=0;i<m_viewDatas.GetCount();i++)
			{
				GDocViewTableRecordData& tmp=m_viewDatas.GetAt(i);
				if(tmp.m_pDoc==pCmpRc->m_pDoc1||tmp.m_pDoc==pCmpRc->m_pDoc2)
				{
					dat=tmp;
					bFind=true;
				}
			}
			if(bFind)
			{
				if(m_lastSameData.m_pDoc==NULL||(!m_lastSameData.IsSame(dat)))
				{
					if(dat.m_pDoc==pCmpRc->m_pDoc1)
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
					else if(dat.m_pDoc==pCmpRc->m_pDoc2)
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
					AcApDocument* pDoc2=dat.m_pDoc==pCmpRc->m_pDoc1?pCmpRc->m_pDoc2:pCmpRc->m_pDoc1;
					if(pDoc2!=pDoc1)
						acDocManager->activateDocument(pDoc2);
					AcDbViewTableRecord pVwRec;
					pVwRec.setCenterPoint(dat.m_CenterPt);
					pVwRec.setWidth(dat.m_dWidth);
					pVwRec.setHeight(dat.m_dHeight);
					m_lastSameData=dat;
					CString strInfo;
					strInfo.Format(_T("同步视图[%s]:中心点(%.2f,%.2f),Width=%.2f,Height=%.2f"),dat.m_pDoc==pCmpRc->m_pDoc1?_T("原始文件->比较文件"):_T("比较文件->原始文件"),dat.m_CenterPt.x,dat.m_CenterPt.y,dat.m_dWidth,dat.m_dHeight);
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