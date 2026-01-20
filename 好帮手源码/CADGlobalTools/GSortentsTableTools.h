/*
* Copyright (c) 2013,CAD工具之家(www.cadgj.com)
* All rights reserved.
* 
* 文件名称：GSortentsTableTools.h
* 摘    要：声明排序表的操作工具类
* 完成日期：2013年10月10日
*/
#pragma once
#ifndef ACAD_2004
#include "sorttab.h"
class GLOBALTOOLS_EXT GSortentsTableTools
{
public:
	GSortentsTableTools(void);
	~GSortentsTableTools(void);
	// 功能：将实体移动到最底层显示
	// 参数：sortEntIds			实体id数组
	Acad::ErrorStatus moveEntityToBottom(const AcDbObjectIdArray& sortEntIds);
	// 功能：将实体移动到最上层显示
	// 参数：sortEntIds			实体id数组
	Acad::ErrorStatus moveEntityToTop(const AcDbObjectIdArray& sortEntIds);
	// 功能：将实体移动到目标实体的下面
	// 参数：sortEntIds			实体id数组
	//       targetId           目标实体
	Acad::ErrorStatus moveEntityBelow(const AcDbObjectIdArray& sortEntIds,AcDbObjectId targetId);
	// 功能：将实体移动到目标实体的上面
	// 参数：sortEntIds			实体id数组
	//       targetId           目标实体
	Acad::ErrorStatus moveEntityAbove(const AcDbObjectIdArray& sortEntIds,AcDbObjectId targetId);
	// 功能：将单个实体移动到最底层显示
	// 参数：sortEntId			实体id
	Acad::ErrorStatus moveEntityToBottom(const AcDbObjectId sortEntId);
	// 功能：将单个实体移动到最上层显示
	// 参数：sortEntId			实体id
	Acad::ErrorStatus moveEntityToTop(const AcDbObjectId sortEntId);
	// 功能：将单个实体移动到目标实体的下面
	// 参数：sortEntId			实体id
	//       targetId           目标实体id
	Acad::ErrorStatus moveEntityBelow(const AcDbObjectId sortEntId,AcDbObjectId targetId);
	// 功能：将单个实体移动到目标实体的上面
	// 参数：sortEntId			实体id
	//       targetId           目标实体id
	Acad::ErrorStatus moveEntityAbove(const AcDbObjectId sortEntId,AcDbObjectId targetId);
	// 功能：获取或创建排序表，需先锁定文档
	// 参数：pSortTab			排序表
	Acad::ErrorStatus getOrCreateSortEntsDictionary(AcDbSortentsTable *&pSortTab);
};
#endif
