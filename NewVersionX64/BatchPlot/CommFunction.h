#pragma once
//功能:判断字符串是否在字符串数组中
//参数:	array:字符串数组
//			string:字符串
//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
//返回:	true:在数组中；false:不在
bool IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase=true);
// 功能：将选择集转换为实体ID数组
// 参数：	  ssName,选择集
//			  ObjIds,实体ID数组		
// 返回： 
void SSToIds(ads_name ssName,AcDbObjectIdArray& ObjIds);
BOOL GetCurDwgFileName(CString& strDwgFileName);
//功能:	指定的字符串是否是合理的长整数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回长整数型结果
//返回: 是否是合理的长整数
BOOL IsValidLong(const CString value,long &lValue);
//功能:	指定的字符串是否是合理的整数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回整数型结果
//返回: 是否是合理的整数
BOOL IsValidInt(const CString value,int &nValue);
//功能:	指定的字符串是否是合理的浮点数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回浮点数结果
//返回: 是否是合理的浮点数
BOOL IsValidDouble(const CString value,double& dValue);
BOOL getViewExtent(AcGePoint3d &leftBottom , AcGePoint3d &rightTop,CRect& Region);
void SplitString(CString string,char marker,CStringArray  &stringArray);
void CombineArrayToString(const CStringArray& array,CString &string,CString linkSymbol);
CString CombineArrayToString(const CStringArray& array,CString linkSymbol);
Acad::ErrorStatus OpenAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
