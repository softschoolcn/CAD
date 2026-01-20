#pragma once
struct GFindParam
{
	CString strFind;//查找字符串
	bool m_bCase;//区分大小写
	bool m_bFullMatch;//全字匹配
	bool m_bTPF;//通配符
	void Read();
	void Write();
	bool IsMatch(CString str);
};