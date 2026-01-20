//文件名:IdsTrans.h
//说明:实体ID对应的各种转换
//日期:2013.06.27
#pragma once

// 功能：将选择集转换为实体ID数组
// 参数：	 ObjIds,实体ID数组
// ssName,选择集
// 返回：
void GLOBALTOOLS_EXT SSToIds(AcDbObjectIdArray& ObjIds,ads_name ssName);
// 功能：将实体ID数组转换为选择集
// 参数：	 ssName,选择集
// ObjIds,实体ID数组
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT IdsToSS(ads_name& ssName,AcDbObjectIdArray& ObjIds);
// 功能：将实体ID转换为CAD句柄
// 参数：	 handle,CAD句柄
// objId,实体ID
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT IdToHandle(AcDbHandle& handle,AcDbObjectId objId);
// 功能：将实体ID转换为CAD句柄
// 参数：	 strHandle,CAD句柄(字符串)
// objId,实体ID
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT IdToHandle(CString& strHandle,AcDbObjectId objId);
// 功能：将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// pDB,CAD数据库
// handle,CAD句柄
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT HandleToId(AcDbObjectId& objId,AcDbDatabase* pDB,AcDbHandle handle);
// 功能：将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// pDB,CAD数据库
// strHandle,CAD句柄(字符串)
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT HandleToId(AcDbObjectId& objId,AcDbDatabase* pDB,CString strHandle);
// 功能：在当前数据库中将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// handle,CAD句柄
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT HandleToId(AcDbObjectId& objId,AcDbHandle handle);
// 功能：在当前数据库中将CAD句柄转换为实体ID
// 参数：	 objId,实体ID
// strHandle,CAD句柄(字符串)
// 返回：
Acad::ErrorStatus GLOBALTOOLS_EXT HandleToId(AcDbObjectId& objId,CString strHandle);
// 功能：将实体ID转换为long
// 参数：	 objId,实体ID
// 返回：
long GLOBALTOOLS_EXT IdToLong(AcDbObjectId objId);
// 功能：将long转换为实体ID
// 参数：	 lValue,long值
// 返回：
AcDbObjectId GLOBALTOOLS_EXT LongToId(long lValue);