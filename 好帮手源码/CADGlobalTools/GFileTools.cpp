#include "StdAfx.h"
#include "GFileTools.h"
//枚举一个目录下的目录和文件列表
bool GFileTools::EnumDirectoryContent(CString strDirName , CString strFilter,
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
bool GFileTools::EnumDirectoryContent(CString strDirName ,
									  CStringArray &arrSubDirNames , 
									  CStringArray &arrSubFileNames)
{
	return EnumDirectoryContent(strDirName,_T("*.*"),arrSubDirNames,arrSubFileNames);
}
