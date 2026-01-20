#include "StdAfx.h"
#include "GDimCommand.h"
#include "SpcoordLabel.h"
#include "GDimTextDialog.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "GDimJig.h"
Acad::ErrorStatus GetDimLine(AcGePoint3d& pt1,AcGePoint3d& pt2,AcDbDimension* pDim)
{
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=pDim->explode(entitySet);
	if(es!=Acad::eOk)
	{
		return es;
	}
	AcGePoint3dArray ptArray;
	for(int i=entitySet.logicalLength()-1;i>=0;i--)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef=AcDbBlockReference::cast(pEnt);
		ptArray.append(pRef->position());
		}
		else if(pEnt->isKindOf(AcDbSolid::desc()))
		{
			AcDbSolid* pSolid=AcDbSolid::cast(pEnt);
			AcGePoint3d pntRes;
			pSolid->getPointAt(2,pntRes);
			ptArray.append(pntRes);
		}
		
		delete pEnt;
		pEnt=NULL;
		entitySet.removeAt(i);
	}
	if(ptArray.logicalLength()!=2)
	{
		return Acad::eInvalidInput;
	}
	pt1=ptArray.at(0);
	pt2=ptArray.at(1);
	return Acad::eOk;
}
//81:文字标注
void DimText()
{
	if(!CheckReg())
	{
		return;
	}
	CAcModuleResourceOverride res;
	GDimTextDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	CSpcoordLabel tmp;
	tmp.CreateDimText(dlg.strText,NULL);
}
//坐标标注
void DimXY()
{
	// 锁定文档
	GLockDocument _Lock;

	CSpcoordLabel spoordlabel;
	spoordlabel.CreateCoordLabel();
}
//天正坐标标注
void DimXYTZ()
{
	// 锁定文档
	GLockDocument _Lock;

	CSpcoordLabel spoordlabel;
	spoordlabel.m_bExchangeXY=true;
	spoordlabel.m_dScale=1E-3;
	spoordlabel.CreateCoordLabel();
}

//116:线性标注
//    标注直线距离
//    使用转角标注
void DimXX()
{
	if(!CheckReg())
	{
		return;
	}
	GLockDocument _Lock;

	CSpcoordLabel spoordlabel;
	AcDbObjectId   rcId;
	Acad::ErrorStatus es=UpdateDimStyleByConfig(rcId,true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置标注样式失败:%s"),acadErrorStatusTextChs(es));
		return;
	}
	AcDbObjectId   layerId;
	es=UpdateLayerByConfig(layerId,_T("线性标注"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置线性标注图层失败:%s"),acadErrorStatusTextChs(es));
		return;
	}
	ads_point p2;
	ads_point p3;
	ads_point p1;
	int nCount=0;
	while(true)
	{
		if(nCount==0)
		{
			if(acedGetPoint(NULL, _T("\n 指定第一条尺寸界线原点"), p1)!=RTNORM)
			{
				return;
			}
		}
		else
		{
			p1[X]=p2[X];
			p1[Y]=p2[Y];
			p1[Z]=p2[Z];
		}



		if(nCount==0)
		{
			if(acedGetPoint(p1, _T("\n 指定第二条尺寸界线原点"), p2)!=RTNORM)
			{
				return;
			}

			acutPrintf(_T("\n 指定尺寸线位置"));
		}
		else
		{
			acutPrintf(_T("\n 指定第二条尺寸界线原点"));
		}


		GDimJig jig;
		AcDbRotatedDimension* pDim=new AcDbRotatedDimension();
		pDim->setXLine1Point(AcGePoint3d(p1[X],p1[Y],p1[Z]));
		if(nCount==0)
		{
			pDim->setXLine2Point(AcGePoint3d(p2[X],p2[Y],p2[Z]));
			jig.m_bJigDimLinePoint=true;
		}
		else
		{
			pDim->setDimLinePoint(AcGePoint3d(p3[X]+p2[X]-p1[X],p3[Y]+p2[Y]-p1[Y],p3[Z]+p2[Z]-p1[Z]));
			jig.m_bJigDimLinePoint=false;
		}
		pDim->setDimensionStyle(rcId);
		pDim->setLayer(layerId);
		jig.SetDim(pDim);
		int ret = jig.Do();
		if(ret!=RTNORM)
		{
			return;
		}
		p3[X]=pDim->dimLinePoint().x;
		p3[Y]=pDim->dimLinePoint().y;
		p3[Z]=pDim->dimLinePoint().z;
		if(nCount!=0)
		{
			p2[X]=pDim->xLine2Point().x;
			p2[Y]=pDim->xLine2Point().y;
			p2[Z]=pDim->xLine2Point().z;
		}
		nCount++;
	}
}
//117:对齐标注
void DimDQ()
{
	if(!CheckReg())
	{
		return;
	}
	GLockDocument _Lock;
	AcDbObjectId   rcId;
	Acad::ErrorStatus es=UpdateDimStyleByConfig(rcId,true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置标注样式失败:%s"),acadErrorStatusTextChs(es));
		return;
	}
	AcDbObjectId   layerId;
	es=UpdateLayerByConfig(layerId,_T("线性标注"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置线性标注图层失败:%s"),acadErrorStatusTextChs(es));
		return;
	}
	ads_point p1;
	if(acedGetPoint(NULL, _T("\n 指定第一条尺寸界线原点"), p1)!=RTNORM)
	{
		return;
	}
	ads_point p2;
	if(acedGetPoint(p1, _T("\n 指定第二条尺寸界线原点"), p2)!=RTNORM)
	{
		return;
	}
	acutPrintf(_T("\n 指定尺寸线位置"));
	GDimJig jig;
	AcDbAlignedDimension* pDim=new AcDbAlignedDimension();
	pDim->setXLine1Point(AcGePoint3d(p1[X],p1[Y],p1[Z]));
	pDim->setXLine2Point(AcGePoint3d(p2[X],p2[Y],p2[Z]));
	jig.m_bJigDimLinePoint=true;
	pDim->setDimensionStyle(rcId);
	pDim->setLayer(layerId);
	jig.SetDim(pDim);
	int ret = jig.Do();
	if(ret!=RTNORM)
	{
		return;
	}
	AcGePoint3d ptDimLine;
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	es=GetDimLine(pt1,pt2,pDim);
	if(es!=Acad::eOk)
	{
		return;
	}
	AcGePoint3d pt3=pDim->xLine1Point();
	if(pt3.distanceTo(pt1)>pt3.distanceTo(pt2))
	{
		ptDimLine=pt1;
	}
	else
	{
		ptDimLine=pt2;
	}
	ptDimLine=pDim->dimLinePoint();
	AcGeVector3d vec=ptDimLine-pDim->xLine2Point();
	AcGePoint3d _pt1=pDim->xLine1Point();
	AcGePoint3d _pt2=pDim->xLine2Point();
	while(true)
	{
		GDimJig jig1;
		jig1.m_bInLine=true;
		jig1.m_pt1=_pt1;
		jig1.m_pt2=_pt2;
		pDim=new AcDbAlignedDimension();
		pDim->setXLine1Point(AcGePoint3d(p2[X],p2[Y],p2[Z]));
		pDim->setDimLinePoint(ptDimLine);
		pDim->setDimensionStyle(rcId);
		pDim->setLayer(layerId);

		jig1.m_vec=vec;
		jig1.m_bJigDimLinePoint=false;
		jig1.SetDim(pDim);

		int ret = jig1.Do();
		if(ret!=RTNORM)
		{
			return;
		}
		p2[X]=pDim->xLine2Point().x;
		p2[Y]=pDim->xLine2Point().y;
		p2[Z]=pDim->xLine2Point().z;
		ptDimLine=pDim->dimLinePoint();
	}
}

//118:标注断开
void bzdk()
{
	if(!CheckReg())
	{
		return;
	}
	ads_name ssName;
	resbuf* rb = acutBuildList(RTDXF0, _T("DIMENSION"), 0);
	TCHAR* promptPtrs[2]={_T("\n选择标注:"),_T("\n移除标注:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	acutRelRb(rb);
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()!=1)
	{
		return;
	}
	AcDbObjectId id=ids.at(0);
	AcDbObjectPointer<AcDbDimension> spDim(id,AcDb::kForRead);
	if(spDim.openStatus()!=Acad::eOk)
	{
		return;
	}
	if(spDim->isKindOf(AcDbAlignedDimension::desc())||
		spDim->isKindOf(AcDbRotatedDimension::desc()))
	{
		AcGePoint3d xLine1Point;
		AcGePoint3d xLine2Point;
		AcGePoint3d dimLinePoint;
		if(spDim->isKindOf(AcDbAlignedDimension::desc()))
		{
			AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(spDim.object());
			xLine1Point=pDim->xLine1Point();
			xLine2Point=pDim->xLine2Point();
			dimLinePoint=pDim->dimLinePoint();
		}
		else if(spDim->isKindOf(AcDbRotatedDimension::desc()))
		{
			AcDbRotatedDimension* pDim=AcDbRotatedDimension::cast(spDim.object());
			xLine1Point=pDim->xLine1Point();
			xLine2Point=pDim->xLine2Point();
			dimLinePoint=pDim->dimLinePoint();
		}

		ads_point p0;
		p0[X]=xLine1Point.x;
		p0[Y]=xLine1Point.y;
		p0[Z]=xLine1Point.z;

		AcGePoint3d ptMid=GCADUtils::MidPoint(xLine1Point,xLine2Point);
		ads_point p1;
		p1[X]=ptMid.x;
		p1[Y]=ptMid.y;
		p1[Z]=ptMid.z;

		if(acedGetPoint(p0, _T("\n 断开点"), p1)!=RTNORM)
		{
			return;
		}

		CoordinateD start;
		start.fromCadPoint(xLine1Point);

		CoordinateD end;
		end.fromCadPoint(xLine2Point);

		LineD line(start,end);

		CoordinateD pt(p1[X],p1[Y],p1[Z]);

		double dLen=line.CalcDistance(pt);

		if(dLen<1E-8)
		{
			//在直线上
		}
		else
		{
			//不在直线上
			line.plumbPoint(pt,pt);
		}

		if(spDim->upgradeOpen()==Acad::eOk)
		{
			if(spDim->isKindOf(AcDbAlignedDimension::desc()))
			{
				AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(spDim.object());
				pDim->setXLine2Point(pt.toCadPoint());

				AcDbAlignedDimension* pNewDim=(AcDbAlignedDimension*)pDim->clone();
				pNewDim->setXLine1Point(pt.toCadPoint());
				pNewDim->setXLine2Point(xLine2Point);
				GCADUtils::AddToCurrentSpace(pNewDim);
				pNewDim->close();
			}
			else if(spDim->isKindOf(AcDbRotatedDimension::desc()))
			{
				AcDbRotatedDimension* pDim=AcDbRotatedDimension::cast(spDim.object());
				pDim->setXLine2Point(pt.toCadPoint());

				AcDbRotatedDimension* pNewDim=(AcDbRotatedDimension*)pDim->clone();
				pNewDim->setXLine1Point(pt.toCadPoint());
				pNewDim->setXLine2Point(xLine2Point);
				GCADUtils::AddToCurrentSpace(pNewDim);
				pNewDim->close();
			}
		}


	}
	else
	{
		acutPrintf(_T("\n 只能选择线型标注或者对齐标注"));
	}
}
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
//是否在同一条直线上
bool IsOnOneLine(AcGePoint3d ptA,AcGePoint3d ptB,AcGePoint3d ptC)
{
	ptA.z=0.0;
	ptB.z=0.0;
	ptC.z=0.0;
	double c=ptA.distanceTo(ptB);
	double b=ptA.distanceTo(ptC);
	double a=ptB.distanceTo(ptC);
	double p=(a+b+c)/2.0;
	double tmp=p*(p-a)*(p-b)*(p-c);
	if(tmp<1E-8)
	{
		return true;
	}
	double dLen=ptA.distanceTo(ptB);
	dLen*=0.01;
	return sqrt(tmp)<dLen*dLen;//海伦公式
}
void bzhb(AcDbObjectIdArray& ids)
{
	if(ids.logicalLength()<=1)
	{
		return;
	}
	AcGePoint3dArray xLine1PointArray;
	AcGePoint3dArray xLine2PointArray;
	AcGePoint3dArray dimLinePointArray;
	AcArray<int>     indexArray;
	AcGePoint3dArray pt1Array;
	AcGePoint3dArray pt2Array;
	Acad::ErrorStatus es=Acad::eOk;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectPointer<AcDbDimension> spDim(ids.at(i),AcDb::kForRead);
		if(spDim.openStatus()!=Acad::eOk)
		{
			continue;
		}
		AcGePoint3d pt1;
		AcGePoint3d pt2;
		es=GetDimLine(pt1,pt2,spDim.object());
		if(es!=Acad::eOk)
		{
			continue;
		}
		pt1Array.append(pt1);
		pt2Array.append(pt2);

		if(spDim->isKindOf(AcDbAlignedDimension::desc()))
		{
			AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(spDim.object());
			xLine1PointArray.append(pDim->xLine1Point());
			xLine2PointArray.append(pDim->xLine2Point());
			dimLinePointArray.append(pDim->dimLinePoint());
			indexArray.append(i);
		}
		else if(spDim->isKindOf(AcDbRotatedDimension::desc()))
		{
			AcDbRotatedDimension* pDim=AcDbRotatedDimension::cast(spDim.object());
			xLine1PointArray.append(pDim->xLine1Point());
			xLine2PointArray.append(pDim->xLine2Point());
			dimLinePointArray.append(pDim->dimLinePoint());
			indexArray.append(i);
		}
	}
	while(xLine1PointArray.logicalLength()>1)
	{
		AcGePoint3d xLine1PointLast=xLine1PointArray.at(xLine1PointArray.logicalLength()-1);
		AcGePoint3d xLine2PointLast=xLine2PointArray.at(xLine2PointArray.logicalLength()-1);
		int         indexLast=indexArray.at(indexArray.logicalLength()-1);
		//适合做合并的
		AcArray<int>     fit_indexArray;
		for(int i=0;i<xLine1PointArray.logicalLength()-1;i++)
		{
			if(IsOnOneLine(pt1Array.at(pt1Array.logicalLength()-1),pt2Array.at(pt2Array.logicalLength()-1),pt1Array.at(i))&&
				IsOnOneLine(pt1Array.at(pt1Array.logicalLength()-1),pt2Array.at(pt2Array.logicalLength()-1),pt2Array.at(i)))
			{
				fit_indexArray.append(i);
			}
		}
		if(fit_indexArray.logicalLength()==0)
		{
			//不适合合并
			int nLast=xLine1PointArray.logicalLength()-1;//删除最后一个
			xLine1PointArray.removeAt(nLast);
			xLine2PointArray.removeAt(nLast);
			dimLinePointArray.removeAt(nLast);
			indexArray.removeAt(nLast);
			pt1Array.removeAt(nLast);
			pt2Array.removeAt(nLast);
		}
		else
		{
			int nLast=xLine1PointArray.logicalLength()-1;//删除最后一个
			fit_indexArray.append(nLast);
			//执行合并
			int nIndex1=-1;
			int nIndex2=-1;
			double dMax=-1;

			AcGePoint3d pt1;
			AcGePoint3d pt2;
			for(int i=fit_indexArray.logicalLength()-1;i>=0;i--)
			{
				AcGePoint3d xLine1Point1=xLine1PointArray.at(fit_indexArray.at(i));
				AcGePoint3d xLine2Point1=xLine2PointArray.at(fit_indexArray.at(i));
				for(int j=i-1;j>=0;j--)
				{
					AcGePoint3d xLine1Point2=xLine1PointArray.at(fit_indexArray.at(j));
					AcGePoint3d xLine2Point2=xLine2PointArray.at(fit_indexArray.at(j));
					AcGePoint3dArray pts;
					pts.append(xLine1Point1);
					pts.append(xLine2Point1);

					pts.append(xLine1Point2);
					pts.append(xLine2Point2);

					double dMaxTmp=-1;
					AcGePoint3d pt1Tmp;
					AcGePoint3d pt2Tmp;

					for(int k=0;k<pts.logicalLength();k++)
					{
						for(int l=0;l<pts.logicalLength();l++)
						{
							double dTmp=pts.at(k).distanceTo(pts.at(l));
							if(dTmp>dMaxTmp)
							{
								dMaxTmp=dTmp;
								pt1Tmp=pts.at(k);
								pt2Tmp=pts.at(l);
							}
						}
					}

					if(dMaxTmp>dMax)
					{
						dMax=dMaxTmp;
						nIndex1=i;
						nIndex2=j;
						pt1=pt1Tmp;
						pt2=pt2Tmp;
					}
				}
			}
			if(nIndex1>=0&&nIndex2>=0)
			{
				AcDbObjectPointer<AcDbDimension> spDim(ids.at(fit_indexArray.at(nIndex1)),AcDb::kForWrite);
				if(spDim.openStatus()==Acad::eOk)
				{
					if(spDim->isKindOf(AcDbAlignedDimension::desc()))
					{
						AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(spDim.object());
						pDim->setXLine1Point(pt1);
						pDim->setXLine2Point(pt2);
					}
					else if(spDim->isKindOf(AcDbRotatedDimension::desc()))
					{
						AcDbRotatedDimension* pDim=AcDbRotatedDimension::cast(spDim.object());
						pDim->setXLine1Point(pt1);
						pDim->setXLine2Point(pt2);

					}
				}

				//将其它的标注删除掉
				for(int i=fit_indexArray.logicalLength()-1;i>=0;i--)
				{
					if(i==nIndex1)
					{
						continue;
					}
					AcDbObjectPointer<AcDbDimension> spDim1(ids.at(fit_indexArray.at(i)),AcDb::kForWrite);
					if(spDim1.openStatus()!=Acad::eOk)
					{
						continue;
					}
					spDim1->erase();

				}

			}
			//删除已经合并的了
			for(int i=fit_indexArray.logicalLength()-1;i>=0;i--)
			{
				int nLast=fit_indexArray.at(i);//删除最后一个
				xLine1PointArray.removeAt(nLast);
				xLine2PointArray.removeAt(nLast);
				dimLinePointArray.removeAt(nLast);
				indexArray.removeAt(nLast);
				pt1Array.removeAt(nLast);
				pt2Array.removeAt(nLast);
			}
		}
	}
}
//119:标注合并
void bzhb()
{
	if(!CheckReg())
	{
		return;
	}
label_dimSelect:
	ads_name ssName;
	resbuf* rb = acutBuildList(RTDXF0, _T("DIMENSION"), 0);
	TCHAR* promptPtrs[2]={_T("\n选择标注:"),_T("\n移除标注:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	acutRelRb(rb);
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()<=1)
	{
		acutPrintf(_T("\n 合并的标注数量必须大于1，请重新选择"));
		goto label_dimSelect;
	}
	//归类
	AcDbObjectIdArray ids_xx;//线性标注
	AcDbObjectIdArray ids_dq;//对齐标注
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectPointer<AcDbDimension> spDim(ids.at(i),AcDb::kForRead);
		if(spDim.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spDim->isKindOf(AcDbAlignedDimension::desc()))
		{
			ids_dq.append(ids.at(i));
		}
		else if(spDim->isKindOf(AcDbRotatedDimension::desc()))
		{
			ids_xx.append(ids.at(i));
		}
	}
	bzhb(ids_xx);
	bzhb(ids_dq);
}
