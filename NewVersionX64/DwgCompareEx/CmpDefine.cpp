#include "Stdafx.h"
#include "CmpDefine.h"
CmpEntityDat::CmpEntityDat()
	{
		bDiff=FALSE;
		nIndex=-1;
		m_strName=_T("");
		m_bNotEnt=FALSE;//如果不是实体，则根据m_strName判断是否相同
		bTmp=false;
	}
CmpEntityDat::~CmpEntityDat()
	{
	}
CmpEntityDat::CmpEntityDat(const CmpEntityDat& dat)
	{
		m_ext=dat.m_ext;
		m_type=dat.m_type;
		m_id=dat.m_id;
		m_strHandle=dat.m_strHandle;
		m_strName=dat.m_strName;
		m_bNotEnt=dat.m_bNotEnt;
		nIndex=dat.nIndex;
		subEntityDats.RemoveAll();
		subEntityDats.Append(dat.subEntityDats);
		bTmp=dat.bTmp;
	}
CmpEntityDat& CmpEntityDat::operator=(const CmpEntityDat& dat)
	{
		if(this==&dat)
		{
			return *this;
		}
		m_ext=dat.m_ext;
		m_type=dat.m_type;
		m_id=dat.m_id;
		m_strHandle=dat.m_strHandle;
		m_strName=dat.m_strName;
		m_bNotEnt=dat.m_bNotEnt;
		nIndex=dat.nIndex;
		subEntityDats.RemoveAll();
		subEntityDats.Append(dat.subEntityDats);
		bTmp=dat.bTmp;
		return *this;
	}
void CmpEntityDat::Free()
	{
		for(long i=0;i<subEntityDats.GetCount();i++)
		{
			CmpEntityDat* pDat=subEntityDats.GetAt(i);
			if(pDat==NULL)
				continue;
			pDat->Free();
			delete pDat;
		}
		subEntityDats.RemoveAll();
	}
/////////////////////////////////////////////////////////////////////////////////////////////
CmpDiffItem::CmpDiffItem()
{
	m_bRevise=FALSE;
}
CmpDiffItem::CmpDiffItem(const CmpDiffItem& item)
{
	strName=item.strName;
	nDxfCode=item.nDxfCode;
	m_bOnlyOne=item.m_bOnlyOne;
	strOldValue=item.strOldValue;
	strNewValue=item.strNewValue;
	strOldValueArray.RemoveAll();
	strOldValueArray.Append(item.strOldValueArray);
	strNewValueArray.RemoveAll();
	strNewValueArray.Append(item.strNewValueArray);
	m_bRevise=item.m_bRevise;
}
CmpDiffItem& CmpDiffItem::operator=(const CmpDiffItem& item)
{
	if(&item==this)
		return *this;
	strName=item.strName;
	nDxfCode=item.nDxfCode;
	m_bOnlyOne=item.m_bOnlyOne;
	strOldValue=item.strOldValue;
	strNewValue=item.strNewValue;
	strOldValueArray.RemoveAll();
	strOldValueArray.Append(item.strOldValueArray);
	strNewValueArray.RemoveAll();
	strNewValueArray.Append(item.strNewValueArray);
	m_bRevise=item.m_bRevise;
	return *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////
CmpDiffResult::CmpDiffResult()
{
	nType=0;
	strHandle1=_T("");
	strHandle2=_T("");
	strName=_T("");
	m_bRevise=FALSE;
}
CmpDiffResult::CmpDiffResult(const CmpDiffResult& result)
{
	nType=result.nType;
	strHandle1=result.strHandle1;
	strHandle2=result.strHandle2;
	strName=result.strName;
	m_diffItems.RemoveAll();
	m_diffItems.Append(result.m_diffItems);
	m_bRevise=result.m_bRevise;
	colRecord1=result.colRecord1;
	colRecord2=result.colRecord2;
}
CmpDiffResult& CmpDiffResult::operator=(const CmpDiffResult& result)
{
	if(&result==this)
		return *this;
	nType=result.nType;
	strHandle1=result.strHandle1;
	strHandle2=result.strHandle2;
	strName=result.strName;
	m_diffItems.RemoveAll();
	m_diffItems.Append(result.m_diffItems);
	m_bRevise=result.m_bRevise;
	colRecord1=result.colRecord1;
	colRecord2=result.colRecord2;
	return *this;

}

///////////////////////////////////////////////////////////////////////////////////////////
CmpBlockDiffResult::CmpBlockDiffResult()
{
	nType=0;
	strHandle1=_T("");
	strHandle2=_T("");
	strName=_T("");
}
CmpBlockDiffResult::CmpBlockDiffResult(const CmpBlockDiffResult& result)
{
	nType=result.nType;
	strHandle1=result.strHandle1;
	strHandle2=result.strHandle2;
	strName=result.strName;
	m_diffItems.RemoveAll();
	m_diffItems.Append(result.m_diffItems);
}
CmpBlockDiffResult& CmpBlockDiffResult::operator=(const CmpBlockDiffResult& result)
{
	if(&result==this)
		return *this;
	nType=result.nType;
	strHandle1=result.strHandle1;
	strHandle2=result.strHandle2;
	strName=result.strName;
	m_diffItems.RemoveAll();
	m_diffItems.Append(result.m_diffItems);
	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////
CmpResult::CmpResult()
{
	m_pDoc1=NULL;
	m_pDoc2=NULL;
}
CmpResult::CmpResult(const CmpResult& result)
{
	m_diffsInModelSpace.RemoveAll();
	m_diffsInModelSpace.Append(result.m_diffsInModelSpace);
	m_diffsInBlockDefines.RemoveAll();
	m_diffsInBlockDefines.Append(result.m_diffsInBlockDefines);
	m_diffsInLayerTable.RemoveAll();
	m_diffsInLayerTable.Append(result.m_diffsInLayerTable);

	m_diffsInLinetypeTable.RemoveAll();
	m_diffsInLinetypeTable.Append(result.m_diffsInLinetypeTable);

	m_diffsInTextStyleTable.RemoveAll();
	m_diffsInTextStyleTable.Append(result.m_diffsInTextStyleTable);


	m_pDoc1=result.m_pDoc1;
	m_pDoc2=result.m_pDoc2;
	m_strFile1=result.m_strFile1;
	m_strFile2=result.m_strFile2;
	nType=result.nType;
}
CmpResult& CmpResult::operator=(const CmpResult& result)
{
	if(this==(&result))
		return *this;
	m_diffsInModelSpace.RemoveAll();
	m_diffsInModelSpace.Append(result.m_diffsInModelSpace);
	m_diffsInBlockDefines.RemoveAll();
	m_diffsInBlockDefines.Append(result.m_diffsInBlockDefines);
	m_diffsInLayerTable.RemoveAll();
	m_diffsInLayerTable.Append(result.m_diffsInLayerTable);

	m_diffsInLinetypeTable.RemoveAll();
	m_diffsInLinetypeTable.Append(result.m_diffsInLinetypeTable);

	m_diffsInTextStyleTable.RemoveAll();
	m_diffsInTextStyleTable.Append(result.m_diffsInTextStyleTable);
	m_pDoc1=result.m_pDoc1;
	m_pDoc2=result.m_pDoc2;
	m_strFile1=result.m_strFile1;
	m_strFile2=result.m_strFile2;
	nType=result.nType;
	return *this;
}