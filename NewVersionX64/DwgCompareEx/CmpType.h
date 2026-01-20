#pragma once
#include "CmpItem.h"
class CmpType
{
public:
	CmpType(void);
	CmpType(const CmpType& type);
public:
	~CmpType(void);
public:
	CmpType& operator=(const CmpType& type);
public:
	CmpItem m_cmpItems[128];
	int     m_nCmpItemsCount;
	CArray<int> m_IngoreDxfCodes;//DXF码
	CString m_strName;//名称
	CString m_strClassName;//类名称
	CString m_strParentClassName;//负类名称
	BOOL m_bActive;//是否应用
	public:
	CmpType* m_pParType;
};
