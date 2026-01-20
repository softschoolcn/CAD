#pragma once

struct MTextItem
{
	CString str;//内容
	int nType;//类型,-1:待分析内容,0:文字内容,1:符号控制
};
struct GLOBALTOOLS_EXT MTextItemArray
{
	MTextItemArray()
	{
	}
	MTextItem              item;
	CArray<MTextItemArray> children;
	MTextItemArray(const MTextItemArray& src)
	{
		item.str=src.item.str;
		item.nType=src.item.nType;
		children.RemoveAll();
		children.Append(src.children);
	}
	MTextItemArray& operator=(const MTextItemArray& src)
	{
		item.str=src.item.str;
		item.nType=src.item.nType;
		children.RemoveAll();
		children.Append(src.children);
		return *this;
	}
	void updateChildren();
	CString getText();
	CString getTextCAD();
};
//保护特殊字符
void GLOBALTOOLS_EXT ProtectSpecial(CString& strText);
//恢复保护特殊字符
void GLOBALTOOLS_EXT UnProtectSpecial(CString& strText);
