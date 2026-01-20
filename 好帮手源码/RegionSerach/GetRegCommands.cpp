#include "StdAfx.h"
#include "Mainfunction.h"
#include "math.h"
#include "ol_errno.h"
#include "XdDbUtils.h"

#include "AcProgressBar.h"

#include "acestext.h"

extern double dOffSetX ;
extern double dOffSetY ;

class LockDocument
{
public:
	LockDocument()
	{
		pDoc=NULL;
		if(acDocManager!=NULL)
		{
			try
			{
				pDoc = acDocManager->curDocument();
				if(acDocManager->lockDocument(pDoc)!=Acad::eOk)
				{
					pDoc=NULL;
				}
			}
			catch(...)
			{}
		}
	}
	
	~LockDocument()
	{
		if(pDoc&&acDocManager)
		{
			try
			{
				acDocManager->unlockDocument(pDoc);	
			}
			catch(...)
			{}
		}
	}
	
private:
	AcApDocument* pDoc;
};

//-----------------------------------------------------------------------------
// This is command 'QYSS //区域搜索
//nType==0,创建多段线
//nType==1,创建填充
void _QYSS(int nType)
{
	CString strCurrentLayer;

	strCurrentLayer = GetCurrentLayerName();

	// TODO: Implement the command
	AcDbObjectIdArray dbObjIdArr;
	CString strLayer ;

	//得到要处理的对象
	GetAllLine(strLayer,dbObjIdArr);

	if(dbObjIdArr.logicalLength()>128)
	{
		acutPrintf(_T("\n选择的物体超过128个，请减少要处理的对象，程序退出\n"));
		return ;
	}

	AcProgressBar progressBar;
	progressBar.Create(_T("正在计算..."),100,0);

	progressBar.StepIt(5);

	OutputDebugString(_T("CopyAllLine"));

	AcDbObjectIdArray dbObjIdArrOut;
	//copy 所有的线到特定层
	CopyAllLine( dbObjIdArr,dbObjIdArrOut, "0");

	progressBar.StepIt(20);

	OutputDebugString(_T("CreateCrossLine"));

	//创建交叉的线
	AcDbObjectIdArray dbObjIdArrOutNew;
	CreateCrossLine(dbObjIdArrOut,dbObjIdArrOutNew);

	progressBar.StepIt(30);

	int rejectCount = 0;
	AcDbVoidPtrArray curveSegs;
	AcDbEntity* ent;
	Acad::ErrorStatus es;

	int len = dbObjIdArrOutNew.logicalLength();
	for (int i=0; i<len; i++) 
	{
		es = acdbOpenAcDbEntity(ent, dbObjIdArrOutNew[i], AcDb::kForRead);
		ent->close();    

		if (es == Acad::eOk) 
		{
			if (ent->isKindOf(AcDbCurve::desc()))
			{
				curveSegs.append(static_cast<void*>(ent));
			}
			else 
			{
				rejectCount++;
			}
		}
	}
	if (curveSegs.isEmpty()) 
	{
		return;
	}

	AcDbVoidPtrArray regions;
	regions.setPhysicalLength(0);

	progressBar.StepIt(40);

	OutputDebugString(_T("AcDbRegion::createFromCurves"));
	es = AcDbRegion::createFromCurves(curveSegs, regions);

	OutputDebugString(_T("RemoveEntity"));
	RemoveEntity(dbObjIdArrOutNew);

	progressBar.StepIt(60);

	if(regions.logicalLength()<1)
	{
		acutPrintf(_T("\n计算轮廓线失败，请确认"));
		return ;
	}

	// 将面域根据面积从小到大的顺序重新排序
	double regionArea = 0.0;
	int nIndex = 0;
	for (int i=0; i<regions.logicalLength()-1; i++) 
	{
		progressBar.StepIt(60 + 100 * i / regions.logicalLength());

		for (int j=i+1; j<regions.logicalLength(); j++) 
		{
			AcDbRegion *pReg1 = (AcDbRegion*)regions.at(i);

			if(NULL == pReg1)
			{
				acutPrintf(_T("\n计算轮廓线失败，请确认"));
				return ;
			}

			if(pReg1->isNull())
			{
				acutPrintf(_T("\n计算轮廓线失败，请确认"));
				return ;
			}

			AcDbRegion *pReg2 = (AcDbRegion*)regions.at(j);

			if(NULL == pReg2)
			{
				acutPrintf(_T("\n计算轮廓线失败，请确认"));
				return ;
			}

			if(pReg2->isNull())
			{
				acutPrintf(_T("\n计算轮廓线失败，请确认"));
				return ;
			}

			double dArea1, dArea2;
			pReg1->getArea(dArea1);
			pReg2->getArea(dArea2);

			if (dArea1 > dArea2)
			{
				regions.removeAt(j);
				regions.insertAt(j, pReg1);

				regions.removeAt(i);
				regions.insertAt(i, pReg2);
			}
		}

	}

	progressBar.Restore();

	RegionInputPointMonitor imp(true);

	imp.SetRegionArray(regions);

	while (1)
	{
		ads_point pt;

		if(acedGetPoint(NULL,_T("\n>>指定点<退出>: "),pt) != RTNORM)
		{
			return;
		}

		AcDbVoidPtrArray pPolylines;

		AcDbRegion* pHilightReg = imp.GetHilightReg();

		if (NULL == pHilightReg)
			continue;

		if (pHilightReg->isNull())
			continue;

		XdDbUtils::GetRegionBoundaryPolyline(pHilightReg, pPolylines);

		for (int i=0; i<pPolylines.logicalLength(); i++) 
		{
			AcDbEntity* pEnt = (AcDbEntity*)pPolylines.at(i);
			if(nType==0)
			{
				AcDbPolyline* pLine = (AcDbPolyline*)pEnt;

				pLine->setLayer(strCurrentLayer);

				AcDbObjectId objId = AddEntToDWGDatabase(pEnt);
				pEnt->close();
			}
			else
			{
				AcDbPolyline* pLine = (AcDbPolyline*)pEnt;
				AcDbHatch* pHatch=new AcDbHatch();
				AcGeVector3d normal(0.0,0.0,1.0);
				pHatch->setNormal(normal);
				pHatch->setElevation(0.0);
				pHatch->setLayer(strCurrentLayer);

				pHatch->setAssociative(Adesk::kFalse);
				pHatch->setPatternAngle(0.0);
				pHatch->setPatternScale(1.0);
				pHatch->setPattern(AcDbHatch::kPreDefined,_T("SOLID"));
				pHatch->setHatchStyle(AcDbHatch::kNormal);
				AcGePoint2dArray ptArr;
				AcGeDoubleArray bulgeArr;
				double      bulge;
				AcGePoint2d pt;
				for(int i=0;i<pLine->numVerts();i++)
				{
					pLine->getBulgeAt(i,bulge);
					pLine->getPointAt(i,pt);
					bulgeArr.append(bulge);
					ptArr.append(pt);
				}
				if(pLine->isClosed()&&ptArr.logicalLength()>0)
				{
					ptArr.append(ptArr.at(0));
					bulgeArr.append(0.0);
				}
				pHatch->appendLoop(AcDbHatch::kExternal, ptArr, bulgeArr);
				pHatch->evaluateHatch();
				AcDbObjectId objId = AddEntToDWGDatabase(pHatch);
				pHatch->close();
				delete pEnt;
				pEnt=NULL;
			}

			
		}

		continue;
	}

	for (int i=1; i<regions.logicalLength();i++) 
	{
		AcDbRegion *ppReg = (AcDbRegion*)regions.at(i);
		ppReg->close();
		delete ppReg;
	}

	regions.setPhysicalLength(0);
}
void QYSS()
{
	if(!CheckReg(_T("20131012")))
	{
		return;
	}
	_QYSS(0);
}
void BatchHatch()
{
	if(!CheckReg(_T("20131024")))
	{
		return;
	}
	_QYSS(1);
}

