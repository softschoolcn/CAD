#include "StdAfx.h"
#include "GlobalFunction.h"

//全局函数
void NumCanPlus(CString& strText,CArray<int>& startIndexArray,CArray<int>& endIndexArray)
{
	int nLen=lstrlen(strText);
	for(int i=0;i<nLen;i++)
	{
		TCHAR c=strText.GetAt(i);
		if(c<='9'&&c>='0')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='9'&&c>='0')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}

		else if(c<='Z'&&c>='A')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='Z'&&c>='A')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}

		else if(c<='z'&&c>='a')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='z'&&c>='a')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}
	}
}
//文字递增
//bIncrease:表示是否递增
//bIncrease=true,0->9->10
//bIncrease=false,0->9->0
BOOL NumPlus(CString& strText,int nNum,bool bIncrease)
{
	if(strText.IsEmpty())
	{
		return FALSE;
	}
	int nType=-1;
	for(int i=0;i<lstrlen(strText);i++)
	{
		TCHAR c=strText.GetAt(i);
		if(c<='9'&&c>='0')
		{
			if(nType==1||nType==2)
			{
				return FALSE;
			}
			nType=0;
		}
		else if(c>='A'&&c<='Z')
		{
			if(nType==0||nType==2)
			{
				return FALSE;
			}
			nType=1;
		}
		else if(c>='a'&&c<='z')
		{
			if(nType==0||nType==1)
			{
				return FALSE;
			}
			nType=2;
		}
		else
		{
			return FALSE;
		}

	}
	TCHAR cMin='0';
	TCHAR cMax='9';
	if(nType==1)
	{
		cMin='A';
		cMax='Z';
	}
	else if(nType==2)
	{
		cMin='a';
		cMax='z';
	}
	for(int i=0;i<nNum;i++)
	{
		TCHAR c1=strText.GetAt(lstrlen(strText)-1);
		if(c1<cMax)
		{
			strText.SetAt(lstrlen(strText)-1,c1+1);
		}
		else
		{
			bool bAdd=false;//是否已经增加好了
			strText.SetAt(lstrlen(strText)-1,cMin);
			for(int j=lstrlen(strText)-2;j>=0;j--)
			{
				c1=strText.GetAt(j);
				if(c1<cMax)
				{
					strText.SetAt(j,c1+1);
					bAdd=true;
					break;
				}
				else
				{
					strText.SetAt(j,cMin);
				}
			}
			if(!bAdd)
			{
				if(bIncrease)
				{
					if(nType==1||nType==2)
					{
					strText=cMin+strText;
					}
					else
					{
						strText=_T("1")+strText;
					}
				}
			}
		}
	}
	return TRUE;
}

bool IsXor(long nState,const long nPropState)
{
	if((nState&nPropState)==nPropState)
	{
		return true;
	}
	else
	{
		return false;
	}
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
Acad::ErrorStatus CreatePolylineRgn(AcDbRegion*& pRegion,AcGePoint3dArray& pts)
{
	AcDbPolyline* pLine=new AcDbPolyline(pts.logicalLength());
	for(int i=0;i<pts.logicalLength();i++)
	{
		pLine->addVertexAt(i,AcGePoint2d(pts.at(i).x,pts.at(i).y));
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

//拷贝文字到剪贴板中
BOOL CopyTextToClipboard(CString strText,HWND hWnd)
{
	BOOL rc=OpenClipboard(hWnd);
	if(!rc)
		return rc;
	EmptyClipboard();
	char* szBuf=NULL;
	GStringHelper::ConvertToLPCSTR(szBuf,strText);
	DWORD cch=strlen(szBuf);
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
		(cch + 1) * sizeof(char));
	if(hglbCopy==NULL)
	{
		free(szBuf);
		CloseClipboard();
		return FALSE;
	}
	char* pCopy=(char*)GlobalLock(hglbCopy);
	strcpy(pCopy,szBuf);
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	free(szBuf);
	CloseClipboard();
	return TRUE;
}
