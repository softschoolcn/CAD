#include "StdAfx.h"
#include "IEHelper.h"
LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)   
{   
	HKEY hkey;   
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);   

	if (retval == ERROR_SUCCESS) {   
		long datasize = MAX_PATH;   
		TCHAR data[MAX_PATH];   
		RegQueryValue(hkey, NULL, data, &datasize);   
		lstrcpy(retdata,data);   
		RegCloseKey(hkey);   
	}   

	return retval;   
}
//打开指定后缀的文件
HINSTANCE ShellOpenFile(LPCTSTR ext,LPCTSTR file,int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];   

	// First try ShellExecute()  
	HINSTANCE result = (HINSTANCE)HINSTANCE_ERROR;
	if(lstrcmpi(ext,_T(".htm"))==0)
	{
		//如果是文件怕可能会弹出那个打开方式对话框
		result=ShellExecute(NULL, _T("open"), file, NULL,NULL, showcmd); 
	}

	// If it failed, get the .htm regkey and lookup the program    
	if ((UINT)result <= HINSTANCE_ERROR) {   

		if (GetRegKey(HKEY_CLASSES_ROOT, ext, key) == ERROR_SUCCESS) {   
			lstrcat(key, _T("\\shell\\open\\command"));   

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {   
				TCHAR *pos;   
				pos = _tcsstr(key, _T("\"%1\""));   
				if (pos == NULL) {                     // No quotes found 
#ifdef _UNICODE
					pos = wcsstr(key, _T("%1"));       // Check for %1, without quotes
#else
					pos = strstr(key, _T("%1"));       // Check for %1, without quotes
#endif

					if (pos == NULL)                   // No parameter at all...    
						pos = key+lstrlen(key)-1;   
					else   
						*pos = '\0';                   // Remove the parameter    
				}   
				else   
					*pos = '\0';                       // Remove the parameter    

				lstrcat(pos, _T(" "));   
				lstrcat(pos, file);  
				if(lstrcmpi(ext,_T(".htm"))!=0)
				{
					result=ShellExecute(NULL, _T("open"), file, NULL,NULL, showcmd);
					if ((UINT)result<=HINSTANCE_ERROR)
					{
						result=(HINSTANCE)WinExec((LPCSTR)pos,showcmd);
					}
				}
				else
				{
					result=(HINSTANCE)WinExec((LPCSTR)pos,showcmd);
					if ((UINT)result<=HINSTANCE_ERROR)
					{
						result=ShellExecute(NULL, _T("open"), file, NULL,NULL, showcmd);
					}
				}
			}   
		}   
	}   

	return result; 
}
HINSTANCE GotoURL(LPCTSTR url, int showcmd)   
{   
	return ShellOpenFile(_T(".htm"),url,showcmd);
}