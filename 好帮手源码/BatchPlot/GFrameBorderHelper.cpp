#include "StdAfx.h"
#include "GFrameBorderHelper.h"

GFrameBorderHelper::GFrameBorderHelper(void)
{
	m_strFileName=_T("");
}

GFrameBorderHelper::~GFrameBorderHelper(void)
{
}
Acad::ErrorStatus GFrameBorderHelper::GetPlotItems(CArray<GPlotItem>& plotItems,AcDbDatabase* pDb)
{
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	Acad::ErrorStatus es=spBTR.openStatus();
	if(es!=eOk)
		return es;
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if(pBTR->isLayout())
		{
			GetPlotItems(plotItems,pBTR);
		}
		pBTR->close();
		pBTR=NULL;

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
Acad::ErrorStatus GFrameBorderHelper::GetPlotItems(CArray<GPlotItem>& plotItems,AcDbBlockTableRecord* pBTR)
{
	Acad::ErrorStatus es=eOk;
	AcDbBlockTableRecordIterator* pIter = NULL;
	pBTR->newIterator(pIter);

	std::auto_ptr<AcDbBlockTableRecordIterator> spIter(pIter);
	AcDbObjectId id;
	AcDbObjectIdArray ids;
	for(; !pIter->done(); pIter->step() )
	{
		if(Acad::eOk!=pIter->getEntityId(id))
		{
			continue;
		}
		ids.append(id);
	}
	AcGePoint3dArray ptMinArray;
	AcGePoint3dArray ptMaxArray;
	CStringArray strNameArray;
	GetRects(strNameArray,ptMinArray,ptMaxArray,ids);
	for(int i=0;i<ptMinArray.logicalLength();i++)
	{
		GPlotItem plotItem;
		plotItem.m_ptMin=ptMinArray.at(i);
		plotItem.m_ptMax=ptMaxArray.at(i);
		GLog::WriteLine(_T("打印图框:%s,范围[(%.3f,%.3f)_(%.3f,%.3f)]"),strNameArray.GetAt(i),plotItem.m_ptMin.x,plotItem.m_ptMin.y,plotItem.m_ptMax.x,plotItem.m_ptMax.y);
		plotItem.m_strFile=m_strFileName;
		plotItem.m_strBlockName=strNameArray.GetAt(i);
		plotItems.Add(plotItem);
	}
	return Acad::eOk;
}
void GFrameBorderHelper::ComparePoint(bool& bXSame,bool& bYSame,AcGePoint2d pt1,AcGePoint2d pt2)
{
	if(fabs(pt1.x-pt2.x)<1E-4)
	{
		bXSame=true;
	}
	else
	{
		bXSame=false;
	}
	if(fabs(pt1.y-pt2.y)<1E-4)
	{
		bYSame=true;
	}
	else
	{
		bYSame=false;
	}
}
bool GFrameBorderHelper::GetRects(AcGePoint3d& ptMin,AcGePoint3d& ptMax,AcDbPolyline* pLine,double dMin)
{
	if(pLine->hasBulges())
	{
		return false;
	}
	if(pLine->numVerts()<=3)
	{
		return false;
	}
	AcGePoint2dArray ptArray;
	AcGePoint2d pt;
	for(int i=0;i<pLine->numVerts();i++)
	{
		pLine->getPointAt(i,pt);
		ptArray.append(pt);
	}
	if(pLine->isClosed())
	{
		AcGePoint2d ptFirst=ptArray.at(0);
		AcGePoint2d ptLast=ptArray.at(ptArray.logicalLength()-1);
		if(fabs(ptLast.x-ptFirst.x)>dMin||fabs(ptLast.y-ptFirst.y)>dMin)
		{
			ptArray.append(ptFirst);
		}
	}
	if(ptArray.logicalLength()<5)
	{
		return false;
	}
	AcDbExtents extents;
	pLine->getGeomExtents(extents);
	ptMin=extents.minPoint();
	ptMax=extents.maxPoint();

	//所有两点之间都只能水平或者竖直连接
	for(int i=0;i<ptArray.logicalLength()-1;i++)
	{
		AcGePoint2d pt1=ptArray.at(i);
		AcGePoint2d pt2=ptArray.at(i+1);
		bool bXSame=false;
		bool bYSame=false;
		ComparePoint(bXSame,bYSame,pt1,pt2);
		if((!bXSame)&&(!bYSame))
		{
			return false;
		}
	}

	//找到范围四点
	AcGePoint3dArray ptArray1;
	ptArray1.append(ptMin);
	ptArray1.append(AcGePoint3d(ptMax.x,ptMin.y,0.0));
	ptArray1.append(ptMax);
	ptArray1.append(AcGePoint3d(ptMin.x,ptMax.y,0.0));
	for(int j=0;j<ptArray1.logicalLength();j++)
	{
		AcGePoint3d pt=ptArray1.at(j);
		bool bFind=false;
		for(int i=0;i<ptArray.logicalLength();i++)
		{
			AcGePoint2d pt1=ptArray.at(i);
			if(fabs(pt.x-pt1.x)>1E-4&&fabs(pt.y-pt1.y)>dMin)
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
		{
			return false;
		}
	}
	

	return true;
}
//能构成矩形的分组在一起
void GFrameBorderHelper::TwoLines(AcArray<AcGePoint3dArray*>& ptArrayGroup1,AcArray<AcGePoint3dArray*>& ptArrayGroup2,AcGePoint3dArray& ptTmpArray1,AcGePoint3dArray& ptTmpArray2,bool bHor)
{
	for(int i=ptTmpArray1.logicalLength()-1;i>=0;i--)
	{
		double d1=bHor?ptTmpArray1.at(i).x:ptTmpArray1.at(i).y;
		double d2=bHor?ptTmpArray2.at(i).x:ptTmpArray2.at(i).y;
		AcArray<int> indexArray;
		for(int j=i-1;j>=0;j--)
		{
			double dd1=bHor?ptTmpArray1.at(j).x:ptTmpArray1.at(j).y;
			double dd2=bHor?ptTmpArray2.at(j).x:ptTmpArray2.at(j).y;
			if((fabs(dd1-d1)<1E-4&&fabs(d2-dd2)<1E-4)||
				(fabs(dd1-d2)<1E-4&&fabs(d1-dd2)<1E-4))
			{
				indexArray.append(j);
			}
		}
		if(indexArray.logicalLength()==0)
		{
			ptTmpArray1.removeAt(i);
			ptTmpArray2.removeAt(i);
			continue;
		}
		AcGePoint3dArray* pts1=new AcGePoint3dArray;
		AcGePoint3dArray* pts2=new AcGePoint3dArray;
		pts1->append(ptTmpArray1.at(i));
		pts2->append(ptTmpArray2.at(i));
		ptTmpArray1.removeAt(i);
		ptTmpArray2.removeAt(i);
		for(int k=0;k<indexArray.logicalLength();k++)
		{
			pts1->append(ptTmpArray1.at(indexArray.at(k)));
			pts2->append(ptTmpArray2.at(indexArray.at(k)));
			ptTmpArray1.removeAt(indexArray.at(k));
			ptTmpArray2.removeAt(indexArray.at(k));
		}
		ptArrayGroup1.append(pts1);
		ptArrayGroup2.append(pts2);

	}
}
void GFrameBorderHelper::Release(AcArray<AcGePoint3dArray*>& ptArrayGroup)
{
	for(int i=ptArrayGroup.logicalLength()-1;i>=0;i--)
	{
		AcGePoint3dArray* pGroup=ptArrayGroup.at(i);
		delete pGroup;
		pGroup=NULL;
		ptArrayGroup.removeAt(i);
	}
}
bool GFrameBorderHelper::IsContain(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1)
{
	if(ptMin1.x<ptMin.x-1E-4||ptMin1.x>ptMax.x+1E-4)
	{
		return false;
	}
	if(ptMax1.x<ptMin.x-1E-4||ptMax1.x>ptMax.x+1E-4)
	{
		return false;
	}

	if(ptMin1.y<ptMin.y-1E-4||ptMin1.y>ptMax.y+1E-4)
	{
		return false;
	}
	if(ptMax1.y<ptMin.y-1E-4||ptMax1.y>ptMax.y+1E-4)
	{
		return false;
	}
	return true;

}
bool GFrameBorderHelper::IsSame(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1)
{
	if(ptMin1.x<ptMin.x-1E-4||ptMin1.x>ptMin.x+1E-4)
	{
		return false;
	}
	if(ptMax1.x<ptMax.x-1E-4||ptMax1.x>ptMax.x+1E-4)
	{
		return false;
	}

	if(ptMin1.y<ptMin.y-1E-4||ptMin1.y>ptMin.y+1E-4)
	{
		return false;
	}
	if(ptMax1.y<ptMax.y-1E-4||ptMax1.y>ptMax.y+1E-4)
	{
		return false;
	}
	return true;

}
void Connect(AcGePoint3dArray& ptMinArray,AcGePoint3dArray& ptMaxArray,AcGePoint3dArray* pts1,AcGePoint3dArray* pts2,AcGePoint3dArray* pts3,AcGePoint3dArray* pts4)
{
	for(int i1=0;i1<pts1->logicalLength();i1++)
	{//1
		AcGePoint3d ptHor1=pts1->at(i1);
		AcGePoint3d ptHor2=pts2->at(i1);
		double x1=ptHor1.x;
		double x2=ptHor2.x;
		double xMin1=min(x1,x2);
		double xMax1=max(x1,x2);
		double y1=ptHor1.y;

		for(int j1=0;j1<pts3->logicalLength();j1++)
		{//2
			AcGePoint3d ptVer1=pts3->at(j1);
			AcGePoint3d ptVer2=pts4->at(j1);
			double y3=ptVer1.y;
			double y4=ptVer2.y;
			double yMin2=min(y3,y4);
			double yMax2=max(y3,y4);
			double x3=ptVer1.x;

			if(fabs(x3-xMin1)>1E-4&&fabs(x3-xMax1)>1E-4)
			{
				continue;
			}

			if(fabs(y1-yMin2)>1E-4&&fabs(y1-yMax2)>1E-4)
			{
				continue;
			}


			for(int i2=i1+1;i2<pts1->logicalLength();i2++)
			{//3
				AcGePoint3d ptHor3=pts1->at(i2);
				AcGePoint3d ptHor4=pts2->at(i2);
				double y2=ptHor3.y;

				double yMin1=min(y1,y2);
				double yMax1=max(y1,y2);

				for(int j2=j1+1;j2<pts3->logicalLength();j2++)
				{//4
					AcGePoint3d ptVer3=pts3->at(j2);
					AcGePoint3d ptVer4=pts4->at(j2);
					double x4=ptVer3.x;

					double xMin2=min(x3,x4);
					double xMax2=max(x3,x4);

					if(fabs(xMin1-xMin2)>1E-4||
						fabs(xMax1-xMax2)>1E-4)
					{//5
						continue;
					}//5
					if(fabs(yMin1-yMin2)>1E-4||
						fabs(yMax1-yMax2)>1E-4)
					{//5
						continue;
					}//5
					ptMinArray.append(AcGePoint3d(xMin1,yMin1,0.0));
					ptMaxArray.append(AcGePoint3d(xMax1,yMax1,0.0));
				}//4
			}//3
		}//2
	}//1
}
//点是否在范围内
bool GFrameBorderHelper::IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext)
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
//判断上下左右关系
//0:未知,1:上,2:下,3:左,4:右
void SXZY(AcGePoint3d ptMin1,AcGePoint3d ptMax1,AcGePoint3d ptMin2,AcGePoint3d ptMax2,int& nSX,int& nZY)
{
	AcGePoint3d ptMid1=GCADUtils::MidPoint(ptMin1,ptMax1);
	AcGePoint3d ptMid2=GCADUtils::MidPoint(ptMin2,ptMax2);
	double dx=ptMid2.x-ptMid1.x;
	double dy=ptMid2.y-ptMid1.y;
	//左右关系为主
	if(ptMin2.x-ptMax1.x>1E-6)
	{
		nZY=4;
	}
	else if(ptMin1.x-ptMax2.x>1E-6)
	{
		nZY=3;
	}
	/*
	if(ptMax2.x-ptMax1.x>1E-6&&ptMin2.x-ptMin1.x>1E-6)
	{
		nZY=4;
	}
	if(ptMax1.x-ptMax2.x>1E-6&&ptMin1.x-ptMin2.x>1E-6)
	{
		nZY=3;
	}
	*/
	else
	{
		nZY=0;
	}
	//上下关系为主
	if(ptMin2.y-ptMax1.y>1E-6)
	{
		nSX=1;
	}
	else if(ptMin1.y-ptMax2.y>1E-6)
	{
		nSX=2;
	}
	/*
	if(ptMax2.y-ptMax1.y>1E-6&&ptMin2.y-ptMin1.y>1E-6)
	{
		nZY=1;
	}
	if(ptMax1.y-ptMax2.y>1E-6&&ptMin1.y-ptMin2.y>1E-6)
	{
		nZY=2;
	}
	*/
	else
	{
		nSX=0;
	}
}
bool GFrameBorderHelper::NeedSwap(AcGePoint3d ptMin1,AcGePoint3d ptMax1,AcGePoint3d ptMin2,AcGePoint3d ptMax2)
{
	int nSX=0;
	int nZY=0;
	SXZY(ptMin1,ptMax1,ptMin2,ptMax2,nSX,nZY);
	if(m_frame.m_nOrder==0)
	{
		//0:从左往右打印，至上而下打印
		if(nZY==3)
		{
			return true;
		}
		else if(nZY==4)
		{
			return false;
		}
		else
		{
			if(nSX==1)
			{
				return true;
			}
		}
	}
	else if(m_frame.m_nOrder==1)
	{
		//1:从上往下打印，至左而右打印
		if(nSX==1)
		{
			return true;
		}
		else if(nSX==2)
		{
			return false;
		}
		else
		{
			if(nZY==3)
			{
				return true;
			}
		}
	}
	return false;
}
int FindInAcArray(AcArray<int>& indexArray,int nValue)
{
	for(int i=0;i<indexArray.logicalLength();i++)
	{
		if(indexArray.at(i)==nValue)
		{
			return i;
		}
	}
	return -1;
}
void DeleteInAcArray(AcArray<int>& valueArray,AcArray<int>& indexArray)
{
	AcArray<int> tmpValueArray;
	for(int i=0;i<valueArray.logicalLength();i++)
	{
		if(FindInAcArray(indexArray,i)>=0)
		{
			continue;
		}
		tmpValueArray.append(valueArray.at(i));
	}
	valueArray.setLogicalLength(0);
	valueArray.append(tmpValueArray);
}
void DeleteInAcArray(AcGePoint3dArray& valueArray,AcArray<int>& indexArray)
{
	AcGePoint3dArray tmpValueArray;
	for(int i=0;i<valueArray.logicalLength();i++)
	{
		if(FindInAcArray(indexArray,i)>=0)
		{
			continue;
		}
		tmpValueArray.append(valueArray.at(i));
	}
	valueArray.setLogicalLength(0);
	valueArray.append(tmpValueArray);
}
//打印对象排序
void GFrameBorderHelper::SortOrder(AcArray<int>& indexArray,AcGePoint3dArray& ptMinArray,AcGePoint3dArray& ptMaxArray)
{
	indexArray.setLogicalLength(0);
	for(int i=0;i<ptMinArray.logicalLength();i++)
	{
		indexArray.append(i);
	}
	if(m_frame.m_nOrder==0)//1
	{
		int nCount=indexArray.logicalLength();
		//0:从左往右打印，至上而下打印
		AcArray<int> orderIndexArray;
		AcGePoint3dArray orderPtMinArray;
		AcGePoint3dArray orderPtMaxArray;
		while(orderIndexArray.logicalLength()!=nCount)//2
		{
			//寻找最左边的图纸
			int nLeftMostIndex=-1;
			double xLeftMost=0.0;
			for(int i=0;i<ptMinArray.logicalLength();i++)
			{
				if(nLeftMostIndex<0||ptMinArray.at(i).x<xLeftMost)
				{
					nLeftMostIndex=i;
					xLeftMost=ptMinArray.at(i).x;
				}
			}
			//寻找和最左边图纸并排的图纸
			double xMin=ptMinArray.at(nLeftMostIndex).x;
			double xMax=ptMaxArray.at(nLeftMostIndex).x;

			AcArray<int> bpIndexArray;//并排的图纸编号

			for(int i=0;i<ptMinArray.logicalLength();i++)
			{
				if(nLeftMostIndex==i)
				{
					continue;
				}
				double xMin1=ptMinArray.at(i).x;
				double xMax1=ptMaxArray.at(i).x;
				if((xMin1>=xMin-1E-6&&xMin1<=xMax+1E-6)||
					(xMax1>=xMin-1E-6&&xMax1<=xMax+1E-6))
				{
					bpIndexArray.append(i);
				}
			}
			bpIndexArray.append(nLeftMostIndex);

			//排除处于右边的图纸
			AcArray<int> bpIndexArray1;//并排的图纸编号(去掉右边的图纸)
			for(int i=0;i<bpIndexArray.logicalLength();i++)
			{
				bool bRight=false;
				for(int j=0;j<bpIndexArray.logicalLength();j++)
				{
					if(i==j)
					{
						continue;
					}
					int nSX=0;
					int nZY=0;
					SXZY(ptMinArray.at(bpIndexArray.at(i)),ptMaxArray.at(bpIndexArray.at(i)),ptMinArray.at(bpIndexArray.at(j)),ptMaxArray.at(bpIndexArray.at(j)),nSX,nZY);
					if(nZY==3)
					{
						//j在i的左边
						bRight=true;
						break;
					}
				}
				if(!bRight)
				{
					bpIndexArray1.append(bpIndexArray.at(i));
				}
			}

			//已经确定好并排的这些图纸进行排序
			//yMax越大的越先打印
			for(int i=0;i<bpIndexArray1.logicalLength()-1;i++)
			{
				double yMax=ptMaxArray.at(bpIndexArray1.at(i)).y;
				for(int j=i+1;j<bpIndexArray1.logicalLength();j++)
				{
					double yMax1=ptMaxArray.at(bpIndexArray1.at(j)).y;
					if(yMax1>yMax)
					{
						//j要比i先打印
						int nSwap=bpIndexArray1.at(i);
						bpIndexArray1.setAt(i,bpIndexArray1.at(j));
						bpIndexArray1.setAt(j,nSwap);
						yMax=yMax1;
					}
				}
			}

			//将排序好的放到打印列表里面
			for(int i=0;i<bpIndexArray1.logicalLength();i++)
			{
				int nIndex=bpIndexArray1.at(i);
				orderIndexArray.append(indexArray.at(nIndex));
				orderPtMinArray.append(ptMinArray.at(nIndex));
				orderPtMaxArray.append(ptMaxArray.at(nIndex));
			}

			//将排序好的从还需要排序的列表中删除
			DeleteInAcArray(indexArray,bpIndexArray1);
			DeleteInAcArray(ptMinArray,bpIndexArray1);
			DeleteInAcArray(ptMaxArray,bpIndexArray1);

		}//2
		ptMinArray.append(orderPtMinArray);
		ptMaxArray.append(orderPtMaxArray);
		indexArray.append(orderIndexArray);
	}//1
	else if(m_frame.m_nOrder==1)//1
	{
		//1:从上往下打印，至左而右打印
		int nCount=indexArray.logicalLength();
		AcArray<int> orderIndexArray;
		AcGePoint3dArray orderPtMinArray;
		AcGePoint3dArray orderPtMaxArray;
		while(orderIndexArray.logicalLength()!=nCount)//2
		{
			//寻找最上边的图纸
			int nTopMostIndex=-1;
			double yTopMost=0.0;
			for(int i=0;i<ptMinArray.logicalLength();i++)
			{
				if(nTopMostIndex<0||ptMaxArray.at(i).y>yTopMost)
				{
					nTopMostIndex=i;
					yTopMost=ptMaxArray.at(i).y;
				}
			}
			//寻找和最左边图纸并排的图纸
			double yMin=ptMinArray.at(nTopMostIndex).y;
			double yMax=ptMaxArray.at(nTopMostIndex).y;

			AcArray<int> bpIndexArray;//并排的图纸编号

			for(int i=0;i<ptMinArray.logicalLength();i++)
			{
				if(nTopMostIndex==i)
				{
					continue;
				}
				double yMin1=ptMinArray.at(i).y;
				double yMax1=ptMaxArray.at(i).y;
				if((yMin1>=yMin-1E-6&&yMin1<=yMax+1E-6)||
					(yMax1>=yMin-1E-6&&yMax1<=yMax+1E-6))
				{
					bpIndexArray.append(i);
				}
			}
			bpIndexArray.append(nTopMostIndex);

			//排除处于下边的图纸
			AcArray<int> bpIndexArray1;//并排的图纸编号(去掉下边的图纸)
			for(int i=0;i<bpIndexArray.logicalLength();i++)
			{
				bool bBottom=false;
				for(int j=0;j<bpIndexArray.logicalLength();j++)
				{
					if(i==j)
					{
						continue;
					}
					int nSX=0;
					int nZY=0;
					SXZY(ptMinArray.at(bpIndexArray.at(i)),ptMaxArray.at(bpIndexArray.at(i)),ptMinArray.at(bpIndexArray.at(j)),ptMaxArray.at(bpIndexArray.at(j)),nSX,nZY);
					if(nSX==1)
					{
						//j在i的上边
						bBottom=true;
						break;
					}
				}
				if(!bBottom)
				{
					bpIndexArray1.append(bpIndexArray.at(i));
				}
			}

			//已经确定好并排的这些图纸进行排序
			//xMin越小的越先打印
			for(int i=0;i<bpIndexArray1.logicalLength()-1;i++)
			{
				double xMin=ptMinArray.at(bpIndexArray1.at(i)).x;
				for(int j=i+1;j<bpIndexArray1.logicalLength();j++)
				{
					double xMin1=ptMinArray.at(bpIndexArray1.at(j)).x;
					if(xMin1<xMin)
					{
						//j要比i先打印
						int nSwap=bpIndexArray1.at(i);
						bpIndexArray1.setAt(i,bpIndexArray1.at(j));
						bpIndexArray1.setAt(j,nSwap);
						xMin=xMin1;
					}
				}
			}

			//将排序好的放到打印列表里面
			for(int i=0;i<bpIndexArray1.logicalLength();i++)
			{
				int nIndex=bpIndexArray1.at(i);
				orderIndexArray.append(indexArray.at(nIndex));
				orderPtMinArray.append(ptMinArray.at(nIndex));
				orderPtMaxArray.append(ptMaxArray.at(nIndex));
			}

			//将排序好的从还需要排序的列表中删除
			DeleteInAcArray(indexArray,bpIndexArray1);
			DeleteInAcArray(ptMinArray,bpIndexArray1);
			DeleteInAcArray(ptMaxArray,bpIndexArray1);

		}//2
		ptMinArray.append(orderPtMinArray);
		ptMaxArray.append(orderPtMaxArray);
		indexArray.append(orderIndexArray);
	}//1

}
//获取矩形
void GFrameBorderHelper::GetRects(CStringArray& strNameArray,AcGePoint3dArray& ptMinArray,AcGePoint3dArray& ptMaxArray,AcDbObjectIdArray& ids)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcGePoint3dArray ptTmpArray1;
	AcGePoint3dArray ptTmpArray2;
	if(m_frame.m_bBREF)
	{
		for(int i=0;i<ids.logicalLength();i++)
		{
			AcDbObjectId& id=ids.at(i);
			AcDbEntityPointer spEnt(id,AcDb::kForRead);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			if(!spEnt->isKindOf(AcDbBlockReference::desc()))
			{
				continue;
			}
			AcDbExtents brefExt;
			es=spEnt->getGeomExtents(brefExt);
			if(es!=Acad::eOk)
			{
				continue;
			}
			if(m_frame.IsFrame(AcDbBlockReference::cast(spEnt.object())))
			{
				CString strBlockName=GetBlockName(AcDbBlockReference::cast(spEnt.object()));
				ptMinArray.append(brefExt.minPoint());
				ptMaxArray.append(brefExt.maxPoint());
				strNameArray.Add(strBlockName);
			}
		}
		return;
	}
	
	//暂时先不考虑
	if(ptMinArray.logicalLength()>0&&ptMaxArray.logicalLength()>0)
	{
		return;
	}
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId& id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef=AcDbBlockReference::cast(spEnt.object());
			AcDbVoidPtrArray entitySet;
			es=pRef->explode(entitySet);
			if(es==Acad::eOk)
			{
				for(int i=0;i<entitySet.logicalLength();i++)
				{
					AcDbEntity* pEnt1=(AcDbEntity*)entitySet.at(i);
					if(!m_frame.IsFrameX(pEnt1))
					{
						continue;
					}
					if(pEnt1->isKindOf(AcDbPolyline::desc()))
					{
						AcGePoint3d ptMin;
						AcGePoint3d ptMax;
						if(GetRects(ptMin,ptMax,AcDbPolyline::cast(pEnt1)))
						{
							ptMinArray.append(ptMin);
							ptMaxArray.append(ptMax);
						}

					}
					else if(pEnt1->isKindOf(AcDbLine::desc()))
					{
						AcDbLine* pLine=AcDbLine::cast(pEnt1);
						AcGePoint3d pt1=pLine->startPoint();
						AcGePoint3d pt2=pLine->endPoint();
						if((fabs(pt1.x-pt2.x)<1E-4&&fabs(pt1.y-pt2.y)>1E-4)||
							(fabs(pt1.y-pt2.y)<1E-4&&fabs(pt1.x-pt2.x)>1E-4))
						{
							ptTmpArray1.append(pt1);
							ptTmpArray2.append(pt2);
						}
					}
				}
				for(int i=0;i<entitySet.logicalLength();i++)
				{
					AcDbEntity* pEnt1=(AcDbEntity*)entitySet.at(i);
					delete pEnt1;
				}
			}
		}
		else
		{
			if(!m_frame.IsFrameX(spEnt.object()))
			{
				continue;
			}
			if(spEnt->isKindOf(AcDbPolyline::desc()))
			{
				AcGePoint3d ptMin;
				AcGePoint3d ptMax;
				if(GetRects(ptMin,ptMax,AcDbPolyline::cast(spEnt.object())))
				{
					ptMinArray.append(ptMin);
					ptMaxArray.append(ptMax);
				}

			}
			else if(spEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pLine=AcDbLine::cast(spEnt.object());
				AcGePoint3d pt1=pLine->startPoint();
				AcGePoint3d pt2=pLine->endPoint();
				if((fabs(pt1.x-pt2.x)<1E-4&&fabs(pt1.y-pt2.y)>1E-4)||
					(fabs(pt1.y-pt2.y)<1E-4&&fabs(pt1.x-pt2.x)>1E-4))
				{
					ptTmpArray1.append(pt1);
					ptTmpArray2.append(pt2);
				}
			}
			else
			{
				continue;
			}
		}
		
	}
	//将直线连成矩形
	//1:拆分成横向的直线和纵向的直线
	AcGePoint3dArray ptHorTmpArray1;//横向
	AcGePoint3dArray ptHorTmpArray2;//横向

	AcGePoint3dArray ptVerTmpArray1;//纵向
	AcGePoint3dArray ptVerTmpArray2;//纵向
	for(int i=ptTmpArray1.logicalLength()-1;i>=0;i--)
	{
		AcGePoint3d pt1=ptTmpArray1.at(i);
		AcGePoint3d pt2=ptTmpArray2.at(i);
		if(fabs(pt1.x-pt2.x)<1E-4&&fabs(pt1.y-pt2.y)>1E-4)
		{
			ptVerTmpArray1.append(pt1);
			ptVerTmpArray2.append(pt2);
		}
		else
		{
			ptHorTmpArray1.append(pt1);
			ptHorTmpArray2.append(pt2);
		}

	}

	//横向和纵向的线分别分组
	AcArray<AcGePoint3dArray*> ptHorArrayGroup1;
	AcArray<AcGePoint3dArray*> ptHorArrayGroup2;
	TwoLines(ptHorArrayGroup1,ptHorArrayGroup2,ptHorTmpArray1,ptHorTmpArray2,true);
	AcArray<AcGePoint3dArray*> ptVerArrayGroup1;
	AcArray<AcGePoint3dArray*> ptVerArrayGroup2;
	TwoLines(ptVerArrayGroup1,ptVerArrayGroup2,ptVerTmpArray1,ptVerTmpArray2,false);
	DWORD t1=GetTickCount();

	//将分组后的线连接起来
	for(int i=0;i<ptHorArrayGroup1.logicalLength();i++)
	{
		AcGePoint3dArray* pts1=ptHorArrayGroup1.at(i);
		AcGePoint3dArray* pts2=ptHorArrayGroup2.at(i);
		//和纵向的线进行匹配
		for(int j=0;j<ptVerArrayGroup1.logicalLength();j++)
		{
			AcGePoint3dArray* pts3=ptVerArrayGroup1.at(j);
			AcGePoint3dArray* pts4=ptVerArrayGroup2.at(j);
			Connect(ptMinArray,ptMaxArray,pts1,pts2,pts3,pts4);
		}
	}
	DWORD t2=GetTickCount();
	Release(ptHorArrayGroup1);
	Release(ptHorArrayGroup2);
	Release(ptVerArrayGroup1);
	Release(ptVerArrayGroup2);
	ptTmpArray1.setLogicalLength(0);
	ptTmpArray2.setLogicalLength(0);
	//删除完全相同的
	for(int i=0;i<ptMinArray.logicalLength();i++)
	{
		bool isSame=false;
		for(int j=0;j<ptTmpArray1.logicalLength();j++)
		{
			isSame=IsSame(ptMinArray.at(i),ptMaxArray.at(i),ptTmpArray1.at(j),ptTmpArray2.at(j));
			if(isSame)
			{
				break;
			}
		}
		if(isSame)
		{
			continue;
		}
		ptTmpArray1.append(ptMinArray.at(i));
		ptTmpArray2.append(ptMaxArray.at(i));
	}
	ptMinArray.setLogicalLength(0);
	ptMaxArray.setLogicalLength(0);
	ptMinArray.append(ptTmpArray1);
	ptMaxArray.append(ptTmpArray2);
	ptTmpArray1.setLogicalLength(0);
	ptTmpArray2.setLogicalLength(0);
	AcArray<int> indexTmpArray;
	//删除被包含的
	for(int i=0;i<ptMinArray.logicalLength();i++)
	{
		bool bContain=false;
		for(int j=0;j<ptMinArray.logicalLength();j++)
		{
			if(i==j)
			{
				continue;
			}
			if(IsContain(ptMinArray.at(j),ptMaxArray.at(j),ptMinArray.at(i),ptMaxArray.at(i)))
			{
				bContain=true;
				break;
			}
		}
		if(!bContain)
		{
			ptTmpArray1.append(ptMinArray.at(i));
			ptTmpArray2.append(ptMaxArray.at(i));
			indexTmpArray.append(i);
		}
	}
	for(int i=ptTmpArray1.logicalLength()-1;i>=0;i--)
	{
		bool bFind=false;
		for(int j=0;j<ptMinArray.logicalLength();j++)
		{
			if(indexTmpArray.at(i)==j)
			{
				//去掉自己本身
				continue;
			}
			if(IsInnerBorder(ptTmpArray1.at(i),ptTmpArray2.at(i),ptMinArray.at(j),ptMaxArray.at(j)))
			{
				bFind=true;
				break;
			}
		}
		if(bFind)
		{
			continue;
		}
		ptTmpArray1.removeAt(i);
		ptTmpArray2.removeAt(i);
	}
	ptMinArray.setLogicalLength(0);
	ptMaxArray.setLogicalLength(0);
	ptMinArray.append(ptTmpArray1);
	ptMaxArray.append(ptTmpArray2);
	ptTmpArray1.setLogicalLength(0);
	ptTmpArray2.setLogicalLength(0);
	AcArray<int> indexArray;
	SortOrder(indexArray,ptMinArray,ptMaxArray);
	for(int i=0;i<ptMinArray.logicalLength();i++)
	{
		CString strName;
		strName.Format(_T("图框_%d"),i+1);
		strNameArray.Add(strName);
	}

	if(ptMinArray.logicalLength()<=0)
	{
		return;
	}
}
//是否是内边框
//(ptMin1,ptMax1)是否是(ptMin,ptMax)的内边框
bool GFrameBorderHelper::IsInnerBorder(AcGePoint3d ptMin,AcGePoint3d ptMax,AcGePoint3d ptMin1,AcGePoint3d ptMax1)
{
	if(!IsContain(ptMin,ptMax,ptMin1,ptMax1))
	{
		return false;
	}
	double dWidth=ptMax.x-ptMin.x;

	double dHeight=ptMax.y-ptMin.y;

	double dLeft=ptMin1.x-ptMin.x;

	double dRight=ptMax.x-ptMax1.x;

	double dTop=ptMax.y-ptMax1.y;

	double dBottom=ptMin1.y-ptMin.y;

	double dMin=min(dLeft,dRight);
	double dMax=max(dLeft,dRight);

	if(dMin>dWidth*0.10||dMax>dWidth*0.30)
	{
		return false;
	}

	dMin=min(dTop,dBottom);
	dMax=max(dTop,dBottom);

	if(dMin>dHeight*0.20||dMax>dWidth*0.30)
	{
		return false;
	}
	return true;

}
