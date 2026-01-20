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

//-----------------------------------------------------------------------------
#define szRDS _RXST("")
#include "CommCommand.h"
#include "TextCommCommand.h"
#include "TextStatistics.h"
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADTextToolsApp : public AcRxArxApp {

public:
	CCADTextToolsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here

		//3:文字统计
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("TextStatistics"),
		GetCommandAlias(_T("TextStatistics")),
		0,
		TextStatistics);

		//10:单行文本转换为多行文本
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("TxtToMText"),
		GetCommandAlias(_T("TxtToMText")),
		0,
		TxtToMText);
	//11:多行文字转换为单行文字
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("MTextToTxt"),
		GetCommandAlias(_T("MTextToTxt")),
		0,
		MTextToTxt);
	//12:文字边框
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("TextBorder"),
		GetCommandAlias(_T("TextBorder")),
		0,
		TextBorder);
	//13:文字重叠
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("TextCross"),
		GetCommandAlias(_T("TextCross")),
		0,
		TextCross);

		//38:文字递增复制
	acedRegCmds->addCommand(_T("CADTextTools") ,
		_T("TextBatchCopy") , 
		GetCommandAlias(_T("TextBatchCopy")) ,
		0,
		TextBatchCopy);

		//53:连续修改
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("ContinueModify"),
		GetCommandAlias(_T("ContinueModify")),
		0,
		ContinueModify);
	//54:日期时间
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("AssDateTime"),
		GetCommandAlias(_T("AssDateTime")),
		0,
		AssDateTime);
	//69:清除格式
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("ClearFormat"),
		GetCommandAlias(_T("ClearFormat")),
		0,
		ClearFormat);
	//47:文字刷
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("TextBrush"),
		GetCommandAlias(_T("TextBrush")),
		0,
		TextBrush);

		//52:批量修改
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("BatchModify"),
		Sys_GetCommandAlias(_T("BatchModify")),
		0,
		BatchModify);

	//89:拷贝文字内容
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_CopyText"),
		GetCommandAlias(_T("cadgj_CopyText")),
		0,
		CopyText);
	//92:查找/替换
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_Find"),
		Sys_GetCommandAlias(_T("cadgj_Find")),
		0,
		FindEx);

	//95:下划线
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_xhx"),
		Sys_GetCommandAlias(_T("cadgj_xhx")),
		0,
		TextUnderline);

	//96:删除下划线
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_sxhx"),
		Sys_GetCommandAlias(_T("cadgj_sxhx")),
		0,
		RemoveTextUnderline);

	//97:上划线
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_shx"),
		Sys_GetCommandAlias(_T("cadgj_shx")),
		0,
		TextUpperline);

	//98:删除上划线
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_sshx"),
		Sys_GetCommandAlias(_T("cadgj_sshx")),
		0,
		RemoveTextUpperline);


	//101:文字转换为大写
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_uppertext"),
		Sys_GetCommandAlias(_T("cadgj_uppertext")),
		0,
		UpperText);

	//102:文字转换为小写
	acedRegCmds->addCommand(_T("CADTextTools"),
		_T("cadgj_lowertext"),
		Sys_GetCommandAlias(_T("cadgj_lowertext")),
		0,
		LowerText);


		return (retCode) ;

	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		destroyDlgBatchCopy();
		acedRegCmds->removeGroup(_T("CADTextTools"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADTextToolsApp)

