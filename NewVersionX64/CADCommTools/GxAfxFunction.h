#pragma once
CString BrowseForFolder( HWND hWnd, LPCTSTR pszInitDir=NULL);
//枚举一个目录下的目录和文件列表
bool EnumDirectoryContent(CString strDirName , CString strFilter,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames);
bool EnumDirectoryContent(CString strDirName ,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames);
DWORD GetFileSize(LPCTSTR lpFile);
//读取实体类型别名
BOOL ReadEntTypeAlias(CStringArray&        entTypeNameArray,
	CStringArray&        entTypeAliasArray);
//文字递增
//bIncrease:表示是否递增
//bIncrease=true,0->9->10
//bIncrease=false,0->9->0
BOOL NumPlus(CString& strText,int nNum,bool bIncrease=false);
//文字可否递增
void NumCanPlus(CString& strText,CArray<int>& startIndexArray,CArray<int>& endIndexArray);
bool IsXor(long nState,const long nPropState);
//拷贝文字到剪贴板中
BOOL CopyTextToClipboard(CString strText,HWND hWnd=NULL);
