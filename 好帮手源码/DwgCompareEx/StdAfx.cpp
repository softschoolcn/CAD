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
//------ StdAfx.cpp : source file that includes just the standard includes
//------  StdAfx.pch will be the pre-compiled header
//------  StdAfx.obj will contain the pre-compiled type information
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//功能:	获取当前视图
//参数:	view		返回的视图
Acad::ErrorStatus getCurrentView(AcDbViewTableRecord &view)
{
	Acad::ErrorStatus es=Acad::eOk;

	//static AcDbViewTableRecord view;
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs; 
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	acedGetVar(_T("VIEWMODE"), &rb);
	view.setPerspectiveEnabled((bool)(rb.resval.rint & 1));
	view.setFrontClipEnabled(bool(rb.resval.rint & 2));
	view.setBackClipEnabled(bool(rb.resval.rint & 4));
	view.setFrontClipAtEye(!(bool(rb.resval.rint & 16)));

	acedGetVar(_T("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X],rb.resval.rpoint[Y]));

	acedGetVar(_T("LENSLENGTH"), &rb);
	view.setLensLength(rb.resval.rreal);

	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X],rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X],rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	acedGetVar(_T("VIEWSIZE"), &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	acedGetVar(_T("SCREENSIZE"), &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	acedGetVar(_T("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	acedGetVar(_T("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;

	acedGetVar(_T("CVPORT"), &rb);
	int cvport = rb.resval.rint;

	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);
	if (!paperspace)
	{
		acedGetVar(_T("FRONTZ"), &rb);
		view.setFrontClipDistance(rb.resval.rreal);
		acedGetVar(_T("BACKZ"), &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}

	return es;
}
void OutputDebugStringX(CString fmt, ...)
{
	try
	{
		CString strBuffer;

		va_list marker;
		va_start(marker, fmt);
		strBuffer.FormatV(fmt, marker);
		va_end(marker);
		OutputDebugString(strBuffer);
	}
	catch(...)
	{

	}
}
//块参照是否在使用(不一定非常准确)
bool IsBlockTableRecordInUse(AcDbBlockTableRecord* pRecord)
{
	AcDbObjectIdArray refIds;
	pRecord->getBlockReferenceIds(refIds);
	AcDbObjectIdArray erased_refIds;
	pRecord->getErasedBlockReferenceIds(erased_refIds);
	if (refIds.length()-erased_refIds.logicalLength() < 1)  //没有块参照的也不处理
	{
		return false;
	}
	AcDbObjectId btrId=pRecord->objectId();
	BOOL bValid=FALSE;
	for(long k=0;k<refIds.logicalLength();k++)
	{
		if(erased_refIds.contains(refIds.at(k)))
			continue;
		AcDbObjectPointer<AcDbBlockReference> spRef(refIds.at(k),AcDb::kForRead);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spRef->isErased()==Adesk::kTrue||spRef->isEraseStatusToggled()==Adesk::kTrue)
		{
			continue;
		}
		AcDbObjectId btrId1=spRef->blockTableRecord();
		if(btrId1!=btrId)
		{
			//如果图框是拷贝的，再重新编辑属性的时候会设置一个新的块定义
			//但是老的块定义依然有一份记录，这也可以算是ACAD的一个BUG吧
			continue;
		}
		bValid=TRUE;
		break;
	}
	if(bValid)
	{
		return true;
	}
	else
	{
		return false;
	}
}
DWORD Glb_TickCount=0;
bool  Glb_bOriginalFile=false;//原始文件
bool Glb_bOriginalColor=false;//原始颜色
int Glb_nColorSame;//相同对象的颜色
int Glb_nColorChange;//改变对象的颜色
int Glb_nColorNew;//增加对象的颜色
int Glb_nColorDelete;//删除对象的颜色