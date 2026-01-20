#include "StdAfx.h"
#include "CmpType.h"

CmpType::CmpType(void)
{
	m_pParType=NULL;
	m_nCmpItemsCount=0;
}
CmpType::CmpType(const CmpType& type)
{
	m_bActive=type.m_bActive;
	m_strName=type.m_strName;
	m_strClassName=type.m_strClassName;
	m_strParentClassName=type.m_strParentClassName;
	m_pParType=type.m_pParType;
	m_IngoreDxfCodes.RemoveAll();
	m_IngoreDxfCodes.Append(type.m_IngoreDxfCodes);
	m_nCmpItemsCount=type.m_nCmpItemsCount;
	for(int i=0;i<m_nCmpItemsCount;i++)
	{
		m_cmpItems[i]=type.m_cmpItems[i];
	}
}
CmpType::~CmpType(void)
{
}
CmpType& CmpType::operator=(const CmpType& type)
{
	if(this==&type)
	{
		return *this;
	}
	m_bActive=type.m_bActive;
	m_strName=type.m_strName;
	m_strClassName=type.m_strClassName;
	m_strParentClassName=type.m_strParentClassName;
	m_pParType=type.m_pParType;
	m_IngoreDxfCodes.RemoveAll();
	m_IngoreDxfCodes.Append(type.m_IngoreDxfCodes);
	m_nCmpItemsCount=type.m_nCmpItemsCount;
	for(int i=0;i<m_nCmpItemsCount;i++)
	{
		m_cmpItems[i]=type.m_cmpItems[i];
	}
	return *this;
}
