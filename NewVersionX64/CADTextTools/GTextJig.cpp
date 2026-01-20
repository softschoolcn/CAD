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
//

//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GTextJig.h"
#include "GBatchCopyDialog.h"
#include "GlobalFunction.h"
//-----------------------------------------------------------------------------
GTextJig::GTextJig () : AcEdJig (),m_pEnt(NULL),m_bStop(FALSE)
{
}

GTextJig::~GTextJig () 
{
}
Adesk::Boolean GTextJig::update()
{
	if(m_pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText::cast(m_pEnt)->setLocation(m_pointTopLast);
	}
	else if(m_pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText::cast(m_pEnt)->setPosition(m_pointTopLast);
	}
	if(m_bStop)
	{
		return Adesk::kFalse;
	}
	return Adesk::kTrue;
}
AcEdJig::DragStatus GTextJig::sampler()
{
	setSpecialCursorType(AcEdJig::kCrosshair);
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNoZeroResponseAccepted));


	DragStatus stat;
	AcGePoint3d oldPoint = m_pointTopLast;
	stat = acquirePoint(m_pointTopLast,oldPoint);
	if (stat == kNormal)
	{
		if (oldPoint == m_pointTopLast)
		{
			stat = AcEdJig::kNoChange;
		}
	}
	if(m_bStop)
	{
		return AcEdJig::kCancel;
	}

	return stat;
}
void GTextJig::SetEntity(AcDbEntity* pEnt)
{
	m_pEnt=pEnt;
	if(m_pEnt->isKindOf(AcDbMText::desc()))
	{
		m_point=AcDbMText::cast(m_pEnt)->location();
	}
	else if(m_pEnt->isKindOf(AcDbText::desc()))
	{
		m_point=AcDbText::cast(m_pEnt)->position();
	}
}
int GTextJig::Do()
{
	int nRet = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch(stat)
	{
	case kNormal:
	case kNoChange:
		append();
		nRet = RTNORM;
		break ;
	case kCancel:
		nRet = RTCAN;
		delete m_pEnt;
		m_pEnt = NULL;
		break;
	default: 
		delete m_pEnt;
		m_pEnt = NULL;
		break ;
	}

	return nRet;
}
Adesk::Boolean GBatchCopyTextJig::update()
{
	CString strText=m_pDlgBatchCopy->m_strMid;
	CString strNum;
	m_pDlgBatchCopy->m_editAdd.GetWindowText(strNum);
	NumPlus(strText,_ttoi(strNum),!m_pDlgBatchCopy->m_bLoop);
	strLastText=strText;
	CString strContents=m_pDlgBatchCopy->m_strLeft+strText+m_pDlgBatchCopy->m_strRight;
	if(!m_pDlgBatchCopy->m_btnDis.GetCheck())
	{
		int nRet=0;
		GCADUtils::GetCADIntVar(_T("ORTHOMODE"),nRet);
		AcGePoint3d ptLocation=m_pointTopLast;
		if(nRet==1)
		{
			//开启了正交模式
			if(fabs(m_pointTopLast.x-m_point.x)>
				fabs(m_pointTopLast.y-m_point.y))
			{
				ptLocation.y=m_point.y;
			}
			else
			{
				ptLocation.x=m_point.x;
			}
		}
		if(m_pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText::cast(m_pEnt)->setLocation(ptLocation);
			AcDbMText::cast(m_pEnt)->setContents(strContents);
		}
		else if(m_pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText::cast(m_pEnt)->setPosition(ptLocation);
			AcDbText::cast(m_pEnt)->setTextString(strContents);
		}
		if(m_bStop)
		{
			return Adesk::kFalse;
		}
		double dis=m_pointTopLast.distanceTo(m_pDlgBatchCopy->pt);
		CString strTmp;
		strTmp.Format(_T("%5.2f"),dis);
		m_pDlgBatchCopy->m_editDis.SetWindowText(strTmp);
		return Adesk::kTrue;
	}
	else
	{
		if(fabs(m_pointTopLast.x-m_pDlgBatchCopy->pt.x)<1E-6&&fabs(m_pointTopLast.y-m_pDlgBatchCopy->pt.y)<1E-6)
		{
			if(m_pEnt->isKindOf(AcDbMText::desc()))
			{
				AcDbMText::cast(m_pEnt)->setLocation(m_pointTopLast);
				AcDbMText::cast(m_pEnt)->setContents(strContents);
			}
			else if(m_pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText::cast(m_pEnt)->setPosition(m_pointTopLast);
				AcDbText::cast(m_pEnt)->setTextString(strContents);
			}
			return Adesk::kTrue;
		}
		else
		{
			CString strText;
			m_pDlgBatchCopy->m_editDis.GetWindowText(strText);
			double dis=_tstof(strText);
			double dLen=m_pointTopLast.distanceTo(m_pDlgBatchCopy->pt);
			double dx=dis/dLen*(m_pointTopLast.x-m_pDlgBatchCopy->pt.x);
			double dy=dis/dLen*(m_pointTopLast.y-m_pDlgBatchCopy->pt.y);
			if(m_pEnt->isKindOf(AcDbMText::desc()))
			{
				AcDbMText::cast(m_pEnt)->setLocation(AcGePoint3d(m_pDlgBatchCopy->pt.x+dx,m_pDlgBatchCopy->pt.y+dy,0.0));
				AcDbMText::cast(m_pEnt)->setContents(strContents);
			}
			else if(m_pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText::cast(m_pEnt)->setPosition(AcGePoint3d(m_pDlgBatchCopy->pt.x+dx,m_pDlgBatchCopy->pt.y+dy,0.0));
				AcDbText::cast(m_pEnt)->setTextString(strContents);
			}
			return Adesk::kTrue;

		}

		if(m_bStop)
		{
			return Adesk::kFalse;
		}
	}
}
int GBatchCopyTextJig::Do()
{
	int nRet = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch(stat)
	{
	case kNormal:
	case kNoChange:
		m_pDlgBatchCopy->m_strMid=strLastText;
		append();
		nRet = RTNORM;
		break ;
	case kCancel:
		nRet = RTCAN;
		delete m_pEnt;
		m_pEnt = NULL;
		break;
	default: 
		delete m_pEnt;
		m_pEnt = NULL;
		break ;
	}

	return nRet;
}
