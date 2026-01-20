#pragma once
//查找类型
class GFindTypes
{
public:
	GFindTypes(void);
public:
	~GFindTypes(void);
public:
	bool m_bAttr;//属性,0
	bool m_bDim;//标注,1
	bool m_bText;//文字,2
	bool m_bTable;//表格文字,3
	bool m_bLinkDesc;//超链接说明,4
	bool m_bLink;//超链接,5
	bool m_bBlock;//深入块中查找,6
	void Read();
	void Write();
	int GetCount();
	static CString GetTypeDesc(int nType);
};
