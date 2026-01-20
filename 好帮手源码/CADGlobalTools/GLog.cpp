#include "StdAfx.h"
#include "GLog.h"
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
bool GLogConfigOpen=true;
bool GLogConfigRead=false;//是否开启日志配置文件是否读取
CString strFileName;//日志文件
bool IsLogConfigOpen()
{
	if(GLogConfigRead)
	{
		return GLogConfigOpen;
	}
	TCHAR path[1024] = {0};
	GetModuleFileName(_hdllInstance, path, 1023);
	PathRemoveFileSpec(path);
	PathRemoveFileSpec(path);
	PathAppend(path,_T("Log"));
	PathAppend(path,_T("LogConfig.ini"));
	TCHAR lpResult[128+1]={0};
	GetPrivateProfileString(_T("System"),_T("LogOpen"),_T("False"),lpResult,128,path);
	if(lstrcmpi(lpResult,_T("True"))==0)
	{
		GLogConfigOpen=true;
		GLogConfigRead=true;
		return true;
	}
	else
	{
		GLogConfigRead=true;
		GLogConfigOpen=false;
		return false;
	}

}
void GLog::WriteLine(CString fmt, ...)
{
	if(!IsLogConfigOpen())
	{
		return;
	}
	try
	{
		CString strBuffer;

		va_list marker;
		va_start(marker, fmt);
		strBuffer.FormatV(fmt, marker);
		va_end(marker);
		CTime t_now=CTime::GetCurrentTime();
                CString strBufferEx=strBuffer;
		strBufferEx.Format(_T("%.2d:%.2d:%.2d    %s"),t_now.GetHour(),t_now.GetMinute(),t_now.GetSecond(),strBuffer);
		if(strFileName.IsEmpty())
		{
			TCHAR path[1024] = {0};
			GetModuleFileName(_hdllInstance, path, 1023);
			PathRemoveFileSpec(path);
			PathRemoveFileSpec(path);
			PathAppend(path,_T("Log"));
			if(!PathFileExists(path))
			{
				CreateDirectory(path,NULL);
			}
			strFileName.Format(_T("%.4d-%.2d-%.2d %.2d：%.2d：%.2d.log"),t_now.GetYear(),t_now.GetMonth(),t_now.GetDay(),t_now.GetHour(),t_now.GetMinute(),t_now.GetSecond());
			PathAppend(path,strFileName);
			strFileName=path;
			try
			{
				CStdioFile file(path, CFile::modeCreate | CFile::modeWrite);
				file.WriteString(strBufferEx);
				file.WriteString(_T("\n"));
				file.Close();
			}
			catch (CFileException* pe) 
			{
				CString strErr;
				strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
				pe->Delete();
			}
		}
		else
		{
			try
			{
				CStdioFile file(strFileName, CFile::modeWrite);
				file.SeekToEnd();
				file.WriteString(strBufferEx);
				file.WriteString(_T("\n"));
				file.Close();
			}
			catch (CFileException* pe) 
			{
				CString strErr;
				strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
				pe->Delete();
			}
		}


		OutputDebugString(strBuffer);
	}
	catch (...) 
	{
	}
}
