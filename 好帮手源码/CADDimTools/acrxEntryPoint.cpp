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
#include "GDimCommand.h"
#include "SpcoordLabel.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADDimToolsApp : public AcRxArxApp {

public:
	CCADDimToolsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		//48:坐标标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("DimXY"),
		GetCommandAlias(_T("DimXY")),
		0,
		DimXY);

	//55:多段线标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("DimPLine"),
		GetCommandAlias(_T("DimPLine")),
		0,
		CreatLabelPlineAllPt);

	//81:文字标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_DimText"),
		GetCommandAlias(_T("cadgj_DimText")),
		0,
		DimText);

	//105:天正坐标标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("DimXYTZ"),
		GetCommandAlias(_T("DimXYTZ")),
		0,
		DimXYTZ);
	//106:多段线天正标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("DimPLineTZ"),
		GetCommandAlias(_T("DimPLineTZ")),
		0,
		CreatLabelPlineAllPtTZ);

	//107:坐标点标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_DimCoords"),
		GetCommandAlias(_T("cadgj_DimCoords")),
		0,
		DimCoords);

	//116:线性标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_DimXX"),
		GetCommandAlias(_T("cadgj_DimXX")),
		0,
		DimXX);

	//117:对齐标注
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_DimDQ"),
		GetCommandAlias(_T("cadgj_DimDQ")),
		0,
		DimDQ);

	//118:标注断开
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_bzdk"),
		GetCommandAlias(_T("cadgj_bzdk")),
		0,
		bzdk);

	//119:标注合并
	acedRegCmds->addCommand(_T("CADDimTools"),
		_T("cadgj_bzhb"),
		GetCommandAlias(_T("cadgj_bzhb")),
		0,
		bzhb);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here

		acedRegCmds->removeGroup(_T("CADDimTools"));

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADDimToolsApp)

