#include "StdAfx.h"
#include "EnumEntity.h"
//#include "AcCommFunction.h"
//获取图层的ID
Acad::ErrorStatus GetLayerId(AcDbObjectId& layerId,const CString strLayerName,AcDbDatabase* pDb)
{
	CString lyName=strLayerName;
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable*     pTable=NULL;
	Acad::ErrorStatus es=pDb->getSymbolTable(pTable,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	bool bHave=pTable->has(lyName.GetBuffer());
	lyName.ReleaseBuffer();
	if(!bHave)
	{
		pTable->close();
		return Acad::eNotInDatabase;
	}
	es=pTable->getAt(lyName.GetBuffer(),layerId);
	lyName.ReleaseBuffer();
	pTable->close();
	return es;
}
//获取图层名称
Acad::ErrorStatus GetLayerName(CString& strLayerName,AcDbObjectId layerId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord* pRec=AcDbLayerTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	const TCHAR* pName=NULL;
	es=pRec->getName(pName);
	if(es!=Acad::eOk)
	{
		pObj->close();
		return es;
	}
	strLayerName=pName;
	return Acad::eOk;
}
//获取实体图层ID
Acad::ErrorStatus GetEntityLayerId(AcDbObjectId& layerId,AcDbObjectId entId)
{
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	layerId=pEnt->layerId();
	pEnt->close();
	return Acad::eOk;
}
////////////////////////////////////////////////////////使用选择集///////////////////////////////////////////////////////////////////////////
//1:选择所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus SelectAllEntity(AcDbObjectIdArray& ids)
{
	ids.setLogicalLength(0);
	ads_name ents;
	acedSSGet(_T("A"),NULL,NULL,NULL,ents);
	SSToIds(ids,ents);
	acedSSFree(ents);
	return Acad::eOk;
}
//2:选择某一个图层上的所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus SelectAllEntityInLayer(AcDbObjectIdArray& ids,const CString strLayerName)
{
	ids.setLogicalLength(0);
	int nlen = lstrlen(strLayerName)+1;
	ads_name ents;
	struct resbuf *rb;
	rb=acutNewRb(AcDb::kDxfLayerName);
	rb->restype=AcDb::kDxfLayerName;
	rb->resval.rstring = (TCHAR*) malloc(nlen*sizeof(TCHAR));
	lstrcpy(rb->resval.rstring,strLayerName);
	rb->rbnext=NULL;
	acedSSGet(_T("X"),NULL,NULL,rb,ents);
	SSToIds(ids,ents);
	acedSSFree(ents);
	acutRelRb(rb);
	return Acad::eOk;
}
//3:选择某一个图层上的所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus SelectAllEntityInLayer(AcDbObjectIdArray& ids,AcDbObjectId layerId)
{
	CString strLayerName;
	Acad::ErrorStatus es=GetLayerName(strLayerName,layerId);
	return SelectAllEntityInLayer(ids,strLayerName);
}
////////////////////////////////////////////////////////在块定义中遍历///////////////////////////////////////////////////////////////////////////
//1:枚举块定义上的的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbBlockTableRecord* pBTR)
{
	if(pBTR==NULL)
		return Acad::eNullHandle;
	//新建枚举器
	AcDbBlockTableRecordIterator* pIterator=NULL;
	Acad::ErrorStatus es=pBTR->newIterator(pIterator);
	if(es!=Acad::eOk)
		return es;
	//执行遍历
	AcDbObjectId id;
	for(pIterator->start();!pIterator->done();pIterator->step())
	{
		es=pIterator->getEntityId(id);
		if(es!=Acad::eOk)
			continue;
		ids.append(id);
	}
	//删除释放枚举器
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
//2:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbBlockTableRecord* pBTR)
{
	if(pBTR==NULL)
		return Acad::eNullHandle;
	AcDbObjectIdArray tmp_ids;
	Acad::ErrorStatus es=EnumAllEntityInBTR(tmp_ids,pBTR);
	if(es!=Acad::eOk)
		return es;
	AcDbObjectId id;
	AcDbObjectId layerId1;
	AcDbObjectIdArray tmp_ids1;
	for(int i=0;i<tmp_ids.logicalLength();i++)
	{
		id=tmp_ids.at(i);
		es=GetEntityLayerId(layerId1,id);
		if(es!=Acad::eOk)
			continue;
		if(layerId1!=layerId)
		{
			continue;
		}
		tmp_ids1.append(id);
	}
	ids.append(tmp_ids1);
	return Acad::eOk;
}
//3:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,const CString strLayerName,AcDbBlockTableRecord* pBTR)
{
	if(pBTR==NULL)
		return Acad::eNullHandle;
	AcDbObjectId layerId;
	Acad::ErrorStatus es=GetLayerId(layerId,strLayerName,pBTR->database());
	if(es!=Acad::eOk)
		return es;
	es=EnumAllEntityInBTR(ids,layerId,pBTR);
	return Acad::eOk;
}
//4:枚举块定义上的的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId btrId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,btrId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbBlockTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbBlockTableRecord* pRec=AcDbBlockTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	es=EnumAllEntityInBTR(ids,pRec);
	pObj->close();
	return es;
}
//5:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,const CString strLayerName,AcDbObjectId btrId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,btrId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbBlockTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbBlockTableRecord* pRec=AcDbBlockTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	es=EnumAllEntityInBTR(ids,strLayerName,pRec);
	pObj->close();
	return es;
}
//6:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbObjectId btrId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,btrId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbBlockTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbBlockTableRecord* pRec=AcDbBlockTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	es=EnumAllEntityInBTR(ids,layerId,pRec);
	pObj->close();
	return es;
}
////////////////////////////////////////////////////////在数据库中枚举///////////////////////////////////////////////////////////////////////////
//1:枚举所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus EnumAllEntity(AcDbObjectIdArray& ids,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	Acad::ErrorStatus es=spBTR.openStatus();
	if(es!=eOk)
		return es;
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if(pBTR->isLayout())
		{
			EnumAllEntityInBTR(ids,pBTR);
		}
		pBTR->close();
		pBTR=NULL;

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
//2:枚举某一个图层上的所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus EnumAllEntityInLayer(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb)
{
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	Acad::ErrorStatus es=spBTR.openStatus();
	if(es!=eOk)
		return es;
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if(pBTR->isLayout())
		{
			EnumAllEntityInBTR(ids,layerId,pBTR);
		}
		pBTR->close();
		pBTR=NULL;

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
//3:枚举某一个图层上的所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus EnumAllEntityInLayer(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb)
{
	AcDbObjectId layerId;
	Acad::ErrorStatus es=GetLayerId(layerId,strLayerName,pDb);
	if(es!=Acad::eOk)
		return es;
	es=EnumAllEntityInLayer(ids,layerId,pDb);
	return es;
}
//4:枚举模型空间上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDb, AcDb::kForRead);
	Acad::ErrorStatus es = spBlkRec.openStatus();
	if(Acad::eOk != es)
	{
		return es;
	}
	es=EnumAllEntityInBTR(ids,spBlkRec.object());
	return es;
}
//5:枚举模型空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDb, AcDb::kForRead);
	Acad::ErrorStatus es = spBlkRec.openStatus();
	if(Acad::eOk != es)
	{
		return es;
	}
	es=EnumAllEntityInBTR(ids,layerId,spBlkRec.object());
	return es;
}
//6:枚举模型空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbObjectId layerId;
	Acad::ErrorStatus es=GetLayerId(layerId,strLayerName,pDb);
	if(es!=Acad::eOk)
		return es;
	es=EnumAllEntityInModelSpace(ids,layerId,pDb);
	return es;
}

//7:枚举当前空间上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	Acad::ErrorStatus es=EnumAllEntityInBTR(ids,pDb->currentSpaceId());
	return es;
}
//8:枚举当前空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	Acad::ErrorStatus es=EnumAllEntityInBTR(ids,strLayerName,pDb->currentSpaceId());
	return es;
}
//9:枚举当前空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	Acad::ErrorStatus es=EnumAllEntityInBTR(ids,layerId,pDb->currentSpaceId());
	return es;
}
