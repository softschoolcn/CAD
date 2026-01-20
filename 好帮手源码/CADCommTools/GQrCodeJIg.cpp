#include "StdAfx.h"
#include "GQrCodeJIg.h"
bool GQrCodeEnt::IsFill(bool* s,int X,int Y)
{
	return s[Y*maxX+X];
}
bool GQrCodeEnt::IsAllFill(bool* s,int X,int YStart,int YEnd)
{
	for(int Y=YStart;Y<YEnd;Y++)
	{
		if(!IsFill(s,X,Y))
		{
			return false;
		}
	}
	return true;
}
void GQrCodeEnt::Update(AcGePoint3d newPt,bool bPriview)
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
	double dx=fabs(pt.x-ptBase.x)/maxX;
	double dy=fabs(pt.y-ptBase.y)/maxY;
	double dMin=min(dx,dy);
	dx=dy=dMin;
	double dMinX=min(pt.x,ptBase.x);
	double dMinY=min(pt.y,ptBase.y);
	double dMaxX=dMinX+maxX*dx;
	double dMaxY=dMinY+maxY*dy;
	if(dx<1E-6||dy<1E-6)
	{
		return;
	}
	int nCount=0;
	bool* s1=new bool[maxX*maxY];
	memcpy(s1,s,maxX*maxY*sizeof(bool));
	for(int X=0;X<maxX;X++)
	{
		CArray<int> yStartArray;
		CArray<int> yEndArray;
		for(int Y=0;Y<maxY;Y++)
		{
			bool bHatch=s1[Y*maxX+X];
			if(bHatch)
			{
				//是黑色点
				if(yStartArray.GetCount()==yEndArray.GetCount())
				{
					yStartArray.Add(Y);
				}
				else
				{
					//下一个黑色的点
					if(Y==maxY-1)
					{
						//最后一个是黑色点
						yEndArray.Add(Y);
					}
				}
			}
			else
			{
				//不是黑色点
				if(yStartArray.GetCount()==yEndArray.GetCount()+1)
				{
					yEndArray.Add(Y-1);
				}
			}
		}

		for(int i=0;i<yStartArray.GetCount();i++)
		{
			int yStart=yStartArray.GetAt(i);
			int yEnd=yEndArray.GetAt(i);
			nCount++;
			if((nCount%100)!=1&&bPriview)
			{
				continue;
			}
			int xAdd=0;
			for(int X1=X+1;X1<maxX;X1++)
			{
				if(!IsAllFill(s1,X1,yStart,yEnd))
			{
				break;
			}
				for(int Y=yStart;Y<=yEnd;Y++)
				{
					s1[Y*maxX+X1]=false;
				}
				xAdd++;
			}
			

			AcGePoint3d pt0=AcGePoint3d(X*dx+dMinX,(maxY-yEnd-1)*dy+dMinY,0);
			AcGePoint3d pt2=AcGePoint3d(X*dx+dMinX,(maxY-yStart)*dy+dMinY,0);

			AcGePoint3d pt1=AcGePoint3d((X+1+xAdd)*dx+dMinX,(maxY-yEnd-1)*dy+dMinY,0.0);
			AcGePoint3d pt3=AcGePoint3d((X+1+xAdd)*dx+dMinX,(maxY-yStart)*dy+dMinY,0.0);
			AcDbSolid* pSolid=new AcDbSolid(pt0,pt1,pt2,pt3);
			mEntArr.append(pSolid);
		}
	}
	delete[] s1;
	s1=NULL;
}
GQrCodeJIg::GQrCodeJIg(void)
{
	pEnt=NULL;
}

GQrCodeJIg::~GQrCodeJIg(void)
{
}
void GQrCodeJIg::InitEnt()
{
	pEnt=new GQrCodeEnt;
	pEnt->maxX=maxX;
	pEnt->maxY=maxY;
	pEnt->s=s;
	pEnt->ptBase=ptBase;
}
Adesk::Boolean GQrCodeJIg::update()
{
	pEnt->Update(m_pointTopLast);
	return Adesk::kTrue;
}
AcEdJig::DragStatus GQrCodeJIg::sampler()
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

int GQrCodeJIg::Do()
{
	int nRet = RTERROR;
	AcEdJig::DragStatus stat;
	stat = drag();
	switch(stat)
	{
	case kNormal:
	case kNoChange:
		pEnt->Update(m_pointTopLast,false);
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
