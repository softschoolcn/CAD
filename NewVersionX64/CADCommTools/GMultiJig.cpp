#include "StdAfx.h"
#include "GMultiJig.h"

GMultiJig::GMultiJig()
{
	mEntity = new GMultiJigEntity();
}

GMultiJig::~GMultiJig()
{
	delete mEntity;
}

AcEdJig::DragStatus GMultiJig::sampler()
{
	setSpecialCursorType(AcEdJig::kCrosshair);
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNoZeroResponseAccepted));

	DragStatus stat;
	AcGePoint3d oldPoint = mPoint;
	stat = acquirePoint(mPoint);
	if (stat == kNormal)
	{
		if(oldPoint == mPoint)
		{
			stat = AcEdJig::kNoChange;
		}
	}

	return stat;    
}

Adesk::Boolean GMultiJig::update()
{
	mEntity->Update(mPoint);
	return Adesk::kTrue;
}

AcDbEntity* GMultiJig::entity() const
{
	return mEntity;
}

int GMultiJig::Do()
{
	int ret = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch (stat)
	{
	case kNormal:
	case kNoChange:
		mEntity->AddToDb(m_ids);
		ret = RTNORM;
		break ;
	case kCancel:
		ret = RTCAN;
		mEntity->Release();
		break;
	default:
		mEntity->Release();
		break ;
	}

	return ret;
}
void GMultiJig::GetIds(AcDbObjectIdArray& ids)
{
	ids.append(m_ids);
}
