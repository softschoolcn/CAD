//文件名:IdsTrans.cpp
//说明:实体ID对应的各种转换
//日期:2013.06.27
#include "StdAfx.h"
#include "IdsTrans.h"
// 功能：将选择集转换为实体ID数组
// 参数：	 ObjIds,实体ID数组
// ssName,选择集
// 返回：
void SSToIds(AcDbObjectIdArray& ObjIds,ads_name ssName)
{
	AcDbObjectId ObjId;
	ads_name EntName;
#if _MSC_VER>=1900
	Adesk::Int32 nLength = 0;
#else
	long nLength = 0;
#endif
	acedSSLength(ssName,&nLength);

	for(int nLen=0; nLen<nLength;nLen++) 
	{
		acedSSName(ssName,nLen,EntName);
			acdbGetObjectId(ObjId,EntName);
		ObjIds.append(ObjId);
	}
}
// 功能：将实体ID数组转换为选择集
// 参数：	 ObjIds,实体ID数组
// ssName,选择集
// 返回：
Acad::ErrorStatus IdsToSS(ads_name& ssName,AcDbObjectIdArray& ObjIds)
{
	//预防性释放
	acedSSFree(ssName);
	//Set ss to a null selection set
	acedSSAdd(NULL,NULL,ssName);
	int rc=RTNORM;
	Acad::ErrorStatus es=Acad::eOk;
	for(int i=0;i<ObjIds.logicalLength();i++)
	{
		ads_name tmpss;
			AcDbObjectId ObjId=ObjIds.at(i);
		es=acdbGetAdsName(tmpss,ObjId);
		if(es=Acad::eOk)
		{
			rc=acedSSFree(ssName);
			rc=acedSSFree(tmpss);
			return es;
		}
		rc=acedSSAdd(tmpss,ssName,ssName);
		if(rc!=RTNORM)
		{
			rc=acedSSFree(ssName);
			rc=acedSSFree(tmpss);
			return Acad::eInvalidInput;
		}
		rc=acedSSFree(tmpss);
	}
	return Acad::eOk;

}
// 功能：将实体ID转换为CAD句柄
// 参数：	 handle,CAD句柄
// objId,实体ID
// 返回：
Acad::ErrorStatus IdToHandle(AcDbHandle& handle,AcDbObjectId objId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,objId,AcDb::kForRead);
	if(es!=eOk)
		return es;
	pObj->getAcDbHandle(handle);
	pObj->close();
	return Acad::eOk;
}
// 功能：将实体ID转换为CAD句柄
// 参数：	 strHandle,CAD句柄(字符串)
// objId,实体ID
// 返回：
Acad::ErrorStatus IdToHandle(CString& strHandle,AcDbObjectId objId)
{
	AcDbHandle handle;
	Acad::ErrorStatus es=IdToHandle(handle,objId);
	if(es!=Acad::eOk)
		return Acad::eOk;
	TCHAR tmpStr[256]={0};
	handle.getIntoAsciiBuffer(tmpStr);
	strHandle=tmpStr;
	return es;
}
// 功能：将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// pDB,CAD数据库
// handle,CAD句柄
// 返回：
Acad::ErrorStatus HandleToId(AcDbObjectId& objId,AcDbDatabase* pDB,AcDbHandle handle)
{
	Acad::ErrorStatus es=Acad::eOk;
	if(pDB==NULL)
	{
		return Acad::eNullHandle;
	}
	es=pDB->getAcDbObjectId(objId,false,handle);
	return es;
}
// 功能：将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// pDB,CAD数据库
// strHandle,CAD句柄(字符串)
// 返回：
Acad::ErrorStatus HandleToId(AcDbObjectId& objId,AcDbDatabase* pDB,CString strHandle)
{
	AcDbHandle handle=strHandle;
	return HandleToId(objId,pDB,handle);
}
// 功能：在当前数据库中将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// handle,CAD句柄
// 返回：
Acad::ErrorStatus HandleToId(AcDbObjectId& objId,AcDbHandle handle)
{
	return HandleToId(objId,acdbHostApplicationServices()->workingDatabase(),handle);
}
// 功能：在当前数据库中将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// strHandle,CAD句柄(字符串)
// 返回：
Acad::ErrorStatus HandleToId(AcDbObjectId& objId,CString strHandle)
{
	return HandleToId(objId,acdbHostApplicationServices()->workingDatabase(),strHandle);
}
// 功能：将实体ID转换为long
// 参数：	 objId,实体ID
// 返回：
long IdToLong(AcDbObjectId objId)
{
	return objId.asOldId();
}
// 功能：将long转换为实体ID
// 参数：	 lValue,long值
// 返回：
AcDbObjectId LongToId(long lValue)
{
	AcDbObjectId tmpId;
#ifdef _WIN64
	Adesk::IntDbId oldId=lValue;
	tmpId.setFromOldId(oldId);
#else
	tmpId.setFromOldId(lValue);
#endif
	return tmpId;
}