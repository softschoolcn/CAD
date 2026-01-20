#include "StdAfx.h"
#include "CmpSymbolDiffFinder.h"

CmpSymbolDiffFinder::CmpSymbolDiffFinder(CmpConfig* pConfig,AcDbSymbolTable* pTable1,AcDbSymbolTable* pTable2):CmpDiffFinder(pConfig)
{
	m_pTable1=pTable1;
	m_pTable2=pTable2;
}

CmpSymbolDiffFinder::~CmpSymbolDiffFinder(void)
{
}
Acad::ErrorStatus CmpSymbolDiffFinder::Find(CArray<CmpDiffResult>& diffResults,long& diffCount)
{
	for(int i=0;i<2;i++)
	{
		//读取图纸实体对象
		AcDbSymbolTableIterator* pIterator1=NULL;
		Acad::ErrorStatus es=eOk;
		if(i==0)
		{
			es=m_pTable1->newIterator(pIterator1);
		}
		else if(i==1)
		{
			es=m_pTable2->newIterator(pIterator1);
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
			es=pIterator1->getRecordId(id);
			if(es!=eOk)
				continue;
			CmpEntityDat* dat=new CmpEntityDat;
			dat->nIndex=-1;
			es=ReadSymbolDat(dat,id);
			if(es!=eOk)
				continue;
			m_cmpDat[i].Add(dat);
		}
		delete pIterator1;
		pIterator1=NULL;
	}
	return CmpDiffFinder::Find(diffResults,diffCount);
}
Acad::ErrorStatus CmpSymbolDiffFinder::ReadSymbolDat(CmpEntityDat* dat,AcDbObjectId& id)
{
	dat->m_id=id;
	AcDbObjectPointer<AcDbSymbolTableRecord> spEnt(id,AcDb::kForRead);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
	{
		return es;
	}
	dat->m_type=spEnt->isA();
	AcDbHandle handle;
	spEnt->getAcDbHandle(handle);
	TCHAR tmpStr[256]={0};
	handle.getIntoAsciiBuffer(tmpStr);
	dat->m_strHandle=tmpStr;
	dat->m_bNotEnt=TRUE;
	const ACHAR* pName=NULL;
	es=spEnt->getName(pName);
	if(es!=eOk)
	{
		return es;
	}
	dat->m_strName=pName;
	return eOk;
}
