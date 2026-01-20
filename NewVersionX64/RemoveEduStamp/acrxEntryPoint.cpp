// (C) Copyright 1990-2002 by Autodesk, Inc. 
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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GCommand.h"
#include "GEditorReactor.h"
#if _MSC_VER<1700
GEditorReactor* Glb_EditorReactor=NULL;
#endif
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CRemoveEduStampApp : public AcRxArxApp {

public:
	CRemoveEduStampApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
#if _MSC_VER<1700
		GDatabaseReactorEx::rxInit();
		GEditorReactor::rxInit();
		Glb_EditorReactor=new GEditorReactor;
#endif
		//75:È¥½ÌÓý°æ
		acedRegCmds->addCommand(_T("RemoveEduStamp"),
			_T("qjyb"),
			_T("qjyb"),
			ACRX_CMD_SESSION,
			RemoveEduStamp);
		acedRegCmds->addCommand(_T("RemoveEduStamp"),
			_T("noedu"),
			_T("noedu"),
			ACRX_CMD_SESSION,
			RemoveEduStamp);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		acedRegCmds->removeGroup(_T("RemoveEduStamp"));
#if _MSC_VER<1700
		deleteAcRxClass(GDatabaseReactorEx::desc());
		deleteAcRxClass(GEditorReactor::desc());
		if(Glb_EditorReactor!=NULL)
		{
			delete Glb_EditorReactor;
			Glb_EditorReactor=NULL;
		}
#endif
		return (retCode) ;
	}

	virtual void RegisterServerComponents () 
	{
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CRemoveEduStampApp)

