/*
* Copyright (c) 2013,CAD工具之家(www.cadgj.com)
* All rights reserved.
* 
* 文件名称：GSortentsTableTools.cpp
* 摘    要：声明排序表的操作工具类
* 完成日期：2013年10月10日
*/
#include "StdAfx.h"
#include "GSortentsTableTools.h"
#include "GLockDocument.h"
#ifndef ACAD_2004
GSortentsTableTools::GSortentsTableTools(void)
{
}

GSortentsTableTools::~GSortentsTableTools(void)
{
}
// 功能：获取或创建排序表，需先锁定文档
// 参数：pSortTab			排序表
Acad::ErrorStatus GSortentsTableTools::getOrCreateSortEntsDictionary(AcDbSortentsTable *&pSortTab)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
	{
		es=Acad::eNullPtr;
		return es;
	}
	AcDbBlockTable *pBT	= NULL;
	AcDbBlockTableRecord *pBTR	= NULL;
	//获取块表指针
	es=pDb->getBlockTable(pBT,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return es;
	}
	//获取模型空间表记录指针
	es=pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		pBT->close();
		return es;
	}
	pBT->close();
	es=pBTR->getSortentsTable(pSortTab, AcDb::kForWrite, true);
	if(es!=Acad::eOk)
	{
		pBTR->close();
		return es;
	}
	pBTR->close();
	return Acad::eOk;
}
// 功能：将实体移动到最底层显示
// 参数：sortEntIds			实体id数组
Acad::ErrorStatus GSortentsTableTools::moveEntityToBottom(const AcDbObjectIdArray& sortEntIds)
{
	GLockDocument _Lock;//必须先锁定文档
	AcDbSortentsTable *pSortTab=NULL;
	Acad::ErrorStatus es=getOrCreateSortEntsDictionary(pSortTab);
	if(es!=Acad::eOk)
		return es;
	es=pSortTab->moveToBottom(sortEntIds);
	pSortTab->close();
	return es;
}
// 功能：将实体移动到最上层显示
// 参数：sortEntIds			实体id数组
Acad::ErrorStatus GSortentsTableTools::moveEntityToTop(const AcDbObjectIdArray& sortEntIds)
{
	GLockDocument _Lock;//必须先锁定文档
	AcDbSortentsTable *pSortTab=NULL;
	Acad::ErrorStatus es=getOrCreateSortEntsDictionary(pSortTab);
	if(es!=Acad::eOk)
		return es;
	pSortTab->moveToTop(sortEntIds);
	es=pSortTab->moveToTop(sortEntIds);
	pSortTab->close();
	return es;
}
// 功能：将实体移动到目标实体的下面
// 参数：sortEntIds			实体id数组
//       targetId           目标实体
Acad::ErrorStatus GSortentsTableTools::moveEntityBelow(const AcDbObjectIdArray& sortEntIds,AcDbObjectId targetId)
{
	GLockDocument _Lock;//必须先锁定文档
	AcDbSortentsTable *pSortTab=NULL;
	Acad::ErrorStatus es=getOrCreateSortEntsDictionary(pSortTab);
	if(es!=Acad::eOk)
		return es;
	pSortTab->moveBelow(sortEntIds,targetId);
	pSortTab->close();
	return es;
}
// 功能：将实体移动到目标实体的上面
// 参数：sortEntIds			实体id数组
//       targetId           目标实体
Acad::ErrorStatus GSortentsTableTools::moveEntityAbove(const AcDbObjectIdArray& sortEntIds,AcDbObjectId targetId)
{
	GLockDocument _Lock;//必须先锁定文档
	AcDbSortentsTable *pSortTab=NULL;
	Acad::ErrorStatus es=getOrCreateSortEntsDictionary(pSortTab);
	if(es!=Acad::eOk)
		return es;
	es=pSortTab->moveAbove(sortEntIds,targetId);
	pSortTab->close();
	return es;
}
// 功能：将单个实体移动到最底层显示
// 参数：sortEntId			实体id
Acad::ErrorStatus GSortentsTableTools::moveEntityToBottom(const AcDbObjectId sortEntId)
{
	AcDbObjectIdArray sortEntIds;
	sortEntIds.append(sortEntId);
	return moveEntityToBottom(sortEntIds);
}
// 功能：将单个实体移动到最上层显示
// 参数：sortEntId			实体id
Acad::ErrorStatus GSortentsTableTools::moveEntityToTop(const AcDbObjectId sortEntId)
{
	AcDbObjectIdArray sortEntIds;
	sortEntIds.append(sortEntId);
	return moveEntityToTop(sortEntIds);
}
// 功能：将单个实体移动到目标实体的下面
// 参数：sortEntId			实体id
//       targetId           目标实体id
Acad::ErrorStatus GSortentsTableTools::moveEntityBelow(const AcDbObjectId sortEntId,AcDbObjectId targetId)
{
	AcDbObjectIdArray sortEntIds;
	sortEntIds.append(sortEntId);
	return moveEntityBelow(sortEntIds,targetId);
}
// 功能：将单个实体移动到目标实体的上面
// 参数：sortEntId			实体id
//       targetId           目标实体id
Acad::ErrorStatus GSortentsTableTools::moveEntityAbove(const AcDbObjectId sortEntId,AcDbObjectId targetId)
{
	AcDbObjectIdArray sortEntIds;
	sortEntIds.append(sortEntId);
	return moveEntityAbove(sortEntIds,targetId);
}
#endif
