#include "StdAfx.h"
#include "AcCommFunction.h"
//获取样条曲线的拟合点
bool getSplineSamplePoints(AcDbSpline *&spline, AcGePoint3dArray &pnts)
{
	assert(spline != NULL);
	AcGePoint3d orgPt;
	spline->getStartPoint(orgPt);
	pnts.append(orgPt);
	double dStartParam=0.0;
	spline->getStartParam(dStartParam);
	double dEntParam=0.0;
	spline->getEndParam(dEntParam);
	double dDist=0.0;
	spline->getDistAtParam(dEntParam,dDist);
	int nNum=(int)dDist/0.005;
	if(nNum<25)
	{
		nNum=25;
	}
	if(nNum>10000)
	{
		nNum=10000;
	}
	for(int i=0;i<nNum;i++)
	{
		double dTmpParam=0.0;
		spline->getParamAtDist(dDist*(i+1)/nNum,dTmpParam);
		spline->getPointAtParam(dTmpParam,orgPt);
		pnts.append(orgPt);
	}
	return pnts.logicalLength()>2;
}
AcGePoint2d convert3dPointTo2d(AcGePoint3d pt)
{
	AcGePoint2d pm;
	pm.set(pt.x,pt.y);
	return pm;
}
//获取三个点构成的夹角
double GetIntersectionAngle(AcGePoint2d pt1,AcGePoint2d pt2,AcGePoint2d pt3)
{
	AcGeVector2d vt1=pt1-pt2;
	AcGeVector2d vt2=pt3-pt2;
	return vt1.angleTo(vt2);
}
//获取三个点构成的夹角
double GetIntersectionAngle(AcGePoint3d pt1,AcGePoint3d pt2,AcGePoint3d pt3)
{
	return GetIntersectionAngle(convert3dPointTo2d(pt1),convert3dPointTo2d(pt2),convert3dPointTo2d(pt3));
}
//删除构成直线的三个点中间的点
//dMinAngle,构成的夹角最小值,默认为0.5度
void RemovePointBetweenLine(AcGePoint3dArray &pnts,double dMinAngle)
{
	AcGePoint3dArray rcPts;
	if(pnts.logicalLength()<=2)
	{
		return;
	}
	rcPts.append(pnts.at(0));
	rcPts.append(pnts.at(1));
	for(int i=2;i<pnts.logicalLength();i++)
	{
		AcGePoint3d pt1=rcPts.at(rcPts.logicalLength()-2);
		AcGePoint3d pt2=rcPts.at(rcPts.logicalLength()-1);
		AcGePoint3d pt3=pnts.at(i);
		double dAngle=GetIntersectionAngle(pt1,pt2,pt3);
		if(dAngle>=dMinAngle&&dAngle<=PI)
		{
			rcPts.setAt(rcPts.logicalLength()-1,pt3);
		}
		else
		{
			rcPts.append(pt3);
		}
	}
	pnts.setLogicalLength(0);
	pnts.append(rcPts);
}
// 功能：添加实体到块表记录中
// 参数：	  pEnt：待添加的实体指针
//			  btrID,AcDbBlockTableRecord的ID		
// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
AcDbObjectId AppendEntity(AcDbEntity* pEnt,const AcDbObjectId btrID)
{
	AcDbObjectId resultId;
	resultId.setNull();

	AcDbBlockTableRecord* pBlkRec = NULL;//打开表
	Acad::ErrorStatus es = acdbOpenObject(pBlkRec, btrID, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开模型空间失败！%s"),acadErrorStatusText(es));
		return resultId;
	}

	es = pBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
	if (es != Acad::eOk) 
	{
		acutPrintf(_T("\n 添加对象到模型空间失败！%s"),acadErrorStatusText(es));
		pBlkRec->close();
		return resultId;
	}
	es=pBlkRec->close();

	return resultId;
}
// 功能：添加实体到数据库中
// 参数：	  pEnt：待添加的实体指针
//			  pDB：接收实体的数据库		
// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
AcDbObjectId AddToCurrentSpace(AcDbEntity* pEnt,AcDbDatabase* pDB)
{
	AcDbObjectId resultId;
	resultId.setNull();
	if(pDB==NULL)
		return resultId;

	AcDbBlockTableRecord* pBlkRec = NULL;//打开表
	Acad::ErrorStatus es = acdbOpenObject(pBlkRec, pDB->currentSpaceId(), AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开模型空间失败！%s"),acadErrorStatusText(es));
		return resultId;
	}

	es = pBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
	if (es != Acad::eOk) 
	{
		acutPrintf(_T("\n 添加对象到模型空间失败！%s"),acadErrorStatusText(es));
		pBlkRec->close();
		return resultId;
	}
	es=pBlkRec->close();

	return resultId;
}
//功能:	激活当前视图
Acad::ErrorStatus activeCurrentview()
{
	Acad::ErrorStatus es=Acad::eOk;

	if(acedGetAcadFrame()!=NULL)
	{
		CFrameWnd	*pFrame;
		pFrame	= acedGetAcadFrame()->GetActiveFrame();

		if(pFrame != NULL)
		{
			CView	*pView;
			pView	= pFrame->GetActiveView();
			if(pView != NULL)
				pView->SetFocus();
			else
				es=Acad::eNotApplicable;
		}
		else
			es=Acad::eNotApplicable;
	}
	else
		es=Acad::eNotApplicable;

	return es;
}
//获取当前图层
Acad::ErrorStatus getCurrentLayer(CString &strLayerName)
{
	Acad::ErrorStatus es=Acad::eOk;
	resbuf rb;
	acedGetVar(_T("clayer"), &rb);
	strLayerName=rb.resval.rstring;
	return es;
}
//获取图层的ID
Acad::ErrorStatus GetLayerId(AcDbObjectId& layerId,const CString strLayerName,AcDbDatabase* pDb)
{
	CString lyName=strLayerName;
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable*     pTable=NULL;
	Acad::ErrorStatus es=pDb->getSymbolTable(pTable,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	bool bHave=pTable->has(lyName.GetBuffer());
	lyName.ReleaseBuffer();
	if(!bHave)
	{
		pTable->close();
		return Acad::eNotInDatabase;
	}
	es=pTable->getAt(lyName.GetBuffer(),layerId);
	lyName.ReleaseBuffer();
	pTable->close();
	return es;
}
//获取图层名称
Acad::ErrorStatus GetLayerName(CString& strLayerName,AcDbObjectId layerId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord* pRec=AcDbLayerTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	const TCHAR* pName=NULL;
	es=pRec->getName(pName);
	if(es!=Acad::eOk)
	{
		pObj->close();
		return es;
	}
	strLayerName=pName;
	pObj->close();
	return Acad::eOk;
}
//获取实体图层ID
Acad::ErrorStatus GetEntityLayerId(AcDbObjectId& layerId,AcDbObjectId entId)
{
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	layerId=pEnt->layerId();
	pEnt->close();
	return Acad::eOk;
}
CString strMText=_T("");
int EnumMText(AcDbMTextFragment * pFragment, void *pData)
{
	strMText+=pFragment->text;
	return 0;
}
//获取多行文字中的文字内容
CString GetMText(AcDbMText* pText)
{
	strMText=_T("");
	pText->explodeFragments(EnumMText,pText,NULL);
	return strMText;
}
//设置为相同的属性
//将pTargetEnt设置为和pSourceEnt设置为一样的属性
void SameEntityAttr(AcDbEntity* pTargetEnt,AcDbEntity* pSourceEnt)
{
	pTargetEnt->setColor(pSourceEnt->color());
	pTargetEnt->setLayer(pSourceEnt->layer());
	pTargetEnt->setLinetype(pSourceEnt->linetype());
	pTargetEnt->setLineWeight(pSourceEnt->lineWeight());
#if _MSC_VER>=1400
	pTargetEnt->setMaterial(pSourceEnt->materialId());
#endif
}
Acad::ErrorStatus CreatePolylineRgn(AcDbRegion*& pRegion,AcGePoint3dArray& pts)
{
	AcDbPolyline* pLine=new AcDbPolyline(pts.logicalLength());
	for(int i=0;i<pts.logicalLength();i++)
	{
		pLine->addVertexAt(i,convert3dPointTo2d(pts.at(i)));
	}
	pLine->setClosed(Adesk::kTrue);
	AcDbVoidPtrArray curveSegments;
	curveSegments.append(pLine);
	AcDbVoidPtrArray regions;
	Acad::ErrorStatus es=AcDbRegion::createFromCurves(curveSegments,regions);
	delete pLine;
	pLine=NULL;
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(regions.logicalLength()==0)
	{
		return Acad::eInvalidInput;
	}
	pRegion=(AcDbRegion*)regions.at(0);
	return Acad::eOk;

}
// Function name : RgnInRgn
// Descrīption : is Region1 in Region2?
// Return type : bool
// Argument : const AcDbRegion* pRegion1
// Argument : const AcDbRegion* pRegion2
bool RgnInRgn(const AcDbRegion* pRegion1,const AcDbRegion* pRegion2)
{
	if (pRegion1==NULL||pRegion2==NULL) return false;

	AcDbObjectPointer< AcDbRegion > spRegion1;
	AcDbObjectPointer< AcDbRegion > spRegion2;
	if (spRegion1.create()!=Acad::eOk)
	{
		acdbFail(_T("\n内存不足"));
		return false;
	}
	if (spRegion2.create()!=Acad::eOk)
	{
		acdbFail(_T("\n内存不足"));
		return false;
	}

	if ((spRegion1->copyFrom(pRegion1)!= Acad::eOk)||
		(spRegion2->copyFrom(pRegion2)!= Acad::eOk))
	{
		acdbFail(_T("\n无法复制对象"));
		return false;
	}

	bool bResult=false;
	if(spRegion1->booleanOper(AcDb::kBoolIntersect, spRegion2.object()) == Acad::eOk)
	{
		if ((spRegion2->isNull()==Adesk::kTrue)&&(spRegion1->isNull()!=Adesk::kTrue)){
			double area1,area0;
			spRegion1->getArea(area1);//相交部分的大小
			pRegion1->getArea(area0);//Region1大小

			if ((area0 - area1) < AcGeContext::gTol.equalPoint())
			{
				bResult=true;
			}
		}
	}

	return bResult;
}
// Function name : RgnInRgn
// Descrīption : is Region1 in Region2?
// Return type : bool
//              dArea,返回重叠部分的面积
// Argument : const AcDbRegion* pRegion1
// Argument : const AcDbRegion* pRegion2
bool RgnIntersect(double& dArea,const AcDbRegion* pRegion1,const AcDbRegion* pRegion2)
{
	if (pRegion1==NULL||pRegion2==NULL) return false;

	AcDbObjectPointer< AcDbRegion > spRegion1;
	AcDbObjectPointer< AcDbRegion > spRegion2;
	if (spRegion1.create()!=Acad::eOk)
	{
		acdbFail(_T("\n内存不足"));
		return false;
	}
	if (spRegion2.create()!=Acad::eOk)
	{
		acdbFail(_T("\n内存不足"));
		return false;
	}

	if ((spRegion1->copyFrom(pRegion1)!= Acad::eOk)||
		(spRegion2->copyFrom(pRegion2)!= Acad::eOk))
	{
		acdbFail(_T("\n无法复制对象"));
		return false;
	}

	bool bResult=false;
	if(spRegion1->booleanOper(AcDb::kBoolIntersect, spRegion2.object()) == Acad::eOk)
	{
		if ((spRegion2->isNull()==Adesk::kTrue)&&(spRegion1->isNull()!=Adesk::kTrue))
		{
			bResult=true;
			double area1;
			spRegion1->getArea(area1);//相交部分的大小
			dArea=area1;
		}
	}

	return bResult;
}
bool RgnIntersect(double& dScale,AcGePoint3dArray& pts1,AcGePoint3dArray& pts2)
{
	AcDbRegion* pRegion1=NULL;
	Acad::ErrorStatus es=CreatePolylineRgn(pRegion1,pts1);
	if(es!=Acad::eOk)
		return false;
	AcDbRegion* pRegion2=NULL;
	es=CreatePolylineRgn(pRegion2,pts2);
	if(es!=Acad::eOk)
	{
		pRegion1->close();
		return false;
	}
	double dArea=0.0;
	bool rc=RgnIntersect(dArea,pRegion1,pRegion2);
	if(!rc)
	{
		pRegion1->close();
		pRegion2->close();
		return rc;
	}
	double area1;
	double area2;
	pRegion1->getArea(area1);
	pRegion2->getArea(area2);
	pRegion1->close();
	pRegion2->close();
	dScale=max(dArea/area1,dArea/area2);
	return true;

}
//设置实体的选中状态
//参数：ObjId,实体的ObjectId
//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
void SetEntitySelected(AcDbObjectId ObjId,bool Clear)
{
	AcDbEntityPointer spEnt(ObjId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId realId=ObjId;
	if(spEnt->isKindOf(AcDbAttribute::desc()))
	{
		realId=spEnt->ownerId();
	}
	ads_name tempselect;
	ads_name tempss;
	int es1=0;
	Acad::ErrorStatus es2=acdbGetAdsName(tempss,realId);
	if(es2!=Acad::eOk)
	{
		acutPrintf(_T("实体转换成选择集失败\n"));
		acedSSFree(tempselect);
		acedSSFree(tempss);
		return;
	}
	es1=acedSSAdd(tempss,NULL,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("将实体加入选择集失败\n"));
		acedSSFree(tempselect);
		acedSSFree(tempss);
		return;
	}
	acedSSFree(tempss);//加入成功后，tempss必须释放
	es1=acedSSSetFirst(tempselect,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("设置当前选择集失败\n"));
		acedSSFree(tempselect);
		return;
	}
	acedUpdateDisplay();
	acedSSFree(tempselect);
}
//设置实体为选中状态
void SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear)
{
	ads_name tempselect;
	ads_name tempss;
	int es1=0;
	Acad::ErrorStatus es2=Acad::eOk;
	for(int i=0;i<ObjIds.logicalLength();i++)
	{
		AcDbObjectId ObjId=ObjIds.at(i);
		es2=acdbGetAdsName(tempss,ObjId);
		if(es2!=Acad::eOk)
		{
			acutPrintf(_T("实体转换成选择集失败\n"));
			es1=acedSSFree(tempselect);
			es1=acedSSFree(tempss);
			return;
		}
		es1=acedSSAdd(tempss,i==0?NULL:tempselect,tempselect);
		if(es1!=RTNORM)
		{
			acutPrintf(_T("将实体加入选择集失败\n"));
			es1=acedSSFree(tempselect);
			es1=acedSSFree(tempss);
			return;
		}
		es1=acedSSFree(tempss);
	}
	es1=acedSSSetFirst(tempselect,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("设置当前选择集失败\n"));
		es1=acedSSFree(tempselect);
		return;
	}
	acedUpdateDisplay();
	es1=acedSSFree(tempselect);
}
//功能:	设置视图（相当于Zoom Window命令）
//参数:	Pt1		视图左上角点
//			Pt2		视图右下角点
//			ex_ratio	扩展比率,一般为1.0
Acad::ErrorStatus view_set(AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio/*=1.0*/)
{
	Acad::ErrorStatus es=Acad::eOk;

	AcGePoint2d CenterPt;
	if ((fabs(Pt1.x-Pt2.x)<1e-6)||(fabs(Pt1.y-Pt2.y)<1e-6))
		return Acad::eInvalidExtents;//若X坐标或Y坐标重合，判为意外，不进行操作

	if (Pt1.x>Pt2.x) 
	{//确保两个坐标点分别为左上角和右下角
		double tmp;
		tmp=Pt1.x;
		Pt1.x=Pt2.x;
		Pt2.x=tmp;
	}

	if (Pt2.y>Pt1.y) 
	{
		double tmp;
		tmp=Pt1.y;
		Pt1.y=Pt2.y;
		Pt2.y=tmp;
	}

	//获取当前DwgView的尺寸
	CRect CADrect;
	acedGetAcadDwgView()->GetClientRect(&CADrect);
	double width,height,ratio;
	ratio=(double)(CADrect.right-CADrect.left)/(double)(CADrect.bottom-CADrect.top);
	if (fabs(ratio)<1e-6)
		return Acad::eInvalidExtents;

	if ((Pt2.x-Pt1.x)/(Pt1.y-Pt2.y)>ratio) 
	{
		width=Pt2.x-Pt1.x;
		height=width/ratio;
	}
	else
	{
		height=Pt1.y-Pt2.y;
		width=height * ratio;
	}

	//设置当前视图中心点
	CenterPt.x=(Pt1.x+Pt2.x)/2;
	CenterPt.y=(Pt1.y+Pt2.y)/2;

	//改变当前视图
	AcDbViewTableRecord pVwRec;
	pVwRec.setCenterPoint(CenterPt);
	pVwRec.setWidth(width*ex_ratio);
	pVwRec.setHeight(height*ex_ratio);

	es=acedSetCurrentView(&pVwRec,NULL);
	return es;
}
bool GetMaxExtPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax)
{
	bool bHasEntity=false;
	int nCount = ObjIdArray.length();
	for(int k=0; k<nCount; k++)
	{
		AcDbObjectId EntId;
		EntId = ObjIdArray[k];
		AcDbEntity *pEnt=NULL;
		AcDbExtents ex;
		if(acdbOpenObject(pEnt,EntId,AcDb::kForRead)==Acad::eOk)
		{
			Acad::ErrorStatus es=pEnt->getGeomExtents(ex);
			pEnt->close();
			pEnt=NULL;
			if(es!=Acad::eOk)
			{
				continue;
			}

			AcGePoint3d tempMax;
			AcGePoint3d tempMin;
			if (!bHasEntity)
			{
				tempMax = ex.maxPoint();
				ptMax.x = tempMax.x;
				ptMax.y = tempMax.y;
				ptMax.z = tempMax.z;

				tempMin = ex.minPoint();
				ptMin.x = tempMin.x;
				ptMin.y = tempMin.y;
				ptMin.z = tempMin.z;

			}
			else
			{
				tempMax = ex.maxPoint();
				if(tempMax.x > ptMax.x)
					ptMax.x = tempMax.x;
				if(tempMax.y > ptMax.y)
					ptMax.y = tempMax.y;
				if(tempMax.z > ptMax.z)
					ptMax.z = tempMax.z;

				tempMin = ex.minPoint();
				if(tempMin.x < ptMin.x)
					ptMin.x = tempMin.x;
				if(tempMin.y < ptMin.y)
					ptMin.y = tempMin.y;
				if(tempMin.z < ptMin.z)
					ptMin.z = tempMin.z;
			}
			bHasEntity=true;
		}
	}
	return bHasEntity;
}
bool DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio)
{
	AcGePoint3d ptMin,ptMax;

	bool rc=GetMaxExtPoint(ObjIdArray,ptMin,ptMax);
	if(ptMax.x-ptMin.x<1E-2)
	{
		double max_tmp=max(ptMax.y-ptMin.y,20);
		ptMax.x=ptMax.x+max_tmp*0.2;
		ptMin.x=ptMin.x-max_tmp*0.2;
	}
	else if(ptMax.y-ptMin.y<1E-2)
	{
		double max_tmp=max(ptMax.x-ptMin.x,20);
		ptMax.y=ptMax.y+max_tmp*0.2;
		ptMin.y=ptMin.y-max_tmp*0.2;
	}
	if(!rc)
		return rc;
	return view_set(ptMin,ptMax,ex_ratio)==Acad::eOk;

}
bool DyZoom(const AcDbObjectId ObjId,double ex_ratio)
{
	AcDbObjectIdArray ObjIdArray;
	ObjIdArray.append(ObjId);
	return DyZoom(ObjIdArray,ex_ratio);
}
void GetEntVertPntsAndBlgs(AcDbObjectId CurId,AcGePoint3dArray &VertPnts,AcGeDoubleArray &VertBlgs)
{
	AcGePoint3dArray ptArray;
	AcGeDoubleArray ptBlgs;
	double dStartAngle;
	double dEndAngle;
	AcDbObjectId VertId;
	AcGeVector3d vF;
	AcGePoint3d CirPt;
	double dRadius=0.0;
	AcGePoint3d ptCenter;
	double dStartBulge=0.0;
	double dEndBulge=0.0;
	AcDbCurve *pCur=NULL;
	int nLength;
	AcGePoint3d ptAt;
	AcGePoint3d ptStart;
	AcGePoint3d ptEnd;
	double dBulge=0;
	AcDb2dVertex* pVert=NULL ;
	double dAngle;
	if(acdbOpenObject(pCur,CurId,AcDb::kForRead)!=Acad::eOk)
		return  ;
	if(pCur->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *pLine=AcDbLine::cast(pCur);
		VertPnts.append(pLine->startPoint());
		VertPnts.append(pLine->endPoint());
		VertBlgs.append(0);
		VertBlgs.append(0);
		pLine->close();
		pLine=NULL;
	}
	if(pCur->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline *pLine=AcDbPolyline::cast(pCur);
		nLength=pLine->numVerts();
		for(int nVert=0;nVert<nLength;nVert++)
		{
			pLine->getPointAt(nVert,ptAt);
			pLine->getBulgeAt(nVert,dBulge);
			VertPnts.append(ptAt);
			VertBlgs.append(dBulge);

		}
		pLine->close();
		pLine=NULL;

	}
	if(pCur->isKindOf(AcDb2dPolyline::desc()))
	{

		AcDb2dPolyline *pLine=AcDb2dPolyline::cast(pCur);
		AcDbObjectIterator *pIter=pLine->vertexIterator();
		for(pIter->start();!pIter->done();pIter->step())
		{

			VertId=pIter->objectId();
			if(acdbOpenObject(pVert,VertId,AcDb::kForRead)==Acad::eOk)
			{
				ptAt=pVert->position();
				dBulge=pVert->bulge();
				VertPnts.append(ptAt);
				VertBlgs.append(dBulge);
				pVert->close();
				pVert=NULL;
			}

		}
		pLine->close();
		delete pIter;

	}
	if(pCur->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pArc=AcDbCircle::cast(pCur);
		pArc->getStartPoint(ptStart);
		ptCenter=pArc->center();
		ptEnd=ptCenter+(ptCenter-ptStart);
		dAngle=2*PI;
		dAngle=(dAngle)/4;
		dBulge= tan(dAngle);
		vF=pArc->normal();
		VertPnts.append(ptStart);
		VertPnts.append(ptEnd);

		VertBlgs.append(1);
		VertBlgs.append(1);
		pArc->close();

	}
	else if(pCur->isKindOf(AcDbArc::desc()))
	{
		AcDbArc *pArc=AcDbArc::cast(pCur);
		pArc->getStartPoint(ptStart);
		pArc->getEndPoint(ptEnd);
		dStartAngle=pArc->startAngle();
		dEndAngle=pArc->endAngle();
		dAngle=dEndAngle-dStartAngle;
		if(dAngle<0)
			dAngle=2*PI+dAngle;
		dAngle=(dAngle)/4;
		dBulge= tan(dAngle);
		vF=pArc->normal();
		VertPnts.append(ptStart);
		VertPnts.append(ptEnd);

		VertBlgs.append(dBulge);
		VertBlgs.append(0);
		pArc->close();
	}
	if(pCur->isClosed())
	{
		if(VertPnts.logicalLength()>0)
		{
			VertPnts.append(VertPnts[0]);
			VertBlgs.append(0);
		}

	}
	pCur->close();
	pCur=NULL;

}

BOOL JudgeCureIsSelInterSect(const AcGePoint3dArray &VertPnts,const AcGeDoubleArray &ptBlgs,AcGePoint2dArray &ptBacks) //判断一
{
	ptBacks.setLogicalLength(0);
	AcGePoint2dArray ptArr;
	for(int nLen=0;nLen<VertPnts.logicalLength();nLen++)
	{
		ptArr.append(AcGePoint2d(VertPnts[nLen].x,VertPnts[nLen].y));
	}
	if(ptArr.logicalLength()<2)
		return FALSE;
	AcGePoint2d ptAt;
	AcGeTol tol;
	tol.setEqualPoint(0.00001);
	for(int i=0;i<ptArr.logicalLength()-2;i++)
	{
		if(fabs(ptBlgs[i])<0.00001)
		{
			AcGeLineSeg2d line(ptArr[i],ptArr[i+1]);
			for(int j=i+1;j<ptArr.logicalLength()-1;j++)
			{
				if(fabs(ptBlgs[j])<0.00001)
				{
					AcGeLineSeg2d line1(ptArr[j],ptArr[j+1]);
					if(line.intersectWith(line1,ptAt)==Adesk::kTrue)
					{
						if((ptAt.isEqualTo(ptArr[i],tol)||
							ptAt.isEqualTo(ptArr[i+1],tol))&&
							(ptAt.isEqualTo(ptArr[j],tol)||
							ptAt.isEqualTo(ptArr[j+1],tol)))
							continue;
						ptBacks.append(ptAt);
						//	return TRUE;
					}
				}
				else
				{
					int nNum;
					AcGePoint2d pt1;
					AcGePoint2d pt2;
					AcGeCircArc2d cir1(ptArr[j],ptArr[j+1],ptBlgs[j]);
					if(cir1.intersectWith(line,nNum,pt1,pt2)==Adesk::kTrue)
					{
						if(nNum==1)
						{
							if((pt1.isEqualTo(ptArr[i],tol)||
								pt1.isEqualTo(ptArr[i+1],tol))&&
								(pt1.isEqualTo(ptArr[j],tol)||
								pt1.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							//	return TRUE;
						}
						else
						{
							if((pt2.isEqualTo(ptArr[i],tol)||
								pt2.isEqualTo(ptArr[i+1],tol))&&
								(pt2.isEqualTo(ptArr[j],tol)||
								pt2.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							//	return TRUE;
						}
					}
				} 
			}
		}

		else //圆狐
		{
			AcGeCircArc2d arc(ptArr[i],ptArr[i+1],ptBlgs[i]);
			for(int j=i+1;j<ptArr.logicalLength()-1;j++)
			{
				if(fabs(ptBlgs[j])<0.00001)
				{
					AcGeLineSeg2d line1(ptArr[j],ptArr[j+1]);
					int nNum;
					AcGePoint2d pt1;
					AcGePoint2d pt2;
					if(arc.intersectWith(line1,nNum,pt1,pt2)==Adesk::kTrue)
					{
						if(nNum==1)
						{
							if((pt1.isEqualTo(ptArr[i],tol)||
								pt1.isEqualTo(ptArr[i+1],tol))&&
								(pt1.isEqualTo(ptArr[j],tol)||
								pt1.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							//	return TRUE;
						}
						else
						{
							if((pt2.isEqualTo(ptArr[i],tol)||
								pt2.isEqualTo(ptArr[i+1],tol))&&
								(pt2.isEqualTo(ptArr[j],tol)||
								pt2.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							//	return TRUE;
						}
					}
				}
				else
				{
					int nNum;
					AcGePoint2d pt1;
					AcGePoint2d pt2;
					AcGeCircArc2d arc1(ptArr[j],ptArr[j+1],ptBlgs[j]);
					if(arc.intersectWith(arc1,nNum,pt1,pt2)==Adesk::kTrue)
					{
						if(nNum==1)
						{
							if((pt1.isEqualTo(ptArr[i],tol)||
								pt1.isEqualTo(ptArr[i+1],tol))&&
								(pt1.isEqualTo(ptArr[j],tol)||
								pt1.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							acutPrintf(_T("\n%0.3f,%0.3f"),pt1.x,pt1.y);
							//	return TRUE;
						}
						else
						{
							if((pt2.isEqualTo(ptArr[i],tol)||
								pt2.isEqualTo(ptArr[i+1],tol))&&
								(pt2.isEqualTo(ptArr[j],tol)||
								pt2.isEqualTo(ptArr[j+1],tol)))
								continue;
							ptBacks.append(ptAt);
							//	return TRUE;
						}
					}
				} 
			}
		}
	}
	if(ptBacks.logicalLength()>0)
		return TRUE;
	return FALSE;
}
Acad::ErrorStatus DyZoom(AcGePoint2d& CenterPt,double dHeight,double dWidth)
{
	AcDbDatabase* pCurDB = acdbHostApplicationServices()->workingDatabase();
	if(pCurDB == NULL)
		return Acad::eNullHandle;

	Acad::ErrorStatus es;
	AcDbViewTableRecord *pView = new AcDbViewTableRecord();
	pView->setCenterPoint(CenterPt);
	pView->setFrontClipAtEye(false);
	pView->setHeight(dHeight);
	pView->setWidth(dWidth);
	es=acedSetCurrentView(pView,NULL);
	pView->close();
	delete pView;
	return es;
}
//枚举图层
Acad::ErrorStatus EnumAllLayer(AcDbObjectIdArray& layeIds,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbObjectId id;
		es=pLayerTableIterator->getRecordId(id);
		if(es!=Acad::eOk)
			continue;
		layeIds.append(id);
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
//枚举图层
Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		const ACHAR* pName=NULL;
		es=pLayerTableRecord->getName(pName);
		if(es!=Acad::eOk)
		{
			pLayerTableRecord->close();
			continue;
		}
		CString strName=pName;
		layeNames.Add(strName);
		//关闭(切记)
		pLayerTableRecord->close();
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
//枚举图层
Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbObjectIdArray& layeIds,AcDbDatabase* pDb)
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable *pLayerTable=NULL;
	Acad::ErrorStatus es=Acad::eOk;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk )
	{
		pLayerTable->close () ;
		return es;
	}

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		const ACHAR* pName=NULL;
		es=pLayerTableRecord->getName(pName);
		if(es!=Acad::eOk)
		{
			pLayerTableRecord->close();
			continue;
		}
		CString strName=pName;
		layeNames.Add(strName);
		layeIds.append(pLayerTableRecord->objectId());
		//关闭(切记)
		pLayerTableRecord->close();
	}
	//删除层表迭代器
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	//关闭层表(切记)
	pLayerTable->close () ;
	return Acad::eOk;

}
//调用自动更新
//bSys=true,系统调用,初次打开的时候调用,如果是最新版本不提示
//bSys=false,用户调用
void CallUpdate(bool bSys)
{
	if(bSys)
	{
		TCHAR lpFile[MAX_PATH+1];
		GetModuleFileName(_hdllInstance, lpFile, MAX_PATH);
		PathRemoveFileSpec(lpFile);
		PathRemoveFileSpec(lpFile);
		PathAppend(lpFile,_T("AutoUpdate\\Update.exe"));
		if(!PathFileExists(lpFile))
		{
			//acedAlert(_T("未找到自动更新程序，请联系管理员"));
			return;
		}
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW; 
		si.wShowWindow = SW_NORMAL;
		ZeroMemory(&pi, sizeof(pi));
		if(!CreateProcess(lpFile,   // No module name (use command line). 
			NULL, // Command line.
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 

		{
			//todo log
			//
			//acedAlert(_T("自动更新程序启动失败，请联系管理员"));
			return;
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		TCHAR lpFile[MAX_PATH+1];
		GetModuleFileName(_hdllInstance, lpFile, MAX_PATH);
		PathRemoveFileSpec(lpFile);
		PathRemoveFileSpec(lpFile);
		PathAppend(lpFile,_T("AutoUpdate\\Update.exe"));
		if(!PathFileExists(lpFile))
		{
			acedAlert(_T("未找到自动更新程序，请联系软件开发人员"));
			return;
		}
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW; 
		si.wShowWindow = SW_NORMAL;
		ZeroMemory(&pi, sizeof(pi));
		if(!CreateProcess(lpFile,   // No module name (use command line). 
			_T("/UserCheck"), // Command line.
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 

		{
			//todo log
			//
			acedAlert(_T("自动更新程序启动失败，请联系软件开发人员"));
			return;
		}
		WaitForSingleObject(pi.hProcess,INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
//点是否在范围内
bool IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext)
{
	if(pt.x>ext.maxPoint().x)
		return false;
	if(pt.y>ext.maxPoint().y)
		return false;
	if(pt.x<ext.minPoint().x)
		return false;
	if(pt.y<ext.minPoint().y)
		return false;
	return true;
}
AcGePoint3d MinPoint(AcDbExtents& ext)
{
	return AcGePoint3d(0.5*(ext.minPoint().x+ext.maxPoint().x),
		0.5*(ext.minPoint().y+ext.maxPoint().y),
		0.5*(ext.minPoint().z+ext.maxPoint().z));
}
//功能:	将点从块引用里面经过矩阵转换转换到块外面(不考虑旋转)
//参数:	pRef,块引用
//      pt,点
//返回:	转换结果
Acad::ErrorStatus TransPointOutBlockRef(const AcDbBlockReference* pRef,AcGePoint3d& pt)
{
	if(pRef==NULL)
	{
		return eNullHandle;
	}
	AcDbObjectPointer<AcDbBlockTableRecord> spRec(pRef->blockTableRecord(),AcDb::kForRead);
	if(spRec.openStatus()!=Acad::eOk)
	{
		acutPrintf(_T("\nTransPointOutBlockRef->%s"),acadErrorStatusText(spRec.openStatus()));
		return spRec.openStatus();
	}
	AcGePoint3d       originPt=spRec->origin();//定义的基点
#if _MSC_VER < 1400
	spRec->close();
#else
	spRec.close();
#endif
	double  scaleX=pRef->scaleFactors().sx;
	double  scaleY=pRef->scaleFactors().sy;
	double  scaleZ=pRef->scaleFactors().sz;
	AcGePoint3d insertPt=pRef->position();
	double dx=(pt.x-originPt.x)*scaleX+insertPt.x;
	double dy=(pt.y-originPt.y)*scaleY+insertPt.y;
	double dz=(pt.z-originPt.z)*scaleZ+insertPt.z;
	pt.set(dx,dy,pt.z);
	AcGeMatrix3d mat;
	pt.rotateBy(pRef->rotation(),AcGeVector3d::kZAxis,pRef->position());
	return eOk;
}
Acad::ErrorStatus EraseEntity(const AcDbObjectId objId)
{
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt,objId,AcDb::kForWrite);
	if(es!=eOk)
		return es;
	es=pEnt->erase();
	pEnt->close();
	pEnt=NULL;
	return es;
}
Acad::ErrorStatus EraseBlockTableRecord(AcDbBlockTable* pTable,const CString strName)
{
	if(pTable==NULL)
		return eNullHandle;
	if(!pTable->has(strName))
	{
		return eOk;
	}
	AcDbBlockTableRecord* pRec=NULL;
	Acad::ErrorStatus es=pTable->getAt(strName,pRec,AcDb::kForWrite);
	if(es!=eOk)
		return es;
	pRec->erase();
	pRec->close();
	pRec=NULL;
	//应该还要删除绑定它的实体对象
	return eOk;

}
Acad::ErrorStatus EraseBlockTableRecord(AcDbObjectId btrId)
{
	AcDbBlockTableRecordPointer spRec(btrId,AcDb::kForRead);
	Acad::ErrorStatus es=spRec.openStatus();
	if(es!=Acad::eOk)
		return es;
	AcDbBlockTablePointer spTable(spRec->ownerId(),AcDb::kForRead);
	es=spTable.openStatus();
	if(es!=Acad::eOk)
	{
		return es;
	}
	const TCHAR* lpName=NULL;
	es=spRec->getName(lpName);
	spRec->close();
	es=EraseBlockTableRecord(spTable.object(),lpName);
	spTable->close();
	return es;
}
//功能： 设置int型环境变量
Acad::ErrorStatus SetCADIntVar(CString sMode,int nRet)
{
	Acad::ErrorStatus es=Acad::eOk;

	resbuf *pNewRb=acutBuildList(RTSHORT,nRet,0);
	acedSetVar(sMode,pNewRb);
	acutRelRb(pNewRb);

	return es;
}
//功能： 获取int型环境变量
Acad::ErrorStatus GetCADIntVar(CString sMode,int &nRet)
{
	Acad::ErrorStatus es=Acad::eOk;

	resbuf *pRb=acutBuildList(RTSHORT,23,0);
	acedGetVar(sMode,pRb);
	if(pRb==NULL)
	{
		acutRelRb(pRb);
		return Acad::eNullHandle;
	}
	nRet=pRb->resval.rint;
	acutRelRb(pRb);

	return es;
}
//计算三角形的面积
double CalcTriangleArea(AcGePoint3d ptA,AcGePoint3d ptB,AcGePoint3d ptC)
{
	ptA.z=0;
	ptB.z=0;
	ptC.z=0;
	double c=ptA.distanceTo(ptB);
	double b=ptA.distanceTo(ptC);
	double a=ptB.distanceTo(ptC);
	double p=(a+b+c)/2.0;
	double tmp=p*(p-a)*(p-b)*(p-c);
	return sqrt(tmp);//海伦公式
}
//点是否在面域中(暂时只建议计算四个点构成的面域)
bool IsPointInRegion(AcGePoint3d pt,AcGePoint3dArray& pts)
{
	for(int i=0;i<pts.logicalLength();i++)
	{
		if(fabs(pts.at(i).x-pt.x)<1E-8&&fabs(pts.at(i).y-pt.y)<1E-8)
		{
			return true;
		}
	}
	if(pts.logicalLength()<3)
		return false;
	//面域本身的面积
	double dArea1=0.0;
	for(int i=2;i<pts.logicalLength();i++)
	{
		dArea1+=CalcTriangleArea(pts.at(0),pts.at(i-1),pts.at(i));
	}
	double dArea2=0.0;
	for(int i=1;i<pts.logicalLength();i++)
	{
		dArea2+=CalcTriangleArea(pt,pts.at(i-1),pts.at(i));
	}
	return fabs(dArea1-dArea2)<1E-8*fabs(max(dArea1,dArea2));

}
LPCTSTR  objToHandleStr(const AcDbObject* obj, CString& str)
{
	ASSERT(obj != NULL);

	AcDbHandle handle;

	obj->getAcDbHandle(handle);
	TCHAR tmpStr[256];
	handle.getIntoAsciiBuffer(tmpStr);
	str = tmpStr;
	return str;
}
//功能:判断字符串是否在字符串数组中
//参数:	array:字符串数组
//			string:字符串
//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
//返回:	true:在数组中；false:不在
bool IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase/*=true*/)
{
	for (int i=0;i<array.GetCount();i++) 
	{
		CString tmp=array.GetAt(i);
		if(bNoCase)
		{
			if(stringX.CompareNoCase(tmp)==0)
			{
				return true;
			}
		}
		else
		{
			if(stringX.Compare(tmp)==0) 
			{
				return true;
			}
		}
	}
	return false;
}
/*!
@zhangshl 生成GUID
*/
CString NewGuid()
{
	OLECHAR szGuid[128] =OLESTR("");
	GUID g_guid;
	CoCreateGuid(&g_guid);	
	StringFromGUID2(g_guid,szGuid,sizeof(szGuid)/sizeof(OLECHAR));
	USES_CONVERSION;
	CString m_GuidPrt =W2T(szGuid);
	return m_GuidPrt;
}
string BSTRToString(const BSTR bstr)
{
	if (bstr==NULL)
	{
		return "";
	}
	char* szBuf = _com_util::ConvertBSTRToString(bstr);
	if (szBuf != NULL)
	{
		string strValue = szBuf;
		delete[] szBuf;
		szBuf = NULL;
		return strValue;
	}
	return "";
}
CString IntToString(const int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"),nValue);
	return strValue;
}
CString DoubleToString(const double dValue)
{
	CString strValue;
	strValue.Format(_T("%.f"),dValue);
	return strValue;
}
//ANSI字符串转换为宽字符串
LPWSTR ConvertToLPWSTR(LPCSTR szString)
{
	LPWSTR	wszBuf = NULL;
	UINT	nLen   = NULL;

	nLen = strlen(szString);

	if ((wszBuf = (LPWSTR)malloc((nLen+1)*sizeof(WCHAR))) && 
		(0 == MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szString, nLen, wszBuf, nLen + 1)))
	{
		free(wszBuf); 
		wszBuf = NULL;
	}

	return wszBuf;
}
CString GetCadProductName()
{
#ifdef ACAD_2004
	return _T("AutoCAD 2004(X32)");
#elif _MSC_VER>=1900
	return _T("AutoCAD 2017/8(X64)");
#else
	int nMinorVersion=acdbHostAppMinorVersion();
	CString strProductName=_T("AutoCAD");
#if _MSC_VER >= 1600
	if(nMinorVersion==0)
	{
		strProductName+=_T(" 2013");
	}
	else if(nMinorVersion==1)
	{
		strProductName+=_T(" 2014");
	}
#elif _MSC_VER >= 1500
	if(nMinorVersion==0)
	{
		strProductName+=_T(" 2010");
	}
	else if(nMinorVersion==1)
	{
		strProductName+=_T(" 2011");
	}
	else if(nMinorVersion==2)
	{
		strProductName+=_T(" 2012");
	}
#elif _MSC_VER >= 1400
	if(nMinorVersion==0)
	{
		strProductName+=_T(" 2007");
	}
	else if(nMinorVersion==1)
	{
		strProductName+=_T(" 2008");
	}
	else if(nMinorVersion==2)
	{
		strProductName+=_T(" 2009");
	}
#elif _MSC_VER >= 1300
	if(nMinorVersion==0)
	{
		strProductName+=_T(" 2004");
	}
	else if(nMinorVersion==1)
	{
		strProductName+=_T(" 2005");
	}
	else if(nMinorVersion==2)
	{
		strProductName+=_T(" 2006");
	}
#endif
#ifdef _WIN64
	return strProductName+_T("(X64)");
#else
	return strProductName+_T("(X32)");
#endif
#endif

}
CString GetBlockName(AcDbBlockTableRecord* pRec)
{
	if (pRec == NULL)
	{
		return _T("");
	}
	const ACHAR* pName;
	pRec->getName(pName);
	return CString(pName);
}
CString GetBlockName(AcDbObjectId objId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,objId,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return _T("");
	}
	CString strName=_T("");
	if(pObj->isKindOf(AcDbBlockTableRecord::desc()))
	{
		strName=GetBlockName(AcDbBlockTableRecord::cast(pObj));
	}
	else if(pObj->isKindOf(AcDbBlockReference::desc()))
	{
		strName=GetBlockName(AcDbBlockReference::cast(pObj));
	}
	pObj->close();
	return strName;
}

CString GetBlockName(AcDbBlockReference* pRef)
{
	if (pRef == NULL)
	{
		return _T("");
	}
	AcDbObjectId id = pRef->blockTableRecord();
	AcDbBlockTableRecord* pRec = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)pRec, id, AcDb::kForRead);
	if (es == Acad::eOk)
	{
		pRec->close();
		return GetBlockName(pRec);
	}
	return _T("");
}
//功能:将字符串数组连成一个字符串
CString CombineArrayToString(const CStringArray& array,CString linkSymbol)
{
	CString str=_T("");
	int n=array.GetSize();
	//zhangshl BUG修改
	if(n==0)
	{
		str +=_T("");
		return str;
	}
	//zhangshl BUG修改
	for(int i=0;i<n-1;i++)
	{
		str += array.GetAt(i);
		str += linkSymbol;
	}
	str += array.GetAt(n-1);
	return str;
}
//功能，保存CAD数据库
//pDb,数据库
//lpFileName,文件名称，如果为NULL，表示按原有的文件名称保存
//ver,保存的DWG的版本号
//pSecParams,加密参数
Acad::ErrorStatus SaveDatabase(AcDbDatabase* pDb,const TCHAR* lpFileName,AcDb::AcDbDwgVersion ver,const SecurityParams* pSecParams)
{
	Acad::ErrorStatus es=Acad::eOk;
	if(pDb==NULL)
	{
		return Acad::eNullHandle;
	}
	TCHAR fileName[MAX_PATH+1]={0};
	if(lpFileName!=NULL)
	{
		lstrcpy(fileName,lpFileName);
	}
	else
	{
		const TCHAR* lpTmp=NULL;
		es=pDb->getFilename(lpTmp);
		if(es!=Acad::eOk)
		{
			return es;
		}
		lstrcpy(fileName,lpTmp);
	}
	if(lstrcmpi(PathFindExtension(fileName),_T(".dwg"))!=0)
	{
		return Acad::eInvalidFileExtension;
	}
	AcDb::AcDbDwgVersion inner_ver=ver;
	if(inner_ver==AcDb::kDHL_Unknown)
	{
		inner_ver=pDb->lastSavedAsVersion();
	}
	if(inner_ver==AcDb::kDHL_MC0_0)
	{
		//文件没有保存过,且未设置版本
#if _MSC_VER>=1400
		inner_ver=AcDb::kDHL_1021;//2007 final
#else
		inner_ver=AcDb::kDHL_1800;//2004 final
#endif
	}
	if(inner_ver==AcDb::kDHL_1500||inner_ver==AcDb::kDHL_1015)
	{
		//2000
		return acdbSaveAs2000(pDb,fileName);
	}
	else if(inner_ver==AcDb::kDHL_1800||inner_ver==AcDb::kDHL_1800a)
	{
		//2004
		#if _MSC_VER>=1400
		return acdbSaveAs2004(pDb,fileName);
		#elif _MSC_VER>=1300
		return pDb->saveAs(fileName,pSecParams);
#else
		return Acad::eInvalidInput;
#endif
	}
	#if _MSC_VER>=1400
	else if(inner_ver==AcDb::kDHL_2100a||inner_ver==AcDb::kDHL_1021)
	{
		//2007
		#if _MSC_VER>=1500
		return pDb->saveAs(fileName,false,inner_ver);
		#elif _MSC_VER>=1400
		return pDb->saveAs(fileName,pSecParams);
#else
		return Acad::eInvalidInput;
#endif
	}
#endif
#if _MSC_VER>=1500
	else if(inner_ver==AcDb::kDHL_1024)
	{
		//2010
		return pDb->saveAs(fileName,false,inner_ver);
	}
	
#endif
	#if _MSC_VER>=1600
	else if(inner_ver==AcDb::kDHL_1027)
	{
		//2013
		return pDb->saveAs(fileName,false,inner_ver);
	}
#endif
	else
	{
		return Acad::eInvalidInput;
	}
	
}
Acad::ErrorStatus UnLockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord *pLayerTableRecord=AcDbLayerTableRecord::cast(pObj);
	if(pLayerTableRecord==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	bLock=pLayerTableRecord->isLocked();
	bFrozen=pLayerTableRecord->isFrozen();
	es=pLayerTableRecord->upgradeOpen();
	if(es!=eOk)
	{
		pObj->close();
		return es;
	}
	pLayerTableRecord->setIsLocked(false);
	pLayerTableRecord->setIsFrozen(false);
	pObj->close();
	return Acad::eOk;
}
Acad::ErrorStatus LockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord *pLayerTableRecord=AcDbLayerTableRecord::cast(pObj);
	if(pLayerTableRecord==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	es=pLayerTableRecord->upgradeOpen();
	if(es!=eOk)
	{
		pObj->close();
		return es;
	}
	pLayerTableRecord->setIsLocked(bLock);
	pLayerTableRecord->setIsFrozen(bFrozen);
	pObj->close();
	return Acad::eOk;
}
//强制删除实体
Acad::ErrorStatus ForceEraseEntity(const AcDbObjectId objId)
{
	AcDbEntityPointer spEnt(objId,AcDb::kForRead);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=Acad::eOk)
	{
		return es;
	}
	es=spEnt->upgradeOpen();
	if(es==Acad::eOk)
	{
		es=spEnt->erase();
		return es;
	}
	else if(es==Acad::eOnLockedLayer)
	{
		bool bLock=false;
		bool bFrozen=false;
		es=UnLockLayer(spEnt->layerId(),bLock,bFrozen);
		if(es!=Acad::eOk)
		{
			return es;
		}
		es=spEnt->upgradeOpen();
		if(es!=Acad::eOk)
		{
			LockLayer(spEnt->layerId(),bLock,bFrozen);
			return es;
		}
		es=spEnt->erase();
		LockLayer(spEnt->layerId(),bLock,bFrozen);
		return es;
	}
	else
	{
		return es;
	}
}
//选择范围
bool WindowSelect(AcGePoint3d& ptMin,AcGePoint3d& ptMax)
{
	ads_point p1, p2;  //交互矩形点坐标获取
	if (acedGetPoint(NULL, _T("\n>>指定第一个角点: "), p1) != RTNORM)
		return false;

	if (acedGetCorner(p1, _T("\n>>指定第二个角点: "), p2) != RTNORM) 
		return false;

	if (p1[0] == p2[0] || p1[1] == p2[1]) //矩形框合法性检查
		return false;
	ptMin.x=min(p1[0],p2[0]);
	ptMin.y=min(p1[1],p2[1]);
	ptMin.z=0.0;

	ptMax.x=max(p1[0],p2[0]);
	ptMax.y=max(p1[1],p2[1]);
	ptMax.z=0.0;
	return true;
}
//获取绝对的颜色号
int GetAbsColorIndex(AcDbEntity* pEnt)
{
	if(pEnt->colorIndex()==256)
	{
		//ByLayer
		AcDbLayerTableRecord* pRecord=NULL;
		Acad::ErrorStatus es=acdbOpenObject(pRecord,pEnt->layerId(),AcDb::kForRead);
		if(es!=Acad::eOk)
		{
			return pEnt->colorIndex();
		}
		int nColorIndex=pRecord->color().colorIndex();
		pRecord->close();
		return nColorIndex;
	}
	else if(pEnt->colorIndex()==0)
	{
		//ByBlock
		//?
		AcDbEntityPointer spEnt(pEnt->ownerId(),AcDb::kForRead);
		if(spEnt.openStatus()==Acad::eOk)
		{
			return GetAbsColorIndex(spEnt.object());
		}
		else
		{
			return pEnt->colorIndex();
		}
	}
	else
	{
		return pEnt->colorIndex();
	}
}


