#pragma once

class CmpItem
{
public:
	CmpItem(void);
	CmpItem(const CmpItem& item);
public:
	~CmpItem(void);
public:
	CmpItem& operator=(const CmpItem& item);
public:
	CString m_strName;//名称
	CArray<int> m_dxfCodes;//DXF码
	BOOL m_bOnlyOne;//是否唯一
	BOOL m_bIngoreOrder;//是否忽略顺序,只有m_bOnlyOne=FALSE才有效
	BOOL m_bActive;//是否应用
	BOOL m_bIngoreNoCase;//忽略大小写
};
