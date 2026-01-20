/*
* Copyright (c) 2012,上海金曲信息技术有限公司
* All rights reserved.
* 
* 文件名称：GUserDataRecord.h
* 摘    要：用于记录用户的数据
*           存放在Config/GoldUserData.ini文件中
*           以便方便的进行打包时设置好初始值
* 完成日期：2012年11月21日
*/
#pragma once

class GLOBALTOOLS_EXT GUserDataRecord
{
public:
	GUserDataRecord(void);
	//功能:	获取唯一实例，一定存在
	//返回:	唯一实例
	static GUserDataRecord* getInstance();
	// Retrieve an integer value from INI file or registry.
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault=0);
	

	// Sets an integer value to INI file or registry.
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	// Retrieve an Double value from INI file or registry.
	double GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault=0.0);

	// Sets an Double value to INI file or registry.
	BOOL WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue);

	// Retrieve a string value from INI file or registry.
	CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,int nSize=2048,
		LPCTSTR lpszDefault = NULL);

	// Sets a string value to INI file or registry.
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		LPCTSTR lpszValue);

	bool GetProfileBool(LPCTSTR lpszSection, LPCTSTR lpszEntry,bool bDefault=false);
	BOOL WriteProfileBool(LPCTSTR lpszSection, LPCTSTR lpszEntry,bool bDefault);
private:
	~GUserDataRecord(void);
	void initFile();
	TCHAR m_lpFile[MAX_PATH];
	static GUserDataRecord m_Record;
	
};
