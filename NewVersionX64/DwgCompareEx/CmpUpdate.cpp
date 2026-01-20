#include "StdAfx.h"
#include "CmpUpdate.h"
#include "GHttpService.h"
CmpUpdate::CmpUpdate(void)
{
	m_bHaveNewVersion=false;
	m_bFirstTime=true;
	m_bNeedReadServer=false;
}

CmpUpdate::~CmpUpdate(void)
{
}
CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName)
{
	MSXML2::IXMLDOMNodePtr lpAttrNode = lpNode->attributes->getNamedItem(strName.GetBuffer());
	strName.ReleaseBuffer();
	if(lpAttrNode==NULL)
		return _T("");
	return (char*)lpAttrNode->text;
}
BOOL CmpUpdate::ReadLocal()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgExUpdate.xml"));
	if(!PathFileExists(lpPath))
		return FALSE;
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpPath))
	{
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr lpVersionNode=lpDocElement->selectSingleNode(_T("Version"));
	if(lpVersionNode==NULL)
	{
		return FALSE;
	}
	m_LocalInfo.m_strCurrentVersion=GetNodeAttrValue(lpVersionNode,_T("Current"));
	m_LocalInfo.m_strVersiuonURL=GetNodeAttrValue(lpVersionNode,_T("VersiuonURL"));
	m_LocalInfo.m_strURL=GetNodeAttrValue(lpVersionNode,_T("URL"));
	m_LocalInfo.m_strLastUpdateTime=GetNodeAttrValue(lpVersionNode,_T("LastUpdateTime"));
	MSXML2::IXMLDOMNodePtr lpUpdatDescribeNode=lpVersionNode->selectSingleNode(_T("UpdatDescribe"));
	if(lpVersionNode==NULL)
	{
		return FALSE;
	}
	m_LocalInfo.m_UpdatDescribe=(char*)lpUpdatDescribeNode->text;
	return TRUE;
}
BOOL ReadAdHtml()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\DwgCmpExAd.html"));

	TCHAR lpTmpPath[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpTmpPath);
	PathAppend(lpTmpPath,_T("DwgCmpExAd.html"));
	DWORD dwStatusCode;
	if(!GHttpService::Download(_T("http://www.cadgj.com/DwgCmpExAd.html"),lpTmpPath,dwStatusCode,NULL))
	{
		return FALSE;
	}

	TCHAR lpPath1[MAX_PATH+1]={0};
	GetModuleFileName(hModule,lpPath1,MAX_PATH);
	PathRemoveFileSpec(lpPath1);
	PathRemoveFileSpec(lpPath1);
	PathAppend(lpPath1,_T("config\\DwgCmpExAd.ini"));

	TCHAR lpTmpPath1[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpTmpPath1);
	PathAppend(lpTmpPath1,_T("DwgCmpExAd.ini"));
	if(!GHttpService::Download(_T("http://www.cadgj.com/DwgCmpExAd.ini"),lpTmpPath1,dwStatusCode,NULL))
	{
		return FALSE;
	}

	CopyFile(lpTmpPath,lpPath,FALSE);
	CopyFile(lpTmpPath1,lpPath1,FALSE);
	return TRUE;

}
BOOL CmpUpdate::ReadLocalServerInfo()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgExUpdate_Server.xml"));
	if(!PathFileExists(lpPath))
		return FALSE;
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpPath))
	{
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr lpVersionNode=lpDocElement->selectSingleNode(_T("Version"));
	if(lpVersionNode==NULL)
	{
		return FALSE;
	}
	m_ServerInfo.m_strCurrentVersion=GetNodeAttrValue(lpVersionNode,_T("Current"));
	m_ServerInfo.m_strVersiuonURL=GetNodeAttrValue(lpVersionNode,_T("VersiuonURL"));
	m_ServerInfo.m_strURL=GetNodeAttrValue(lpVersionNode,_T("URL"));
	m_ServerInfo.m_strLastUpdateTime=GetNodeAttrValue(lpVersionNode,_T("LastUpdateTime"));
	MSXML2::IXMLDOMNodePtr lpUpdatDescribeNode=lpVersionNode->selectSingleNode(_T("UpdatDescribe"));
	if(lpVersionNode==NULL)
	{
		return FALSE;
	}
	m_ServerInfo.m_UpdatDescribe=(char*)lpUpdatDescribeNode->text;
	return TRUE;

}
BOOL CmpUpdate::UpdateServerInfo()
{
	if(!ReadAdHtml())
	{
		return FALSE;
	}
	TCHAR lpTmpPath[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpTmpPath);
	PathAppend(lpTmpPath,_T("CompareDwgExUpdate_Server.xml"));

TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgExUpdate_Server.xml"));
	DWORD dwStatusCode;
	if(!GHttpService::Download(m_LocalInfo.m_strVersiuonURL,lpTmpPath,dwStatusCode,NULL))
	{
		return FALSE;
	}
	CopyFile(lpTmpPath,lpPath,FALSE);
	return ReadLocalServerInfo();
}
BOOL CmpUpdate::WriteTryTime()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("DwgCompare.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgUpdate.xml"));
	if(!PathFileExists(lpPath))
		return FALSE;
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpPath))
	{
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpVersionNode=lpDocElement->selectSingleNode(_T("Version"));
	if(lpVersionNode==NULL)
	{
		return FALSE;
	}
	CTime lastTryTime=CTime::GetCurrentTime();
	CString strTmp=lastTryTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	lpVersionNode->setAttribute(_T("LastUpdateTime"),_variant_t(strTmp));
	lpDocument->save(lpPath);
	return TRUE;
}
BOOL CmpUpdate::NeedTryUpdate()
{
	COleDateTime ct;
	bool rc=ct.ParseDateTime(m_LocalInfo.m_strLastUpdateTime,0,0);
	if(!rc)
		return TRUE;
	CTime lastTryTime=CTime(ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CTime ct_now=CTime::GetCurrentTime();
	CTimeSpan ts=ct_now-lastTryTime;
	CTimeSpan ts1(GetTickCount()/1000);
	if(ts>ts1)
	{
		//时间间隔大于开机到现在的时间
		//说明开机后还没有更新
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
