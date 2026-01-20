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
#include "BlockCommCommand.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADBlockToolsApp : public AcRxArxApp {

public:
	CCADBlockToolsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		//49:属性转文字
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("AttrToTxt"),
		GetCommandAlias(_T("AttrToTxt")),
		0,
		AttrToTxt);
		//56:改块基点
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("BlockBasePt"),
		GetCommandAlias(_T("BlockBasePt")),
		0,
		BlockBasePt);
	//57:改块名称
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("ModifyBlcokName"),
		GetCommandAlias(_T("ModifyBlcokName")),
		0,
		ModifyBlcokName);
	//66:快速定义块
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("QuickBlockDefine"),
		GetCommandAlias(_T("QuickBlockDefine")),
		0,
		QuickBlockDefine);

	//120:块爆破
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("BlastBlock"),
		GetCommandAlias(_T("BlastBlock")),
		ACRX_CMD_MODAL|ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW,
		BlastBlock);


		//122:文字块（最后一个功能）
	acedRegCmds->addCommand(_T("CADBlockToolsApp"),
		_T("TextBlock"),
		GetCommandAlias(_T("TextBlock")),
		ACRX_CMD_MODAL,
		TextBlock);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		acedRegCmds->removeGroup(_T("CADBlockToolsApp"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADBlockToolsApp)

