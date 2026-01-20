#include "StdAfx.h"
#include "GTransConfig.h"
#include "CommFunction.h"
GTransConfig::GTransConfig(void)
{
	m_bBlackWhite=false;
	m_EntitysMode=kTransEntitysAll;
	m_bGetExt=false;
	m_PaperMode=kISOAOPaper;
}

GTransConfig::~GTransConfig(void)
{
}
BOOL GTransConfig::Read()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("UserConfig\\DwgTrans.ini"));
	if(!PathFileExists(lpPath))
	{
		return FALSE;
	}
	m_strConfigFile=lpPath;
	CString strValue;
	ReadString(_T("TransType"),strValue);
	m_TypeArray.RemoveAll();
	::SplitString(strValue,',',m_TypeArray);
	int nTmp=0;
	ReadInt(_T("BlackWhite"),nTmp);
	m_bBlackWhite=nTmp!=0?true:false;
	ReadInt(_T("EntitysMode"),nTmp);
	m_EntitysMode=(GTransEntitysMode)nTmp;
	ReadString(_T("Floder"),m_strFloder);
	ReadString(_T("FileName"),m_strFileName);
	ReadInt(_T("PaperMode"),nTmp);
	m_PaperMode=(GTransPaperMode)nTmp;


	return TRUE;
}
BOOL GTransConfig::Write()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("UserConfig\\DwgTrans.ini"));
	m_strConfigFile=lpPath;
	WriteString(_T("TransType"),CombineArrayToString(m_TypeArray,_T(",")));
	WriteInt(_T("BlackWhite"),m_bBlackWhite?1:0);
	WriteInt(_T("EntitysMode"),(int)m_EntitysMode);
	WriteString(_T("Floder"),m_strFloder);
	WriteString(_T("FileName"),m_strFileName);
	WriteInt(_T("PaperMode"),(int)m_PaperMode);


	return TRUE;
}
BOOL GTransConfig::WriteString(CString strKey,CString strValue)
{
	return ::WritePrivateProfileString(_T("config"),strKey,strValue,m_strConfigFile);
}
BOOL GTransConfig::WriteInt(CString strKey,int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"),nValue);
	return WriteString(strKey,strValue);
}
BOOL GTransConfig::WriteDouble(CString strKey,double dValue)
{
	CString strValue;
	strValue.Format(_T("%f"),dValue);
	return WriteString(strKey,strValue);
}
BOOL GTransConfig::ReadString(CString strKey,CString& strValue)
{
	BOOL rc=GetPrivateProfileString(_T("config"),strKey,_T(""),strValue.GetBuffer(512+1),512,m_strConfigFile);
	strValue.ReleaseBuffer();
	return rc;
}
BOOL GTransConfig::ReadInt(CString strKey,int& nValue)
{
	CString strValue;
	BOOL rc=ReadString(strKey,strValue);
	if(!rc)
		return rc;
	nValue=_ttoi(strValue);
	return TRUE;
}
BOOL GTransConfig::ReadDouble(CString strKey,double& dValue)
{
	CString strValue;
	BOOL rc=ReadString(strKey,strValue);
	if(!rc)
		return rc;
	dValue=_tstof(strValue);
	return TRUE;
}
