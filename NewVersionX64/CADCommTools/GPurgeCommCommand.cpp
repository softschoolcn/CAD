#include "StdAfx.h"
#include "GPurgeCommCommand.h"
#include "GPurgeDialog.h"
//枚举符号化表
Acad::ErrorStatus EnumSymbolTable(AcDbObjectIdArray& ids,CStringArray& strNameArray,AcDbSymbolTable* pTable)
{
	AcDbSymbolTableIterator* pIterator=NULL;
	Acad::ErrorStatus es=pTable->newIterator(pIterator);
	if(es!=Acad::eOk)
		return es;
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbObjectId id;
	for(pIterator->start();!pIterator->done();pIterator->step())
	{
		es=pIterator->getRecordId(id);
		if(es!=Acad::eOk)
		{
			continue;
		}
		AcDbSymbolTableRecord* pRec=NULL;
		es=pIterator->getRecord(pRec,AcDb::kForRead);
		if(es!=Acad::eOk)
		{
			continue;
		}
		const TCHAR* lpName=NULL;
		es=pRec->getName(lpName);
		if(es!=Acad::eOk)
		{
			pRec->close();
			continue;
		}
		pRec->close();
		strNameArray.Add(lpName);
		ids.append(id);

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
//枚举字典
Acad::ErrorStatus EnumDictionary(AcDbObjectIdArray& ids,CStringArray& strNameArray,AcDbDictionary* pDict)
{
	AcDbDictionaryIterator* pIter=pDict->newIterator();
	if(pIter==NULL)
	{
		return Acad::eNullHandle;
	}
	for(;!pIter->done();pIter->next())
	{
		ids.append(pIter->objectId());
		strNameArray.Add(pIter->name());
	}
	return Acad::eOk;
}
Acad::ErrorStatus EnumNamedObjectsDictionary(AcDbObjectIdArray& ids,CStringArray& strNameArray,AcDbDatabase* pDb,const CString strName)
{
	AcDbDictionary *pNamedObj = NULL, *pNameList = NULL;
	Acad::ErrorStatus es;
	es=pDb->getNamedObjectsDictionary(pNamedObj,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return es;
	}
	es=pNamedObj->getAt(strName,(AcDbObject*&)pNameList,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		pNamedObj->close();
		return es;
	}
	es=EnumDictionary(ids,strNameArray,pNameList);
	pNameList->close();
	pNamedObj->close();
	return es;
}
//清理数据库
//pDb,需要清理的数据库
//bPrompt,提示
Acad::ErrorStatus PurgeDatabase(AcDbDatabase* pDb,MSGOUTPUTPROC pMsgProc,void* pArg)
{
	AcDbObjectIdArray ids;
	CStringArray strTypeArray;
	CStringArray strNameArray;
	int nCount=0;
	int nLastCount=0;
	//标注样式
	AcDbDimStyleTable*  pDimStyleTable=NULL;
	if(pDb->getSymbolTable(pDimStyleTable,AcDb::kForRead)==Acad::eOk)
	{
		EnumSymbolTable(ids,strNameArray,pDimStyleTable);
		pDimStyleTable->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("标注样式"));
		}
		nLastCount=nCount;
	}
	//表格样式
	EnumNamedObjectsDictionary(ids,strNameArray,pDb,_T("acad_tablestyle"));
	nCount=ids.logicalLength();
	for(int i=0;i<nCount-nLastCount;i++)
	{
		strTypeArray.Add(_T("表格样式"));
	}
	nLastCount=nCount;
	//材质
	AcDbDictionary* pMaterialDictionary=NULL;
	if(pDb->getMaterialDictionary(pMaterialDictionary,AcDb::kForRead)==Acad::eOk)
	{
		EnumDictionary(ids,strNameArray,pMaterialDictionary);
		pMaterialDictionary->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("材质"));
		}
		nLastCount=nCount;
	}
	//打印样式
	EnumNamedObjectsDictionary(ids,strNameArray,pDb,_T("acad_plotstylename"));
	nCount=ids.logicalLength();
	for(int i=0;i<nCount-nLastCount;i++)
	{
		strTypeArray.Add(_T("打印样式"));
	}
	nLastCount=nCount;
	//多线样式
	AcDbDictionary* pMLStyleDictionary=NULL;
	if(pDb->getMLStyleDictionary(pMLStyleDictionary,AcDb::kForRead)==Acad::eOk)
	{
		EnumDictionary(ids,strNameArray,pMLStyleDictionary);
		pMLStyleDictionary->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("多线样式"));
		}
		nLastCount=nCount;
	}
	//多重引线样式
	EnumNamedObjectsDictionary(ids,strNameArray,pDb,_T("acad_mleaderstyle"));
	nCount=ids.logicalLength();
	for(int i=0;i<nCount-nLastCount;i++)
	{
		strTypeArray.Add(_T("多重引线样式"));
	}
	nLastCount=nCount;
	//块
	AcDbBlockTable* pBlckTable=NULL;
	if(pDb->getSymbolTable(pBlckTable,AcDb::kForRead)==Acad::eOk)
	{
		EnumSymbolTable(ids,strNameArray,pBlckTable);
		pBlckTable->close();
	}
	nCount=ids.logicalLength();
	for(int i=0;i<nCount-nLastCount;i++)
	{
		strTypeArray.Add(_T("块"));
	}
	nLastCount=nCount;
	//视觉样式
	EnumNamedObjectsDictionary(ids,strNameArray,pDb,_T("acad_visualstyle"));
	nCount=ids.logicalLength();
	for(int i=0;i<nCount-nLastCount;i++)
	{
		strTypeArray.Add(_T("视觉样式"));
	}
	nLastCount=nCount;
	//图层
	AcDbLayerTable* pLayerTable=NULL;
	if(pDb->getSymbolTable(pLayerTable,AcDb::kForRead)==Acad::eOk)
	{
		EnumSymbolTable(ids,strNameArray,pLayerTable);
		pLayerTable->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("图层"));
		}
		nLastCount=nCount;
	}
	//文字样式
	AcDbTextStyleTable* pTextStyleTable=NULL;
	if(pDb->getSymbolTable(pTextStyleTable,AcDb::kForRead)==Acad::eOk)
	{
		EnumSymbolTable(ids,strNameArray,pTextStyleTable);
		pTextStyleTable->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("文字样式"));
		}
		nLastCount=nCount;
	}
	//线型
	AcDbLinetypeTable* pLinetypeTable=NULL;
	if(pDb->getSymbolTable(pLinetypeTable,AcDb::kForRead)==Acad::eOk)
	{
		EnumSymbolTable(ids,strNameArray,pLinetypeTable);
		pLinetypeTable->close();
		nCount=ids.logicalLength();
		for(int i=0;i<nCount-nLastCount;i++)
		{
			strTypeArray.Add(_T("线型"));
		}
		nLastCount=nCount;
	}
	//形
	//暂时不知道如何获取
	//清理
	try
	{
		//清理一下以便节省空间
		//pDb->purge(ids);
		Adesk::UInt32* pCount=new Adesk::UInt32[ids.logicalLength()];
		for(int i=0;i<ids.logicalLength();i++)
		{
			pCount[i]=0;
		}
		pDb->countHardReferences(ids,pCount);
		AcDbObjectIdArray idsTmp;
		CStringArray strTmpTypeArray;
		CStringArray strTmpNameArray;
		for(int i=0;i<ids.logicalLength();i++)
		{
			if(pCount[i]==0)
			{
				idsTmp.append(ids.at(i));
				strTmpTypeArray.Add(strTypeArray.GetAt(i));
				strTmpNameArray.Add(strNameArray.GetAt(i));
			}
		}
		delete[] pCount;
		pCount=NULL;
		ids.setLogicalLength(0);
		ids.append(idsTmp);
		strTypeArray.RemoveAll();
		strTypeArray.Append(strTmpTypeArray);
		strNameArray.RemoveAll();
		strNameArray.Append(strTmpNameArray);

	}
	catch(...)
	{

	}
	Acad::ErrorStatus es=Acad::eOk;
	for(int i=0;i<ids.logicalLength();i++)
	{
		CString strType=strTypeArray.GetAt(i);
		CString strName=strNameArray.GetAt(i);
		if(strType.CompareNoCase(_T("标注样式"))==0)
		{
			if(strName.CompareNoCase(_T("STANDARD"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("表格样式"))==0)
		{
			if(strName.CompareNoCase(_T("Standard"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("材质"))==0)
		{
			if(strName.CompareNoCase(_T("BYBLOCK"))==0||
				strName.CompareNoCase(_T("BYLAYER"))==0||
				strName.CompareNoCase(_T("GLOBAL"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("打印样式"))==0)
		{
			if(strName.CompareNoCase(_T("NORMAL"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("多线样式"))==0)
		{
			if(strName.CompareNoCase(_T("STANDARD"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("多重引线样式"))==0)
		{
			if(strName.CompareNoCase(_T("Standard"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("视觉样式"))==0)
		{
			//视觉样式清理会出错，先不清理
			continue;
		}
		else if(strType.CompareNoCase(_T("图层"))==0)
		{
			if(strName.CompareNoCase(_T("0"))==0||
				strName.CompareNoCase(_T("DEFPOINTS"))==0)
			{
				continue;
			}
		}
		else if(strType.CompareNoCase(_T("线型"))==0)
		{
			if(strName.CompareNoCase(_T("BYBLOCK"))==0||
				strName.CompareNoCase(_T("BYLAYER"))==0||
				strName.CompareNoCase(_T("CONTINUOUS"))==0)
			{
				continue;
			}
		}
		AcDbObject* pObj=NULL;
		es=acdbOpenAcDbObject(pObj,ids.at(i),AcDb::kForWrite);
		CString strMsg;
		if(es!=Acad::eOk)
		{
			strMsg.Format(_T("清理%s<%s>失败:%s"),strTypeArray.GetAt(i),strNameArray.GetAt(i),acadErrorStatusTextChs(es));
			if(pMsgProc!=NULL)
			{
				pMsgProc(strMsg,pArg);
			}
			continue;
		}
		es=pObj->erase();
		if(es!=Acad::eOk)
		{
			strMsg.Format(_T("清理%s<%s>失败:%s"),strTypeArray.GetAt(i),strNameArray.GetAt(i),acadErrorStatusTextChs(es));
			if(pMsgProc!=NULL)
			{
				pMsgProc(strMsg,pArg);
			}
			pObj->close();
			continue;
		}
		pObj->close();
		strMsg.Format(_T("已清理%s<%s>"),strTypeArray.GetAt(i),strNameArray.GetAt(i));
		if(pMsgProc!=NULL)
		{
			pMsgProc(strMsg,pArg);
		}
	}
	return Acad::eOk;
}
void PurgeEx()
{
	PurgeDatabase();
}
//批量文件清理
void BatchPurge()
{
	CAcModuleResourceOverride res;
	GPurgeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();
}
void PurgeMsgOutput(CString strMsg,void*)
{
	acutPrintf(_T("\n%s\n"),strMsg);
}
//统一文件版本
void SameDwgVer()
{
	CAcModuleResourceOverride res;
	GPurgeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.m_nType=1;
	dlg.DoModal();
}
