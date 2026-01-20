// (C) Copyright 2005 by Autodesk, Inc. 
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

//-----------------------------------------------------------------------------
//- GTextJig.h
#pragma once

//-----------------------------------------------------------------------------
class GTextJig : public AcEdJig 
{
public:
	GTextJig();
	~GTextJig();
	virtual int Do();
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const{return m_pEnt;}
	void SetEntity(AcDbEntity* pEnt);
	AcDbEntity* m_pEnt;
	AcGePoint3d m_point;             //插入点
	AcGePoint3d m_pointTopLast;//插入点对应的上面的点(上一次用户输入)
	BOOL        m_bStop;
} ;
class GBatchCopyTextJig:public GTextJig
{
	friend class GBatchCopyDialog;
	GBatchCopyDialog * m_pDlgBatchCopy;
	CString strLastText;
	virtual Adesk::Boolean update();
	virtual int Do();
};
