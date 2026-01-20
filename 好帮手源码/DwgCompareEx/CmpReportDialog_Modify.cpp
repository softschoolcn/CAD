#include "StdAfx.h"
#include "resource.h"
#include "CmpReportDialog.h"
#include "CommCommand.h"
#include "GLockDocument.h"
bool CmpReportDialog::CheckEditDocument()
{
	if(CmpReportHelper::instance.m_bInChangeDwg)
	{
		AfxMessageBox(_T("正在切换文档，请稍后..."));
		return false;
	}
	if(m_bInChangeEnt)
	{
		AfxMessageBox(_T("正在切换实体，请稍后..."));
		return false;
	}
	CmpResult* pRC=GCmpResultInstance::instance.GetCurrentResult();
	if(pRC->m_pDoc1==NULL||pRC->m_pDoc2==NULL)
	{
		AfxMessageBox(_T("请先打开图纸，然后再编辑，文件夹比较的时候双击文件列表中的文件就可以打开图纸"));
		return false;
	}
	if(!GCmpResultInstance::instance.CheckDocument())
	{
		AfxMessageBox(_T("图纸比较过程中请不要关闭文档，否则可能导致未知错误"));
		return false;
	}
	return true;
}
void CmpReportDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//DXF左到右
	if(!CheckEditDocument())
	{
		return;
	}
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	nSelect = m_ListRight.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffItem* pDiffItem=(CmpDiffItem*)m_ListRight.GetItemData(nSelect);
	if(!pDiffItem->m_bOnlyOne)
	{
		AfxMessageBox(_T("暂时不支持DXF码多个值的情况，请关注版本更新"));
		return;
	}
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	if(pCmpRc==NULL)
	{
		return;
	}
	AcDbObjectId retId;
	AcDbHandle objHandle=pRC->strHandle1;
	CString strInfo;
	Acad::ErrorStatus es=pCmpRc->m_pDoc1->database()->getAcDbObjectId(retId,false,objHandle);
	if(es!=Acad::eOk)
	{
		strInfo.Format(_T("获取原始对象失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	ads_name ent;
	es=acdbGetAdsName(ent,retId);
	if(es!=eOk)
	{
		strInfo.Format(_T("获取原始对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	struct resbuf* rb1=acdbEntGet(ent);
	if(rb1==NULL)
	{
		strInfo=_T("获取原始对象DXF数据失败");
		AfxMessageBox(strInfo);
		return;
	}

	objHandle=pRC->strHandle2;
	es=pCmpRc->m_pDoc2->database()->getAcDbObjectId(retId,false,objHandle);
	if(es!=Acad::eOk)
	{
		acutRelRb(rb1);
		strInfo.Format(_T("获取比较对象失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	ads_name ent1;
	es=acdbGetAdsName(ent1,retId);
	if(es!=eOk)
	{
		acutRelRb(rb1);
		strInfo.Format(_T("获取比较对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	struct resbuf* rb2=acdbEntGet(ent1);
	if(rb2==NULL)
	{
		acutRelRb(rb1);
		strInfo=_T("获取比较对象DXF数据失败");
		AfxMessageBox(strInfo);
		return;
	}
	AcApDocument* pDoc=pCmpRc->m_pDoc2;
	AcApDocument* pDocCur=acDocManager->curDocument();
	AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
	acDocManager->activateDocument(pDoc);
	acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
	acDocManager->lockDocument(pDoc);
	GSameDxfHelper helper;
	helper.SameDxf(rb2,rb1,pDiffItem->nDxfCode);
	if(acdbEntMod(rb2)!=RTNORM)
	{
		acDocManager->unlockDocument(pDoc);
		strInfo=_T("修改比较对象DXF数据失败");
		AfxMessageBox(strInfo);
		acutRelRb(rb1);
		acutRelRb(rb2);
		return;
	}

	acdbEntUpd(ent1);
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	acDocManager->unlockDocument(pDoc);
	acutRelRb(rb1);
	acutRelRb(rb2);
	acDocManager->activateDocument(pDoc1);
	acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
	pDiffItem->m_bRevise=TRUE;
	nSelect = m_ListRight.GetSelectionMark();
	m_ListRight.DeleteItem(nSelect);
	if(m_ListRight.GetItemCount()==0)
	{
		nSelect = m_ListLeft.GetSelectionMark();
		pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
		pRC->m_bRevise=TRUE;
		m_ListLeft.DeleteItem(nSelect);
	}
}

void CmpReportDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//DXF右到左
	if(!CheckEditDocument())
	{
		return;
	}
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	nSelect = m_ListRight.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffItem* pDiffItem=(CmpDiffItem*)m_ListRight.GetItemData(nSelect);
	if(!GCmpResultInstance::instance.CheckDocument())
	{
		return;
	}
	if(!pDiffItem->m_bOnlyOne)
	{
		AfxMessageBox(_T("暂时不支持DXF码多个值的情况，请关注版本更新"));
		return;
	}
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	if(pCmpRc==NULL)
	{
		return;
	}
	AcDbObjectId retId;
	AcDbHandle objHandle=pRC->strHandle2;
	CString strInfo;
	Acad::ErrorStatus es=pCmpRc->m_pDoc2->database()->getAcDbObjectId(retId,false,objHandle);
	if(es!=Acad::eOk)
	{
		strInfo.Format(_T("获取比较对象失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	ads_name ent;
	es=acdbGetAdsName(ent,retId);
	if(es!=eOk)
	{
		strInfo.Format(_T("获取比较对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	struct resbuf* rb1=acdbEntGet(ent);
	if(rb1==NULL)
	{
		strInfo=_T("获取原始对象DXF数据失败");
		AfxMessageBox(strInfo);
		return;
	}

	objHandle=pRC->strHandle1;
	es=pCmpRc->m_pDoc1->database()->getAcDbObjectId(retId,false,objHandle);
	if(es!=Acad::eOk)
	{
		acutRelRb(rb1);
		strInfo.Format(_T("获取原始对象失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	ads_name ent1;
	es=acdbGetAdsName(ent1,retId);
	if(es!=eOk)
	{
		acutRelRb(rb1);
		strInfo.Format(_T("获取原始对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
		AfxMessageBox(strInfo);
		return;
	}
	struct resbuf* rb2=acdbEntGet(ent1);
	if(rb2==NULL)
	{
		acutRelRb(rb1);
		strInfo=_T("获取原始对象DXF数据失败");
		AfxMessageBox(strInfo);
		return;
	}
	AcApDocument* pDoc=pCmpRc->m_pDoc1;
	AcApDocument* pDocCur=acDocManager->curDocument();
	AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
	acDocManager->activateDocument(pDoc);
	acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
	acDocManager->lockDocument(pDoc);
	GSameDxfHelper helper;
	helper.SameDxf(rb2,rb1,pDiffItem->nDxfCode);
	if(acdbEntMod(rb2)!=RTNORM)
	{
		acDocManager->unlockDocument(pDoc);
		strInfo=_T("修改原始对象DXF数据失败");
		AfxMessageBox(strInfo);
		acutRelRb(rb1);
		acutRelRb(rb2);
		return;
	}

	acdbEntUpd(ent1);
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	acDocManager->unlockDocument(pDoc);
	acutRelRb(rb1);
	acutRelRb(rb2);
	acDocManager->activateDocument(pDoc1);
	acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
	pDiffItem->m_bRevise=TRUE;
	nSelect = m_ListRight.GetSelectionMark();
	m_ListRight.DeleteItem(nSelect);
	if(m_ListRight.GetItemCount()==0)
	{
		nSelect = m_ListLeft.GetSelectionMark();
		pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
		pRC->m_bRevise=TRUE;
		m_ListLeft.DeleteItem(nSelect);
	}
}
void CmpReportDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//实体,左->右
	if(!CheckEditDocument())
	{
		return;
	}
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	if(pCmpRc==NULL)
	{
		return;
	}
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	CString strInfo;
	if(pRC->nType==1)
	{
		AcDbObjectId retId;
		AcDbHandle objHandle=pRC->strHandle1;
		Acad::ErrorStatus es=pCmpRc->m_pDoc1->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			strInfo.Format(_T("获取原始对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		ads_name ent;
		es=acdbGetAdsName(ent,retId);
		if(es!=eOk)
		{
			strInfo.Format(_T("获取原始对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		struct resbuf* rb1=acdbEntGet(ent);
		if(rb1==NULL)
		{
			strInfo=_T("获取原始对象DXF数据失败");
			AfxMessageBox(strInfo);
			return;
		}

		objHandle=pRC->strHandle2;
		es=pCmpRc->m_pDoc2->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			acutRelRb(rb1);
			strInfo.Format(_T("获取比较对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		ads_name ent1;
		es=acdbGetAdsName(ent1,retId);
		if(es!=eOk)
		{
			acutRelRb(rb1);
			strInfo.Format(_T("获取比较对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		struct resbuf* rb2=acdbEntGet(ent1);
		if(rb2==NULL)
		{
			acutRelRb(rb1);
			strInfo=_T("获取比较对象DXF数据失败");
			AfxMessageBox(strInfo);
			return;
		}
		AcApDocument* pDoc=pCmpRc->m_pDoc2;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		strInfo.Empty();
		for(int i=m_ListRight.GetItemCount()-1;i>=0;i--)
		{
			CmpDiffItem* pDiffItem=(CmpDiffItem*)m_ListRight.GetItemData(i);
			if(!pDiffItem->m_bOnlyOne)
			{
				if(!strInfo.IsEmpty())
				{
					strInfo+=_T("\r\n");
				}
				strInfo.AppendFormat(_T("修改[%s]失败,暂时不支持DXF码多个值的情况，请关注版本更新"),pDiffItem->strName);
				continue;
			}
			GSameDxfHelper helper;
			helper.SameDxf(rb2,rb1,pDiffItem->nDxfCode);
			if(acdbEntMod(rb2)!=RTNORM)
			{
				strInfo.AppendFormat(_T("修改[%s]失败,修改比较对象DXF数据失败"),pDiffItem->strName);
				acutRelRb(rb1);
				acutRelRb(rb2);
				continue;
			}
			pDiffItem->m_bRevise=TRUE;
			m_ListRight.DeleteItem(i);
		}
		acDocManager->unlockDocument(pDoc);


		acdbEntUpd(ent1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);
		acutRelRb(rb1);
		acutRelRb(rb2);
		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
		if(m_ListRight.GetItemCount()==0)
		{
			nSelect = m_ListLeft.GetSelectionMark();
			pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
			pRC->m_bRevise=TRUE;
			m_ListLeft.DeleteItem(nSelect);
		}
		if(!strInfo.IsEmpty())
		{
			AfxMessageBox(strInfo);
		}
	}
	else if(pRC->nType==2)
	{
		//新增
		//把新增加的对象删除掉
		AcDbObjectId retId;
		AcDbHandle objHandle=pRC->strHandle2;
		Acad::ErrorStatus es=pCmpRc->m_pDoc2->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			strInfo.Format(_T("获取比较对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		ads_name ent1;
		es=acdbGetAdsName(ent1,retId);
		if(es!=eOk)
		{
			strInfo.Format(_T("获取比较对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		AcApDocument* pDoc=pCmpRc->m_pDoc2;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		es=ForceEraseEntity(retId);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);
		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
		if(es==Acad::eOk)
		{
			nSelect = m_ListLeft.GetSelectionMark();
			pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
			pRC->m_bRevise=TRUE;
			m_ListLeft.DeleteItem(nSelect);
			m_ListRight.DeleteAllItems();
		}
		else
		{
			strInfo.Format(_T("删除新增对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}


    }
	else if(pRC->nType==3)
	{
		AfxMessageBox(_T("暂时不支持删除对象的恢复，请关注版本更新"));
		return;
	}
}

void CmpReportDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//实体,右->左
	if(!CheckEditDocument())
	{
		return;
	}
	int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpResult* pCmpRc=GCmpResultInstance::instance.GetCurrentResult();
	if(pCmpRc==NULL)
	{
		return;
	}
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	CString strInfo;
	if(pRC->nType==1)
	{
		AcDbObjectId retId;
		AcDbHandle objHandle=pRC->strHandle2;
		Acad::ErrorStatus es=pCmpRc->m_pDoc2->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			strInfo.Format(_T("获取比较对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		ads_name ent;
		es=acdbGetAdsName(ent,retId);
		if(es!=eOk)
		{
			strInfo.Format(_T("获取比较对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		struct resbuf* rb1=acdbEntGet(ent);
		if(rb1==NULL)
		{
			strInfo=_T("获取比较对象DXF数据失败");
			AfxMessageBox(strInfo);
			return;
		}

		objHandle=pRC->strHandle1;
		es=pCmpRc->m_pDoc1->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			acutRelRb(rb1);
			strInfo.Format(_T("获取原始对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		ads_name ent1;
		es=acdbGetAdsName(ent1,retId);
		if(es!=eOk)
		{
			acutRelRb(rb1);
			strInfo.Format(_T("获取原始对象AdsName失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		struct resbuf* rb2=acdbEntGet(ent1);
		if(rb2==NULL)
		{
			acutRelRb(rb1);
			strInfo=_T("获取原始对象DXF数据失败");
			AfxMessageBox(strInfo);
			return;
		}
		AcApDocument* pDoc=pCmpRc->m_pDoc1;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		strInfo.Empty();
		for(int i=m_ListRight.GetItemCount()-1;i>=0;i--)
		{
			CmpDiffItem* pDiffItem=(CmpDiffItem*)m_ListRight.GetItemData(i);
			if(!pDiffItem->m_bOnlyOne)
			{
				if(!strInfo.IsEmpty())
				{
					strInfo+=_T("\r\n");
				}
				strInfo.AppendFormat(_T("修改[%s]失败,暂时不支持DXF码多个值的情况，请关注版本更新"),pDiffItem->strName);
				continue;
			}
			GSameDxfHelper helper;
			helper.SameDxf(rb2,rb1,pDiffItem->nDxfCode);
			if(acdbEntMod(rb2)!=RTNORM)
			{
				strInfo.AppendFormat(_T("修改[%s]失败,修改原始对象DXF数据失败"),pDiffItem->strName);
				acutRelRb(rb1);
				acutRelRb(rb2);
				continue;
			}
			pDiffItem->m_bRevise=TRUE;
			m_ListRight.DeleteItem(i);
		}
		acDocManager->unlockDocument(pDoc);


		acdbEntUpd(ent1);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);
		acutRelRb(rb1);
		acutRelRb(rb2);
		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
		if(m_ListRight.GetItemCount()==0)
		{
			nSelect = m_ListLeft.GetSelectionMark();
			pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
			pRC->m_bRevise=TRUE;
			m_ListLeft.DeleteItem(nSelect);
		}
		if(!strInfo.IsEmpty())
		{
			AfxMessageBox(strInfo);
		}
	}
	else if(pRC->nType==2)
	{
		//新增对象
		AfxMessageBox(_T("暂时不支持新增对象拷贝到原始文件中，请关注版本更新"));
		return;
	}
	else if(pRC->nType==3)
	{
		AcDbObjectId retId;
		AcDbHandle objHandle=pRC->strHandle1;
		Acad::ErrorStatus es=pCmpRc->m_pDoc1->database()->getAcDbObjectId(retId,false,objHandle);
		if(es!=Acad::eOk)
		{
			strInfo.Format(_T("获取原始对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
		AcApDocument* pDoc=pCmpRc->m_pDoc1;
		AcApDocument* pDocCur=acDocManager->curDocument();
		AcApDocument* pDoc1=acDocManager->mdiActiveDocument();
		acDocManager->activateDocument(pDoc);
		acDocManager->setCurDocument(pDoc,AcAp::kNone,true);
		acDocManager->lockDocument(pDoc);
		es=ForceEraseEntity(retId);
		actrTransactionManager->queueForGraphicsFlush();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		acDocManager->unlockDocument(pDoc);
		acDocManager->activateDocument(pDoc1);
		acDocManager->setCurDocument(pDoc1,AcAp::kNone,true);
		if(es==Acad::eOk)
		{
			nSelect = m_ListLeft.GetSelectionMark();
			pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
			pRC->m_bRevise=TRUE;
			m_ListLeft.DeleteItem(nSelect);
			m_ListRight.DeleteAllItems();
		}
		else
		{
			strInfo.Format(_T("删除对象失败,失败原因:%s"),acadErrorStatusText(es));
			AfxMessageBox(strInfo);
			return;
		}
	}
}