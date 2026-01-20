#include "StdAfx.h"
#include "GZYJXJig.h"

void GZYJXEnt::Update(AcGePoint3d newPt)
{
	m_pt3=newPt;
}
#if _MSC_VER<1500
Adesk::Boolean      GZYJXEnt::worldDraw(AcGiWorldDraw*        mode)
#else
Adesk::Boolean      GZYJXEnt::subWorldDraw(AcGiWorldDraw*        mode)
#endif
{
	return Adesk::kFalse;
	CoordinateD cd1;
	cd1.fromCadPoint(m_pt1);
	CoordinateD cd2;
	cd2.fromCadPoint(m_pt2);
	CoordinateD cd3;
	cd3.fromCadPoint(m_pt3);
	LineD l1(cd1,cd2);
	CoordinateD cd4;
	if(!l1.plumbPoint(cd4,cd3))
	{
		return Adesk::kTrue;
	}
	AcGePoint3d pt4=cd4.toCadPoint();
	AcGeVector3d vc=m_pt3-pt4;
	m_pt5=m_pt1+vc;
	m_pt6=m_pt2+vc;
	AcDbPolyline pl;
	pl.addVertexAt(0,AcGePoint2d(m_pt1.x,m_pt1.y));
	pl.addVertexAt(1,AcGePoint2d(m_pt2.x,m_pt2.y));
	pl.addVertexAt(2,AcGePoint2d(m_pt6.x,m_pt6.y));
	pl.addVertexAt(3,AcGePoint2d(m_pt5.x,m_pt5.y));
	pl.setClosed(Adesk::kTrue);
	mode->geometry().draw(&pl);
	return Adesk::kTrue;
}
#if _MSC_VER<1500
	void      GZYJXEnt::viewportDraw(AcGiViewportDraw* mode)
#else
	
	void      GZYJXEnt::subViewportDraw(AcGiViewportDraw* mode)
#endif
	{
		CoordinateD cd1;
	cd1.fromCadPoint(m_pt1);
	CoordinateD cd2;
	cd2.fromCadPoint(m_pt2);
	CoordinateD cd3;
	cd3.fromCadPoint(m_pt3);
	LineD l1(cd1,cd2);
	CoordinateD cd4;
	if(!l1.plumbPoint(cd4,cd3))
	{
		return;
	}
	AcGePoint3d pt4=cd4.toCadPoint();
	AcGeVector3d vc=m_pt3-pt4;
	m_pt5=m_pt1+vc;
	m_pt6=m_pt2+vc;
	AcDbPolyline pl;
	pl.addVertexAt(0,AcGePoint2d(m_pt1.x,m_pt1.y));
	pl.addVertexAt(1,AcGePoint2d(m_pt2.x,m_pt2.y));
	pl.addVertexAt(2,AcGePoint2d(m_pt6.x,m_pt6.y));
	pl.addVertexAt(3,AcGePoint2d(m_pt5.x,m_pt5.y));
	pl.setClosed(Adesk::kTrue);
	mode->geometry().draw(&pl);
	//return Adesk::kTrue;
	}
AcDbObjectId GZYJXEnt::Append()
{
	AcDbPolyline* pl=new AcDbPolyline(4);
	pl->addVertexAt(0,AcGePoint2d(m_pt1.x,m_pt1.y));
	pl->addVertexAt(1,AcGePoint2d(m_pt2.x,m_pt2.y));
	pl->addVertexAt(2,AcGePoint2d(m_pt6.x,m_pt6.y));
	pl->addVertexAt(3,AcGePoint2d(m_pt5.x,m_pt5.y));
	pl->setClosed(Adesk::kTrue);
	AcDbObjectId id=AddToCurrentSpace(pl);
	pl->close();
	return id;
}
GZYJXJig::GZYJXJig(void)
{
	m_pEnt=new GZYJXEnt;
	m_bStop=FALSE;
}

GZYJXJig::~GZYJXJig(void)
{
	delete m_pEnt;
	m_pEnt=NULL;
}
Adesk::Boolean GZYJXJig::update()
{
	m_pEnt->Update(m_pointTopLast);
	if(m_bStop)
	{
		return Adesk::kFalse;
	}
	return Adesk::kTrue;
}
AcEdJig::DragStatus GZYJXJig::sampler()
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

int GZYJXJig::Do()
{
	int nRet = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch(stat)
	{
	case kNormal:
	case kNoChange:
		m_pEnt->Append();
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
