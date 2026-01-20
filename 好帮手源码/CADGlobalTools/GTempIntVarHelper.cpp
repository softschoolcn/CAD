#include "StdAfx.h"
#include "GTempIntVarHelper.h"
#include "GCADUtils.h"
GTempIntVarHelper::GTempIntVarHelper(CString sMode)
{
	m_nOldValue=0;
	m_bChanged=FALSE;
	m_sMode=sMode;
	GCADUtils::GetCADIntVar(m_sMode,m_nOldValue);
}
GTempIntVarHelper::GTempIntVarHelper(CString sMode,int nValue)
{
	m_nOldValue=0;
	m_bChanged=FALSE;
	m_sMode=sMode;
	Acad::ErrorStatus es=GCADUtils::GetCADIntVar(m_sMode,m_nOldValue);
	if(es==Acad::eOk)
	{
		if(m_nOldValue!=nValue)
		{
			es=GCADUtils::SetCADIntVar(sMode,nValue);
			if(es==Acad::eOk)
			{
				m_bChanged=TRUE;
			}
		}
	}
}
Acad::ErrorStatus GTempIntVarHelper::SetValue(int nValue)
{
	if(!m_bChanged)
	{
		Acad::ErrorStatus es=Acad::eOk;
		if(m_nOldValue!=nValue)
		{
			es=GCADUtils::SetCADIntVar(m_sMode,nValue);
			if(es==Acad::eOk)
			{
				m_bChanged=TRUE;
			}
		}
		return es;
	}
	else
	{
		return GCADUtils::SetCADIntVar(m_sMode,nValue);
	}
}

GTempIntVarHelper::~GTempIntVarHelper(void)
{
	if(m_bChanged)
	{
		GCADUtils::SetCADIntVar(m_sMode,m_nOldValue);
	}
}
