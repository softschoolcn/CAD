#include "StdAfx.h"
#include "GZoom.h"

GZoom::GZoom(void)
{
}

GZoom::~GZoom(void)
{
}
//获取最大化视口范围
bool GZoom::GetMaxViewPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax)
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
			if(!bHasEntity)
			{
				bHasEntity=true;
				ptMin=ex.minPoint();
				ptMax=ex.maxPoint();
				continue;
			}
			AcGePoint3d tempMax;
			AcGePoint3d tempMin;
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
	}
	return bHasEntity;
}
//枚举块表中的所有实体
Acad::ErrorStatus GZoom::EnumAllEntitys(AcDbObjectIdArray& ObjIdArray,AcDbBlockTableRecord* pRec)
{
	Acad::ErrorStatus es=eOk;
	AcDbBlockTableRecordIterator* pIter = NULL;
	es=pRec->newIterator(pIter);
	if(es!=Acad::eOk)
	{
		return es;
	}
	std::auto_ptr<AcDbBlockTableRecordIterator> spIter(pIter);
	AcDbObjectId entityId;
	for(; !pIter->done(); pIter->step() )
	{
		es=pIter->getEntityId(entityId);
		if(es!=Acad::eOk)
		{
			continue;
		}
		ObjIdArray.append(entityId);
	}
	return Acad::eOk;
}
//枚举模型空间中的所有实体
Acad::ErrorStatus GZoom::EnumAllEntitysInModelSpace(AcDbObjectIdArray& ObjIdArray,AcDbDatabase* pDb)
{
	AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDb, AcDb::kForRead);
	Acad::ErrorStatus es = spBlkRec.openStatus();
	if(es!=Acad::eOk)
	{
		return es;
	}
	return EnumAllEntitys(ObjIdArray,spBlkRec.object());

}
//功能:	设置视图（相当于Zoom Window命令）
//参数:	Pt1		视图左上角点
//			Pt2		视图右下角点
//			ex_ratio	扩展比率,一般为1.0
Acad::ErrorStatus GZoom::view_set(AcDbViewportTableRecord* pViewPortRec,AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio)
{
	Acad::ErrorStatus es=eOk;

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
		return eInvalidInput;

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
	pViewPortRec->setCenterPoint(CenterPt);
	pViewPortRec->setWidth(width*ex_ratio);
	pViewPortRec->setHeight(height*ex_ratio);
	return Acad::eOk;
}
//执行缩放
Acad::ErrorStatus GZoom::DyZoom(AcDbViewportTableRecord* pViewPortRec,AcGePoint3d ptMin, AcGePoint3d ptMax,double ex_ratio)
{
	AcGePoint3d pt11,pt22;
	pt11.set(ptMin.x,ptMax.y,0);
	pt22.set(ptMax.x,ptMin.y,0);
	return view_set(pViewPortRec,pt11,pt22,ex_ratio);
}
//缩放到数据库对应的范围，相当于执行ZOOM/E命令
Acad::ErrorStatus GZoom::ZoomDatabaseExtent(AcDbDatabase* pDb)
{
	AcDbObjectId vId=pDb->viewportTableId();
	AcDbViewportTablePointer spView(vId,AcDb::kForRead);
	if(spView.openStatus()!=Acad::eOk)
	{
		return spView.openStatus();
	}
	AcDbViewportTableRecord* pViewPortRec = NULL;
	Acad::ErrorStatus es = spView->getAt(_T("*Active"), pViewPortRec, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		return es;
	}
	AcDbObjectIdArray ObjIdArray;
	es=EnumAllEntitysInModelSpace(ObjIdArray,pDb);
	if(es!=Acad::eOk)
	{
		pViewPortRec->close();
		return es;
	}
	AcGePoint3d ptMin;
	AcGePoint3d ptMax;
	if(!GetMaxViewPoint(ObjIdArray,ptMin,ptMax))
	{
		pViewPortRec->close();
		return Acad::eInvalidExtents;
	}
	es=DyZoom(pViewPortRec,ptMin,ptMax,1.2);
	pViewPortRec->close();
	return es;
}
//功能:	设置视图（相当于Zoom Window命令）
//参数:	Pt1		视图左上角点
//			Pt2		视图右下角点
//			ex_ratio	扩展比率,一般为1.0
Acad::ErrorStatus GZoom::view_set(AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio/*=1.0*/)
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
bool GZoom::DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio)
{
	AcGePoint3d ptMin,ptMax;

	bool rc=GetMaxViewPoint(ObjIdArray,ptMin,ptMax);
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
bool GZoom::DyZoom(const AcDbObjectId ObjId,double ex_ratio)
{
	AcDbObjectIdArray ObjIdArray;
	ObjIdArray.append(ObjId);
	return DyZoom(ObjIdArray,ex_ratio);
}
