#include "StdAfx.h"
#include "CmpDwgDiffFinder.h"
#include "CommCommand.h"
#include "CmpSymbolDiffFinder.h"
CmpDwgDiffFinder::CmpDwgDiffFinder(CmpConfig* pConfig,AcDbDatabase* pDb1,AcDbDatabase* pDb2)
{
	m_pConfig=pConfig;
	m_pDb1=pDb1;
	m_pDb2=pDb2;
}
CmpDwgDiffFinder::~CmpDwgDiffFinder()
{

}
Acad::ErrorStatus CmpDwgDiffFinder::Find(CmpResult& result,long& diffCount)
{
	//读取图纸实体对象
	Acad::ErrorStatus es=FindInSymbolTable(result,diffCount);
	if(es!=eOk)
		return es;
	es=FindInBT(result,diffCount);
	if(es!=eOk)
		return es;
	return es;

}
bool CmpDwgDiffFinder::IsIngoreLayer(AcDbLayerTableRecord* pRecord)
{
	if((m_pConfig->m_bIngoreCloseLayer&&pRecord->isOff())||
			(m_pConfig->m_bIngoreLockLayer&&pRecord->isLocked()))
	{
		return true;
	}
	const TCHAR* lpName=NULL;
	pRecord->getName(lpName);
	for(int i=0;i<m_pConfig->m_strIngoreLayerArray.GetCount();i++)
	{
		CString strLayer=m_pConfig->m_strIngoreLayerArray.GetAt(i);
		if(strLayer.CompareNoCase(lpName)==0)
		{
			return true;
		}
	}
	return false;
}
Acad::ErrorStatus CmpDwgDiffFinder::IngoreLayers(AcDbObjectIdArray& ids,AcDbLayerTable* pTable)
{
	AcDbLayerTableIterator* pIterator=NULL;
	Acad::ErrorStatus es=pTable->newIterator(pIterator);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(pIterator->start();!pIterator->done();pIterator->step())
	{
		AcDbLayerTableRecord* pRecord=NULL;
		es=pIterator->getRecord(pRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(IsIngoreLayer(pRecord))
		{
			ids.append(pRecord->objectId());
		}
		pRecord->close();
	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
Acad::ErrorStatus CmpDwgDiffFinder::FindInSymbolTable(CmpResult& result,long& diffCount)
{
	//比较图层
	AcDbLayerTable* pTable1=NULL;
	AcDbLayerTable* pTable2=NULL;
	Acad::ErrorStatus es=m_pDb1->getSymbolTable(pTable1,AcDb::kForRead);
	if(es!=eOk)
	{
		return es;
	}
	es=m_pDb2->getSymbolTable(pTable2,AcDb::kForRead);
	if(es!=eOk)
	{
		pTable1->close();
		return es;
	}
	CmpSymbolDiffFinder layerDiffFinder(m_pConfig,pTable1,pTable2);
	layerDiffFinder.strName=_T("图层");
	es=layerDiffFinder.Find(result.m_diffsInLayerTable,diffCount);
	layerDiffFinder.Free();
	IngoreLayers(m_ingoreLayerIdArray[0],pTable1);
	IngoreLayers(m_ingoreLayerIdArray[1],pTable2);
	pTable1->close();
	pTable2->close();
	if(es!=eOk)
	{
		return es;
	}
	//比较线形表
	AcDbLinetypeTable* pTable3=NULL;
	AcDbLinetypeTable* pTable4=NULL;
	es=m_pDb1->getSymbolTable(pTable3,AcDb::kForRead);
	if(es!=eOk)
	{
		return es;
	}
	es=m_pDb2->getSymbolTable(pTable4,AcDb::kForRead);
	if(es!=eOk)
	{
		pTable3->close();
		return es;
	}
	CmpSymbolDiffFinder linetypeDiffFinder(m_pConfig,pTable3,pTable4);
	linetypeDiffFinder.strName=_T("线型");
	es=linetypeDiffFinder.Find(result.m_diffsInLinetypeTable,diffCount);
	linetypeDiffFinder.Free();
	pTable3->close();
	pTable4->close();
	if(es!=eOk)
	{
		return es;
	}
	//比较文字样式表
	AcDbTextStyleTable* pTable5=NULL;
	AcDbTextStyleTable* pTable6=NULL;
	es=m_pDb1->getSymbolTable(pTable5,AcDb::kForRead);
	if(es!=eOk)
	{
		return es;
	}
	es=m_pDb2->getSymbolTable(pTable6,AcDb::kForRead);
	if(es!=eOk)
	{
		pTable5->close();
		return es;
	}
	CmpSymbolDiffFinder textStyleDiffFinder(m_pConfig,pTable5,pTable6);
	textStyleDiffFinder.strName=_T("文字样式");
	es=textStyleDiffFinder.Find(result.m_diffsInTextStyleTable,diffCount);
	textStyleDiffFinder.Free();
	pTable5->close();
	pTable6->close();
	if(es!=eOk)
	{
		return es;
	}
	return Acad::eOk;
}
Acad::ErrorStatus CmpDwgDiffFinder::FindInBT(CmpResult& result,long& diffCount)
{
	//在块表记录中寻找差异
	AcDbObjectId ModelSpaceId1;
	AcDbObjectIdArray PaperSpaceIds1;
	AcDbObjectIdArray BlockDefineIds1;
	CStringArray BlockDefineNames1;
	Acad::ErrorStatus es=SplitBTRS(m_pDb1,ModelSpaceId1,PaperSpaceIds1,BlockDefineIds1,BlockDefineNames1);
	if(es!=eOk)
		return es;
	//在模型空间中寻找差异
	if(!ModelSpaceId1.isValid())
	{
		return eNullObjectId;
	}

	AcDbObjectId ModelSpaceId2;
	AcDbObjectIdArray PaperSpaceIds2;
	AcDbObjectIdArray BlockDefineIds2;
	CStringArray BlockDefineNames2;
	es=SplitBTRS(m_pDb2,ModelSpaceId2,PaperSpaceIds2,BlockDefineIds2,BlockDefineNames2);
	if(es!=eOk)
		return es;
	if(!ModelSpaceId1.isValid())
	{
		return eNullObjectId;
	}
	//读取图纸实体对象
	AcDbBlockTableRecordPointer spBlkRec1(ModelSpaceId1, AcDb::kForRead);
	es = spBlkRec1.openStatus();
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开文件模型空间失败！%s"),acadErrorStatusText(es));
		return es;
	}
	AcDbBlockTableRecordPointer spBlkRec2(ModelSpaceId2, AcDb::kForRead);
	es = spBlkRec1.openStatus();
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开文件模型空间失败！%s"),acadErrorStatusText(es));
		return es;
	}
	//比较模型空间
	GLog::WriteLine(_T("开始比较模型空间"));
	CmpBTRDiffFinder finder(m_pConfig,spBlkRec1.object(),spBlkRec2.object());
	finder.m_ingoreLayerIdArray[0].append(m_ingoreLayerIdArray[0]);
	finder.m_ingoreLayerIdArray[1].append(m_ingoreLayerIdArray[1]);
	finder.strName=_T("模型空间");
	es=finder.Find(result.m_diffsInModelSpace,diffCount);
	finder.Free();
	GLog::WriteLine(_T("完成比较模型空间"));
	if(es!=eOk)
		return es;
	//比较块定义
	int nMode=m_pConfig->m_nMode;
	m_pConfig->m_nMode=0;//在块定义的比较中强制改为位置比较模式，主要是因为块定义中的句柄是会改变的，暂时无法知道原因
	es=FindInBT(result,diffCount,BlockDefineIds1,BlockDefineNames1,BlockDefineIds2,BlockDefineNames2);
	if(es!=eOk)
	{
		m_pConfig->m_nMode=nMode;
		return es;
	}
	m_pConfig->m_nMode=nMode;
	return es;
}
Acad::ErrorStatus CmpDwgDiffFinder::FindInBT(CmpResult& result,long& diffCount,AcDbObjectIdArray& BlockIds1,CStringArray& BlockNames1,AcDbObjectIdArray& BlockIds2,CStringArray& BlockNames2)
{
	Acad::ErrorStatus es=eOk;
	for(long i=0;i<BlockIds1.logicalLength();i++)
	{
		CString strName1=BlockNames1.GetAt(i);
		BOOL bFind=FALSE;
		for(long j=0;j<BlockIds2.logicalLength();j++)
		{
			CString strName2=BlockNames2.GetAt(j);
			if(strName1.Compare(strName2)==0)
			{
				bFind=TRUE;
				//读取图纸实体对象
				AcDbBlockTableRecordPointer spBlkRec1(BlockIds1.at(i), AcDb::kForRead);
				es = spBlkRec1.openStatus();
				if (es != Acad::eOk)
				{
					acutPrintf(_T("\n 打开文件模型空间失败！%s"),acadErrorStatusText(es));
					break;
				}
				AcDbBlockTableRecordPointer spBlkRec2(BlockIds2.at(j), AcDb::kForRead);
				es = spBlkRec1.openStatus();
				if (es != Acad::eOk)
				{
					acutPrintf(_T("\n 打开文件模型空间失败！%s"),acadErrorStatusText(es));
					break;
				}
				if((!spBlkRec1->isLayout())&&
					(!spBlkRec2->isLayout())&&
					(!IsBlockTableRecordInUse(spBlkRec1.object()))&&
					(!IsBlockTableRecordInUse(spBlkRec2.object())))
				{
					GLog::WriteLine(_T("完成比较块,没有用到的块忽略:%s"),strName2);
					break;
				}
				CmpBlockDiffResult result1;
				result1.nType=0;//删除
				result1.strName=strName1;
				GLog::WriteLine(_T("开始比较块:%s"),strName2);
				CmpBTRDiffFinder finder(m_pConfig,spBlkRec1.object(),spBlkRec2.object());
				finder.m_ingoreLayerIdArray[0].append(m_ingoreLayerIdArray[0]);
	finder.m_ingoreLayerIdArray[1].append(m_ingoreLayerIdArray[1]);
				finder.strName.Format(_T("块:%s"),strName2);
				long tmp_diffCount=0;
				es=finder.Find(result1.m_diffItems,tmp_diffCount);
				finder.Free();
				GLog::WriteLine(_T("完成比较块:%s"),strName2);
				if(es!=eOk)
					break;
				diffCount+=tmp_diffCount;
				result1.nType=tmp_diffCount==0?0:1;
				result.m_diffsInBlockDefines.Add(result1);
				break;
			}
		}
		if(!bFind)
		{
			CmpBlockDiffResult result1;
			result1.nType=3;//删除
			result1.strName=strName1;
			es=objIdToHandle(result1.strHandle1,BlockIds1.at(i));
			if(es==eOk)
			{
				result.m_diffsInBlockDefines.Add(result1);
				diffCount++;
			}
		}
	}
	for(long j=0;j<BlockIds2.logicalLength();j++)
	{
		CString strName2=BlockNames2.GetAt(j);
		BOOL bFind=FALSE;
		for(long i=0;i<BlockIds1.logicalLength();i++)
		{
			CString strName1=BlockNames1.GetAt(i);
			if(strName1.Compare(strName2)==0)
			{
				bFind=TRUE;
				break;
			}
		}
		if(!bFind)
		{
			CmpBlockDiffResult result1;
			result1.nType=2;//新增
			result1.strName=strName2;
			es=objIdToHandle(result1.strHandle2,BlockIds2.at(j));
			if(es==eOk)
			{
				result.m_diffsInBlockDefines.Add(result1);
				diffCount++;
			}
		}
	}

	return eOk;
}
Acad::ErrorStatus CmpDwgDiffFinder::SplitBTRS(AcDbDatabase* pDb,AcDbObjectId& ModelSpaceId,AcDbObjectIdArray& PaperSpaceIds,AcDbObjectIdArray& BlockDefineIds,CStringArray& BlockDefineNames)
{
	AcDbBlockTable* pBT=NULL;
	Acad::ErrorStatus es=pDb->getBlockTable(pBT,AcDb::kForRead);
	if(es!=eOk)
	{
		return es;
	}
	AcDbObjectPointer<AcDbBlockTable> spBT;
	spBT.acquire(pBT);
	AcDbBlockTableIterator* pIterator=NULL;
	es=spBT->newIterator(pIterator);
	if(es!=eOk)
	{
		return es;
	}
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(;!pIterator->done();pIterator->step())
	{
		AcDbBlockTableRecord* pRecord=NULL;
		es=pIterator->getRecord(pRecord,AcDb::kForRead);
		if(es!=eOk)
			continue;
		if(pRecord->isAnonymous())
		{
			//匿名块先不处理
			pRecord->close();
			continue;
		}
		const TCHAR* lpName=NULL;
		pRecord->getName(lpName);
		if(pRecord->isLayout())
		{
			//模型空间或布局空间
			if(lstrcmpi(lpName,ACDB_MODEL_SPACE)==0)
			{
				ModelSpaceId=pRecord->objectId();
			}
			else
			{
				PaperSpaceIds.append(pRecord->objectId());
			}
		}
		else
		{
			BlockDefineNames.Add(lpName);
			BlockDefineIds.append(pRecord->objectId());
		}
		pRecord->close();
	}
	delete pIterator;
	pIterator=NULL;
	return eOk;
}
