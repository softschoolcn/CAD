#pragma once

class GLOBALTOOLS_EXT GStringHelper
{
public:
	GStringHelper(void);
public:
	~GStringHelper(void);
	//功能:	指定的字符串是否是合理的长整数（只能处理十进制的）
	//参数:	value,指定的字符串
	//      nValue,返回长整数型结果
	//返回: 是否是合理的长整数
	static BOOL IsValidLong(const CString value,long &lValue);
	//功能:	指定的字符串是否是合理的整数（只能处理十进制的）
	//参数:	value,指定的字符串
	//      nValue,返回整数型结果
	//返回: 是否是合理的整数
	static BOOL IsValidInt(const CString value,int &nValue);
	//功能:	指定的字符串是否是合理的浮点数（只能处理十进制的）
	//参数:	value,指定的字符串
	//      nValue,返回浮点数结果
	//返回: 是否是合理的浮点数
	static BOOL IsValidDouble(const CString value,double& dValue);
	//功能:	将CString转换成char*,自动区分UNICODE和ASCII
	//参数:	swString,需要转化的字符串
	//      szBuffer,char*指针，使用完后记得使用free释放
	//返回: 
	static void ConvertToLPCSTR(char*& szBuffer,CString swString);
	static std::string ConvertToLPCSTR(CString swString);
	/*!
     @zhangshl 生成GUID
     */
	static CString NewGuid();
	/*!
     @brief 根据token拆分字符串src，放到result里
     */
    static bool Split(CString& src, CString token, CStringArray& resultArray);
    /*!
     @brief 根据token拆分字符串src，放到result里
	 ingoreEmptyString:是否忽略空字符串
     */
    static bool Split(CString& src, CString token, CStringArray& resultArray,bool ingoreEmptyString);
	//功能:判断字符串是否在字符串数组中
	//参数:	array:字符串数组
	//			string:字符串
	//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
	//返回:	true:在数组中；false:不在
	static bool IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase=true);
};
