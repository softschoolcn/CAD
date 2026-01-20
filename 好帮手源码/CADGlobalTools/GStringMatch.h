#pragma once

class GLOBALTOOLS_EXT GStringMatch
{
public:
	GStringMatch(void);
public:
	~GStringMatch(void);
	//是否匹配
	//str是否符合strMatch的描述
	bool IsMatch(CString str,CString strMatch);
	bool m_bIngoreCase;//忽略大小写
private:
	bool IsMatch(CString str,CStringArray& strMatchArray);
};
