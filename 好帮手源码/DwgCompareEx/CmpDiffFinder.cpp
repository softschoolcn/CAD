#include "StdAfx.h"
#include "CmpDiffFinder.h"
#include "CommCommand.h"
#include "AcProgressBar.h"
CmpDiffFinder::CmpDiffFinder(CmpConfig* pConfig)
{
	m_pConfig=pConfig;
	strName=_T("");
}

CmpDiffFinder::~CmpDiffFinder(void)
{
}
BOOL CmpDiffFinder::IsDiffType(AcRxClass* pType1,AcRxClass* pType2)
{
	if(pType1==pType2)
		return FALSE;
	if(lstrcmp(pType1->appName(),pType2->appName())!=0)
		return TRUE;
	if(lstrcmp(pType1->dxfName(),pType2->dxfName())!=0)
		return TRUE;
	if(lstrcmp(pType1->name(),pType2->name())!=0)
		return TRUE;
	/*
	int dwgVer1;
	int maintVer1;
	pType1->getClassVersion(dwgVer1,maintVer1);
	int dwgVer2;
	int maintVer2;
	pType2->getClassVersion(dwgVer2,maintVer2);
	if(dwgVer1!=dwgVer2)
	return TRUE;
	if(maintVer1!=maintVer2)
	return TRUE;
	*/
	return FALSE;
}
BOOL CmpDiffFinder::_IsDiff(struct resbuf* rb1,struct resbuf* rb2,BOOL ingoreNoCase)
{
	if(rb1==NULL&&rb2==NULL)
		return FALSE;
	if(rb1==NULL&&rb2!=NULL)
		return TRUE;
	if(rb1!=NULL&&rb2==NULL)
		return TRUE;
	if(rb1->restype!=rb2->restype)
		return TRUE;
	int dataType = dxfCodeToDataType(rb1->restype);
	if(dataType==RTSHORT)
	{
		return rb1->resval.rint!=rb2->resval.rint;
	}
	else if(dataType==RTLONG)
	{
		return rb1->resval.rlong!=rb2->resval.rlong;
	}
	else if(dataType==RTREAL)
	{
		return fabs(rb1->resval.rreal-rb2->resval.rreal)>1E-8;
	}
	else if(dataType==RTSTR)
	{
		if(ingoreNoCase)
		{
			return lstrcmpi(rb1->resval.rstring,rb2->resval.rstring)!=0;
		}
		else
		{
		return lstrcmp(rb1->resval.rstring,rb2->resval.rstring)!=0;
		}
	}
	else if(dataType==RT3DPOINT)
	{
		return (fabs(rb1->resval.rpoint[X]-rb2->resval.rpoint[X])>1E-8||fabs(rb1->resval.rpoint[Y]-rb2->resval.rpoint[Y])>1E-8||fabs(rb1->resval.rpoint[Z]-rb2->resval.rpoint[Z])>1E-8);
	}
	else
	{
		return FALSE;
	}
}
BOOL CmpDiffFinder::IsDiff(CmpItem* pItem,CArray<int>& ingoreDxfCodes,struct resbuf* rb1,struct resbuf* rb2,CmpDiffResult& result)
{
	for(long i=0;i<pItem->m_dxfCodes.GetCount();i++)
	{
		int nDxfCode=pItem->m_dxfCodes.GetAt(i);
		BOOL IsIngore=FALSE;
		for(long l=0;l<ingoreDxfCodes.GetCount();l++)
		{
			if(ingoreDxfCodes.GetAt(l)==nDxfCode)
			{
				IsIngore=TRUE;
				break;
			}
		}
		if(IsIngore)
		{
			continue;
		}
		if(pItem->m_bOnlyOne)
		{
			struct resbuf* rb1_value=NULL;
			struct resbuf* pTmp=rb1;
			while(pTmp!=NULL)
			{
				if(pTmp->restype==nDxfCode)
				{
					rb1_value=pTmp;
					break;
				}
				pTmp=pTmp->rbnext;
			}
			struct resbuf* rb2_value=NULL;
			pTmp=rb2;
			while(pTmp!=NULL)
			{
				if(pTmp->restype==nDxfCode)
				{
					rb2_value=pTmp;
					break;
				}
				pTmp=pTmp->rbnext;
			}
			BOOL bDiff=_IsDiff(rb1_value,rb2_value,pItem->m_bIngoreNoCase);
			if(bDiff)
			{
				CmpDiffItem item;
				item.m_bOnlyOne=pItem->m_bOnlyOne;
				item.nDxfCode=nDxfCode;
				item.strName=pItem->m_strName;
				item.strOldValue=dxfToStr(rb1_value);
				item.strNewValue=dxfToStr(rb2_value);
				result.m_diffItems.Add(item);
			}
			return bDiff;
		}
		else
		{
			CArray<struct resbuf*> rb1_value_array;
			struct resbuf* pTmp=rb1;
			while(pTmp!=NULL)
			{
				if(pTmp->restype==nDxfCode)
				{
					rb1_value_array.Add(pTmp);
				}
				pTmp=pTmp->rbnext;
			}
			CArray<struct resbuf*> rb2_value_array;
			pTmp=rb2;
			while(pTmp!=NULL)
			{
				if(pTmp->restype==nDxfCode)
				{
					rb2_value_array.Add(pTmp);
				}
				pTmp=pTmp->rbnext;
			}
			if(rb1_value_array.GetCount()!=rb2_value_array.GetCount())
			{
				CmpDiffItem item;
				item.nDxfCode=nDxfCode;
				item.strName=pItem->m_strName;
				item.strOldValue=dxfToStr(rb1_value_array,item.strOldValueArray);
				item.strNewValue=dxfToStr(rb2_value_array,item.strOldValueArray);
				item.m_bOnlyOne=pItem->m_bOnlyOne;
				result.m_diffItems.Add(item);
				return TRUE;
			}
			if(!pItem->m_bIngoreOrder)
			{
				for(long j=0;j<rb1_value_array.GetCount();j++)
				{
					BOOL bDiff=_IsDiff(rb1_value_array.GetAt(j),rb2_value_array.GetAt(j),pItem->m_bIngoreNoCase);
					if(bDiff)
					{
						CmpDiffItem item;
						item.nDxfCode=nDxfCode;
						item.strName=pItem->m_strName;
						item.m_bOnlyOne=pItem->m_bOnlyOne;
						item.strOldValue=dxfToStr(rb1_value_array,item.strOldValueArray);
						item.strNewValue=dxfToStr(rb2_value_array,item.strNewValueArray);
						result.m_diffItems.Add(item);
						return TRUE;
					}
				}
			}
			else
			{
				while(rb1_value_array.GetCount()!=0)
				{
					struct resbuf* pTmp1=rb1_value_array.GetAt(0);
					BOOL bFind=FALSE;
					for(long j=0;j<rb2_value_array.GetCount();j++)
					{
						BOOL bDiff=_IsDiff(pTmp1,rb2_value_array.GetAt(j),pItem->m_bIngoreNoCase);
						if(!bDiff)
						{
							bFind=TRUE;
							rb2_value_array.RemoveAt(j);
							break;
						}
					}
					if(!bFind)
					{
						CmpDiffItem item;
						item.nDxfCode=nDxfCode;
						item.strName=pItem->m_strName;
						item.strOldValue=dxfToStr(rb1_value_array,item.strOldValueArray);
						item.strNewValue=dxfToStr(rb2_value_array,item.strNewValueArray);
						item.m_bOnlyOne=pItem->m_bOnlyOne;
						result.m_diffItems.Add(item);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
CmpType* CmpDiffFinder::GetCmpType(AcRxClass* pType)
{
	if(pType==NULL)
		return NULL;
	CmpType* pTmpType=NULL;
	if(m_TypeDict.Lookup(pType->name(),pTmpType))
	{
		return pTmpType;
	}
	for(long i=m_pConfig->m_nCmpTypeCount-1;i>=0;i--)
	{
		CmpType* pCmpType=m_pConfig->m_cmpTypes[i];
		if(pCmpType->m_strClassName.Compare(pType->name())==0)
		{
			m_TypeDict[pType->name()]=pCmpType;
			return pCmpType;
		}
	}
	if(lstrcmp(pType->name(),_T("AcDbEntity"))==0)
	{
		m_TypeDict[pType->name()]=NULL;
		return NULL;
	}
	return GetCmpType(pType->myParent());
}
BOOL CmpDiffFinder::NeedSwap(CmpEntityDat* dat1,CmpEntityDat* dat2)
{
	if(!dat1->m_bNotEnt)
	{
		if((!dat1->m_strName.IsEmpty())&&(!dat2->m_strName.IsEmpty()))
		{
			return dat1->m_strName.Compare(dat2->m_strName)>0;
		}
		if(m_pConfig->m_nMode==0)
		{
			//位置模式
			AcGePoint3d minPoint1=dat1->m_ext.minPoint();

			AcGePoint3d minPoint2=dat2->m_ext.minPoint();

			AcGePoint2d minPoint1_2d=minPoint1.convert2d(AcGePlane());

			AcGePoint2d minPoint2_2d=minPoint2.convert2d(AcGePlane());

			return minPoint1_2d.x>minPoint2_2d.x;
		}
		else
		{
			//句柄模式
			return dat1->m_strHandle.Compare(dat2->m_strHandle)>0;

		}
	}
	else
	{
		return dat1->m_strName.Compare(dat2->m_strName)>0;
	}
}
BOOL CmpDiffFinder::IsOneObj(CmpEntityDat* dat1,CmpEntityDat* dat2)
{
	if(!dat1->m_bNotEnt)
	{
		if((!dat1->m_strName.IsEmpty())&&(!dat2->m_strName.IsEmpty()))
		{
			return dat1->m_strName==dat2->m_strName;
		}
		if(m_pConfig->m_nMode==0)
		{
			//位置模式
			AcGePoint3d minPoint1=dat1->m_ext.minPoint();
			AcGePoint3d maxPoint1=dat1->m_ext.maxPoint();

			AcGePoint3d minPoint2=dat2->m_ext.minPoint();
			AcGePoint3d maxPoint2=dat2->m_ext.maxPoint();

			AcGePoint2d minPoint1_2d=minPoint1.convert2d(AcGePlane());
			AcGePoint2d maxPoint1_2d=maxPoint1.convert2d(AcGePlane());

			AcGePoint2d minPoint2_2d=minPoint2.convert2d(AcGePlane());
			AcGePoint2d maxPoint2_2d=maxPoint2.convert2d(AcGePlane());
			if(minPoint1_2d.distanceTo(minPoint2_2d)>1E-3)
				return FALSE;
			if(maxPoint1_2d.distanceTo(maxPoint2_2d)>1E-3)
				return FALSE;
			if(IsDiffType(dat1->m_type,dat2->m_type))
			{
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			//句柄模式
			if(IsDiffType(dat1->m_type,dat2->m_type))
			{
				return FALSE;
			}
			return dat1->m_strHandle.CompareNoCase(dat2->m_strHandle)==0;
		}
	}
	else
	{
		return dat1->m_strName==dat2->m_strName;
	}

}
BOOL CmpDiffFinder::IsDiff(CmpEntityDat* dat1,CmpEntityDat* dat2,CmpDiffResult& result)
{
	if(IsDiffType(dat1->m_type,dat2->m_type))
	{
		return TRUE;
	}
	CmpType* pCmpType=GetCmpType(dat1->m_type);
	if(pCmpType==NULL)
		return FALSE;//不需要比较的类型
	CArray<int> ingoreDxfCodes;
	CArray<CmpType*> pCmpTypes;
	if(pCmpType->m_bActive)
	{
		ingoreDxfCodes.Append(pCmpType->m_IngoreDxfCodes);
		pCmpTypes.Add(pCmpType);
		while(pCmpType->m_pParType!=NULL)
		{
			pCmpType=pCmpType->m_pParType;
			if(pCmpType->m_bActive)
			{
				if(pCmpType->m_nCmpItemsCount>0)
				{
					pCmpTypes.Add(pCmpType);
				}
				ingoreDxfCodes.Append(pCmpType->m_IngoreDxfCodes);
			}
		}
	}
	if(pCmpTypes.GetCount()==0)
		return FALSE;
	ads_name ent;
	Acad::ErrorStatus es=acdbGetAdsName(ent,dat1->m_id);
	if(es!=eOk)
	{
		return FALSE;
	}
	struct resbuf* rb1=acdbEntGet(ent);
	if(rb1==NULL)
	{
		return FALSE;
	}
	es=acdbGetAdsName(ent,dat2->m_id);
	if(es!=eOk)
	{
		acutRelRb(rb1);
		return FALSE;
	}
	struct resbuf* rb2=acdbEntGet(ent);
	if(rb2==NULL)
	{
		acutRelRb(rb1);
		return FALSE;
	}
	BOOL bDiff=FALSE;
	for(long j=pCmpTypes.GetCount()-1;j>=0;j--)
	{
		pCmpType=pCmpTypes.GetAt(j);
		for(long i=0;i<pCmpType->m_nCmpItemsCount;i++)
		{
			CmpItem* pItem=&pCmpType->m_cmpItems[i];
			if(!pItem->m_bActive)
				continue;
			if(IsDiff(pItem,ingoreDxfCodes,rb1,rb2,result))
			{
				bDiff=TRUE;
			}
		}

	}



	acutRelRb(rb1);
	acutRelRb(rb2);


	return bDiff;
}
void CmpDiffFinder::AddSubEntityToResult(CArray<CmpEntityDat*>& dats,int nType,CArray<CmpDiffResult>& diffResults,long& diffCount)
{
	for(long i=0;i<dats.GetCount();i++)
	{
		CmpEntityDat* dat=dats[i];
		CmpDiffResult result;
		result.nType=nType;
		CmpType* pCmpType=GetCmpType(dat->m_type);
		if(nType==3)
		{
			//删除
			result.strHandle1=dat->m_strHandle;
			result.strName=pCmpType==NULL?_T("未知类型"):pCmpType->m_strName;
		}
		else if(nType==2)
		{
			//新增
			result.strHandle2=dat->m_strHandle;
			result.strName=pCmpType==NULL?_T("未知类型"):pCmpType->m_strName;
		}


		diffResults.Add(result);
		diffCount++;
		AddSubEntityToResult(dat->subEntityDats,nType,diffResults,diffCount);
	}
}
void CmpDiffFinder::DiffSubEntity(CArray<CmpEntityDat*>& dats1,CArray<CmpEntityDat*>& dats2,CArray<CmpDiffResult>& diffResults,long& diffCount)
{
	CmpDiffFinder finder(m_pConfig);
	finder.m_cmpDat[0].Append(dats1);
	finder.m_cmpDat[1].Append(dats2);
	finder.Find(diffResults,diffCount,false);
	//finder.Free();//最后整体释放，否则可能导致释放两次
}
//从小排到大
void SortByHandle(CArray<CmpEntityDat*>*        m_cmpDat)
{
	for(long i=0;i<m_cmpDat->GetCount()-1;i++)
	{
		CmpEntityDat* pDat1=m_cmpDat->GetAt(i);
		for(long j=i+1;j<m_cmpDat->GetCount();j++)
		{
			CmpEntityDat* pDat2=m_cmpDat->GetAt(j);
			if(pDat1->m_strHandle.Compare(pDat2->m_strHandle)>0)
			{
				m_cmpDat->SetAt(i,pDat2);
				m_cmpDat->SetAt(j,pDat1);
			}
		}
	}
}
Acad::ErrorStatus CmpDiffFinder::Find(CArray<CmpDiffResult>& diffResults,long& diffCount,bool bRelease,AcProgressBar* pBar)
{
	//pBar,外部传递过来的进度条
	int rc=0;
	BOOL bFind=FALSE;
	AcProgressBar proBar;
	long nCount1=m_cmpDat[0].GetCount();
	long nCount2=m_cmpDat[1].GetCount();
	long nCount=nCount1*nCount2;
	if(pBar==NULL&&(nCount>40000||nCount1>40000||nCount2>40000))
	{
		if(strName.IsEmpty())
		{
			proBar.Create(_T("正在比较......"),m_cmpDat[0].GetCount());
		}
		else
		{
			CString strTmp;
			strTmp.Format(_T("正在比较<%s>......"),strName);
			proBar.Create(strTmp,m_cmpDat[0].GetCount());
		}
	}
	int oldPos=pBar!=NULL?pBar->getCurPos():proBar.getCurPos();
	for(long i=m_cmpDat[0].GetCount()-1;i>=0;i--)
	{
		if(pBar!=NULL)
		{
			if(pBar->IsCreate())
			{
				pBar->stepIt();
				if(oldPos!=pBar->getCurPos())
				{
					oldPos=pBar->getCurPos();
				}
			}
		}
		else if(proBar.IsCreate())
		{
			proBar.stepIt();
			if(oldPos!=proBar.getCurPos())
			{
				oldPos=proBar.getCurPos();
			}
		}
		CmpEntityDat* pDat1=m_cmpDat[0][i];
		bFind=FALSE;
		for(int j=m_cmpDat[1].GetCount()-1;j>=0;j--)
		{
			CmpEntityDat* pDat2=m_cmpDat[1][j];
			if(!IsOneObj(pDat1,pDat2))
			{
				continue;
			}

			pDat1->nIndex=j;
			pDat2->nIndex=i;
			CmpDiffResult result;
			result.nType=3;//删除
			result.strHandle1=pDat1->m_strHandle;
			result.strHandle2=pDat2->m_strHandle;
			CmpType* pCmpType=GetCmpType(m_cmpDat[0][i]->m_type);
			result.strName=pCmpType==NULL?_T("未知类型"):pCmpType->m_strName;
			if(!m_cmpDat[0][i]->m_strName.IsEmpty())
			{
				result.strName=m_cmpDat[0][i]->m_strName;
			}
			BOOL bDiff= IsDiff(pDat1,pDat2,result);
			pDat1->bDiff=bDiff;
			pDat2->bDiff=bDiff;

			if(bDiff)
			{
				diffCount++;
				result.nType=1;//修改
				diffResults.Add(result);

			}
			else
			{
				result.nType=0;//完全相同
				diffResults.Add(result);//完全相同的就不需要增加进去了，浪费资源
			}
			DiffSubEntity(pDat1->subEntityDats,pDat2->subEntityDats,diffResults,diffCount);
			if(bRelease)
			{
				pDat1->Free();
				delete pDat1;
				pDat2->Free();
				delete pDat2;
			}
			m_cmpDat[0].RemoveAt(i);
			m_cmpDat[1].RemoveAt(j);
			break;
		}
	}
	for(long i=m_cmpDat[0].GetCount()-1;i>=0;i--)
	{
		CmpEntityDat* pDat=m_cmpDat[0][i];
		if(pDat->nIndex<0)
		{
			CmpDiffResult result;
			result.nType=3;//删除
			result.strHandle1=pDat->m_strHandle;
			CmpType* pCmpType=GetCmpType(pDat->m_type);
			result.strName=pCmpType==NULL?_T("未知类型"):pCmpType->m_strName;
			if(!pDat->m_strName.IsEmpty())
			{
				result.strName=pDat->m_strName;
			}
			diffResults.Add(result);
			diffCount++;
			//整体删除不需要再考虑子节点
			AddSubEntityToResult(pDat->subEntityDats,3,diffResults,diffCount);
		}
		if(bRelease)
		{
			pDat->Free();
			delete pDat;
		}
		m_cmpDat[0].RemoveAt(i);
	}
	for(int j=0;j<m_cmpDat[1].GetCount();j++)
	{
		CmpEntityDat* pDat=m_cmpDat[1][j];
		if(pDat->nIndex<0)
		{
			CmpDiffResult result;
			result.nType=2;//新增
			result.strHandle2=pDat->m_strHandle;
			CmpType* pCmpType=GetCmpType(pDat->m_type);
			result.strName=pCmpType==NULL?_T("未知类型"):pCmpType->m_strName;
			if(!pDat->m_strName.IsEmpty())
			{
				result.strName=pDat->m_strName;
			}
			diffResults.Add(result);
			diffCount++;
			//整体新增不需要再考虑子节点
			AddSubEntityToResult(pDat->subEntityDats,2,diffResults,diffCount);
		}
	}
	
	return eOk;
}
void CmpDiffFinder::Free()
{
	for(long i=0;i<m_cmpDat[0].GetCount();i++)
	{
		CmpEntityDat* pDat=m_cmpDat[0][i];
		if(pDat==NULL)
			continue;
		pDat->Free();
		delete pDat;
	}
	m_cmpDat[0].RemoveAll();
	for(long i=0;i<m_cmpDat[1].GetCount();i++)
	{
		CmpEntityDat* pDat=m_cmpDat[1][i];
		if(pDat==NULL)
			continue;
		pDat->Free();
		delete pDat;
	}
	m_cmpDat[1].RemoveAll();
}