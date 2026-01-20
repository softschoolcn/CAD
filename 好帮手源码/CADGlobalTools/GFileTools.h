#pragma once

class GLOBALTOOLS_EXT GFileTools
{
public:
	//枚举一个目录下的目录和文件列表
	static bool EnumDirectoryContent(CString strDirName , CString strFilter,
		CStringArray &arrSubDirNames , 
		CStringArray &arrSubFileNames);
	static bool EnumDirectoryContent(CString strDirName ,
		CStringArray &arrSubDirNames , 
		CStringArray &arrSubFileNames);
};
