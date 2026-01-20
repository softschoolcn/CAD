#include "StdAfx.h"
#include "GDimJig.h"
//-----------------------------------------------------------------------------
GDimJig::GDimJig () : AcEdJig (),m_pDim(NULL),m_bStop(FALSE),m_bJigDimLinePoint(true),m_bInLine(false)
{
}

GDimJig::~GDimJig () 
{
}
Adesk::Boolean GDimJig::update()
{
	if(m_pDim->isKindOf(AcDbAlignedDimension::desc()))
	{
		AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(m_pDim);
		if(m_bJigDimLinePoint)
		{
			pDim->setDimLinePoint(m_pointTopLast);
		}
		else
		{
			pDim->setXLine2Point(m_pointTopLast);
			pDim->setDimLinePoint(m_pointTopLast+m_vec);
		}
	}
	else if(m_pDim->isKindOf(AcDbRotatedDimension::desc()))
	{
		AcDbRotatedDimension* pDim=AcDbRotatedDimension::cast(m_pDim);
		if(m_bJigDimLinePoint)
		{
			pDim->setDimLinePoint(m_pointTopLast);
		}
		else
		{
			AcGePoint3d pt=pDim->dimLinePoint();
			pDim->setXLine2Point(m_pointTopLast);
			pDim->setDimLinePoint(pt);
		}
	}
	if(m_bStop)
	{
		return Adesk::kFalse;
	}
	return Adesk::kTrue;
}
AcEdJig::DragStatus GDimJig::sampler()
{
	setSpecialCursorType(AcEdJig::kCrosshair);
	setUserInputControls((UserInputControls)
		(AcEdJig::kNullResponseAccepted|AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNoZeroResponseAccepted));


	DragStatus stat;
	AcGePoint3d oldPoint = m_pointTopLast;
	stat = acquirePoint(m_pointTopLast,oldPoint);
	if (stat == kNormal)
	{
		if((!m_bJigDimLinePoint)&&m_bInLine&&
			m_pDim->isKindOf(AcDbAlignedDimension::desc()))
		{
			CoordinateD from;
			from.fromCadPoint(m_pt1);
			CoordinateD to;
			to.fromCadPoint(m_pt2);
			LineD l(from,to);
			CoordinateD ptOut;
			CoordinateD coord;
			coord.fromCadPoint(m_pointTopLast);
			if(l.plumbPoint(ptOut,coord))
			{
				m_pointTopLast=ptOut.toCadPoint();
			}

		}
		if (oldPoint == m_pointTopLast)
		{
			stat = AcEdJig::kNoChange;
		}
	}
	else
	{
		stat =AcEdJig::kCancel;
	}
	if(m_bStop)
	{
		return AcEdJig::kCancel;
	}

	return stat;
}
int GDimJig::Do()
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
		if(m_pDim!=NULL)
		{
			delete m_pDim;
			m_pDim = NULL;
		}
		break;
	default: 
		if(m_pDim!=NULL)
		{
			delete m_pDim;
			m_pDim = NULL;
		}
		break ;
	}

	return nRet;
}
