#include "StdAfx.h"
#include "GPlotDefine.h"
void GPlotItem::Copy(const GPlotItem& src)
{
	m_ptMin=src.m_ptMin;
	m_ptMax=src.m_ptMax;
	m_strFile=src.m_strFile;
	m_ownerHandle=src.m_ownerHandle;
	m_strBlockName=src.m_strBlockName;
	m_strOwnerName=src.m_strOwnerName;
}
bool GFrameDefine::IsFrame(AcDbBlockReference* pRef)
{
	if(m_bFrameLayerCheck)
	{
		CStringArray strLayerArray;
		GStringHelper::Split(m_strFrameLayer,_T(";"),strLayerArray,false);
		CString strLayerName;
		Acad::ErrorStatus es=GCADUtils::GetLayerName(strLayerName,pRef->layerId());
		if(es!=Acad::eOk)
		{
			return false;
		}
		if(!GStringHelper::IsStringInArray(strLayerArray,strLayerName,true))
		{
			return false;
		}
	}
	if(m_bFrameNameCheck)
	{
		CStringArray strMatchArray;
		GStringHelper::Split(m_strFrameName,_T("|"),strMatchArray,false);
		GStringMatch match;
		CString strBlockName=GetBlockName(pRef);
		for(long i=0;i<strMatchArray.GetCount();i++)
		{
			if(match.IsMatch(strBlockName,strMatchArray.GetAt(i)))
			{
				return true;
			}
		}
		return false;
	}
	return true;
}
//是闭合线的时候过滤对象
bool GFrameDefine::IsFrameX(AcDbEntity* pEnt)
{
	if(m_bBHXLayer)
	{
		CStringArray strLayerArray;
		GStringHelper::Split(m_strBHXLayer,_T(";"),strLayerArray,false);
		CString strLayerName;
		Acad::ErrorStatus es=GCADUtils::GetLayerName(strLayerName,pEnt->layerId());
		if(es!=Acad::eOk)
		{
			return false;
		}
		if(!GStringHelper::IsStringInArray(strLayerArray,strLayerName,true))
		{
			return false;
		}
	}
	if(m_bPLine)
	{
		if(pEnt->isKindOf(AcDbPolyline::desc()))
		{
			return true;
		}
	}
	if(m_bLine)
	{
		if(pEnt->isKindOf(AcDbLine::desc()))
		{
			return true;
		}
	}
	return false;
}
