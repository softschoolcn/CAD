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
//----- GCommandReactor.cpp : Implementation of GCommandReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GCommandReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(GCommandReactor, AcEditorReactor, 1)

//-----------------------------------------------------------------------------
GCommandReactor::GCommandReactor (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
	m_pFilter=new GLayerSSGetFilter();
}

//-----------------------------------------------------------------------------
GCommandReactor::~GCommandReactor () {
	Detach () ;
	delete m_pFilter;
}

//-----------------------------------------------------------------------------
void GCommandReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void GCommandReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *GCommandReactor::Subject () const {
	return (acedEditor) ;
}

bool GCommandReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}
void GCommandReactor::commandEnded    (const ACHAR* cmdStr)
{
	if(lstrcmpi(cmdStr,_T("layermatchprop"))==0||
		lstrcmpi(cmdStr,_T("typematchprop"))==0)
	{
		removeSSgetFilterInputContextReactor(curDoc(), m_pFilter);
		Detach ();
	}
}
void GCommandReactor::commandCancelled(const ACHAR* cmdStr)
{
	//if(lstrcmpi(cmdStr,_T("matchprop"))==0)
	//{
	//	removeSSgetFilterInputContextReactor(curDoc(), m_pFilter);
	//	Detach ();
	//}
}
void GCommandReactor::commandFailed   (const ACHAR* cmdStr)
{
	//if(lstrcmpi(cmdStr,_T("matchprop"))==0)
	//{
	//	removeSSgetFilterInputContextReactor(curDoc(), m_pFilter);
	//	Detach ();
	//}
}
