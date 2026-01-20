#include "StdAfx.h"
#include "XdDbUtils.h"

//转换AcDbPolyline到AcGeCurve2d
Acad::ErrorStatus XdDbUtils::convertPolylineToGeCurve2d(AcDbPolyline* pPoly, AcGeCurve2d*& pGeCurve)
{
	AcGeLineSeg2d *pLine = NULL; 
	AcGeCircArc2d *pArc = NULL;
	AcGeVoidPointerArray GeCurves;

	for( int i = 0; i < pPoly->numVerts(); i++ )
	{
		if( pPoly->segType(i) == AcDbPolyline::kLine )
		{
			pLine = new AcGeLineSeg2d;
			pPoly->getLineSegAt(i, *pLine);
			GeCurves.append(pLine); 
		}
		else if( pPoly->segType(i) == AcDbPolyline::kArc )
		{
			pArc = new AcGeCircArc2d;
			pPoly->getArcSegAt(i, *pArc);
			GeCurves.append(pArc);
		}
	}

	pGeCurve = new AcGeCompositeCurve2d(GeCurves);

	return Acad::eOk;
}

//转换AcDbPolyline到AcGeCurve3d
Acad::ErrorStatus XdDbUtils::convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve3d*& pGeCurve)
{
	AcGeLineSeg3d *pLine = NULL; 
	AcGeCircArc3d *pArc = NULL;
	AcGeVoidPointerArray GeCurves;

	for( int i = 0; i < pPoly->numVerts(); i++ )
	{
		if( pPoly->segType(i) == AcDbPolyline::kLine )
		{
			pLine = new AcGeLineSeg3d;
			pPoly->getLineSegAt(i, *pLine);
			GeCurves.append(pLine); 
		}
		else if( pPoly->segType(i) == AcDbPolyline::kArc )
		{
			pArc = new AcGeCircArc3d;
			pPoly->getArcSegAt(i, *pArc);
			GeCurves.append(pArc);
		}
	}

	pGeCurve = new AcGeCompositeCurve3d(GeCurves);

	return Acad::eOk;
}

//转换AcGeCurve2d到AcDbPolyline
Acad::ErrorStatus XdDbUtils::convertGeCurveToPolyline(AcGeCurve2d* pCurve, AcDbPolyline*& pResultPoly)
{
	AcGeVoidPointerArray resultCurves;

	AcGeCompositeCurve2d* pResultCurve;
	AcGeCurve2d* pThisCurve;
	AcGeCircArc2d* pArc;
	AcGeLineSeg2d* pLine;
	AcGePoint2d endPt;

	int nCurves;
	double bulge, ang;

	if(pCurve->isKindOf(AcGe::kCompositeCrv2d))
	{
		pResultCurve = (AcGeCompositeCurve2d*)pCurve;
		pResultCurve->getCurveList(resultCurves );
	}
	else
	{
		resultCurves.append(pCurve);
	}

	nCurves = resultCurves.length();

	pResultPoly = new AcDbPolyline(nCurves);
	int i=0;
	for(; i < nCurves; i++)
	{
		pThisCurve = (AcGeCurve2d*)(resultCurves[i]);

		if(pThisCurve->isKindOf(AcGe::kCircArc2d))
		{
			pArc = (AcGeCircArc2d*)pThisCurve;
			bulge = 0.0;
			ang = 0.25 * (pArc->endAng() - pArc->startAng()); 
			bulge = tan(ang);
			if(pArc->isClockWise())
			{
				bulge = -bulge;
			}

			pResultPoly->addVertexAt(i, pArc->startPoint(), bulge);
		}
		else if(pThisCurve->isKindOf( AcGe::kLineSeg2d))
		{
			pLine = (AcGeLineSeg2d*)pThisCurve;
			pResultPoly->addVertexAt(i, pLine->startPoint(), 0 );
		}
	}// for
	if(pThisCurve->hasEndPoint(endPt))
	{
		pResultPoly->addVertexAt(i, endPt, 0);
	}
	pResultPoly->setClosed(pCurve->isClosed());
	return Acad::eOk;
}

//转换AcDbCurve到AcGeCurve3d
Acad::ErrorStatus XdDbUtils::convertDbCurveToGeCurve(AcDbCurve *pDbCurve,AcGeCurve3d *&pGeCurve)
{
	pGeCurve=NULL;
	if (pDbCurve->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *pL=(AcDbLine *)pDbCurve;
		AcGeLineSeg3d *pGL=new AcGeLineSeg3d;
		pGL->set(pL->startPoint(),pL->endPoint());
		pGeCurve=(AcGeCurve3d *)pGL;
	}
	else if (pDbCurve->isKindOf(AcDbArc::desc()))
	{
		AcDbArc *pArc=(AcDbArc *)pDbCurve;
		double ans,ane;
		ans=pArc->startAngle();
		ane=pArc->endAngle();
		AcGeCircArc3d *pGArc=new AcGeCircArc3d;
		pGArc->setCenter(pArc->center());
		pGArc->setRadius(pArc->radius());
		pGArc->setAngles(ans,ane);
		pGeCurve=(AcGeCurve3d *)pGArc;
	}
	else if (pDbCurve->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pCir=(AcDbCircle *)pDbCurve;
		AcGeCircArc3d * pGCir=new AcGeCircArc3d;
		pGCir->setCenter(pCir->center());
		pGCir->setRadius(pCir->radius());
		pGeCurve=(AcGeCurve3d *)pGCir;
	}
	else if (pDbCurve->isKindOf(AcDbEllipse::desc()))
	{
		AcDbEllipse *pEli=(AcDbEllipse *)pDbCurve;
		AcGePoint3d pt1,center=pEli->center();
		AcGeEllipArc3d *pGEli=new AcGeEllipArc3d;
		pGEli->setCenter(center);
		pGEli->setAxes(pEli->majorAxis(),pEli->minorAxis());
		pEli->getClosestPointTo(center,pt1,Adesk::kTrue);
		pGEli->setMajorRadius(pt1.distanceTo(center)/pEli->radiusRatio());
		pGEli->setMinorRadius(pt1.distanceTo(center));
		double endang=pEli->endAngle(),startang=pEli->startAngle();
		if (startang>endang){
			endang+=2*PI;
		}
		pGEli->setAngles(endang,startang);
		pGeCurve=(AcGeCurve3d *)pGEli;
	}
	else if (pDbCurve->isKindOf(AcDbSpline::desc()))
	{
		AcDbSpline *pSL=(AcDbSpline *)pDbCurve;
		if (!pSL)
			return Acad::eNotImplemented;
		if (pSL->isNull()==Adesk::kTrue)
			return Acad::eNotImplemented;

		int degree;
		Adesk::Boolean rational;
		Adesk::Boolean closed;
		Adesk::Boolean periodic;
		AcGePoint3dArray controlPoints;
		AcGeDoubleArray knots;
		AcGeDoubleArray weights;
		double controlPtTol;
		double knotTol;
		AcGeTol tol;
		Acad::ErrorStatus es;
		es=pSL->getNurbsData(degree,rational,closed,periodic,controlPoints,knots,weights,
			controlPtTol,knotTol);
		if (es!=Acad::eOk)
			return Acad::eNotImplemented;

		if (rational==Adesk::kTrue)
		{
			AcGeNurbCurve3d *pNurb=new AcGeNurbCurve3d(degree,knots,controlPoints,weights,periodic);
			if (closed==Adesk::kTrue)
				pNurb->makeClosed();
			if (pSL->hasFitData()==Adesk::kTrue)
			{
				AcGePoint3dArray fitPoints;
				double fitTolerance;
				Adesk::Boolean tangentsExist;
				AcGeVector3d startTangent;
				AcGeVector3d endTangent;
				pSL->getFitData(fitPoints,degree,fitTolerance,tangentsExist,startTangent,endTangent);
				tol.setEqualPoint(fitTolerance);
				if (tangentsExist==Adesk::kTrue)
					pNurb->setFitData(fitPoints,startTangent,endTangent,tol);
				else
					pNurb->setFitData(degree,fitPoints,tol);

			}
			pGeCurve=(AcGeCurve3d *)pNurb;
		}
		else
		{
			AcGeNurbCurve3d *pNurb=new AcGeNurbCurve3d(degree,knots,controlPoints,periodic);
			if (closed==Adesk::kTrue)
				pNurb->makeClosed();
			if (pSL->hasFitData()==Adesk::kTrue)
			{
				AcGePoint3dArray fitPoints;
				double fitTolerance;
				Adesk::Boolean tangentsExist;
				AcGeVector3d startTangent;
				AcGeVector3d endTangent;
				pSL->getFitData(fitPoints,degree,fitTolerance,tangentsExist,startTangent,endTangent);
				tol.setEqualPoint(fitTolerance);
				if (tangentsExist==Adesk::kTrue)
					pNurb->setFitData(fitPoints,startTangent,endTangent,tol);
				else
					pNurb->setFitData(degree,fitPoints,tol);

			}
			pGeCurve=(AcGeCurve3d *)pNurb;
		}
	}
	else if ((pDbCurve->isKindOf(AcDb2dPolyline::desc()))||
		(pDbCurve->isKindOf(AcDbPolyline::desc())))
	{
		int type=0;
		AcDbPolyline *pPoly;
		if (pDbCurve->isKindOf(AcDb2dPolyline::desc()))
		{
			AcDb2dPolyline *p2L=(AcDb2dPolyline *)pDbCurve;
			XdDbUtils::Poly2dToLWPoly(p2L,pPoly);
			type=1;
		}
		else 
			pPoly=(AcDbPolyline *)pDbCurve;
		XdDbUtils::convertPolylineToGeCurve(pPoly,pGeCurve);
		if (type)
			delete pPoly;
	}
	return (pGeCurve) ? Acad::eOk : Acad::eNotImplemented;
}

// convert AcDbLine to AcGeLineSeg3d
AcGeLineSeg3d* XdDbUtils::LineDb2GE(AcDbLine* pDbLine)
{
	return(new AcGeLineSeg3d(pDbLine->startPoint(), pDbLine->endPoint()));
}

// convert AcDbArc to AcGeCircArc3d
AcGeCircArc3d* XdDbUtils::ArcDb2Ge( AcDbArc* pDbArc)
{
	return(new AcGeCircArc3d(
		pDbArc->center(),
		pDbArc->normal(),
		pDbArc->normal().perpVector(),
		pDbArc->radius(),
		pDbArc->startAngle(),
		pDbArc->endAngle()));
}

// convert AcDbCircle to AcGeCircArc3d
AcGeCircArc3d* XdDbUtils::CircleDb2Ge(AcDbCircle* pDbCircle)
{
	return(new AcGeCircArc3d(
		pDbCircle->center(),
		pDbCircle->normal(),
		pDbCircle->radius()));

}

// convert AcDbSpline to AcGeNurbCurve3d
AcGeNurbCurve3d* XdDbUtils::SplineDb2Ge(AcDbSpline* pDbSpline)
{
	AcGeNurbCurve3d* pGeSpline;
	AcGePoint3dArray fitPoints;
	int degree;
	double fitTolerance;
	Adesk::Boolean tangentsExist;
	AcGeVector3d startTangent, endTangent;
	AcGeTol tol;

	Adesk::Boolean rational, closed, periodic;
	AcGePoint3dArray controlPoints;
	AcGeDoubleArray knots, weights;
	double controlPtTol, knotTol;

	if (pDbSpline->hasFitData()) {
		pDbSpline->getFitData(fitPoints, degree, fitTolerance,
			tangentsExist,startTangent, endTangent);

		tol.setEqualPoint(fitTolerance);
		pGeSpline=new AcGeNurbCurve3d(fitPoints, startTangent,
			endTangent, tangentsExist, tangentsExist,tol);		
	}else{
		pDbSpline->getNurbsData(degree, rational, closed, periodic,
			controlPoints, knots, weights, controlPtTol, knotTol);

		pGeSpline=new AcGeNurbCurve3d(degree, knots, controlPoints,
			weights, periodic);
		if (closed==Adesk::kTrue)
			pGeSpline->makeClosed();

	};
	return(pGeSpline);

}

// convert AcDbEllipse to AcGeEllipArc3d
AcGeEllipArc3d* XdDbUtils::EllipseDb2Ge(AcDbEllipse* pDbEllise)
{
	return(new AcGeEllipArc3d(
		pDbEllise->center(),
		pDbEllise->majorAxis(),
		pDbEllise->minorAxis(),
		pDbEllise->majorAxis().length(),
		pDbEllise->minorAxis().length(),
		pDbEllise->startAngle(),
		pDbEllise->endAngle()));
}


AcDbPolyline * AcGeCurveToAcDbCurve(const AcGeCompositeCurve3d * pGe)
{ 
	AcGePoint3d startPnt,endPnt;

	if( pGe->hasEndPoint(endPnt) == Adesk ::kFalse ||
		pGe->hasStartPoint(startPnt) == Adesk::kFalse)
	{
		return NULL;
	}

	//get the plane of Curve3d

	AcGePlane plane;
	AcGeLine3d line;
	AcGePoint3d p1,p2,p3;

	if(pGe->isPlanar(plane))
	{
		if(pGe->isLinear(line)) //Oh,it's a little tricky!
		{
			line.getPerpPlane(startPnt,plane); 
			plane.get(p1,p2,p3);
			plane.set(p2,p3-p2);
		}
		plane.get(p1,p2,p3);
	}
	else
	{
		return NULL;
	}

	//Creat a polyline
	AcDbPolyline *pPoly = new AcDbPolyline();
	AcGeVoidPointerArray curveList;
	pGe->getCurveList(curveList); //get all the segments
	AcGeCurve3d *pCurve = NULL;
	AcGeCircArc3d *pArc = NULL;

	int i;
	double b;
	AcGePoint2d pt;
	for(i = 0;i < curveList.length();i++)
	{
		pCurve = (AcGeCurve3d *) (curveList[i]);
		pCurve->hasStartPoint(startPnt);
		pt = startPnt.convert2d(plane);

		if (pCurve->isKindOf(AcGe::kCircArc3d))
		{
			pArc = (AcGeCircArc3d *)(pCurve);
			b = tan(0.25*(pArc->endAng() - pArc->startAng()));
			if (pArc->normal().z < 0.0)
			{
				pPoly->addVertexAt(i,pt,-b);
			}
			else
			{
				pPoly->addVertexAt(i,pt,b);
			}
		}
		else
		{
			pPoly->addVertexAt(i,pt);
		}
	}

	if(!pGe->isClosed())
	{
		pt = endPnt.convert2d(plane);
		pPoly->addVertexAt(i,pt);
	}
	else
	{
		pPoly->setClosed(Adesk::kTrue);
	}

	//the most important step;
	AcGeMatrix3d xform;
	AcGeVector3d XAxis = p1-p2;
	AcGeVector3d YAxis = p3-p2;
	AcGeVector3d ZAxis = XAxis.crossProduct(YAxis);
	xform.setCoordSystem(p2,XAxis,YAxis,ZAxis);
	pPoly->transformBy(xform);

	return pPoly;
}

void Sort(int nCurves, AcGeIntArray& isArcs, AcGePoint3dArray& Vertexes, AcGeDoubleArray& bulges)
{
	AcGeIntArray resIsArcs;
	AcGePoint3dArray resVertexes;
	AcGeDoubleArray resBulges;

	AcGePoint3d startPt,endPt;
	double bulge;
	int isArc;

	isArc = isArcs[0];
	startPt = Vertexes[0];
	endPt = Vertexes[1];
	bulge = bulges[0];

	resIsArcs.append(isArc);
	resVertexes.append(startPt);
	resVertexes.append(endPt);
	resBulges.append(bulge);

	isArcs.removeAt(0);
	Vertexes.removeAt(1);
	Vertexes.removeAt(0);
	bulges.removeAt(0);

	AcGeTol tol;
	// tol.setEqualPoint(1e-4);

	for (int i=0; i<nCurves-1; i++) 
	{
		// 剩余需要参与计算的curve数量
		int nCurvesLeft = isArcs.length();

		// 剩下的vertexes中距离当前endPoint最近一点的索引值
		int nNextCurve = 0;

		// 记录其他点与endPoint的距离，求出距离最短的一点，以便计算nNextCurve的值
		double dLength = 100;

		for (int j=0; j<nCurvesLeft; j++) 
		{
			// 起始点与endPoint的距离
			double dTempLength1 = (Vertexes[2*j]-endPt).length();
			// 终止点与endPoint的距离
			double dTempLength2 = (Vertexes[2*j+1]-endPt).length();

			if (dTempLength1 < dLength)
			{
				dLength = dTempLength1;
				nNextCurve = 2*j;

				if (dLength <= tol.equalPoint())
				{
					break;
				}
			}

			if (dTempLength2 < dLength)
			{
				dLength = dTempLength2;
				nNextCurve = 2*j+1;

				if (dLength <= tol.equalPoint())
				{
					break;
				}
			}
		}

		int nIndex = nNextCurve/2;
		if (nNextCurve % 2)
		{
			isArc = isArcs[nIndex];
			startPt = Vertexes[2*nIndex+1];
			endPt = Vertexes[2*nIndex];
			bulge = -bulges[nIndex];
		}
		else
		{
			isArc = isArcs[nIndex];
			startPt = Vertexes[2*nIndex];
			endPt = Vertexes[2*nIndex+1];
			bulge = bulges[nIndex];
		}

		// 保存结果
		resIsArcs.append(isArc);
		resVertexes.append(startPt);
		resVertexes.append(endPt);
		resBulges.append(bulge);

		// 计算出的curve不再参与下一次计算
		isArcs.removeAt(nIndex);
		Vertexes.removeAt(2*nIndex+1);
		Vertexes.removeAt(2*nIndex);
		bulges.removeAt(nIndex);
	}

	isArcs.setLogicalLength(0);
	Vertexes.setLogicalLength(0);
	bulges.setLogicalLength(0);

	isArcs.append(resIsArcs);
	Vertexes.append(resVertexes);
	bulges.append(resBulges);
}

//转换AcGeCompositeCurve3d到AcDbPolyline, This routine only called by GetRegionBoundaryPolyline
AcDbPolyline* XdDbUtils::convertGeCurveToPolyline(AcGeCompositeCurve3d* pCurve) 
{
	AcDbPolyline* pResultPolyline;
	AcGeCurve3d* pThisCurve;
	AcGeCircArc3d* pArc;
	AcGeLineSeg3d* pLine;
	AcGePoint3d startPt,endPt;

	if( pCurve->hasEndPoint(endPt) == Adesk ::kFalse ||
		pCurve->hasStartPoint(startPt) == Adesk::kFalse)
	{
		return NULL;
	}

	int nCurves,i,j;
	double bulge, ang;

	AcGeVoidPointerArray resultCurves;
	pCurve->getCurveList(resultCurves);

	bool bCannotConvert=false;
	nCurves = resultCurves.length();
	for(i=0; i<nCurves; i++)
	{
		pThisCurve = (AcGeCurve3d*)(resultCurves[i]);
		if (pThisCurve->isKindOf(AcGe::kSplineEnt3d) || pThisCurve->isKindOf(AcGe::kEllipArc3d))
		{
			bCannotConvert=true;
			break;
		}
	};
	if (bCannotConvert) 
	{
		for(i=0; i<nCurves; i++) 
			delete (AcGeCurve3d*)(resultCurves[i]);

		acedPrompt(_T("\nCan't Convert to Polyline."));
		return(NULL);
	};

	AcGeIntArray isArcs;
	AcGePoint3dArray Vertexes;//存放每一线段的起点和终点
	AcGeDoubleArray bulges;

	for(i=0; i<nCurves; i++)
	{
		pThisCurve = (AcGeCurve3d*)(resultCurves[i]);

		if(pThisCurve->isKindOf(AcGe::kCircArc3d))
		{
			pArc = (AcGeCircArc3d*)pThisCurve;
			isArcs.append(1);
			Vertexes.append(pArc->startPoint());
			Vertexes.append(pArc->endPoint());
			ang = 0.25 * (pArc->endAng() - pArc->startAng()); 
			bulge = tan(ang);
			bulges.append(bulge);
		}
		else if(pThisCurve->isKindOf( AcGe::kLineSeg3d))
		{
			pLine = (AcGeLineSeg3d*)pThisCurve; 
			isArcs.append(0);
			Vertexes.append(pLine->startPoint());
			Vertexes.append(pLine->endPoint());
			bulges.append(0.0);
		}
		else //else nothing, This routine only called by GetRegionBoundaryPolyline
		{
			assert(0);
		}
		
		//delete pThisCurve;//Ge对象不再有用，删掉
	}
	j=-1;
	for (i=0; i<nCurves; i++)
	{
		if (isArcs[i]==0)//找到第一条直线
		{
			j=i;
			break;
		}
	}

	// 重新排序
	Sort(nCurves, isArcs, Vertexes, bulges);

//	pResultPolyline = new AcDbPolyline(nCurves);
	pResultPolyline = new AcDbPolyline();

	bool bClockWise=false;
	if (j==-1) //polyline全部由arc构成
	{
		if (Vertexes[0]==Vertexes[3]) bClockWise=true;
		if (bClockWise)
		{
			for(i=0; i<nCurves; i++)
				pResultPolyline->addVertexAt(i, AcGePoint2d(Vertexes[2*i+1].x,Vertexes[2*i+1].y), -bulges[i]);
		}
		else
		{
			for(i=0; i<nCurves; i++)
				pResultPolyline->addVertexAt(i, AcGePoint2d(Vertexes[2*i].x,Vertexes[2*i].y), bulges[i]);
		}
	}
	else
	{
		for(i=0; i<nCurves; i++)
			pResultPolyline->addVertexAt(i, AcGePoint2d(Vertexes[2*i].x,Vertexes[2*i].y), bulges[i]);
	}

	pResultPolyline->setClosed(Adesk::kTrue);

	return(pResultPolyline);
}

//获取Region的边界PolyLines/Circles/Ellipses/Splines, 返回环的数目
int XdDbUtils::GetRegionBoundaryPolyline(AcDbRegion *pRegion, AcDbVoidPtrArray& pPolylines)
{
	AcDbVoidPtrArray subEntityArray;
	AcGeVoidPointerArray tmpGeCurves;
	int i, count=0;;

	if (pRegion->explode(subEntityArray)!=Acad::eOk)
	{
		for (i=0;i< subEntityArray.length();i++)
			delete (AcDbObject*)subEntityArray[i];//To XDSoft: 需要手工删除

		return 0; 
	}

	AcDbCurve* pDbCurve;

	for (i=0; i<subEntityArray.length(); i++)
	{
		pDbCurve=(AcDbCurve*)subEntityArray[i];
		if (pDbCurve->isClosed()) //this curve(Circle/Spline/Ellipse) is closed, then return the boundary(Circle/Spline/Ellipse)
		{
			pPolylines.append(pDbCurve);
			count++;
		}
		else
		{
			if(pDbCurve->isKindOf(AcDbLine::desc()))
				tmpGeCurves.append(LineDb2GE(AcDbLine::cast(pDbCurve)));
			else if(pDbCurve->isKindOf(AcDbArc::desc()))
				tmpGeCurves.append(ArcDb2Ge(AcDbArc::cast(pDbCurve)));
			else if(pDbCurve->isKindOf(AcDbSpline::desc()))
				tmpGeCurves.append(AcDbSpline::cast(pDbCurve));
			else if(pDbCurve->isKindOf(AcDbEllipse::desc()))
				tmpGeCurves.append(AcDbEllipse::cast(pDbCurve));
			else //else I don't know
			{
				assert(0);
			}
			
			delete pDbCurve;
		}
	};

	AcGeIntArray isOwnerOfCurves;
	AcGeCompositeCurve3d* pGeCompositeCurve;

	while (tmpGeCurves.length()>0)
	{
		isOwnerOfCurves.setLogicalLength(0);

		for (i=0;i< tmpGeCurves.length();i++)
			isOwnerOfCurves.append(1);

		pGeCompositeCurve = new AcGeCompositeCurve3d(tmpGeCurves,isOwnerOfCurves);

		if (pGeCompositeCurve==NULL)
		{
			for (i=0;i< tmpGeCurves.length();i++)
				delete (AcGeCurve3d*)tmpGeCurves[i];

			count=-count;//负数表示有部分边界被获得，但出错
			break;
		}

		AcDbPolyline *pPloyline = NULL;
		pPloyline=convertGeCurveToPolyline(pGeCompositeCurve);
		delete pGeCompositeCurve;//连带删除tmpGecurve

		if (pPloyline!=NULL)
		{
			pPolylines.append(pPloyline);
			count++;
		}

		break;
	}
	return(count);
}

Acad::ErrorStatus XdDbUtils::Poly2dToLWPoly(AcDb2dPolyline *pPoly,AcDbPolyline *& pLwpoly)
{
	if (!pPoly)
		return Acad::eInvalidInput; 

	Acad::ErrorStatus ret; 
	AcDb::Poly2dType type;
	type=pPoly->polyType();
	if ((type==AcDb::k2dSimplePoly)||(type==AcDb::k2dFitCurvePoly)){
		pLwpoly=new AcDbPolyline;
		ret=pLwpoly->convertFrom((AcDbEntity *&)pPoly,Adesk::kFalse);
		if (ret!=Acad::eOk){
			delete pLwpoly;
			pLwpoly=NULL;
			return Acad::eNotImplementedYet;
		}
	}
	else{
		return Acad::eNotImplementedYet;
	}
	return Acad::eOk;
} 

void XdDbUtils::SSToIds(ads_name ssName,AcDbObjectIdArray &EnIds)
{
	AcDbObjectId EntId;
	ads_name EntName;
#if _MSC_VER>=1900
	Adesk::Int32 nLength = 0;
#else
	long nLength = 0;
#endif
	acedSSLength(ssName,&nLength);
	for(int nLen=0;nLen<nLength;nLen++)
	{
		acedSSName(ssName,nLen,EntName);
		acdbGetObjectId(EntId,EntName);
		EnIds.append(EntId);
	}
}