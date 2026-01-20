#include "StdAfx.h"
#include "GQRCodeManager.h"
GQRCodeManager::GQRCodeManager()
{

}
GQRCodeManager::~GQRCodeManager()
{

}
CString GQRCodeManager::MakeQRCode(CString dat)
{
	char* szBuf=NULL;
	GStringHelper::ConvertToLPCSTR(szBuf,dat);
	TCHAR m_path[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,m_path);
	::PathAppend(m_path,_T("QcCode.txt"));
	try
	{
	CStdioFile file(m_path, CFile::modeCreate | CFile::modeWrite);
	file.Write(szBuf,strlen(szBuf));
		file.Close();
	}
	catch(CFileException* pe)
	{
		CString strErr;
		strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
		AfxMessageBox(strErr);
		pe->Delete();
		return _T("");
	}

	TCHAR lpFile[MAX_PATH+1];
	GetModuleFileName(_hdllInstance, lpFile, MAX_PATH);
	PathRemoveFileSpec(lpFile);
	PathRemoveFileSpec(lpFile);
	PathAppend(lpFile,_T("bin\\QrCodeCreater.exe"));
	if(!PathFileExists(lpFile))
	{
		acedAlert(_T("未找到QrCodeCreater.exe，请联系软件开发人员"));
		return _T("");
	}
	CString strArg;
	strArg.Format(_T("\"%s\" \"%s\""),lpFile,m_path);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(NULL,   // No module name (use command line). 
		strArg.GetBuffer(), // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 

	{
		//todo log
		//
		acedAlert(_T("调用QrCodeCreater.exe出错，请联系软件开发人员"));
		strArg.ReleaseBuffer();
		return _T("");
	}
	strArg.ReleaseBuffer();
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if(!PathFileExists(m_path))
	{
		return _T("");
	}
	try
	{
	CFile file(m_path, CFile::modeRead|CFile::typeBinary);
		int nLen = file.GetLength();
		if(nLen == 0)
		{
			file.Close(); 
			return _T("");
		}
		char* strBuf = new char[nLen+1];
		memset(strBuf,0,sizeof(char)*(nLen+1));
		file.Read(strBuf,nLen);
		file.Close();
		CString str=CString(strBuf);
		delete [] strBuf;
		return str;
	}
	catch(CFileException* pe)
	{
		CString strErr;
		strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
		AfxMessageBox(strErr);
		pe->Delete();
		return _T("");
	}
}