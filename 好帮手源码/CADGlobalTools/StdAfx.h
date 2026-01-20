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
#pragma pack (pop)
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include "GlobalToolsHeaders.h"

//定义PI
#ifndef PI
#define PI 4*atan(1.0)//提高pi的精度
#endif

//Double值的精度
#ifndef DoublePrecision
#define DoublePrecision 1E-8
#endif

//距离精度，由于需要进行开方计算，距离的精度不宜设置的过大
#ifndef DistancePrecision
#define DistancePrecision 1E-4
#endif