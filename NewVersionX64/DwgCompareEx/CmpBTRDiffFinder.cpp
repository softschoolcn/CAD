#include "StdAfx.h"
#include "CmpBTRDiffFinder.h"
CmpBTRDiffFinder::CmpBTRDiffFinder(CmpConfig* pConfig,AcDbBlockTableRecord* pBTR1,AcDbBlockTableRecord* pBTR2):
CmpDiffFinder(pConfig)
{
	m_pBTR1=pBTR1;
	m_pBTR2=pBTR2;
}
CmpBTRDiffFinder::~CmpBTRDiffFinder()
{

}
void Remove(CArray<CmpEntityDat*>&        m_cmpDat,CArray<long>& indexArray)
{
	for(long j=indexArray.GetCount()-1;j>=0;j--)
	{
		m_cmpDat[indexArray.GetAt(j)]->bTmp=true;
	}
	CArray<CmpEntityDat*>        tmp;
	int nFind=-1;
	for(long i=0;i<m_cmpDat.GetCount();i++)
	{
		if(!m_cmpDat[i]->bTmp)
		{
			m_cmpDat[i]->bTmp=false;//还原
			tmp.Add(m_cmpDat.GetAt(i));
		}
		else
		{
			m_cmpDat[i]->bTmp=false;//还原
		}
	}
	m_cmpDat.RemoveAll();
	m_cmpDat.Append(tmp);

}
TCHAR GetChar(CString& str,int nIndex)
{
	if(nIndex<0||nIndex>=str.GetLength())
	{
		return _T('\0');
	}
	else
	{
		return str.GetAt(str.GetLength()-nIndex-1);
	}
}
Acad::ErrorStatus CmpBTRDiffFinder::SplitByLen(CArray<CmpDiffResult>& diffResults,long& diffCount,CmpDiffFinder& finder1)
{
	long nCount1=finder1.m_cmpDat[0].GetCount();
	long nCount2=finder1.m_cmpDat[1].GetCount();
	long nCount=nCount1*nCount2;
	//return finder1.Find(diffResults,diffCount);
	if(nCount1<100||nCount2<100||(nCount>=0&&nCount<1E6))
	{
		GLog::WriteLine(_T("执行比较[%d]-[%d]"),nCount1,nCount2);
		//1000*1000
		return finder1.Find(diffResults,diffCount);
	}
	else
	{
		if(m_pConfig->m_nMode==0)
		{
			if(nCount>1E8||nCount<0)
			{
				acutPrintf(_T("\r\n需要比较的对象太多%d*%d,请使用句柄模式或者关闭一些不需要比较的图层\r\n"),nCount1,nCount2);
				return Acad::eOk;
			}
			else
			{
				GLog::WriteLine(_T("执行比较[%d]-[%d]"),nCount1,nCount2);
				//1000*1000
				return finder1.Find(diffResults,diffCount);
			}
		}
		m_progressBar.Restore();
		if(finder1.strName.IsEmpty())
		{
			m_progressBar.Create(_T("正在比较......"),nCount1);
		}
		else
		{
			CString strTmp;
			strTmp.Format(_T("正在比较<%s>......"),finder1.strName);
			m_progressBar.Create(strTmp,nCount1);
		}
		//按长度拆分
		GLog::WriteLine(_T("按长度拆分[%d]-[%d]"),nCount1,nCount2);
		while(finder1.m_cmpDat[0].GetCount()>0)
		{
			CArray<long> indexArray1;
			CmpDiffFinder finder2(m_pConfig);
			finder2.strName=finder1.strName;
			CmpEntityDat* pDat=finder1.m_cmpDat[0].GetAt(finder1.m_cmpDat[0].GetCount()-1);
			indexArray1.Add(finder1.m_cmpDat[0].GetCount()-1);
			long nLen=pDat->m_strHandle.GetLength();
			finder2.m_cmpDat[0].Add(pDat);
			for(long i=finder1.m_cmpDat[0].GetCount()-2;i>=0;i--)
			{
				pDat=finder1.m_cmpDat[0].GetAt(i);
				long nLen1=pDat->m_strHandle.GetLength();
				if(nLen==nLen1)
				{
					finder2.m_cmpDat[0].Add(pDat);
					indexArray1.Add(i);
				}
			}
			Remove(finder1.m_cmpDat[0],indexArray1);
			indexArray1.RemoveAll();
			for(long i=finder1.m_cmpDat[1].GetCount()-1;i>=0;i--)
			{
				pDat=finder1.m_cmpDat[1].GetAt(i);
				long nLen1=pDat->m_strHandle.GetLength();
				if(nLen==nLen1)
				{
					finder2.m_cmpDat[1].Add(pDat);
					indexArray1.Add(i);
				}
			}
			Remove(finder1.m_cmpDat[1],indexArray1);
			indexArray1.RemoveAll();
			GLog::WriteLine(_T("按长度拆分[%d]-[%d]-[长度:%d]"),finder2.m_cmpDat[0].GetCount(),finder2.m_cmpDat[1].GetCount(),nLen);
			Find(diffResults,diffCount,finder2,0);

		}
		if(finder1.m_cmpDat[1].GetCount()>0)
		{
			finder1.Find(diffResults,diffCount);
		}
		m_progressBar.Restore();
	}
	
	return Acad::eOk;
}
Acad::ErrorStatus CmpBTRDiffFinder::Find(CArray<CmpDiffResult>& diffResults,long& diffCount,CmpDiffFinder& finder1,int nIndex)
{
	long nCount1=finder1.m_cmpDat[0].GetCount();
	long nCount2=finder1.m_cmpDat[1].GetCount();
	long nCount=nCount1*nCount2;
	//return finder1.Find(diffResults,diffCount);
	if(nCount1<100||nCount2<100||(nCount>=0&&nCount<1E6)||nIndex>5)
	{
		GLog::WriteLine(_T("执行比较[%d]-[%d]"),nCount1,nCount2);
		//1000*1000
		return finder1.Find(diffResults,diffCount,true,&m_progressBar);
	}
	else
	{
		
		//拆分
		GLog::WriteLine(_T("拆分[%d]-[%d]"),nCount1,nCount2);
		while(finder1.m_cmpDat[0].GetCount()>0)
		{
			CArray<long> indexArray1;
			CmpDiffFinder finder2(m_pConfig);
			finder2.strName=finder1.strName;
			CmpEntityDat* pDat=finder1.m_cmpDat[0].GetAt(finder1.m_cmpDat[0].GetCount()-1);
			indexArray1.Add(finder1.m_cmpDat[0].GetCount()-1);
			TCHAR c=GetChar(pDat->m_strHandle,nIndex);
			finder2.m_cmpDat[0].Add(pDat);
			for(long i=finder1.m_cmpDat[0].GetCount()-2;i>=0;i--)
			{
				pDat=finder1.m_cmpDat[0].GetAt(i);
				TCHAR c1=GetChar(pDat->m_strHandle,nIndex);
				if(c==c1)
				{
					finder2.m_cmpDat[0].Add(pDat);
					indexArray1.Add(i);
				}
			}
			long nCount3=finder1.m_cmpDat[0].GetCount();
			long nCount4=finder1.m_cmpDat[1].GetCount();
			Remove(finder1.m_cmpDat[0],indexArray1);
			if(finder1.m_cmpDat[0].GetCount()+indexArray1.GetCount()!=nCount3)
			{
				GLog::WriteLine(_T("****Error****finder1.m_cmpDat[0].GetCount()+indexArray1.GetCount()[%d]!=nCount3[%d]"),finder1.m_cmpDat[0].GetCount()+indexArray1.GetCount(),nCount3);
			}
			indexArray1.RemoveAll();
			for(long i=finder1.m_cmpDat[1].GetCount()-1;i>=0;i--)
			{
				pDat=finder1.m_cmpDat[1].GetAt(i);
				TCHAR c1=GetChar(pDat->m_strHandle,nIndex);
				if(c==c1)
				{
					finder2.m_cmpDat[1].Add(pDat);
					indexArray1.Add(i);
				}
			}
			Remove(finder1.m_cmpDat[1],indexArray1);
			if(finder1.m_cmpDat[1].GetCount()+indexArray1.GetCount()!=nCount4)
			{
				GLog::WriteLine(_T("****Error****finder1.m_cmpDat[1].GetCount()+indexArray1.GetCount()[%d]!=nCount4[%d]"),finder1.m_cmpDat[1].GetCount()+indexArray1.GetCount(),nCount4);
			}
			indexArray1.RemoveAll();
			Find(diffResults,diffCount,finder2,nIndex+1);

		}
		if(finder1.m_cmpDat[1].GetCount()>0)
		{
			finder1.Find(diffResults,diffCount);
		}
	}
	
	return Acad::eOk;
}
Acad::ErrorStatus CmpBTRDiffFinder::Find(CArray<CmpDiffResult>& diffResults,long& diffCount)
{
	//按实体类型做分类
	CMap<AcRxClass*,AcRxClass*,CArray<CmpEntityDat*>*,CArray<CmpEntityDat*>*> m_cmpDatDict[2];
	Acad::ErrorStatus es=eOk;
	DWORD t1=GetTickCount();
	for(int i=0;i<2;i++)
	{
		//读取图纸实体对象
		AcDbBlockTableRecordIterator* pIterator1=NULL;
		if(i==0)
		{
			es=m_pBTR1->newIterator(pIterator1);
		}
		else if(i==1)
		{
			es=m_pBTR2->newIterator(pIterator1);
		}
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\n 枚举文件模型空间实体失败！%s"),acadErrorStatusText(es));
			return es;
		}
		if(pIterator1==NULL)
		{
			return eNullHandle;
		}
		for(pIterator1->start();!pIterator1->done();pIterator1->step())
		{
			AcDbObjectId id;
			es=pIterator1->getEntityId(id);
			if(es!=eOk)
				continue;
			CmpEntityDat* dat=new CmpEntityDat;
			dat->nIndex=-1;
			es=ReadEntityDat(dat,id,i);
			if(es!=eOk)
			{
				delete dat;
				continue;
			}
			CArray<CmpEntityDat*>* pArr=NULL;
			if(m_cmpDatDict[i].Lookup(dat->m_type,pArr))
			{
				pArr->Add(dat);
			}
			else
			{
				pArr=new CArray<CmpEntityDat*>;
				pArr->Add(dat);
				m_cmpDatDict[i].SetAt(dat->m_type,pArr);

			}
		}
		delete pIterator1;
		pIterator1=NULL;
		if(i==0)
		{
			es=m_pBTR1->close();
		}
		else if(i==1)
		{
			es=m_pBTR2->close();
		}
	}
	DWORD t2=GetTickCount();
	//es=CmpDiffFinder::Find(diffResults,diffCount);
	DWORD t3=GetTickCount();
	GLog::WriteLine(_T("读取实体数据历时:%d毫秒,对比耗时:%d毫秒,共计历时:%d毫秒"),t2-t1,t3-t2,t3-t1);
	POSITION pos=m_cmpDatDict[0].GetStartPosition();
	while(pos)
	{
		AcRxClass* pType=NULL;
		CArray<CmpEntityDat*>* pArr=NULL;
		CArray<CmpEntityDat*>* pArr1=NULL;
		m_cmpDatDict[0].GetNextAssoc(pos,pType,pArr);
		CmpDiffFinder finder1(m_pConfig);
		finder1.m_cmpDat[0].Append(*pArr);
		finder1.strName.Format(_T("%s%s"),strName.IsEmpty()?_T(""):strName+_T("|"),pType->dxfName()!=NULL?pType->dxfName():_T("NULL"));
		if(m_cmpDatDict[1].Lookup(pType,pArr1))
		{
			finder1.m_cmpDat[1].Append(*pArr1);
			m_cmpDatDict[1].RemoveKey(pType);
		}
		SplitByLen(diffResults,diffCount,finder1);
	}
	pos=m_cmpDatDict[1].GetStartPosition();
	while(pos)
	{
		AcRxClass* pType=NULL;
		CArray<CmpEntityDat*>* pArr=NULL;
		CArray<CmpEntityDat*>* pArr1=NULL;
		m_cmpDatDict[1].GetNextAssoc(pos,pType,pArr);
		CmpDiffFinder finder1(m_pConfig);
		finder1.m_cmpDat[1].Append(*pArr);
		finder1.strName.Format(_T("%s%s"),strName.IsEmpty()?_T(""):strName+_T("|"),pType->dxfName()!=NULL?pType->dxfName():_T("NULL"));
		SplitByLen(diffResults,diffCount,finder1);
	}


	
	return es;
}
int FindId(AcDbObjectIdArray& ids,AcDbObjectId id)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(ids.at(i)==id)
		{
			return i;
		}
	}
	return -1;
}
Acad::ErrorStatus CmpBTRDiffFinder::ReadEntityDat(CmpEntityDat* dat,AcDbObjectId& id,int nIndex)
{
	dat->m_id=id;
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenObject<AcDbEntity>(pEnt,id,AcDb::kForRead);
	if(es!=eOk)
	{
		return es;
	}
	int nTmp=0;
	if((!pEnt->isKindOf(AcDbBlockReference::desc()))&&FindId(m_ingoreLayerIdArray[nIndex],pEnt->layerId())>=0)
	{
		pEnt->close();
		return Acad::eNullHandle;
	}
	dat->m_type=pEnt->isA();
	if(m_pConfig->m_nMode==0)
	{
		pEnt->getGeomExtents(dat->m_ext);
	}
	AcDbHandle handle;
		pEnt->getAcDbHandle(handle);
		TCHAR tmpStr[256]={0};
		handle.getIntoAsciiBuffer(tmpStr);
		dat->m_strHandle=tmpStr;
	dat->m_bNotEnt=FALSE;
	dat->m_strName=_T("");
	if(pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		//读取所有的属性
		AcDbBlockReference* pRef=AcDbBlockReference::cast(pEnt);
		AcDbObjectIterator* pObjIter=pRef->attributeIterator();
		if(pObjIter!=NULL)
		{
			pObjIter->start();
			while(!pObjIter->done())
			{
				AcDbObjectId id1=pObjIter->objectId();
				CmpEntityDat* dat1=new CmpEntityDat;
				es=ReadEntityDat(dat1,id1,nIndex);
				if(es==eOk)
				{
					dat->subEntityDats.Add(dat1);
				}
				pObjIter->step();
			}
			delete pObjIter;
			pObjIter=NULL;
		}
	}
	if(pEnt->isKindOf(AcDbAttribute::desc()))
	{
		AcDbAttribute* pAttr=AcDbAttribute::cast(pEnt);
		dat->m_strName=pAttr->tag();
	}
	pEnt->close();
	return eOk;
}
