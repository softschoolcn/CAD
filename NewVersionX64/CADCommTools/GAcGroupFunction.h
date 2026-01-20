#pragma once
//创建分组
//groupId,分组ID
//ids,实体ID数组
Acad::ErrorStatus makeGroup(AcDbObjectId groupId,AcDbObjectIdArray& ids);
//不存在的分组名称
//pDb:数据库
//strName:分组名称
Acad::ErrorStatus notExistGroupName(AcDbDatabase* pDb,
									CString& strName);
//创建分组
//pDb:数据库
//strName:分组名称
//bRenameIfExist:如果已经存在是否需要重新命名
//ids,实体ID数组
Acad::ErrorStatus makeGroup(AcDbDatabase* pDb,
							CString& strName,
							bool bRenameIfExist,
							AcDbObjectIdArray& ids);
