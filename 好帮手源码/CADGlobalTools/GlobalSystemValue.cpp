#include "StdAfx.h"
#include "GlobalSystemValue.h"
GlobalSystemValue GlobalSystemValue::instance;
GlobalSystemValue::GlobalSystemValue(void)
{
	m_bInit=false;
}

GlobalSystemValue::~GlobalSystemValue(void)
{
}
//初始化
void GlobalSystemValue::Init()
{
	if(m_bInit)
	{
		return;
	}
	m_bInit=true;
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("Config\\System.ini"));
	CString strSysName=_T("HaoBangShou");
	if(PathFileExists(m_lpFile))
	{
		TCHAR lpInnerName[MAX_PATH+1]={0};
		if(GetPrivateProfileString(_T("System"),_T("InnerKey"),strSysName.GetBuffer(),lpInnerName,MAX_PATH,m_lpFile))
		{
			strSysName.ReleaseBuffer();
			strSysName=lpInnerName;
		}
		else
		{
			strSysName.ReleaseBuffer();
		}
	}
	else
	{
	}
	if(strSysName.CompareNoCase(_T("ZhongXianCAD"))==0)
	{
		m_strSoftName=_T("中线CAD");
		m_strWebSite=_T("www.xianshu.cn");
		m_strRegWebSite=_T("http://www.xianshu.cn");
	}
	else if(strSysName.CompareNoCase(_T("XiaoZhuShou"))==0)
	{
		m_strSoftName=_T("小助手");
		m_strWebSite=_T("cadgj12345.ys168.com");
		m_strRegWebSite=_T("http://cadgj12345.ys168.com");
	}
	else
	{
		strSysName=_T("HaoBangShou");
		m_strSoftName=_T("CAD好帮手");
		m_strWebSite=_T("www.cadgj.com");
		m_strRegWebSite=_T("http://www.cadgj.com/?p=364");
	}
	m_strSysName=strSysName;

}
void    GlobalSystemValue::ResetWindowText(HWND hWnd)
{
	TCHAR m_text[MAX_PATH+1]={0};
	GetWindowText(hWnd,m_text,MAX_PATH);
	CString strText;
	if(m_strSysName.CompareNoCase(_T("ZhongXianCAD"))==0)
	{
		strText.Format(_T("%s[中线CAD 热线电话400-8002007]"),m_text);
	}
	else if(m_strSysName.CompareNoCase(_T("XiaoZhuShou"))==0)
	{
		strText.Format(_T("%s[CAD小助手]"),m_text);
	}
	else
	{
		strText.Format(_T("%s[CAD好帮手 官网:CAD工具之家(cadgj.com)]"),m_text);
	}
	SetWindowText(hWnd,strText);
}
