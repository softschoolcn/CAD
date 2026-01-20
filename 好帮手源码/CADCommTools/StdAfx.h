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
#include "afxtempl.h"
#endif
//定义PI
#ifndef PI
#define PI 4*atan(1.0)//提高pi的精度
#endif
#pragma pack (pop)
#include "GComputerId.h"
#include "DES.h"
#include "ByteString.h"

#include "ExportHeaders.h"
#include "AcCommFunction.h"
#include "AcDbEntityPointerEx.h"
#include "GxAfxFunction.h"
#include "AcDbEntityPointerEx.h"
#include "GLockDocument.h"
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#include <msxml2.h>
#include <afxwin.h>
#ifdef _WIN64
#import <msxml6.dll>
#else
#import <msxml4.dll>
#endif
void ads_regen();

extern CString Glb_ComputerId;
extern BOOL    Glb_bReg;
extern CString Glb_strRegDate;
#include "..\\GGridCtrl\\GridCtrlHeaders.h"
#include "..\CADGlobalTools\\GlobalToolsHeaders.h"
#include "..\\CADConfigTools\\ConfigToolsHeaders.h"
#include "GRegDialog.h"
#if _MSC_VER>=1900
#pragma comment(lib, "..\\Release\\sys21(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys21(X64)\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys21(X64)\\CADConfigTools.lib")
#elif _MSC_VER>=1700
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys20(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys20(X64)\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys20(X64)\\CADConfigTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys20\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys20\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys20\\CADConfigTools.lib")
#endif
#elif _MSC_VER>=1600
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys19(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys19(X64)\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys19(X64)\\CADConfigTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys19\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys19\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys19\\CADConfigTools.lib")
#endif
#elif _MSC_VER>=1500
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys18(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys18(X64)\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys18(X64)\\CADConfigTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys18\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys18\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys18\\CADConfigTools.lib")
#endif
#elif _MSC_VER>=1400
#pragma comment(lib, "..\\Release\\sys17\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys17\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys17\\CADConfigTools.lib")
#else
#ifdef ACAD_2004 
#pragma comment(lib, "..\\Release\\sys16.0\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys16.0\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys16.0\\CADConfigTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys16\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys16\\CADGlobalTools.lib")
#pragma comment(lib, "..\\Release\\sys16\\CADConfigTools.lib")
#endif
#endif


#import "..\\OfficeLib\\MSO.DLL" rename("RGB","MsoRGB") rename("SearchPath","MsoSearchPath") 
  
#import "..\\OfficeLib\\VBE6EXT.OLB" 
  
#import "..\\OfficeLib\\EXCEL.EXE" rename( "DialogBox", "ExcelDialogBox" ) rename( "RGB", "ExcelRGB" ) rename( "CopyFile", "ExcelCopyFile" ) rename( "ReplaceText", "ExcelReplaceText" ) exclude( "IFont", "IPicture" ) no_dual_interfaces

#include "GRegDefine.h"

//Double值的精度
#ifndef DoublePrecision
#define DoublePrecision 1E-8
#endif

//距离精度，由于需要进行开方计算，距离的精度不宜设置的过大
#ifndef DistancePrecision
#define DistancePrecision 1E-4
#endif


//读取XML节点的值
CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName);