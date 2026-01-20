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
#include "GConfigCommand.h"
#include "GCommandAliasDlg.h"
#include "ConfigToolsHeaders.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
CArray<GCommandDefine> Glb_SysCmds;
CString GetCommandAlias(CString strCmd)
{
	for(int i=0;i<Glb_SysCmds.GetCount();i++)
	{
		GCommandDefine& cmd=Glb_SysCmds.GetAt(i);
		if(cmd.strCmd.CompareNoCase(strCmd)==0)
		{
			return cmd.strAlias;
		}
	}
	return strCmd;
}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADConfigToolsApp : public AcRxArxApp {

public:
	CCADConfigToolsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here


		TCHAR lpPath[MAX_PATH+1]={0};
		GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
		PathRemoveFileSpec(lpPath);
		PathRemoveFileSpec(lpPath);
		TCHAR lpPath1[MAX_PATH+1]={0};
		lstrcpy(lpPath1,lpPath);
		PathAppend(lpPath1,_T("config\\CommandAlias.xml"));
		CArray<GCommandDefine> sys_cmds;
		CArray<GCommandDefine> user_cmds;
		if(PathFileExists(lpPath1))
		{
			ReadCommands(sys_cmds,lpPath1,FALSE);
		}

		TCHAR lpPath2[MAX_PATH+1]={0};
		lstrcpy(lpPath2,lpPath);
		PathAppend(lpPath2,_T("UserConfig\\CommandAlias.xml"));
		if(PathFileExists(lpPath2))
		{
			ReadCommands(user_cmds,lpPath2,TRUE);
		}
		ResetUserCommands(sys_cmds,user_cmds);
		Glb_SysCmds.RemoveAll();
		Glb_SysCmds.Append(sys_cmds);

		//115:ÏµÍ³ÅäÖÃ
	acedRegCmds->addCommand(_T("CADConfigTools") ,
		_T("SystemConfig") , 
		GetCommandAlias(_T("SystemConfig")) ,
		0 ,
		SystemConfig);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here

		acedRegCmds->removeGroup(_T("CADConfigTools"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADConfigToolsApp)

