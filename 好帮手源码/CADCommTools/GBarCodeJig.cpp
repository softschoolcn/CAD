#include "StdAfx.h"
#include "GBarCodeJig.h"
#include "GBarcodeMgr.h"
void GBarCodeEnt::Update(AcGePoint3d newPt)
{
	for(int i=0;i<mEntArr.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)mEntArr.at(i);
		if(pEnt!=NULL)
		{
			delete pEnt;
		}
	}
	mEntArr.setLogicalLength(0);
	pt=newPt;
	AcGePoint3d position=GCADUtils::MidPoint(pt,ptBase);
	double dHeight=fabs(ptBase.y-pt.y);
	GBarcodeMgr mgr;
	mgr.SetOriginCode(strBarCode);
	mgr.Create(mEntArr,position,dHeight,bText);
}
GBarCodeJig::GBarCodeJig(void)
{
}

GBarCodeJig::~GBarCodeJig(void)
{

}
void GBarCodeJig::InitEntity()
{
	pEnt=new GBarCodeEnt;
	pEnt->ptBase=ptBase;
	pEnt->strBarCode=strBarCode;
	pEnt->bText=bText;
}
AcEdJig::DragStatus GBarCodeJig::sampler()
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

	return stat;
}
Adesk::Boolean GBarCodeJig::update()
{
	pEnt->Update(m_pointTopLast);
	return Adesk::kTrue;
}
int GBarCodeJig::Do()
{
	int nRet = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch(stat)
	{
	case kNormal:
	case kNoChange:
		pEnt->Update(m_pointTopLast);
		pEnt->AddToDb(m_ids);
		delete pEnt;
		pEnt = NULL;
		nRet = RTNORM;
		break ;
	case kCancel:
		nRet = RTCAN;
		delete pEnt;
		pEnt = NULL;
		break;
	default: 
		delete pEnt;
		pEnt = NULL;
		break ;
	}

	return nRet;
}
