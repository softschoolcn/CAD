#pragma once
typedef void (*GTextOprEnum)(CString&);
class GTextTools
{
public:
	GTextTools(void);
public:
	~GTextTools(void);
public:
	///////////////////////////公有操作//////////////////////////////////////
	///////////////////////////单行文字//////////////////////////////////////
	//保护文字中的特殊字符
	static Acad::ErrorStatus ProtectTextSpecial(CString& strText);
	//解除文字中的特殊字符保护
	static Acad::ErrorStatus UnProtectTextSpecial(CString& strText);
	//删除文字中的下滑线
	static Acad::ErrorStatus RemoveTextUnderline(CString& strText);
	//删除文字中的上滑线
	static Acad::ErrorStatus RemoveTextUpperline(CString& strText);
	//增加文字中的下滑线
	static Acad::ErrorStatus AddTextUnderline(CString& strText);
	//增加文字中的上滑线
	static Acad::ErrorStatus AddTextUpperline(CString& strText);

	//////////////////////////多行文字///////////////////////////////////////
	//保护多行文字中的特殊字符
	static Acad::ErrorStatus ProtectMTextSpecial(CString& strText);
	//解除多行文字中的特殊字符保护
	static Acad::ErrorStatus UnProtectMTextSpecial(CString& strText);
	//删除多行文字中的下滑线
	static Acad::ErrorStatus RemoveMTextUnderline(CString& strText);
	//删除多行文字中的上滑线
	static Acad::ErrorStatus RemoveMTextUpperline(CString& strText);
	//增加多行文字中的下滑线
	static Acad::ErrorStatus AddMTextUnderline(CString& strText);
	//增加多行文字中的上滑线
	static Acad::ErrorStatus AddMTextUpperline(CString& strText);
	//遍历多行文字纯内容
	static Acad::ErrorStatus EnumMText(CString& strText,GTextOprEnum oprEnum);
private:
	static void MTextGroupIndex(CArray<int>& nStartIndexArray,CArray<int>& nEndIndexArray,CString& strText);
	static void MTextSubGroupIndex(CArray<int>& nStartIndexArray,CArray<int>& nEndIndexArray,CString& strText,int nStartIndex);
	//遍历多行文字纯内容，处理大括号
	static BOOL _EnumMText(CString& strText,GTextOprEnum oprEnum);
	//遍历多行文字纯内容，处理控制字符
	static BOOL _SubEnumMText(CString& strText,GTextOprEnum oprEnum);
};
