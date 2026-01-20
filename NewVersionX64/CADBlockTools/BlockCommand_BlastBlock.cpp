#include "StdAfx.h"
#include "BlockCommCommand.h"
Acad::ErrorStatus BlastBlock(AcDbObjectIdArray& block_ids,AcDbBlockReference* pRef,AcDbBlockTableRecord* pRec)
{
	//先将属性转换成文字
	BrefAttrToText(pRef);
	if(GCADUtils::FindObjectId(block_ids,pRef->blockTableRecord())<0)
	{
		block_ids.append(pRef->blockTableRecord());
	}
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=pRef->explode(entitySet);
	if(es!=Acad::eOk)
	{
		return es;
	}
	pRef->erase();
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//基本不可能
			continue;
		}
		pRec->appendAcDbEntity(pEnt);
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			BlastBlock(block_ids,AcDbBlockReference::cast(pEnt),pRec);
		}
		pEnt->close();
	}
	entitySet.setLogicalLength(0);
	return Acad::eOk;
}
//清理一下
Acad::ErrorStatus MyPurge(AcDbObjectIdArray& ids)
{
	if(ids.logicalLength()==0)
	{
		return Acad::eOk;
	}
	::AcDbDatabase* pDb=ids.at(0).database();
	//清理
	AcDbObjectIdArray idsTmp;
	try
	{
		//清理一下以便节省空间
		Adesk::UInt32* pCount=new Adesk::UInt32[ids.logicalLength()];
		for(int i=0;i<ids.logicalLength();i++)
		{
			pCount[i]=0;
		}
		pDb->countHardReferences(ids,pCount);
		
		for(int i=0;i<ids.logicalLength();i++)
		{
			if(pCount[i]==0)
			{
				idsTmp.append(ids.at(i));
			}
		}
		delete[] pCount;
		pCount=NULL;

	}
	catch(...)
	{

	}
	Acad::ErrorStatus es=Acad::eOk;
	for(int i=0;i<idsTmp.logicalLength();i++)
	{
		AcDbObject* pObj=NULL;
		es=acdbOpenAcDbObject(pObj,idsTmp.at(i),AcDb::kForWrite);
		CString strMsg;
		if(es!=Acad::eOk)
		{
			continue;
		}
		es=pObj->erase();
		pObj->close();
	}
	return Acad::eOk;
}
//120:块爆破
void BlastBlock()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("INSERT"), 0);
	AcDbObjectIdArray ids1;
	GCADUtils::GetCurrentSelectObjIds(ids1,rb);
	if(ids1.logicalLength()==0)
	{
		ads_name ssName;
		TCHAR* promptPtrs[2]={_T("\n选择块:"),_T("\n移除块:")};
		int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
		if(rc!=RTNORM)
		{
			acutRelRb(rb);
			return;
		}
		acutRelRb(rb);

		SSToIds(ids1,ssName);
		acedSSFree(ssName);
	}
	else
	{
		acutRelRb(rb);
	}
	
	AcDbObjectIdArray block_ids;
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId id=ids1.at(i);
		AcDbObjectPointer<AcDbBlockReference> spRef(id,AcDb::kForWrite);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		AcDbBlockTableRecordPointer spRec(spRef->blockId(),AcDb::kForWrite);
		if(spRec.openStatus()!=Acad::eOk)
		{
			continue;
		}
		BlastBlock(block_ids,spRef.object(),spRec.object());
	}
	MyPurge(block_ids);

}