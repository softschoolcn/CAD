#pragma once

class GFindResultItem
{
public:
	GFindResultItem(void);
public:
	~GFindResultItem(void);
public:
	CString m_strText;
	AcDbHandle m_handle;
	CString m_strFile;
	int m_nType;
	CString m_strEntType;
};
