#include "stdafx.h"
#include "MainFunction.h"


double dOffSetX = 0.0;
double dOffSetY = 0.0;


//得到所要组合的直线
BOOL GetAllLine(const CString &strLayer,AcDbObjectIdArray& dbObjIdArr)
{
	ads_name ssOut;

	struct resbuf * preb = NULL;
	preb = acutBuildList(-4, _T("<or"),
						 RTDXF0, _T("LINE"), 
						 RTDXF0, _T("LWPOLYLINE"),
						 RTDXF0, _T("ARC"),
						 -4, _T("or>"), 
						 0);

	//得到选择集
	if(acedSSGet(NULL, NULL, NULL, preb, ssOut)!=RTNORM)
	{
		//失败
		acedSSFree(ssOut);
		acutRelRb(preb);
		return FALSE;
	}
#if _MSC_VER>=1900
	Adesk::Int32 lNum = 0;
#else
	long lNum = 0;
#endif
	acedSSLength(ssOut,&lNum);

	AcDbObjectId dbObjId;
	ads_name ssTemp;

	for(int i=0 ;i < lNum; i++)
	{
		acedSSName(ssOut,i,ssTemp);
		acdbGetObjectId(dbObjId,ssTemp);
		dbObjIdArr.append(dbObjId);
	}


	acedSSFree(ssOut);
	acutRelRb(preb);
	return TRUE;

}

//copy 所有得到的直线，放到指定的临时层
void CopyAllLine(AcDbObjectIdArray& dbObjIdArr,AcDbObjectIdArray &arrObjIdOut, CString strLayer)
{

	AcDbDatabase * pdbDatab = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable			* pBlockTable = NULL;
	AcDbBlockTableRecord	* pBlockTableRecord = NULL;

	pdbDatab->getSymbolTable(pBlockTable, AcDb::kForRead);
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);


	long lNum = dbObjIdArr.logicalLength();
	AcDbObject *pdbObj = NULL;
	for(int i=0 ; i<lNum ;i++)
	{
		AcDbObjectId objId = dbObjIdArr.at(i);
		acdbOpenObject(pdbObj, objId, AcDb::kForWrite);

		if(pdbObj)
		{
			if(pdbObj->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline* pPolyLine = (AcDbPolyline*)pdbObj;

				if (pPolyLine->isClosed()) // 首尾点不重合
				{
					AcGePoint2d ptStart, ptEnd;
					pPolyLine->getPointAt(0, ptStart);
					pPolyLine->getPointAt(pPolyLine->numVerts()-1, ptEnd);

					if ((ptStart-ptEnd).length() < 1)
						pPolyLine->setPointAt(pPolyLine->numVerts()-1, ptStart);
				}
				else if (pPolyLine->numVerts() >= 4) // 自动闭合
				{
					AcGePoint2d ptStart, ptEnd;
					pPolyLine->getPointAt(0, ptStart);
					pPolyLine->getPointAt(pPolyLine->numVerts()-1, ptEnd);

					if ((ptStart-ptEnd).length() < 1)
						pPolyLine->setPointAt(pPolyLine->numVerts()-1, ptStart);
				}

				AcDbVoidPtrArray entitySet;
				if (pPolyLine->explode(entitySet) == Acad::eOk)
				{
					for(int j=0; j<entitySet.logicalLength();j++)
					{
						AcDbObjectId dbObjId = NULL;
						pBlockTableRecord->appendAcDbEntity(dbObjId,(AcDbEntity*)entitySet.at(j));
						AcDbEntity *pEntity = (AcDbEntity*)entitySet.at(j);

						arrObjIdOut.append(dbObjId);

						pEntity->setLayer(strLayer);
						pEntity->close();
//						pEntity->draw();
					}
				}
			}
			if(pdbObj->isKindOf(AcDbLine::desc()))
			{
				AcDbObjectId dbObjId;
				AcDbLine *pLine = new AcDbLine();

				AcGePoint3d strPt = ((AcDbLine*)pdbObj)->startPoint();
				AcGePoint3d endPt = ((AcDbLine*)pdbObj)->endPoint();

				pLine->setStartPoint(strPt);
				pLine->setEndPoint(endPt);

				pBlockTableRecord->appendAcDbEntity(dbObjId,pLine);
				arrObjIdOut.append(dbObjId);
			
				pLine->setLayer(strLayer);
				
				pLine->close();
//				pLine->draw();
			}
			else if(pdbObj->isKindOf(AcDbArc::desc()))
			{
				AcDbObjectId dbObjId;

				double dStAngle  =  ((AcDbArc*)pdbObj)->startAngle();
				double dEndAngle =  ((AcDbArc*)pdbObj)->endAngle();
				AcGePoint3d PtCenter = ((AcDbArc*)pdbObj)->center();
				double dRadius = ((AcDbArc*)pdbObj)->radius();

				AcDbArc *pArc = new AcDbArc(PtCenter,
											dRadius,
											dStAngle,
											dEndAngle
											);


				pBlockTableRecord->appendAcDbEntity(dbObjId,pArc);
				
				arrObjIdOut.append(dbObjId);
				pArc->setLayer(strLayer);
				
				pArc->close();
//				pArc->draw();
			}
		}
		pdbObj->close();
	}

	pBlockTable->close();
	pBlockTableRecord->close();
}



BOOL CrossPTLine(AcGePoint3d &gePoint,AcDbObjectIdArray &arrObjId)
{

	struct resbuf * prebFilter   =NULL;
	struct resbuf * prebPointList=NULL;

	long lNumVer = 0;
	ads_point *ppt = new ads_point[5];

	ppt[0][X] = gePoint.x + 0.005;
	ppt[0][Y] = gePoint.y + 0.005 ;
	ppt[0][Z] = 0.00;

	ppt[1][X] = gePoint.x + 0.005;
	ppt[1][Y] = gePoint.y - 0.005 ;
	ppt[1][Z] = 0.00;

	ppt[2][X] = gePoint.x - 0.005;
	ppt[2][Y] = gePoint.y - 0.005 ;
	ppt[2][Z] = 0.00;

	ppt[3][X] = gePoint.x - 0.005;
	ppt[3][Y] = gePoint.y + 0.005 ;
	ppt[3][Z] = 0.00;
	
	ppt[4][X] = gePoint.x + 0.005;
	ppt[4][Y] = gePoint.y + 0.005 ;
	ppt[4][Z] = 0.00;


	//创建选择条件
	/////////////////////////////////////////////////////////////
	struct resbuf* prebNext=NULL;
	struct resbuf* prebPrev=NULL;
	prebPointList = acutBuildList(RTPOINT,ppt[0],0);
	prebPrev=prebPointList;
	ASSERT(prebPrev!=NULL);
	int i=1;
	for(; i<5-1; i++)
	{	
		prebNext = NULL;
		prebNext = acutBuildList(RTPOINT,ppt[i],0);
		ASSERT(prebNext!=NULL);
		prebPrev->rbnext= prebNext;
		prebPrev = prebNext;		
	}

	prebNext = NULL;
	prebNext = acutBuildList(RTPOINT,ppt[lNumVer-1],0);
	ASSERT(prebNext!=NULL);
	prebPrev->rbnext = prebNext;
	////////////////////////////////////////////////////////////////////

	prebFilter = acutBuildList(-4, "<or",0);
	prebPrev=prebFilter;
	ASSERT(prebPrev!=NULL);

	int iLayNum = 1;
	for(i=0; i<iLayNum; i++)
	{	
		prebNext = NULL;
		prebNext = acutBuildList(8,"规整图层",0);
		ASSERT(prebNext!=NULL);
		prebPrev->rbnext= prebNext;
		prebPrev = prebNext;		
	}
	prebNext = NULL;
	prebNext = acutBuildList(-4,_T("or>"),0);
	ASSERT(prebNext!=NULL);
	prebPrev->rbnext = prebNext;

	ads_name ssOut;
	ads_name ssName;

	if(acedSSGet(_T("f"),prebPointList,NULL,prebFilter,ssOut)!=RTNORM)
	{
		delete []ppt;
		ppt = NULL;

		acutRelRb(prebPointList);
		acutRelRb(prebFilter);
		acedSSFree(ssOut);
		return false;
	}
	else 
	{
		delete []ppt;
		ppt = NULL;
		acutRelRb(prebPointList);
		acutRelRb(prebFilter);

#if _MSC_VER>=1900
		Adesk::Int32 lNum = 0;
#else
		long lNum = 0 ;
#endif
		acedSSLength(ssOut,&lNum);
		if(lNum <=1)
		{
			acedSSFree(ssOut);
			return FALSE;
		}
		else
		{
			AcDbObjectId dbObjId;
			
			for(int i=0 ;i<lNum;i++)
			{
				acedSSName(ssOut,i,ssName);
				acdbGetObjectId(dbObjId,ssName);
				arrObjId.append(dbObjId);
			}
			acedSSFree(ssOut);
			return TRUE;
		}
	}
}

void CreateCrossLine(AcDbObjectIdArray&dbObjIdArrOut,
					 AcDbObjectIdArray&dbObjIdArrOutNew)
{
	AcDbObject *pdbObj = NULL;

	int nObjNum = dbObjIdArrOut.logicalLength();
	dbObjIdArrOutNew = dbObjIdArrOut;

	AcDbObjectIdArray dbIdRemove;
	int i=0;
	for( ;i<nObjNum ;i++)
	{
		acdbOpenObject(pdbObj,dbObjIdArrOut.at(i),AcDb::kForRead);
		pdbObj->close();

		CreateTempLine(pdbObj,dbObjIdArrOut,dbObjIdArrOutNew,dbIdRemove);
	}

	nObjNum = dbIdRemove.logicalLength();
	for(i=0 ;i<nObjNum;i++)
	{
		acdbOpenObject(pdbObj,dbIdRemove.at(i),AcDb::kForWrite);
		if(pdbObj)
		{
			pdbObj->erase();
			pdbObj->close();
		}
	}

	
}


//创建临时的线。
void CreateTempLine(AcDbObject*pdbObj,
					AcDbObjectIdArray& entity,//in
					AcDbObjectIdArray &dbIdArray,//out
					AcDbObjectIdArray&dbIdRemove)
{
	 if(pdbObj->isKindOf(AcDbLine::desc()))
	 {
		AcGePoint3dArray crossPointarr;

		AcDbObjectId dbObjId;
		AcDbObject   *pdbObjTemp;
		long lNum = entity.logicalLength();

		for(int i=0 ;i<lNum ;i++)
		{
			AcGePoint3dArray points;

			acdbOpenObject(pdbObjTemp,entity.at(i),AcDb::kForRead);
			pdbObjTemp->close();
			if(pdbObjTemp)
			{
				if(pdbObjTemp == pdbObj)
				{
					continue;
				}

				((AcDbEntity*)pdbObj)->intersectWith((AcDbEntity*)pdbObjTemp,
													  AcDb::kOnBothOperands,
													  points);

				crossPointarr.append(points);
			}
		}

		RemoveAllVertPoint(pdbObj,crossPointarr);
		CreateFromPoints(pdbObj,dbIdArray,crossPointarr,dbIdRemove);
	}
	 else if(pdbObj->isKindOf(AcDbArc::desc()))
	 {
		AcGePoint3dArray crossPointarr;

		AcDbObjectId dbObjId;
		AcDbObject   *pdbObjTemp;
		long lNum = entity.logicalLength();
		for(int i=0 ;i<lNum ;i++)
		{
			AcGePoint3dArray points;

			acdbOpenObject(pdbObjTemp,entity.at(i),AcDb::kForRead);
			pdbObjTemp->close();
			if(pdbObjTemp)
			{
				if(pdbObjTemp == pdbObj)
				{
					continue;
				}

				((AcDbEntity*)pdbObj)->intersectWith((AcDbEntity*)pdbObjTemp,
													  AcDb::kOnBothOperands,
													  points);
				crossPointarr.append(points);
			}
		}

		RemoveAllVertPoint(pdbObj,crossPointarr);
		CreateArcForPoints(pdbObj,dbIdArray,crossPointarr,dbIdRemove);
	 }
}

//去除所有的端点
void RemoveAllVertPoint(AcDbObject*pdbObj,AcGePoint3dArray&insertPoints)
{
	AcGeTol tol;
	tol.setEqualPoint(1e-4);

	if(pdbObj->isKindOf(AcDbLine::desc()))
	{
		AcGePoint3d gePoi3d ;
		AcGePoint3d gePoi3d1 ;
		gePoi3d = ((AcDbLine*)pdbObj)->startPoint();
		gePoi3d1 = ((AcDbLine*)pdbObj)->endPoint();
		
		for(int i=0;i<insertPoints.logicalLength();i++)
		{
			if (((insertPoints.at(i)-gePoi3d).length() <= tol.equalPoint()) ||
				((insertPoints.at(i)-gePoi3d1).length() <= tol.equalPoint()))
			{
				insertPoints.removeAt(i);
				i--;
			}
		}
	}
	else
	{
		AcGePoint3d gePoi3d ;
		AcGePoint3d gePoi3d1 ;

		GetArcStAndEdPt((AcDbArc*)pdbObj,gePoi3d,gePoi3d1);

		
		for(int i=0;i<insertPoints.logicalLength();i++)
		{
			if (((insertPoints.at(i)-gePoi3d).length() <= tol.equalPoint()) ||
				((insertPoints.at(i)-gePoi3d1).length() <= tol.equalPoint()))
			{
				insertPoints.removeAt(i);
				i--;
			}
		}
	}
}


AcDbObjectId CreateDistArc(double dStAngle,double dEndAngle,double dRadius,AcGePoint3d &gePoiCenter)
{

	AcDbDatabase * pDbDatab = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId			DbOId=NULL;
	AcDbBlockTable			* pDbBT;
	AcDbBlockTableRecord	* pDbBTR;
	AcDbEntity				* pDbEn;
	
	pDbDatab->getSymbolTable(pDbBT, AcDb::kForRead);
	pDbBT->getAt(ACDB_MODEL_SPACE, pDbBTR, AcDb::kForWrite);
	pDbBT->close();


	pDbEn = new AcDbArc(gePoiCenter,
						dRadius,
						dStAngle,
						dEndAngle);

	pDbBTR->appendAcDbEntity(DbOId,pDbEn);
	pDbBTR->close();

	

	pDbEn->setLayer(_T("规整图层"));
	pDbEn->close();
//	pDbEn->draw();

	return DbOId;

}
void CreateArcForPoints(AcDbObject*pdbObj,
						AcDbObjectIdArray &dbIdArray,
						AcGePoint3dArray &gePoi3dArr,
						AcDbObjectIdArray &dbIdRemove)
{
	int nNum = gePoi3dArr.logicalLength();

	if(nNum == 1)
	{
		AcDbObjectId dbObjId;

		AcGePoint3d gePoi3d;
		gePoi3d = gePoi3dArr.at(0);


		double dStAngle = ((AcDbArc*)pdbObj)->startAngle();
		double dEdAngle = ((AcDbArc*)pdbObj)->endAngle();

		AcGePoint3d gePointCenter = ((AcDbArc*)pdbObj)->center();
		double dRadius = ((AcDbArc*)pdbObj)->radius();

		//计算角度
		ads_point pt1,pt2;
		pt1[X] = gePointCenter.x;
		pt1[Y] = gePointCenter.y;
		pt1[Z] = 0.0;
		
		pt2[X] = gePoi3d.x;
		pt2[Y] = gePoi3d.y;
		pt2[Z] = 0.0;
		double dAngle = acutAngle(pt1,pt2);

		dbObjId = CreateDistArc(dStAngle,dAngle,dRadius,gePointCenter);
		dbIdArray.append(dbObjId);

		dbObjId = CreateDistArc(dAngle,dEdAngle,dRadius,gePointCenter);
		dbIdArray.append(dbObjId);


		dbObjId = pdbObj->objectId();
		dbIdArray.remove(dbObjId);
		dbIdRemove.append(dbObjId);
	}
	else
	{
		if(nNum<1)
			return;

		AcArray<double > AngleArr;
		ads_point pt1,pt2;
		AcGePoint3d gePointCenter = ((AcDbArc*)pdbObj)->center();

		double dStAngle = ((AcDbArc*)pdbObj)->startAngle();
		double dEdAngle = ((AcDbArc*)pdbObj)->endAngle();
		double dRadius = ((AcDbArc*)pdbObj)->radius();
	
		if (dStAngle > dEdAngle)
			dStAngle -=PI*2;

		pt1[X] = gePointCenter.x;
		pt1[Y] = gePointCenter.y;
		pt1[Z] = 0.0;

		double dAngle =0.0;

		for(int i=0; i<nNum;i++)
		{
			pt2[X] = gePoi3dArr.at(i).x;
			pt2[Y] = gePoi3dArr.at(i).y;
			pt2[Z] = 0.0;

			dAngle = acutAngle(pt1,pt2);

			if (dAngle >dEdAngle)
				dAngle -= PI*2;

			AngleArr.append(dAngle);
		}

		SortByAngle(dStAngle,AngleArr);

		AcDbObjectId dbObjId;
		dbObjId = CreateDistArc(dStAngle,AngleArr.at(0),dRadius,gePointCenter);
		dbIdArray.append(dbObjId);


		for(int j=0 ;j<nNum-1;j++)
		{
			dbObjId = CreateDistArc(AngleArr.at(j),AngleArr.at(j+1),dRadius,gePointCenter);
			dbIdArray.append(dbObjId);

		}

		dbObjId = CreateDistArc(AngleArr.at(nNum-1),dEdAngle,dRadius,gePointCenter);
		dbIdArray.append(dbObjId);

		dbObjId = pdbObj->objectId();
		dbIdArray.remove(dbObjId);
		dbIdRemove.append(dbObjId);
	}

}

void SortByAngle(double dStAngle,AcArray<double> &AngleArr)
{
	int nPtNum = AngleArr.logicalLength();
	for(int i=nPtNum ;i>1;i--)
	{
		BubbleByAngle(dStAngle,AngleArr,i);
	}
}

//创建直线
void CreateFromPoints(AcDbObject*pdbObj,
					  AcDbObjectIdArray &dbIdArray,
					  AcGePoint3dArray &gePoi3dArr,
					  AcDbObjectIdArray &dbIdRemove)
{
	int Num = gePoi3dArr.logicalLength();

	if(Num == 1)
	{
		AcDbObjectId dbObjId;

		AcGePoint3d gePoi3d ;
		gePoi3d = gePoi3dArr.at(0);

		AcGePoint3d gePoi3d1 = ((AcDbLine*)pdbObj)->startPoint();

		dbObjId= 	CreateDistLine(gePoi3d,((AcDbLine*)pdbObj)->startPoint());
		dbIdArray.append(dbObjId);

		gePoi3d1 = ((AcDbLine*)pdbObj)->endPoint();
		
		dbObjId= 	CreateDistLine(gePoi3d,((AcDbLine*)pdbObj)->endPoint());
		dbIdArray.append(dbObjId);


		dbObjId = pdbObj->objectId();
		dbIdArray.remove(dbObjId);
		//
		dbIdRemove.append(dbObjId);
	}

	else
	{

		if(Num<1)
			return ;

		AcGePoint3d stPt = ((AcDbLine*)pdbObj)->startPoint();
		SortByDistance(stPt,gePoi3dArr);

		AcGePoint3d gePoi3d;
		gePoi3d = gePoi3dArr.at(0);

		AcDbObjectId dbObjId;
		
		dbObjId= 	CreateDistLine(stPt,gePoi3d);
		
		dbIdArray.append(dbObjId);


		for(int i=0;i<Num-1;i++)
		{
			AcDbObjectId dbObjId;
			AcGePoint3d gePoi3d1;
			AcGePoint3d gePoi3d2;
			gePoi3d1 = gePoi3dArr.at(i);
			gePoi3d2 = gePoi3dArr.at(i+1);

			dbObjId= 	CreateDistLine(gePoi3d1,gePoi3d2);

			dbIdArray.append(dbObjId);
		}

		gePoi3d = gePoi3dArr.at(Num-1);
		AcGePoint3d edPt = ((AcDbLine*)pdbObj)->endPoint();
		dbObjId= 	CreateDistLine(gePoi3d,edPt);

		dbIdArray.append(dbObjId);


		dbObjId = pdbObj->objectId();
		dbIdArray.remove(dbObjId);
		dbIdRemove.append(dbObjId);
	}

}


void BubbleByAngle(double dStAngle,
				   AcArray<double> &AngArr
				   ,int n)
{
	for(int i=0 ;i<n-1;i++)
	{
		if(fabs((AngArr.at(i)-dStAngle))-fabs((AngArr.at(i+1)-dStAngle))
			> 0.000001)
		{
			AngArr.swap(i,i+1);
		}
	}


}
void Bubble(AcGePoint3d &stPt,
			AcGePoint3dArray &gePoi3dArr,
			int n)
{
	for(int i=0 ;i<n-1;i++)
	{
		if((pow((gePoi3dArr.at(i).x-stPt.x),2)+pow((gePoi3dArr.at(i).y-stPt.y),2))
			-(pow((gePoi3dArr.at(i+1).x-stPt.x),2)+pow((gePoi3dArr.at(i+1).y-stPt.y),2))>0.00001)
		{
			gePoi3dArr.swap(i,i+1);
		}
	}
}
void SortByDistance(AcGePoint3d &stPt,
					AcGePoint3dArray&gePoi3dArr)
					//AcGePoint3dArray&gePoi3dArrSort)
{
	

	int nPtNum = gePoi3dArr.logicalLength();
	for(int i=nPtNum ;i>1;i--)
	{
		Bubble(stPt,gePoi3dArr,i);
	}
	
	
}

AcDbObjectId CreateDistLine(AcGePoint3d &gePoi3d,AcGePoint3d&gePoi3d1)
{

	AcDbDatabase * pDbDatab = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId			DbOId=NULL;
	AcDbBlockTable			* pDbBT;
	AcDbBlockTableRecord	* pDbBTR;
	AcDbEntity				* pDbEn;
	
	pDbDatab->getSymbolTable(pDbBT, AcDb::kForRead);
	pDbBT->getAt(ACDB_MODEL_SPACE, pDbBTR, AcDb::kForWrite);
	pDbBT->close();


	pDbEn = new AcDbLine();
	((AcDbLine*)pDbEn)->setStartPoint(gePoi3d);
	((AcDbLine*)pDbEn)->setEndPoint(gePoi3d1);

	pDbBTR->appendAcDbEntity(DbOId,pDbEn);
	pDbBTR->close();

	pDbEn->setLayer(_T("规整图层"));
	pDbEn->close();
//	pDbEn->draw();

	return DbOId;


}

BOOL GetArcStAndEdPt(AcDbArc*pdbArc,AcGePoint3d &geStPt,AcGePoint3d &geEdPt)
{

	if(pdbArc)
	{
		if(pdbArc->isKindOf(AcDbArc::desc()))
		{
			double dStAngle = pdbArc->startAngle();
			double dEdAngle = pdbArc->endAngle();
			double dRadius  = pdbArc->radius();

			AcGePoint3d gePtCenter = pdbArc->center();

			geStPt.x = gePtCenter.x+dRadius*cos(dStAngle);
			geStPt.y = gePtCenter.y+dRadius*sin(dStAngle);

			geEdPt.x = gePtCenter.x+dRadius*cos(dEdAngle);
			geEdPt.y = gePtCenter.y+dRadius*sin(dEdAngle);

			return TRUE;

		}
		else
		{
			return FALSE;
		}

	}
	else
	{
		return FALSE;
	}
}


void RemoveEntity(AcDbObjectIdArray&dbObjIdArrOutNew)
{
	for(int nObjNum =0; nObjNum<dbObjIdArrOutNew.logicalLength();nObjNum++)
	{
		AcDbObjectId dbObj;
		AcDbObject *pdbObj;
		acdbOpenObject(pdbObj,dbObjIdArrOutNew.at(nObjNum),AcDb::kForWrite);
		if(pdbObj)
		{
			pdbObj->erase();
			pdbObj->close();
		}
	}

}

void Check2dPoint(AcDbObject *pDbO,AcDbDatabase *pDbDatab)
{
	if (!pDbO->isKindOf(AcDbPolyline::desc())) 
	{
		return ;
	}

	long				lCount=0;
	AcGePoint3d			GePoi3d1,GePoi3d2;
	
	lCount=((AcDbPolyline *)pDbO)->numVerts();
	if (lCount<2) 
	{
		return ;
	}

	for (long lPnt=0;lPnt<lCount-1;lPnt++) 
	{
		((AcDbPolyline *)pDbO)->getPointAt(lPnt,GePoi3d1);
		for(int j=lPnt+1;j<lCount;j++)
		{
			((AcDbPolyline *)pDbO)->getPointAt(j,GePoi3d2);
			if (fabs(GePoi3d1.x - GePoi3d2.x) <  0.000001&& 
				fabs(GePoi3d1.y - GePoi3d2.y) <  0.000001&& 
				fabs(GePoi3d1.z - GePoi3d2.z) < 0.000001) 
			{
				pDbO->upgradeOpen();
				
				if (lCount<=2) 
				{
					pDbO->erase();
					pDbO->downgradeOpen();
					break;
				}
				else
				{
					((AcDbPolyline *)pDbO)->removeVertexAt(j);
				}
				pDbO->downgradeOpen();
				lCount--;
				j--;
			}
		}
	}
}

CString GetCurrentLayerName()
{
	CString strLayer ;
	TCHAR * cLayer = NULL;

	AcDbLayerTableRecord *pLayerTblRcd = NULL;

	if ( Acad::eOk == acdbOpenObject (pLayerTblRcd, acdbHostApplicationServices()->workingDatabase()->clayer (),AcDb::kForWrite) )
	{ 
		pLayerTblRcd->getName (cLayer) ;
		pLayerTblRcd->setIsFrozen(false);
		pLayerTblRcd->setIsLocked(false);
		pLayerTblRcd->setIsOff(false);
		pLayerTblRcd->close () ;

		strLayer = cLayer;
		free (cLayer) ; 
	} 

	return strLayer;

}