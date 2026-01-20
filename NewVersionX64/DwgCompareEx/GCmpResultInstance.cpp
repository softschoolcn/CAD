#include "Stdafx.h"
#include "GCmpResultInstance.h"
GCmpResultInstance GCmpResultInstance::instance;
GCmpResultInstance::GCmpResultInstance(void)
{
	mp_CurResult=NULL;
}

GCmpResultInstance::~GCmpResultInstance(void)
{
}
//删除所有结果
void GCmpResultInstance::DeleteAllResult()
{
	mp_CurResult=NULL;
	for(long i=mp_ResultArray.GetCount()-1;i>=0;i--)
	{
		CmpResult* pRC=mp_ResultArray.GetAt(i);
		delete pRC;
		pRC=NULL;
		mp_ResultArray.RemoveAt(i);
	}

}
//获取结果数量
int GCmpResultInstance::GetResultCount()
{
	return mp_ResultArray.GetCount();
}
//获取特定结果
CmpResult* GCmpResultInstance::GetResult(int nIndex)
{
	if(nIndex<0||nIndex>=mp_ResultArray.GetCount())
	{
		return NULL;
	}
	return mp_ResultArray.GetAt(nIndex);
}
//增加结果
void GCmpResultInstance::AppendResult(CmpResult* pRC)
{
	mp_ResultArray.Add(pRC);
}
//获取当前结果
CmpResult* GCmpResultInstance::GetCurrentResult()
{
	return mp_CurResult;
}
//设置当前结果
void GCmpResultInstance::SetCurrentResult(CmpResult* pRC)
{
	mp_CurResult=pRC;
}
//设置当前结果索引
void GCmpResultInstance::SetCurrentResultIndex(int nIndex)
{
	if(nIndex<0||nIndex>=mp_ResultArray.GetCount())
	{
		return;
	}
	SetCurrentResult(mp_ResultArray.GetAt(nIndex));
}
//检查文档
BOOL GCmpResultInstance::CheckDocument()
{
	if(mp_CurResult==NULL)
	{
		return FALSE;
	}
	return CheckDocument(mp_CurResult);
}
BOOL GCmpResultInstance::CheckDocument(CmpResult* pRC)
{
	if(pRC->m_pDoc1==NULL)
	{
		//AfxMessageBox(_T("打开文档1失败"));
		return FALSE;
	}
	if(pRC->m_pDoc2==NULL)
	{
		//AfxMessageBox(_T("打开文档2失败"));
		return FALSE;
	}
	BOOL bFindDoc1=FALSE;
	BOOL bFindDoc2=FALSE;
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step())
	{
		//acDocManager->closeDocument(pIter->document());
		if(pIter->document()==pRC->m_pDoc1)
		{
			bFindDoc1=TRUE;
		}
		else if(pIter->document()==pRC->m_pDoc2)
		{
			bFindDoc2=TRUE;
		}
	}
	delete pIter;
	pIter=NULL;
	if((!bFindDoc1)||(!bFindDoc1))
	{
		//AfxMessageBox(_T("请不要在比较的过程中关闭文档"));
		return FALSE;
	}
	return TRUE;

}
