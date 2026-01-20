#include "StdAfx.h"
#include "GAcGroupFunction.h"
//创建分组
//groupId,分组ID
//ids,实体ID数组
Acad::ErrorStatus makeGroup(AcDbObjectId groupId,AcDbObjectIdArray& ids)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbGroup *pGroup;
	es=acdbOpenObject(pGroup, groupId, AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	long i, length;
	length=ids.logicalLength();
	ads_name ename;
	pGroup->append(ids);
	pGroup->close();
	return Acad::eOk;
}
//不存在的分组名称
//pDb:数据库
//strName:分组名称
Acad::ErrorStatus notExistGroupName(AcDbDatabase* pDb,
									CString& strName)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbDictionary *pGroupDict;
	es=pDb->getGroupDictionary(pGroupDict, AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return es;
	}
	int nCount=0;
	CString strTmp=strName;
	while(pGroupDict->has(strName))
	{
		nCount++;
		strName.Format(_T("%s%d"),strTmp,nCount);
	}
	pGroupDict->close();
	return Acad::eOk;

}
//创建分组
//pDb:数据库
//strName:分组名称
//bRenameIfExist:如果已经存在是否需要重新命名
//ids,实体ID数组
Acad::ErrorStatus makeGroup(AcDbDatabase* pDb,
							CString& strName,
							bool bRenameIfExist,
							AcDbObjectIdArray& ids)
{
	Acad::ErrorStatus es=Acad::eOk;
	if(bRenameIfExist)
	{
		es=notExistGroupName(pDb,strName);
		if(es!=Acad::eOk)
		{
			return es;
		}
	}
	AcDbGroup *pGroup = new AcDbGroup(strName);

	AcDbDictionary *pGroupDict;
	es=pDb->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}

	AcDbObjectId groupId;
	pGroupDict->setAt(strName, pGroup, groupId);
	pGroupDict->close();
	pGroup->close();

	es=makeGroup(groupId,ids);
	return es;
}