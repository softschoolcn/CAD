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
//----- GDatabaseReactorEx.cpp : Implementation of GDatabaseReactorEx
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GDatabaseReactorEx.h"
#if _MSC_VER<1700
//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(GDatabaseReactorEx, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
GDatabaseReactorEx::GDatabaseReactorEx () : AcDbDatabaseReactor()
{
}

//-----------------------------------------------------------------------------
GDatabaseReactorEx::~GDatabaseReactorEx () 
{
}

void GDatabaseReactorEx::objectAppended       (const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
}
void GDatabaseReactorEx::objectUnAppended     (const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
}
void GDatabaseReactorEx::objectReAppended     (const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
}
void GDatabaseReactorEx::objectOpenedForModify(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	RemoveEdu(dwg,dbObj);
}
void GDatabaseReactorEx::objectModified       (const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
}
void GDatabaseReactorEx::objectErased         (const AcDbDatabase* dwg, const AcDbObject* dbObj,
											   Adesk::Boolean pErased)
{
}
void GDatabaseReactorEx::RemoveEdu(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	if(dbObj==NULL)
	{
		return;
	}
	if(dbObj->database()!=dwg)
	{
		return;
	}
	if(dbObj->isKindOf(AcDbLayout::desc()))
	{
		RemoveEduPlotStamp(dwg);
	}
}
#endif
