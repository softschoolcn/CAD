#include "StdAfx.h"
#include "GComputerId.h"
#include "hdid.h"
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include "md5.h"
# pragma comment(lib, "wbemuuid.lib")
CString Glb_ComputerId=_T("");

GComputerId::GComputerId(void)
{
}

GComputerId::~GComputerId(void)
{
}
typedef struct _T_WQL_QUERY  
{  
	CHAR*   szSelect;       // SELECT语句   
	WCHAR*  szProperty;     // 属性字段
	TCHAR*  szName;//名称
} T_WQL_QUERY;
// WQL查询语句   
const T_WQL_QUERY szWQLQuery[] = {  


	// 主板序列号   
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)",  
	L"SerialNumber", 
	_T("主板序列号"),
	// 主板型号   
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)",  
	L"Product",
	_T("主板型号"),
	// BIOS序列号   
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)",  
	L"SerialNumber",
	_T("BIOS序列号"),
	// 网卡原生MAC地址   
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",  
	L"PNPDeviceID",
	_T("网卡原生MAC地址"), 
	// 处理器ID   
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)",  
	L"ProcessorId",
	_T("处理器ID"),
	// 硬盘序列号   
	"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')",  
	L"SerialNumber",
	_T("硬盘序列号"),
	// 网卡当前MAC地址   
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",  
	L"MACAddress",
	_T("网卡当前MAC地址")
};
int QueryWmiInfo(CString& strResult,int& nGroup,int& nCount)
{
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------
	bool bComInit=false;
	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (hres==RPC_E_CHANGED_MODE)
	{

	}
	else if(hres==S_FALSE)
	{

	}
	else if(hres==S_OK)
	{
		bComInit=true;
		// Step 2: --------------------------------------------------
		// Set general COM security levels --------------------------
		// Note: If you are using Windows 2000, you need to specify -
		// the default authentication credentials for a user by using
		// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
		// parameter of CoInitializeSecurity ------------------------

		hres =  CoInitializeSecurity(
			NULL, 
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities 
			NULL                         // Reserved
			);


		if (FAILED(hres))
		{
			CoUninitialize();
			return 1;                    // Program has failed.
		}
	}
	else
	{
		return 1;
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		return 1;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		pLoc->Release();     
		CoUninitialize();
		return 1;                // Program has failed.
	}



	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return 1;               // Program has failed.
	}
	for(int i=0;i<3;i++)
	{
		const _T_WQL_QUERY query=szWQLQuery[i];
		// Step 6: --------------------------------------------------
		// Use the IWbemServices pointer to make requests of WMI ----

		// For example, get the name of the operating system
		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(
			bstr_t("WQL"), 
			bstr_t(query.szSelect),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

		// Step 7: -------------------------------------------------
		// Get the data from the query in step 6 -------------------

		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;
		int nTmp=0;
		while (pEnumerator)
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);

			if(0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			// Get the value of the Name property
			hr = pclsObj->Get(query.szProperty, 0, &vtProp, 0, 0);
			if(FAILED(hr))
			{
				break;
			}
			CString strTmp=_T("");
			strTmp+=vtProp.bstrVal;
			if(!strResult.IsEmpty())
			{
				strResult+=_T("\r\n");
			}
			strResult+=strTmp;
			VariantClear(&vtProp);
			nTmp++;
			pclsObj->Release();
		}
		nCount+=nTmp;
		if(nTmp>0)
		{
			nGroup++;
		}
		pEnumerator->Release();
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	if(bComInit)
	{
		CoUninitialize();
	}

	return 0;   // Program successfully completed.

}
//获取机器码
CString GComputerId::GetComputerId()
{
	if(!Glb_ComputerId.IsEmpty())
	{
		GLog::WriteLine(_T("返回已有的机器码缓存:%s"),Glb_ComputerId);
		return Glb_ComputerId;
	}
	int nGroup=0;
	int nCount=0;
	
	CString strResult=_T("");
	QueryWmiInfo(strResult,nGroup,nCount);
	if(nGroup<3||nCount<3)
	{
		//机器码为空
		char hardId[20]={0};
		if(GetHardId(hardId))
		{
			nGroup++;
			nCount++;
		}
		if(nGroup<3||nCount<3)
		{
			return _T("");
		}
		strResult+=_T("\r\n")+CString(hardId);
	}
	GLog::WriteLine(_T("计算机器码数据:%s"),strResult);
	std::string strId;
	for(int i=0;i<strResult.GetLength();i++)
	{
		TCHAR c=strResult.GetAt(i);
		if(c>=_T('0')&&c<=_T('9'))
		{
			strId+='0'+c-_T('0');
		}
		else if(c>=_T('a')&&c<=_T('z'))
		{
			strId+='a'+c-_T('a');
		}
		else if(c>=_T('A')&&c<=_T('Z'))
		{
			strId+='A'+c-_T('A');
		}
	}
	MD5 md5(strId);
	std::string strMd5=md5.toString();
	char innerStr[62]={0};
	int index=0;
	for(index=0;index<=9;index++)
		innerStr[index]='0'+index;
	for(index=0;index<=25;index++)
		innerStr[index+10]='A'+index;
	for(index=0;index<=25;index++)
		innerStr[index+36]='a'+index;
	std::string strTmp;
	for(int i=0;i<strMd5.length()-1;i++)
	{
		if(i%2==1)
		{
			continue;
		}
		char c1=strMd5.at(i);
		char c2;
		if(i+1<=strMd5.length()-1)
		{
			c2=strMd5.at(i+1);
		}
		else
		{
			c2='A';
		}
		int nTmp=c1+c2;
		nTmp=nTmp%62;
		strTmp+=innerStr[nTmp];
	}
	strResult=CString(strTmp.c_str());
	Glb_ComputerId=strResult;
	return strResult;
}
