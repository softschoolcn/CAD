#include "StdAfx.h"
#include "GSameColorHelper.h"
GSameColorHelper::GSameColorHelper(void)
{
}

GSameColorHelper::~GSameColorHelper(void)
{
}
//设置当前整个数据库中呈现统一颜色
Acad::ErrorStatus GSameColorHelper::SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,const int nColorIndex)
{
	return SetAllEntityInSameColor(colorRecordArr,acdbHostApplicationServices()->workingDatabase(),nColorIndex);
}
Acad::ErrorStatus GSameColorHelper::SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbDatabase* pDb,const int nColorIndex)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbObjectIdArray ids;
	AcArray<bool> locks;
	AcArray<bool> frozens;
	Acad::ErrorStatus es=OpenAllLayer(pDb,ids,locks,frozens);
	if(es!=Acad::eOk)
		return es;
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	es=spBTR.openStatus();
	if(es!=eOk)
	{
		ResetAllLayer(ids,locks,frozens);
		return es;
	}
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		ResetAllLayer(ids,locks,frozens);
		return Acad::eNullHandle;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if(pBTR->isLayout())
		{
			_SetAllEntityInSameColor(colorRecordArr,pBTR,nColorIndex);
		}
		pBTR->close();
		pBTR=NULL;

	}
	delete pIterator;
	pIterator=NULL;
    ResetAllLayer(ids,locks,frozens);
	return Acad::eOk;
}
Acad::ErrorStatus GSameColorHelper::SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbBlockTableRecord* pBTR,const int nColorIndex)
{
	if(pBTR==NULL)
		return Acad::eNullHandle;
	AcDbObjectIdArray ids;
	AcArray<bool> locks;
	AcArray<bool> frozens;
	Acad::ErrorStatus es=OpenAllLayer(pBTR->database(),ids,locks,frozens);
	if(es!=Acad::eOk)
		return es;
	es=_SetAllEntityInSameColor(colorRecordArr,pBTR,nColorIndex);
	ResetAllLayer(ids,locks,frozens);
	return es;
}
void RemoveColorDesc(CString& strText)
{
	int nStartIndex=0;
label1:
	int nIndex=strText.Find(_T("\\C"),nStartIndex);
	if(nIndex<0)
	{
		return;
	}
	if(nIndex>=1)
	{
		if(strText.GetAt(nIndex-1)==_T('\\'))
		{
			//正好文字内容中也有\C
			nStartIndex=nIndex+3;
			goto label1;
		}
	}
	int nIndex1=strText.Find(_T(";"),nIndex+2);
	if(nIndex1<0)
	{
		return;
	}
	CString strColor=strText.Mid(nIndex+2,(nIndex1-1)-(nIndex+2)+1);
	int nValue=0;
	if(!GStringHelper::IsValidInt(strColor,nValue))
	{
		nStartIndex=nIndex+2;
		goto label1;
	}
	if(nValue<0||nValue>256)
	{
		nStartIndex=nIndex+2;
		goto label1;
	}
	strText=strText.Left(nIndex)+strText.Right((strText.GetLength()-1)-(nIndex1+1)+1);
	RemoveColorDesc(strText);
}
Acad::ErrorStatus GSameColorHelper::SetEntityColor(AcDbEntity* pEnt,const int nColorIndex)
{
	pEnt->setColorIndex(nColorIndex);
	AcCmColor cor;
	cor.setColorIndex(nColorIndex);
	if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		//标注类对象
		AcDbDimension* pDim=AcDbDimension::cast(pEnt);
		pDim->setDimclrd(cor);//尺寸线颜色
		pDim->setDimclre(cor);//尺寸界线颜色
		pDim->setDimclrt(cor);//文字颜色
	}
	else if(pEnt->isKindOf(AcDbHatch::desc()))
	{
		//填充类对象
		AcDbHatch* pHatch=AcDbHatch::cast(pEnt);
		pHatch->setGradientOneColorMode(true);
		pHatch->setHatchObjectType(AcDbHatch::kHatchObject);
	}
	else if(pEnt->isKindOf(AcDbLeader::desc()))
	{
		AcDbLeader* pLeader=AcDbLeader::cast(pEnt);
		pLeader->setDimclrd(cor);
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText=AcDbMText::cast(pEnt);
		TCHAR* lpText=pMText->contents();
		CString strText=lpText;
		acutDelString(lpText);
		RemoveColorDesc(strText);
		pMText->setContents(strText);
	}
	return Acad::eOk;
}
Acad::ErrorStatus GSameColorHelper::SetEntityColor(const AcDbObjectId id,const int nColorIndex)
{
	AcDbObjectPointer<AcDbEntity> spEnt(id,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
		return es;
	return SetEntityColor(spEnt.object(),nColorIndex);

}
Acad::ErrorStatus GSameColorHelper::OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
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
Acad::ErrorStatus GSameColorHelper::ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
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
Acad::ErrorStatus GSameColorHelper::_SetAllEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbBlockTableRecord* pBTR,const int nColorIndex)
{
	if(pBTR==NULL)
		return Acad::eNullHandle;
	AcDbBlockTableRecordIterator* pIter = NULL;
	Acad::ErrorStatus es=pBTR->newIterator(pIter);
	if(es!=eOk)
		return es;
	AcDbObjectIdArray blockIds;
	for(; !pIter->done(); pIter->step() )
	{
		AcDbEntity* pEnt=NULL;
		es=pIter->getEntity(pEnt,AcDb::kForWrite);
		if(es!=Acad::eOk)
			continue;
		GColorRecord rc;
		es=rc.Read(pEnt);
		if(es==Acad::eOk)
		{
			colorRecordArr.Add(rc);
		}
		SetEntityColor(pEnt,nColorIndex);
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef=AcDbBlockReference::cast(pEnt);
			if(!blockIds.contains(pRef->blockTableRecord()))
			{
				blockIds.append(pRef->blockTableRecord());
			}
			AcDbObjectIterator* pObjIter=pRef->attributeIterator();
			if(pObjIter!=NULL)
			{
				pObjIter->start();
				while(!pObjIter->done())
				{
					AcDbObjectId id1=pObjIter->objectId();
					_SetEntityInSameColor(colorRecordArr,blockIds,id1,nColorIndex);
					pObjIter->step();
				}
				delete pObjIter;
				pObjIter=NULL;
			}
		}
		pEnt->close();
	}
	for(int i=0;i<blockIds.logicalLength();i++)
	{
		AcDbBlockTableRecordPointer spRec(blockIds.at(i),AcDb::kForRead);
		if(spRec.openStatus()!=Acad::eOk)
			continue;
		_SetAllEntityInSameColor(colorRecordArr,spRec.object(),nColorIndex);
	}
	delete pIter;
	pIter=NULL;
	return Acad::eOk;
}
//设置整个块定义中呈现统一颜色
Acad::ErrorStatus GSameColorHelper::SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectId objId,const int nColorIndex)
{
	AcDbObjectIdArray blockIds;
	return _SetEntityInSameColor(colorRecordArr,blockIds,objId,nColorIndex);
}
//设置整个块定义中呈现统一颜色
Acad::ErrorStatus GSameColorHelper::_SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectIdArray& blockIds,AcDbObjectId objId,const int nColorIndex)
{
	AcDbObjectIdArray blockIds1;
	AcDbEntityPointer spEnt(objId,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=Acad::eOk)
	{
		if(es!=Acad::eOnLockedLayer)
		{
			return es;
		}
		AcDbEntity* pEnt=NULL;
		es=acdbOpenObject(pEnt,objId,AcDb::kForRead);
		if(es!=Acad::eOk)
			return es;
		es=GColorRecord::LockLayer(pEnt->layerId(),0);
		if(es!=Acad::eOk)
		{
			pEnt->close();
			return es;
		}
		es=pEnt->upgradeOpen();
		if(es!=Acad::eOk)
		{
			pEnt->close();
			pEnt=NULL;
			return es;
		}
		GColorRecord rc;
		es=rc.Read(pEnt);
		if(es==Acad::eOk)
		{
			colorRecordArr.Add(rc);
		}
		SetEntityColor(pEnt,nColorIndex);
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef=AcDbBlockReference::cast(pEnt);
			if(!blockIds.contains(pRef->blockTableRecord()))
			{
				blockIds.append(pRef->blockTableRecord());
				blockIds1.append(pRef->blockTableRecord());
			}
			AcDbObjectIterator* pObjIter=pRef->attributeIterator();
			if(pObjIter!=NULL)
			{
				pObjIter->start();
				while(!pObjIter->done())
				{
					AcDbObjectId id1=pObjIter->objectId();
					_SetEntityInSameColor(colorRecordArr,blockIds,id1,nColorIndex);
					pObjIter->step();
				}
				delete pObjIter;
				pObjIter=NULL;
			}
		}
		pEnt->recordGraphicsModified();
		GColorRecord::LockLayer(pEnt->layerId(),1);
		pEnt->close();
		for(int i=0;i<blockIds1.logicalLength();i++)
		{
			AcDbBlockTableRecordPointer spRec(blockIds1.at(i),AcDb::kForRead);
			if(spRec.openStatus()!=Acad::eOk)
				continue;
			_SetAllEntityInSameColor(colorRecordArr,spRec.object(),nColorIndex);
		}
		return es;
	}
	GColorRecord rc;
	es=rc.Read(spEnt.object());
	if(es==Acad::eOk)
	{
		colorRecordArr.Add(rc);
	}
	SetEntityColor(spEnt.object(),nColorIndex);
	if(spEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pRef=AcDbBlockReference::cast(spEnt.object());
		if(!blockIds.contains(pRef->blockTableRecord()))
		{
			blockIds.append(pRef->blockTableRecord());
			blockIds1.append(pRef->blockTableRecord());
		}
	}
	for(int i=0;i<blockIds1.logicalLength();i++)
	{
		AcDbBlockTableRecordPointer spRec(blockIds1.at(i),AcDb::kForRead);
		if(spRec.openStatus()!=Acad::eOk)
			continue;
		_SetAllEntityInSameColor(colorRecordArr,spRec.object(),nColorIndex);
	}
	return es;
}
//设置实体中呈现统一颜色
Acad::ErrorStatus GSameColorHelper::SetEntityInSameColor(CArray<GColorRecord>& colorRecordArr,AcDbObjectIdArray& ids,const int nColorIndex)
{
	AcDbObjectIdArray blockIds;
	Acad::ErrorStatus es=Acad::eOk;
	for(long i=0;i<ids.logicalLength();i++)
	{
		es=_SetEntityInSameColor(colorRecordArr,blockIds,ids.at(i),nColorIndex);
	}
	return Acad::eOk;
}
