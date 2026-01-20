#include "StdAfx.h"
#include "GLayerUtils.h"

GLayerUtils::GLayerUtils(void)
{
}

GLayerUtils::~GLayerUtils(void)
{
}
//枚举图层
Acad::ErrorStatus GLayerUtils::EnumAllLayer(AcDbObjectIdArray& layeIds,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbObjectId id;
		es=pLayerTableIterator->getRecordId(id);
		if(es!=Acad::eOk)
			continue;
		layeIds.append(id);
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
//枚举图层
Acad::ErrorStatus GLayerUtils::EnumAllLayer(CStringArray& layeNames,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		const ACHAR* pName=NULL;
		es=pLayerTableRecord->getName(pName);
		if(es!=Acad::eOk)
		{
			pLayerTableRecord->close();
			continue;
		}
		CString strName=pName;
		layeNames.Add(strName);
		//关闭(切记)
		pLayerTableRecord->close();
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
//枚举图层
Acad::ErrorStatus GLayerUtils::EnumAllLayer(CStringArray& layeNames,AcDbObjectIdArray& layeIds,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		const ACHAR* pName=NULL;
		es=pLayerTableRecord->getName(pName);
		if(es!=Acad::eOk)
		{
			pLayerTableRecord->close();
			continue;
		}
		CString strName=pName;
		layeNames.Add(strName);
		layeIds.append(pLayerTableRecord->objectId());
		//关闭(切记)
		pLayerTableRecord->close();
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
