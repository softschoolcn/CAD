#include "StdAfx.h"
#include "CmpItem.h"

CmpItem::CmpItem(void)
{
	m_bOnlyOne=TRUE;
	m_bIngoreOrder=FALSE;
	m_bActive=TRUE;
	m_bIngoreNoCase=FALSE;
}
CmpItem::CmpItem(const CmpItem& item)
{
	m_strName=item.m_strName;
	m_dxfCodes.RemoveAll();
	m_dxfCodes.Append(item.m_dxfCodes);
	m_bOnlyOne=item.m_bOnlyOne;
	m_bIngoreOrder=item.m_bIngoreOrder;
	m_bActive=item.m_bActive;
	m_bIngoreNoCase=item.m_bIngoreNoCase;
}
CmpItem::~CmpItem(void)
{
}
CmpItem& CmpItem::operator=(const CmpItem& item)
{
	if(this==&item)
	{
		return *this;
	}
	m_strName=item.m_strName;
	m_dxfCodes.RemoveAll();
	m_dxfCodes.Append(item.m_dxfCodes);
	m_bOnlyOne=item.m_bOnlyOne;
	m_bIngoreOrder=item.m_bIngoreOrder;
	m_bActive=item.m_bActive;
	m_bIngoreNoCase=item.m_bIngoreNoCase;
	return *this;
}
