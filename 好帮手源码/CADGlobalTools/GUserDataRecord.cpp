#include "StdAfx.h"
#include "GUserDataRecord.h"
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
GUserDataRecord GUserDataRecord::m_Record;
GUserDataRecord::GUserDataRecord(void)
{
}

GUserDataRecord::~GUserDataRecord(void)
{
}
//功能:	获取唯一实例，一定存在
//返回:	唯一实例
GUserDataRecord* GUserDataRecord::getInstance()
{
	return &m_Record;
}
// Retrieve an integer value from INI file or registry.
UINT GUserDataRecord::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	initFile();
	return ::GetPrivateProfileInt(lpszSection,lpszEntry,nDefault,m_lpFile);
}

// Sets an integer value to INI file or registry.
BOOL GUserDataRecord::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	initFile();
	TCHAR lpValue[128];
	wsprintf(lpValue,_T("%d"),nValue);
	return ::WritePrivateProfileString(lpszSection,lpszEntry,lpValue,m_lpFile);
}
// Retrieve an Double value from INI file or registry.
double GUserDataRecord::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault)
{
	CString strDefault;
	strDefault.Format(_T("%.f"),dDefault);
	CString strValue;
	initFile();
	BOOL rc=GetPrivateProfileString(lpszSection,lpszEntry,strDefault,strValue.GetBuffer(128+1),128,m_lpFile);
	strValue.ReleaseBuffer();
	if(!rc)
		return dDefault;
	return _tstof(strValue);
}

// Sets an Double value to INI file or registry.
BOOL GUserDataRecord::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue)
{
	CString strValue;
	strValue.Format(_T("%f"),dValue);
	return ::WritePrivateProfileString(lpszSection,lpszEntry,strValue,m_lpFile);
}
// Retrieve a string value from INI file or registry.
CString GUserDataRecord::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,int nSize,
										  LPCTSTR lpszDefault)
{
	CString strValue;
	initFile();
	::GetPrivateProfileString(lpszSection,lpszEntry,lpszDefault,strValue.GetBuffer(nSize+1),nSize,m_lpFile);
	strValue.ReleaseBuffer();
	return strValue;
}

// Sets a string value to INI file or registry.
BOOL GUserDataRecord::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
										 LPCTSTR lpszValue)
{
	initFile();
	return ::WritePrivateProfileString(lpszSection,lpszEntry,lpszValue,m_lpFile);
}
bool GUserDataRecord::GetProfileBool(LPCTSTR lpszSection, LPCTSTR lpszEntry,bool bDefault)
{
	initFile();
	CString strTmp=GetProfileString(lpszSection,lpszEntry,10,bDefault?_T("True"):_T("False"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		return true;
	}
	else if(strTmp.CompareNoCase(_T("False"))==0)
	{
		return false;
	}
	else
	{
		return bDefault;
	}
}
BOOL GUserDataRecord::WriteProfileBool(LPCTSTR lpszSection, LPCTSTR lpszEntry,bool bDefault)
{
	initFile();
	return WriteProfileString(lpszSection,lpszEntry,bDefault?_T("True"):_T("False"));
}
void GUserDataRecord::initFile()
{
	static bool bInit=false;
	if(bInit)
		return;
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	if(!PathFileExists(m_lpFile))
	{
		CreateDirectory(m_lpFile,NULL);
	}
	PathAppend(m_lpFile,_T("UserData.ini"));
	bInit=true;
}
