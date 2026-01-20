#pragma once
//选择范围
class GFindExt
{
public:
	GFindExt(void);
public:
	~GFindExt(void);
public:
	int m_nType;//0:当前选择,1:整个图形,2:指定文件夹,3:指定文件
	AcDbObjectIdArray m_ids;//实体数组，对应0
	CString m_strFloder;//选择文件夹，对应2
	CStringArray m_strFileNames;//选择文件，对应3

};
