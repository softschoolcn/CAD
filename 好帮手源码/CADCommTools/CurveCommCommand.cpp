#include "StdAfx.h"
#include "CurveCommCommand.h"
#include "LineCrossReportFunction.h"
bool IsSamePoint2d(const AcGePoint2d ptA,const AcGePoint2d ptB)
{
	if(fabs(ptA.x-ptB.x)<1E-8&&fabs(ptA.y-ptB.y)<1E-8)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool IsSamePoint2d(const AcGePoint3d ptA,const AcGePoint3d ptB)
{
	if(fabs(ptA.x-ptB.x)<1E-8&&fabs(ptA.y-ptB.y)<1E-8)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//C是否在AB构成的直线上
bool IsPointOnLine(const AcGePoint3d ptA,const AcGePoint3d ptB,const AcGePoint3d ptC)
{
	if(fabs(ptA.x-ptB.x)<1E-8)
	{
		//垂线
		if(fabs(ptC.x-ptA.x)>1E-8)
		{
			return false;
		}
		double yMin=min(ptA.x,ptB.x);
		double yMax=max(ptA.x,ptB.x);
		if(ptC.y<yMin-1E-8||ptC.y>yMax+1E-8)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		//y=kx+b;
		double k=(ptB.y-ptA.y)/(ptB.x-ptA.x);
		double b=ptA.y-k*ptA.x;
		double yC=k*ptC.x+b;
		if(fabs(yC-ptC.y)>1E-8)
		{
			return false;
		}
		double xMin=min(ptA.x,ptB.x);
		double xMax=max(ptA.x,ptB.x);
		if(ptC.x<xMin-1E-8||ptC.x>xMax+1E-8)
		{
			return false;
		}
		else
		{
			return true;
		}

	}

}
//1:完全重叠
//2:AB压盖CD
//3:CD压盖AB
//4:部分重叠
//5:完全不重叠
int CalcRelation(const double dA,const double dB,
				 const double dC,const double dD)
{
	if(fabs(dA-dB)<1E-8)
	{
		//AB完全相同
		if(fabs(dC-dD)<1E-8)
		{
			//CD完全相同
			if(fabs(dA-dC)<1E-8)
			{
				return 1;
			}
			else
			{
				return 5;
			}
		}
		else
		{
			//CD不同
			double dMin=min(dC,dD);
			double dMax=max(dC,dD);
			if(dA<dMin-1E-8||dA>dMax+1E-8)
			{
				return 5;
			}
			else
			{
				return 3;
			}
		}
	}
	else
	{
		//AB不同
		double dMin=min(dA,dB);
		double dMax=max(dA,dB);
		if(fabs(dC-dD)<1E-8)
		{
			//CD完全相同
			if(dC<dMin-1E-8||dD>dMax+1E-8)
			{
				return 5;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			//CD也不相同
			double dMin1=min(dC,dD);
			double dMax1=max(dC,dD);
			if(dMax1<dMin-1E-8)
			{
				return 5;
			}
			else if(dMax1>=dMin-1E-8&&dMax1<=dMax+1E-8)
			{
				if(dMin1<dMin-1E-8)
				{
					return 4;
				}
				else if(dMin1>=dMin-1E-8&&dMin1<=dMin+1E-8&&
					dMax1>=dMax-1E-8&&dMax1<=dMax+1E-8)
				{
					return 1;
				}
				else
				{
					return 2;
				}
			}
			else
			{
				if(dMin1<dMin-1E-8)
				{
					return 3;
				}
				else if(dMin1>=dMin-1E-8&&dMin1<=dMax+1E-8)
				{
					return 4;
				}
				else
				{
					return 5;
				}
			}
		}
	}
}
//1:完全重叠
//2:AB压盖CD
//3:CD压盖AB
//4:部分重叠
//5:完全不重叠
int CalcLineRelation(const AcGePoint3d ptA,const AcGePoint3d ptB,
					 const AcGePoint3d ptC,const AcGePoint3d ptD)
{
	if(fabs(ptA.x-ptB.x)<1E-8)
	{
		//AB是垂线
		if(fabs(ptC.x-ptD.x)>=1E-8)
		{
			//CD不是垂线
			return 5;
		}
		else
		{
			//CD也是垂线
			if(fabs(ptA.x-ptC.x)>=1E-8)
			{
				return 5;
			}
			else
			{
				return CalcRelation(ptA.y,ptB.y,ptC.y,ptD.y);
			}
		}
	}
	else
	{
		if(fabs(ptC.x-ptD.x)<1E-8)
		{
			return 5;
		}
		//y=kx+b;
		double k=(ptB.y-ptA.y)/(ptB.x-ptA.x);
		double b=ptA.y-k*ptA.x;

		double k1=(ptD.y-ptC.y)/(ptD.x-ptC.x);
		double b1=ptC.y-k*ptC.x;

		if(fabs(k-k1)>=1E-8||fabs(b-b1)>=1E-8)
		{
			return 5;
		}
		return CalcRelation(ptA.x,ptB.x,ptC.x,ptD.x);
	}
}
//1:完全重叠
//2:AB压盖CD
//3:CD压盖AB
//4:部分重叠
//5:完全不重叠
int CalcSegRelation(const AcGePoint3d ptA,const AcGePoint3d ptB,const double bulgeAB,
					const AcGePoint3d ptC,const AcGePoint3d ptD,const double bulgeCD)
{
	if((fabs(bulgeAB)<1E-6&&fabs(bulgeCD)>=1E-6)||
		(fabs(bulgeAB)>=1E-6&&fabs(bulgeCD)<1E-6))
	{
		//一个是直线一个是圆弧
		//完全不重叠
		//没有考虑其中一个是弧线，一个是点的情况
		return 5;
	}
	if(fabs(bulgeAB)<1E-6)
	{
		//两条直线
		if(IsSamePoint2d(ptA,ptB))
		{
			//AB是一个点
			if(IsSamePoint2d(ptC,ptD))
			{
				//CD也是一个点
				if(IsSamePoint2d(ptA,ptC))
				{
					return 1;
				}
				else
				{
					return 5;
				}
			}
			else
			{
				//CD是一条直线
				if(IsPointOnLine(ptC,ptD,ptA))
				{
					return 3;
				}
				else
				{
					return 1;
				}

			}
		}
		else
		{
			//AB是一条直线
			if(IsSamePoint2d(ptC,ptD))
			{
				//CD是一个点
				if(IsPointOnLine(ptA,ptB,ptC))
				{
					return 2;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return CalcLineRelation(ptA,ptB,ptC,ptD);
			}
		}

	}
	else
	{
		//既然是弧线，四个点肯定都不重叠
		AcGePoint3d ptA1=ptA;
		AcGePoint3d ptB1=ptB;
		double bulgeAB1=bulgeAB;
		if (bulgeAB1 < 0.0)
		{
			AcGePoint3d tempPoint=ptA1;
			ptA1=ptB1;	
			ptB1=tempPoint;
			bulgeAB1 =bulgeAB1*(-1.0);
		}

		AcGeCircArc2d arcAB(convert3dPointTo2d(ptA1),convert3dPointTo2d(ptB1),bulgeAB1);


		AcGePoint3d ptC1=ptC;
		AcGePoint3d ptD1=ptD;
		double bulgeCD1=bulgeCD;
		if (bulgeCD1 < 0.0)
		{
			AcGePoint3d tempPoint=ptC1;
			ptC1=ptD1;	
			ptD1=tempPoint;
			bulgeCD1 =bulgeCD1*(-1.0);
		}

		AcGeCircArc2d arcCD(convert3dPointTo2d(ptC1),convert3dPointTo2d(ptD1),bulgeCD1);

		if(!IsSamePoint2d(arcAB.center(),arcCD.center()))
		{
			return 5;
		}
		else
		{
			return CalcRelation(arcAB.startAng(),arcAB.endAng(),arcCD.startAng(),arcCD.endAng());
		}

	}
}
//1<<1:完全重叠
//1<<2:AB压盖CD
//1<<3:CD压盖AB
//1<<4:部分重叠
//1<<5:完全不重叠
long CalcSegRelation(AcGePoint3dArray& pts1,AcArray<double>& bulges1,
					 AcGePoint3dArray& pts2,AcArray<double>& bulges2)
{
	long rc=0;
	for(int i=0;i<pts1.logicalLength()-1;i++)
	{
		for(int j=0;j<pts2.logicalLength()-1;j++)
		{
			if(pts1.at(i).distanceTo(pts1.at(i+1))<DistancePrecision||
				pts2.at(j).distanceTo(pts2.at(j+1))<DistancePrecision)
			{
				continue;
			}
			int tmp=CalcSegRelation(pts1.at(i),pts1.at(i+1),bulges1.at(i),pts2.at(j),pts2.at(j+1),bulges2.at(j));
			rc=rc|(1<<tmp);

		}
	}
	return rc;
}
bool IsPressByAB(AcGePoint3dArray& ptsAB,AcArray<double>& bulgesAB,const AcGePoint3d ptC,const AcGePoint3d ptD,const double bulgeCD)
{
	for(int i=0;i<ptsAB.logicalLength()-1;i++)
	{
		if(ptsAB.at(i).distanceTo(ptsAB.at(i+1))<DistancePrecision)
		{
			continue;
		}
		int tmp=CalcSegRelation(ptsAB.at(i),ptsAB.at(i+1),bulgesAB.at(i),ptC,ptD,bulgeCD);
		//1:完全重叠
		//2:AB压盖CD
		//3:CD压盖AB
		//4:部分重叠
		//5:完全不重叠
		if(tmp==1||tmp==2)
		{
			return true;
		}
	}
	return false;
}
//AB是否掩盖住CD
bool IsABPressCD(AcGePoint3dArray& ptsAB,AcArray<double>& bulgesAB,
				 AcGePoint3dArray& ptsCD,AcArray<double>& bulgesCD)
{
	for(int i=0;i<ptsCD.logicalLength()-1;i++)
	{
		if(ptsCD.at(i).distanceTo(ptsCD.at(i+1))<DistancePrecision)
		{
			continue;
		}
		if(!IsPressByAB(ptsAB,bulgesAB,ptsCD.at(i),ptsCD.at(i+1),bulgesCD.at(i)))
		{
			return false;
		}
	}
	return true;
}
//曲线重叠检测
void LineCross()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE,LINE,ARC"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择检测曲线:"),_T("\n移除检测曲线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()<2)
	{
		return;
	}
	AcGePoint3dArray* ptsArray=new AcGePoint3dArray[ids1.logicalLength()];
	AcArray<double>* bulgesArray=new AcArray<double>[ids1.logicalLength()];
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId objId=ids1.at(i);
		AcDbEntityPointer spEnt(objId,AcDb::kForRead);
		es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine=AcDbPolyline::cast(spEnt.object());
			for(int j=0;j<pLine->numVerts();j++)
			{
				AcGePoint3d pt;
				pLine->getPointAt(j,pt);
				double bulge=0.0;
				pLine->getBulgeAt(j,bulge);
				ptsArray[i].append(pt);
				bulgesArray[i].append(bulge);
			}
			if(pLine->isClosed())
			{
				AcGePoint3d pt0;
				pLine->getPointAt(0,pt0);
				ptsArray[i].append(pt0);
				bulgesArray[i].append(0.0);
			}
		}
		else if(spEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine=AcDbLine::cast(spEnt.object());
			AcGePoint3d ptStart;
			pLine->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pLine->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(0.0);
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);
		}
		else if(spEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc* pArc=AcDbArc::cast(spEnt.object());
			AcGePoint3d ptStart;
			pArc->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pArc->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(tan((pArc->endAngle()-pArc->startAngle())/4.0));
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);

		}

	}
	CArray<LineCrossResultItem> rcs;
	for(int i=0;i<ids1.logicalLength()-1;i++)
	{
		AcGePoint3dArray pts1=ptsArray[i];
		AcArray<double> bulges1=bulgesArray[i];
		if(pts1.logicalLength()<2)
		{
			continue;
		}
		for(int j=i+1;j<ids1.logicalLength();j++)
		{
			AcGePoint3dArray pts2=ptsArray[j];
			AcArray<double> bulges2=bulgesArray[j];
			if(pts2.logicalLength()<2)
			{
				continue;
			}
			LineCrossResultItem item;
			item.id1=ids1.at(i);
			item.id2=ids1.at(j);
			item.rc=CalcSegRelation(pts1,bulges1,pts2,bulges2);
			if(item.rc!=(1<<5))
			{
				rcs.Add(item);
			}

		}
	}
	delete[] ptsArray;
	delete[] bulgesArray;
	if(rcs.GetCount()==0)
	{
		ShowLineCrossReportPanel(FALSE);
		return;
	}
	ShowLineCrossReportPanel(TRUE);
	ShowLineCrossResult(rcs);
}
//删除完全重叠的线
void DeleteCrossLine()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LWPOLYLINE,LINE,ARC"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择检测曲线:"),_T("\n移除检测曲线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()<2)
	{
		return;
	}
	AcGePoint3dArray* ptsArray=new AcGePoint3dArray[ids1.logicalLength()];
	AcArray<double>* bulgesArray=new AcArray<double>[ids1.logicalLength()];
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId objId=ids1.at(i);
		AcDbEntityPointer spEnt(objId,AcDb::kForRead);
		es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine=AcDbPolyline::cast(spEnt.object());
			for(int j=0;j<pLine->numVerts();j++)
			{
				AcGePoint3d pt;
				pLine->getPointAt(j,pt);
				double bulge=0.0;
				pLine->getBulgeAt(j,bulge);
				ptsArray[i].append(pt);
				bulgesArray[i].append(bulge);
			}
			if(pLine->isClosed())
			{
				AcGePoint3d pt0;
				pLine->getPointAt(0,pt0);
				ptsArray[i].append(pt0);
				bulgesArray[i].append(0.0);
			}
		}
		else if(spEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine=AcDbLine::cast(spEnt.object());
			AcGePoint3d ptStart;
			pLine->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pLine->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(0.0);
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);
		}
		else if(spEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc* pArc=AcDbArc::cast(spEnt.object());
			AcGePoint3d ptStart;
			pArc->getStartPoint(ptStart);
			AcGePoint3d ptEnd;
			pArc->getEndPoint(ptEnd);
			ptsArray[i].append(ptStart);
			bulgesArray[i].append(tan((pArc->endAngle()-pArc->startAngle())/4.0));
			ptsArray[i].append(ptEnd);
			bulgesArray[i].append(0.0);

		}

	}
	AcDbObjectIdArray deleteIds;
	for(int i=0;i<ids1.logicalLength()-1;i++)
	{
		AcGePoint3dArray pts1=ptsArray[i];
		AcArray<double> bulges1=bulgesArray[i];
		if(pts1.logicalLength()<2)
		{
			continue;
		}
		for(int j=i+1;j<ids1.logicalLength();j++)
		{
			AcGePoint3dArray pts2=ptsArray[j];
			AcArray<double> bulges2=bulgesArray[j];
			if(pts2.logicalLength()<2)
			{
				continue;
			}
			LineCrossResultItem item;
			item.id1=ids1.at(i);
			item.id2=ids1.at(j);

			if(IsABPressCD(pts1,bulges1,pts2,bulges2))
			{
				if(GCADUtils::FindObjectId(deleteIds,ids1.at(j))<0)
				{
					deleteIds.append(ids1.at(j));
				}
			}
			else if(IsABPressCD(pts2,bulges2,pts1,bulges1))
			{
				if(GCADUtils::FindObjectId(deleteIds,ids1.at(i))<0)
				{
					deleteIds.append(ids1.at(i));
				}
			}

		}
	}
	delete[] ptsArray;
	delete[] bulgesArray;
	int nCount=deleteIds.logicalLength();
	for(int i=0;i<nCount;i++)
	{
		ForceEraseEntity(deleteIds.at(i));
	}
	if(nCount==0)
	{
		acutPrintf(_T("\n未找到任何完全重叠的线\n"));
	}
	else
	{
		acutPrintf(_T("\n删除完全重叠的线[%d]条\n"),nCount);
	}
}
//85:两点成线
//两个开口的线，你开发两个功能，一个是两点成线，一个是四个点成两条线
double GetMinDisPoint(int& nIndex,bool& bStart,AcGePoint2dArray& ptStartArray,AcGePoint2dArray& ptEndArray,AcDbObjectIdArray& lineIds,AcGePoint2d pt)
{
	double dMinDis=-1.0;
	for(int i=0;i<ptStartArray.logicalLength();i++)
	{
		AcGePoint2d ptStart=ptStartArray.at(i);
		double dis1=ptStart.distanceTo(pt);
		AcGePoint2d ptEnd=ptEndArray.at(i);
		double dis2=ptEnd.distanceTo(pt);
		if(dis1<dMinDis||dMinDis<0)
		{
			nIndex=i;
			bStart=true;
			dMinDis=dis1;
		}
		if(dis2<dMinDis||dMinDis<0)
		{
			nIndex=i;
			bStart=false;
			dMinDis=dis2;
		}

	}
	return dMinDis;
}
void _LDCX(bool bClosed)
{
	{
		CHECK_REG_VOID(_T("20140126"))
	}
	ads_point p1, p2;  //交互矩形点坐标获取
	if (acedGetPoint(NULL, _T("\n>>指定第一条线连接点或附近的点: "), p1) != RTNORM)
		return;

	if (acedGetPoint(p1, _T("\n>>指定第二条线连接点或附近的点: "), p2) != RTNORM) 
		return;

	if (p1[0] == p2[0] || p1[1] == p2[1]) //矩形框合法性检查
		return;
	GCADUtils::UCS2WCS(p1);
	GCADUtils::UCS2WCS(p2);
	AcGePoint2d pt1(p1[X],p1[Y]);
	AcGePoint2d pt2(p2[X],p2[Y]);
	double dis=pt1.distanceTo(pt2);
	AcDbObjectIdArray ids;
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
	{
		return;
	}
	EnumAllEntityInBTR(ids,pDb->currentSpaceId());
	AcGePoint2dArray ptStartArray;
	AcGePoint2dArray ptEndArray;
	AcDbObjectIdArray lineIds;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine=AcDbLine::cast(spEnt.object());
			ptStartArray.append(AcGePoint2d(pLine->startPoint().x,pLine->startPoint().y));
			ptEndArray.append(AcGePoint2d(pLine->endPoint().x,pLine->endPoint().y));
			lineIds.append(ids.at(i));
		}
		else if(spEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine=AcDbPolyline::cast(spEnt.object());
			if((!pLine->isClosed())&&pLine->numVerts()==2)
			{
				AcGePoint2d pt0,pt1;
				pLine->getPointAt(0,pt0);
				pLine->getPointAt(1,pt1);
				ptStartArray.append(pt0);
				ptEndArray.append(pt1);
			}
		}
	}
	int nIndex[2]={-1};
	bool bStart[2]={false};
	double dis1=GetMinDisPoint(nIndex[0],bStart[0],ptStartArray,ptEndArray,lineIds,pt1);
	if(dis1<0||dis1>dis)
	{
		acutPrintf(_T("\r\n未找到合理的和第一个点靠近的线\r\n"));
		return;
	}
	double dis2=GetMinDisPoint(nIndex[1],bStart[1],ptStartArray,ptEndArray,lineIds,pt2);
	if(dis2<0||dis2>dis)
	{
		acutPrintf(_T("\r\n未找到合理的和第而二个点靠近的线\r\n"));
		return;
	}
	if(nIndex[0]==nIndex[1])
	{
		acutPrintf(_T("\r\n距离两个点最近的线是同一个\r\n"));
		return;
	}
	AcDbPolyline* pLine=new AcDbPolyline();
	if(!bStart[0])
	{
		pLine->addVertexAt(0,ptStartArray.at(nIndex[0]));
		pLine->addVertexAt(1,ptEndArray.at(nIndex[0]));
	}
	else
	{
		pLine->addVertexAt(0,ptEndArray.at(nIndex[0]));
		pLine->addVertexAt(1,ptStartArray.at(nIndex[0]));
	}

	if(bStart[1])
	{
		pLine->addVertexAt(2,ptStartArray.at(nIndex[1]));
		pLine->addVertexAt(3,ptEndArray.at(nIndex[1]));
	}
	else
	{
		pLine->addVertexAt(2,ptEndArray.at(nIndex[1]));
		pLine->addVertexAt(3,ptStartArray.at(nIndex[1]));
	}
	AcDbEntityPointer spEnt1(ids.at(nIndex[0]),AcDb::kForRead);
	if(spEnt1.openStatus()!=Acad::eOk)
	{
		delete pLine;
		return;
	}
	pLine->setClosed(bClosed);
	SameEntityAttr(pLine,spEnt1.object());
	spEnt1->close();
	AddToCurrentSpace(pLine);
	pLine->close();
	ForceEraseEntity(ids.at(nIndex[0]));
	ForceEraseEntity(ids.at(nIndex[1]));


}
//85:两点成线
void LDCX()
{
	{
		CHECK_REG_VOID(_T("20140126"))
	}
	_LDCX(false);
}
//86:两点成四边形
void LDCSBX()
{
	{
		CHECK_REG_VOID(_T("20140126"))
	}
	_LDCX(true);
}
//将直线的交点加入交点数组，并按到起始点的距离从小到大排序
void AddIntersectPoint(AcGePoint3dArray* pts,AcDbLine* pLine,AcGePoint3d pt)
{
	if(IsSamePoint2d(pt,pLine->startPoint())||IsSamePoint2d(pt,pLine->endPoint()))
	{
		return;
	}
	double dis=pt.distanceTo(pLine->startPoint());
	for(int i=0;i<pts->logicalLength();i++)
	{
		double dis1=pts->at(i).distanceTo(pLine->startPoint());
		if(dis<dis1-1E-8)
		{
			//小于
			pts->insertAt(i,pt);
			return;
		}
		else if(dis>=dis1-1E-8&&dis<=dis1+1E-8)
		{
			//相等
			return;
		}
		else
		{
			//大于
		}
	}
	pts->append(pt);
}
//91:直线交点打断
void ZXJDDL()
{
	{
		CHECK_REG_VOID(_T("20140212"))
	}
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("LINE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择直线:"),_T("\n移除直线:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()<=1)
	{
		return;
	}
	//交点数组
	AcArray<AcGePoint3dArray*> intersectPts;
	for(int i=0;i<ids.logicalLength();i++)
	{
		intersectPts.append(new AcGePoint3dArray());
	}
	for(int i=0;i<ids.logicalLength()-1;i++)
	{
		AcDbObjectPointer<AcDbLine> spLine(ids.at(i),AcDb::kForRead);
		if(spLine.openStatus()!=Acad::eOk)
		{
			continue;
		}
		for(int j=i+1;j<ids.logicalLength();j++)
		{
			AcDbObjectPointer<AcDbLine> spLine1(ids.at(j),AcDb::kForRead);
			if(spLine1.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGePoint3dArray pnts;
			if(Acad::eOk!=spLine->intersectWith(spLine1.object(),AcDb::kOnBothOperands,AcGePlane::kXYPlane,pnts))
			{
				continue;
			}
			if(pnts.logicalLength()!=1)
			{
				continue;
			}
			AcGePoint3d pt=pnts.at(0);
			AddIntersectPoint(intersectPts.at(i),spLine.object(),pt);
			AddIntersectPoint(intersectPts.at(j),spLine1.object(),pt);
		}
	}
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcGePoint3dArray* pts=intersectPts.at(i);
		if(pts->logicalLength()==0)
		{
			delete pts;
			pts=NULL;
			intersectPts.setAt(i,NULL);
			continue;
		}
		AcDbEntityPointerEx<AcDbLine> spLine(ids.at(i),AcDb::kForWrite,false,true);
		if(spLine.openStatus()!=Acad::eOk)
		{
			delete pts;
			pts=NULL;
			intersectPts.setAt(i,NULL);
			continue;
		}
		pts->insertAt(0,spLine->startPoint());
		pts->append(spLine->endPoint());
		for(int j=0;j<pts->logicalLength()-1;j++)
		{
			AcDbLine* pNewLine=new AcDbLine();
			pNewLine->setStartPoint(pts->at(j));
			pNewLine->setEndPoint(pts->at(j+1));
			SameEntityAttr(pNewLine,spLine.object());
			AcDbObjectId id=AddToCurrentSpace(pNewLine);
			pNewLine->close();
		}
		spLine->erase();
	}
	intersectPts.setLogicalLength(0);
}
#include "GZYJXJig.h"
//103:自由矩形
void zyjx()
{
	{
		CHECK_REG_VOID(_T("20140309"))
	}
	ads_point p1;
	int rc=acedGetPoint(NULL,_T("\r\n选择第一个点"),p1);
	if(rc!=RTNORM)
	{
		return;
	}
	ads_point p2;
	rc=acedGetPoint(p1,_T("\r\n选择另一个点确定矩形的一条边"),p2);
	if(rc!=RTNORM)
	{
		return;
	}
	GCADUtils::UCS2WCS(p1);
	GCADUtils::UCS2WCS(p2);
	GZYJXJig jig;
	jig.m_pEnt->m_pt1=AcGePoint3d(p1[X],p1[Y],p1[Z]);
	jig.m_pEnt->m_pt2=AcGePoint3d(p2[X],p2[Y],p2[Z]);
	int ret=jig.Do();
	if(RTNORM==ret)
	{

	}
}
