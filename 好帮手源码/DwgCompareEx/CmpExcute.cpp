#include "StdAfx.h"
#include "CmpExcute.h"
#include "GLockDocument.h"
#include "CommCommand.h"
#include "CmpDwgDiffFinder.h"
CmpExcute::CmpExcute(CmpConfig* pConfig)
{
	m_pConfig=pConfig;
}

CmpExcute::~CmpExcute(void)
{
}
Acad::ErrorStatus CmpExcute::Excute(long& diffCount)
{
	GLockDocument _Lock;
	Acad::ErrorStatus es=eOk;
	DWORD ts=0;
	diffCount=0;
	if(m_pConfig->m_pDoc1==NULL)
	{
		//读取文件1
		acutPrintf(_T("\n读取文件1"));
		Glb_TickCount=GetTickCount();
		GLog::WriteLine(_T("开始读取文件1:%s"),m_pConfig->m_strTmpFile1);
		try
		{
			m_cmpdb[0].reset(new AcDbDatabase(false));
			es=m_cmpdb[0]->readDwgFile(m_pConfig->m_strTmpFile1);
			if(es!=eOk)
			{
				acutPrintf(_T("\n 打开文件1失败！%s"),acadErrorStatusText(es));
				return es;
			}
			ts=GetTickCount()-Glb_TickCount;
			Glb_TickCount=GetTickCount();
			GLog::WriteLine(_T("完成读取文件1:%s,耗时:%d毫秒"),m_pConfig->m_strTmpFile1,ts);
		}
		catch(...)
		{
			return eFilerError;
		}
	}
	if(m_pConfig->m_pDoc2==NULL)
	{
		//读取文件2
		acutPrintf(_T("\n读取文件2"));
		Glb_TickCount=GetTickCount();
		GLog::WriteLine(_T("开始读取文件2:%s"),m_pConfig->m_strTmpFile2);
		try
		{
			m_cmpdb[1].reset(new AcDbDatabase(false));
			es=m_cmpdb[1]->readDwgFile(m_pConfig->m_strTmpFile2);
			if(es!=eOk)
			{
				acutPrintf(_T("\n 打开文件2失败！%s"),acadErrorStatusText(es));
				return es;
			}
			ts=GetTickCount()-Glb_TickCount;
			Glb_TickCount=GetTickCount();
			GLog::WriteLine(_T("完成读取文件2:%s,耗时:%d毫秒"),m_pConfig->m_strTmpFile2,ts);
		}
		catch(...)
		{
			return eFilerError;
		}
	}
	//比较差异
	acutPrintf(_T("\n开始比较差异"));
	GLog::WriteLine(_T("开始比较差异"));
	CmpDwgDiffFinder finder(m_pConfig,
		m_pConfig->m_pDoc1==NULL?m_cmpdb[0].get():m_pConfig->m_pDoc1->database(),
		m_pConfig->m_pDoc2==NULL?m_cmpdb[1].get():m_pConfig->m_pDoc2->database());
	Glb_TickCount=GetTickCount();
	es=finder.Find(this->m_Result,diffCount);
	if(!m_pConfig->m_bSourceFile)
	ChangeResultColor();
	ts=GetTickCount()-Glb_TickCount;
		Glb_TickCount=GetTickCount();
	GLog::WriteLine(_T("完成比较,耗时:%d毫秒"),ts);
	return es;
}
Acad::ErrorStatus OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	Acad::ErrorStatus es=Acad::eOk;

	AcDbLayerTable *pLayerTable=NULL;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk ) 
	{
		//acutPrintf (_T("\n不能创建表迭代器")) ;
		pLayerTable->close () ;
		return es;
	}
	pLayerTable->close () ;

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		bool bLock=pLayerTableRecord->isLocked();
		bool bFrozen=pLayerTableRecord->isFrozen();
		if(bLock||bFrozen)
		{
			es=pLayerTableRecord->upgradeOpen();
			if(es!=eOk)
			{
				pLayerTableRecord->close();
				continue;
			}
			pLayerTableRecord->setIsLocked(false);
			pLayerTableRecord->setIsFrozen(false);
			ids.append(pLayerTableRecord->objectId());
			locks.append(bLock);
			frozens.append(bFrozen);

		}
		pLayerTableRecord->close();
	}
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	return Acad::eOk;
}
Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbLayerTableRecordPointer spLayer(ids.at(i),AcDb::kForWrite);
		if(spLayer.openStatus()==Acad::eOk)
		{
			spLayer->setIsLocked(locks.at(i));
			spLayer->setIsFrozen(frozens.at(i));
		}
	}
	return Acad::eOk;
}
//根据结果改变实体的颜色
void CmpExcute::ChangeResultColor()
{
	AcArray<bool> locks[2];
	AcArray<bool> frozens[2];
	AcDbObjectIdArray ids[2];
	OpenAllLayer(m_cmpdb[0].get(),ids[0],locks[0],frozens[0]);
	OpenAllLayer(m_cmpdb[1].get(),ids[1],locks[1],frozens[1]);
	for(long i=0;i<m_Result.m_diffsInBlockDefines.GetCount();i++)
	{
		CmpBlockDiffResult& rc=m_Result.m_diffsInBlockDefines.GetAt(i);
		ChangeResultColor(rc.m_diffItems,true);
	}
	ChangeResultColor(m_Result.m_diffsInModelSpace,false);
	ResetAllLayer(ids[0],locks[0],frozens[0]);
	ResetAllLayer(ids[1],locks[1],frozens[1]);
	const SecurityParams* pSecParams=NULL;
	m_cmpdb[0]->saveAs(m_pConfig->m_strTmpFile1,pSecParams);
	m_cmpdb[1]->saveAs(m_pConfig->m_strTmpFile2,pSecParams);
}
void CmpExcute::ChangeResultColor(CArray<CmpDiffResult>&   diffResults,bool bBlock)
{
	Acad::ErrorStatus es=eOk;
	for(long i=0;i<diffResults.GetCount();i++)
	{
		CmpDiffResult& rc=diffResults.GetAt(i);
		
		if(rc.nType==0||rc.nType==1)
		{
			//完全相同/改变了
			int nColor=rc.nType==0?m_pConfig->m_nColorSame:m_pConfig->m_nColorChange;
			if(bBlock&&rc.nType==0)
			{
				nColor=0;//如果是完全相同的话，就把块设置为ByBlock
			}
			AcDbHandle handle1=rc.strHandle1;
			AcDbObjectId recId;
			es=m_cmpdb[0]->getAcDbObjectId(recId,false,handle1);
			if(es==eOk)
			{
				SetEntityColor(recId,nColor,rc.colRecord1);
			}
			AcDbHandle handle2=rc.strHandle2;
			es=m_cmpdb[1]->getAcDbObjectId(recId,false,handle2);
			if(es==eOk)
			{
				SetEntityColor(recId,nColor,rc.colRecord2);
			}
		}
		else if(rc.nType==2)
		{
			AcDbHandle handle=rc.strHandle2;
			//新增加
			AcDbObjectId recId;
			es=m_cmpdb[1]->getAcDbObjectId(recId,false,handle);
			if(es==eOk)
			{
				SetEntityColor(recId,m_pConfig->m_nColorNew,rc.colRecord2);
			}
		}
		else if(rc.nType==3)
		{
			//删除
			AcDbHandle handle=rc.strHandle1;
			AcDbObjectId recId;
			es=m_cmpdb[0]->getAcDbObjectId(recId,false,handle);
			if(es==eOk)
			{
				SetEntityColor(recId,m_pConfig->m_nColorDelete,rc.colRecord1);
			}
		}
		diffResults.SetAt(i,rc);
	}
}