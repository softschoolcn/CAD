#pragma once

class GLOBALTOOLS_EXT GTempIntVarHelper
{
public:
	GTempIntVarHelper(CString sMode);
	GTempIntVarHelper(CString sMode,int nValue);
	Acad::ErrorStatus SetValue(int nValue);
public:
	~GTempIntVarHelper(void);
private:
	CString m_sMode;
	int m_nOldValue;
	BOOL m_bChanged;
};
