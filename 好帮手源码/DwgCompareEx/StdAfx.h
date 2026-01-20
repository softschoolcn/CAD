// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once
#if _MSC_VER>=1600
#include "StdAfx19.h"
#elif _MSC_VER>=1500
#include "StdAfx18.h"
#elif _MSC_VER>=1400
#include "StdAfx17.h"
#else
#include "StdAfx16.h"
#endif
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#include <msxml2.h>
#ifdef _WIN64
			#import <msxml6.dll>
#else
			#import <msxml4.dll>
#endif
//设置文档的显示方式
LRESULT ShowDocWindow(AcApDocument* pDoc,int nCmdShow);
//功能:	获取当前视图
//参数:	view		返回的视图
Acad::ErrorStatus getCurrentView(AcDbViewTableRecord &view);

#include <afxwin.h>
void OutputDebugStringX(CString fmt, ...);
void SplitString(CString string,char marker,CStringArray  &stringArray);
//定义PI
#ifndef PI
#define PI 4*atan(1.0)//提高pi的精度
#endif
extern DWORD Glb_TickCount;
//块参照是否在使用(不一定非常准确)
bool IsBlockTableRecordInUse(AcDbBlockTableRecord* pRecord);

extern bool  Glb_bOriginalFile;//原始文件
extern bool Glb_bOriginalColor;//原始颜色
extern int Glb_nColorSame;//相同对象的颜色
extern int Glb_nColorChange;//改变对象的颜色
extern int Glb_nColorNew;//增加对象的颜色
extern int Glb_nColorDelete;//删除对象的颜色
Acad::ErrorStatus OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
#ifdef CMP_SINGLE//没有设置为图纸比较独立版本
#include "GLog.h"
#else
#include "..\CADCommTools\\ExportHeaders.h"
#if _MSC_VER>=1900
#pragma comment(lib, "..\\Release\\sys21(X64)\\CADCommTools.lib")
#elif _MSC_VER>=1700

#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys20(X64)\\CADCommTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys20\\CADCommTools.lib")
#endif
#elif _MSC_VER>=1600

#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys19(X64)\\CADCommTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys19\\CADCommTools.lib")
#endif
#elif _MSC_VER>=1500
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys18(X64)\\CADCommTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys18\\CADCommTools.lib")
#endif
#elif _MSC_VER>=1400
#pragma comment(lib, "..\\Release\\sys17\\CADCommTools.lib")
#else
#ifdef ACAD_2004 
#pragma comment(lib, "..\\Release\\sys16.0\\CADCommTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys16\\CADCommTools.lib")
#endif
#endif

#include "..\CADGlobalTools\\GlobalToolsHeaders.h"
#if _MSC_VER>=1900
#pragma comment(lib, "..\\Release\\sys21(X64)\\CADGlobalTools.lib")
#elif _MSC_VER>=1700
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys20(X64)\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys20\\CADGlobalTools.lib")
#endif
#elif _MSC_VER>=1600
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys19(X64)\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys19\\CADGlobalTools.lib")
#endif
#elif _MSC_VER>=1500
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys18(X64)\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys18\\CADGlobalTools.lib")
#endif
#elif _MSC_VER>=1400
#pragma comment(lib, "..\\Release\\sys17\\CADGlobalTools.lib")
#else
#ifdef ACAD_2004 
#pragma comment(lib, "..\\Release\\sys16.0\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys16\\CADGlobalTools.lib")
#endif
#endif
#endif

#include "CmpDefine.h"
#include "GSameDxfHelper.h"
#include "GCmpResultInstance.h"