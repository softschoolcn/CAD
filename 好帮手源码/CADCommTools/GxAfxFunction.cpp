#include "StdAfx.h"
#include "GxAfxFunction.h"
#include "GRegDialog.h"
CString BrowseForFolder( HWND hWnd, LPCTSTR pszInitDir /*= NULL*/)
{
	TCHAR szTitle[] = _T("选择一个文件夹"); 
	TCHAR  szDisplayName[MAX_PATH] = _T("");
	TCHAR  szPath[MAX_PATH] = _T("");
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.lpszTitle = szTitle;
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE|BIF_EDITBOX;
	bi.lpfn=NULL;
	bi.lParam = 0;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
	if( pItemIDList )
	{
		SHGetPathFromIDList(pItemIDList,szPath) ;

		IMalloc *pMalloc;
		if( SHGetMalloc( &pMalloc ) != NOERROR )
		{
			TRACE( "Failed to get pointer to shells task allocator" ) ;
			return szPath;
		}
		pMalloc->Free( pItemIDList );
		if( pMalloc )
			pMalloc->Release();
	}

	return szPath;
}
//枚举一个目录下的目录和文件列表
bool EnumDirectoryContent(CString strDirName , CString strFilter,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	strDirName.TrimLeft();
	strDirName.TrimRight();

	if(strDirName.GetLength() == 0)return false;

	arrSubDirNames.RemoveAll();
	arrSubFileNames.RemoveAll();

	CString			strDirList , strFileList , strDirFileList;
	CFileFind		finder;
	CString			strWildcard;

	if(strDirName.GetAt(strDirName.GetLength() - 1) == _T('\\') ||strDirName.GetAt(strDirName.GetLength() - 1) == _T('/'))
		strWildcard	= strDirName + strFilter;
	else
		strWildcard		= strDirName +"\\"+ strFilter;
	BOOL bWorking = finder.FindFile(strWildcard);

	if(!bWorking)return false;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())continue;
		if(finder.IsSystem())continue;
		if (finder.IsDirectory())
		{
			arrSubDirNames.Add(finder.GetFileName());
			continue;
		}

		arrSubFileNames.Add(finder.GetFileName());
	}
	finder.Close();

	return true;
}
bool EnumDirectoryContent(CString strDirName ,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	return EnumDirectoryContent(strDirName,_T("*.*"),arrSubDirNames,arrSubFileNames);
}
DWORD GetFileSize(LPCTSTR lpFile)
{
	DWORD dSize=0;
	try
	{
		CStdioFile file(lpFile, CFile::modeRead);
		dSize=file.GetLength();
		file.Close();
	}
	catch (CFileException* pe) 
	{
		dSize=0;
		pe->Delete();
	}
	return dSize;
}
//读取实体类型别名
BOOL ReadEntTypeAlias(CStringArray&        entTypeNameArray,
	CStringArray&        entTypeAliasArray)
{
	TCHAR m_lpFile[MAX_PATH+1]={0};
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("Config\\TypeAlias.xml"));
	// 装载XML字符串
	if (!lpDocument->load(m_lpFile))
	{

		OutputDebugString(_T("LoadXML failed!"));

		return FALSE;
	}

	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	lpDocument->get_documentElement(&lpDocElement);

	// 解析Types
	MSXML2::IXMLDOMNodePtr lpTypes = lpDocElement->selectSingleNode(_bstr_t(_T("Types")));

	if (NULL == lpTypes)
		return FALSE;

	// 解析参数列表
	MSXML2::IXMLDOMNodeListPtr lpNodeList = lpTypes->GetchildNodes();

	MSXML2::IXMLDOMNodePtr lpParam = NULL;

	for ( ; (lpParam = lpNodeList->nextNode()) != NULL ; )
	{
		MSXML2::IXMLDOMNodePtr lpNameNode = lpParam->attributes->getNamedItem(_T("Name"));
		CString strName = (char*)lpNameNode->text;

		MSXML2::IXMLDOMNodePtr lpValueNode = lpParam->attributes->getNamedItem(_T("Alias"));
		CString strValue = (char*)lpValueNode->text;
		if(strName.IsEmpty()||strValue.IsEmpty())
		{
			continue;
		}
		entTypeNameArray.Add(strName);
		entTypeAliasArray.Add(strValue);
	}

	return TRUE;
}
extern BOOL    Glb_bReg;
extern CString Glb_strRegDate;
extern BOOL _CheckSpecialReg(CString strKey,CString strId);
extern CString _GetSpecialRegName(CString strKey);
BOOL CheckSpecialReg(CString strKey)
{
	CString strName=_GetSpecialRegName(strKey);
	if(!Glb_bReg)
	{
		if(AfxMessageBox(_T("您的软件未注册，是否现在注册"),MB_YESNO)!=IDYES)
		{
			return FALSE;
		}
		CAcModuleResourceOverride res;
		GRegDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
		dlg.DoModal();
		if(!Glb_bReg)
		{
			return FALSE;
		}
	}
	if(_CheckSpecialReg(strKey,_T("")))
	{
		return TRUE;
	}
	CString strTmp;
	strTmp.Format(_T("您未注册专业功能\"%s\"，是否现在申请注册？"),strName);
	if(AfxMessageBox(strTmp,MB_YESNO)!=IDYES)
	{
		return FALSE;
	}
	CAcModuleResourceOverride res;
	GRegDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();
	if(!Glb_bReg)
	{
		return FALSE;
	}
	if(_CheckSpecialReg(strKey,_T("")))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//检查软件注册
BOOL CheckReg(const CString strDevDate)
{
	CHECK_REG_BOOL(strDevDate)
}
void NumCanPlus(CString& strText,CArray<int>& startIndexArray,CArray<int>& endIndexArray)
{
	int nLen=lstrlen(strText);
	for(int i=0;i<nLen;i++)
	{
		TCHAR c=strText.GetAt(i);
		if(c<='9'&&c>='0')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='9'&&c>='0')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}

		else if(c<='Z'&&c>='A')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='Z'&&c>='A')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}

		else if(c<='z'&&c>='a')
		{
			if(i==nLen-1)
			{
				startIndexArray.Add(i);
				endIndexArray.Add(i);
			}
			else
			{
				for(int j=i+1;j<nLen;j++)
				{
					c=strText.GetAt(j);
					if(c<='z'&&c>='a')
					{
						if(j==nLen-1)
						{
							startIndexArray.Add(i);
							endIndexArray.Add(j);
							i=j;
						}
						continue;
					}
					startIndexArray.Add(i);
					endIndexArray.Add(j-1);
					i=j-1;
					break;
				}
			}
		}
	}
}
//文字递增
//bIncrease:表示是否递增
//bIncrease=true,0->9->10
//bIncrease=false,0->9->0
BOOL NumPlus(CString& strText,int nNum,bool bIncrease)
{
	if(strText.IsEmpty())
	{
		return FALSE;
	}
	int nType=-1;
	for(int i=0;i<lstrlen(strText);i++)
	{
		TCHAR c=strText.GetAt(i);
		if(c<='9'&&c>='0')
		{
			if(nType==1||nType==2)
			{
				return FALSE;
			}
			nType=0;
		}
		else if(c>='A'&&c<='Z')
		{
			if(nType==0||nType==2)
			{
				return FALSE;
			}
			nType=1;
		}
		else if(c>='a'&&c<='z')
		{
			if(nType==0||nType==1)
			{
				return FALSE;
			}
			nType=2;
		}
		else
		{
			return FALSE;
		}

	}
	TCHAR cMin='0';
	TCHAR cMax='9';
	if(nType==1)
	{
		cMin='A';
		cMax='Z';
	}
	else if(nType==2)
	{
		cMin='a';
		cMax='z';
	}
	for(int i=0;i<nNum;i++)
	{
		TCHAR c1=strText.GetAt(lstrlen(strText)-1);
		if(c1<cMax)
		{
			strText.SetAt(lstrlen(strText)-1,c1+1);
		}
		else
		{
			bool bAdd=false;//是否已经增加好了
			strText.SetAt(lstrlen(strText)-1,cMin);
			for(int j=lstrlen(strText)-2;j>=0;j--)
			{
				c1=strText.GetAt(j);
				if(c1<cMax)
				{
					strText.SetAt(j,c1+1);
					bAdd=true;
					break;
				}
				else
				{
					strText.SetAt(j,cMin);
				}
			}
			if(!bAdd)
			{
				if(bIncrease)
				{
					if(nType==1||nType==2)
					{
					strText=cMin+strText;
					}
					else
					{
						strText=_T("1")+strText;
					}
				}
			}
		}
	}
	return TRUE;
}
bool IsXor(long nState,const long nPropState)
{
	if((nState&nPropState)==nPropState)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//拷贝文字到剪贴板中
BOOL CopyTextToClipboard(CString strText,HWND hWnd)
{
	BOOL rc=OpenClipboard(hWnd);
	if(!rc)
		return rc;
	EmptyClipboard();
	char* szBuf=NULL;
	GStringHelper::ConvertToLPCSTR(szBuf,strText);
	DWORD cch=strlen(szBuf);
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
		(cch + 1) * sizeof(char));
	if(hglbCopy==NULL)
	{
		free(szBuf);
		CloseClipboard();
		return FALSE;
	}
	char* pCopy=(char*)GlobalLock(hglbCopy);
	strcpy(pCopy,szBuf);
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	free(szBuf);
	CloseClipboard();
	return TRUE;
}
