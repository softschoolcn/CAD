#include "StdAfx.h"
#include "GTextTools.h"

GTextTools::GTextTools(void)
{
}

GTextTools::~GTextTools(void)
{
}
///////////////////////////公有操作//////////////////////////////////////
///////////////////////////公有操作//////////////////////////////////////

///////////////////////////单行文字//////////////////////////////////////
//保护文字中的特殊字符
Acad::ErrorStatus GTextTools::ProtectTextSpecial(CString& strText)
{
	CString strTmp1;
	strTmp1.Format(_T("%c"),(TCHAR)1);
	strText.Replace(_T("%%%"),strTmp1);
	return Acad::eOk;
}
//解除文字中的特殊字符保护
Acad::ErrorStatus GTextTools::UnProtectTextSpecial(CString& strText)
{
	CString strTmp1;
	strTmp1.Format(_T("%c"),(TCHAR)1);
	strText.Replace(strTmp1,_T("%%%"));
	return Acad::eOk;
}
//删除文字中的下滑线
Acad::ErrorStatus GTextTools::RemoveTextUnderline(CString& strText)
{
	ProtectTextSpecial(strText);
	strText.Replace(_T("%%u"),_T(""));
	strText.Replace(_T("%%U"),_T(""));
	UnProtectTextSpecial(strText);
	return Acad::eOk;
}
//删除文字中的上滑线
Acad::ErrorStatus GTextTools::RemoveTextUpperline(CString& strText)
{
	ProtectTextSpecial(strText);
	strText.Replace(_T("%%o"),_T(""));
	strText.Replace(_T("%%O"),_T(""));
	UnProtectTextSpecial(strText);
	return Acad::eOk;
}
//增加文字中的下滑线
Acad::ErrorStatus GTextTools::AddTextUnderline(CString& strText)
{
	RemoveTextUnderline(strText);//预防性删除
	ProtectTextSpecial(strText);
	strText=_T("%%u")+strText;
	UnProtectTextSpecial(strText);
	return Acad::eOk;
}
//增加文字中的上滑线
Acad::ErrorStatus GTextTools::AddTextUpperline(CString& strText)
{
	RemoveTextUpperline(strText);
	ProtectTextSpecial(strText);
	strText=_T("%%o")+strText;
	UnProtectTextSpecial(strText);
	return Acad::eOk;
}
///////////////////////////单行文字//////////////////////////////////////

//////////////////////////多行文字///////////////////////////////////////
//保护多行文字中的特殊字符
Acad::ErrorStatus GTextTools::ProtectMTextSpecial(CString& strText)
{
	CString strTmp1;
	strTmp1.Format(_T("%c"),(TCHAR)1);
	CString strTmp2;
	strTmp2.Format(_T("%c"),(TCHAR)2);
	CString strTmp3;
	strTmp3.Format(_T("%c"),(TCHAR)3);
	strText.Replace(_T("\\\\"),strTmp1);
	strText.Replace(_T("\\{"),strTmp2);
	strText.Replace(_T("\\}"),strTmp3);
	return Acad::eOk;
}
//解除多行文字中的特殊字符保护
Acad::ErrorStatus GTextTools::UnProtectMTextSpecial(CString& strText)
{
	CString strTmp1;
	strTmp1.Format(_T("%c"),(TCHAR)1);
	CString strTmp2;
	strTmp2.Format(_T("%c"),(TCHAR)2);
	CString strTmp3;
	strTmp3.Format(_T("%c"),(TCHAR)3);
	strText.Replace(strTmp1,_T("\\\\"));
	strText.Replace(strTmp2,_T("\\{"));
	strText.Replace(strTmp3,_T("\\}"));
	return Acad::eOk;
}
//删除多行文字中的下滑线
Acad::ErrorStatus GTextTools::RemoveMTextUnderline(CString& strText)
{
	ProtectMTextSpecial(strText);
	strText.Replace(_T("\\L"),_T(""));
	strText.Replace(_T("\\l"),_T(""));
	UnProtectMTextSpecial(strText);
	return Acad::eOk;
}
//删除多行文字中的上滑线
Acad::ErrorStatus GTextTools::RemoveMTextUpperline(CString& strText)
{
	ProtectMTextSpecial(strText);
	strText.Replace(_T("\\O"),_T(""));
	strText.Replace(_T("\\o"),_T(""));
	UnProtectMTextSpecial(strText);
	return Acad::eOk;
}
//增加多行文字中的下滑线
Acad::ErrorStatus GTextTools::AddMTextUnderline(CString& strText)
{
	RemoveMTextUnderline(strText);
	ProtectMTextSpecial(strText);
	strText=_T("\\L")+strText+_T("\\l");
	UnProtectMTextSpecial(strText);
	return Acad::eOk;
}
//增加多行文字中的上滑线
Acad::ErrorStatus GTextTools::AddMTextUpperline(CString& strText)
{
	RemoveMTextUpperline(strText);
	ProtectMTextSpecial(strText);
	strText=_T("\\O")+strText+_T("\\o");
	UnProtectMTextSpecial(strText);
	return Acad::eOk;
}
//遍历多行文字纯内容，处理控制字符
BOOL GTextTools::_SubEnumMText(CString& strText,GTextOprEnum oprEnum)
{
	CArray<int> nStartIndexArray;
	CArray<int> nEndIndexArray;
	MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strText,0);
	if(nStartIndexArray.GetCount()==0)
	{
		oprEnum(strText);
		return TRUE;
	}
	CString strResult=_T("");
	int nIndex0=nStartIndexArray.GetAt(0);
	if(nIndex0!=0)
	{
		CString str0=strText.Left(nIndex0);
		oprEnum(str0);
		strResult+=str0;
	}
	for(int i=0;i<nStartIndexArray.GetCount();i++)
	{
		if(i!=0)
		{
			int nStartIndex1=nEndIndexArray.GetAt(i-1)+1;
		    int nEndIndex1=nStartIndexArray.GetAt(i)-1;
			int nLen=nEndIndex1-nStartIndex1+1;
			if(nLen>0)
			{
				//两个大括号之间的内容
				CString str3=strText.Mid(nStartIndex1,nEndIndex1-nStartIndex1+1);
				oprEnum(str3);
				strResult+=str3;
			}
		}
		int nStartIndex=nStartIndexArray.GetAt(i);
		int nEndIndex=nEndIndexArray.GetAt(i);
		CString str1=strText.Mid(nStartIndex,(nEndIndex)-(nStartIndex)+1);
		strResult+=str1;
	}
	int nIndexE=nEndIndexArray.GetAt(nEndIndexArray.GetCount()-1);
	int nLen=(strText.GetLength()-1)-(nIndexE+1)+1;
	if(nLen>0)
	{
		CString str2=strText.Right(nLen);
		oprEnum(str2);
		strResult+=str2;
	}
	strText=strResult;
	return TRUE;
}
//遍历多行文字纯内容
BOOL GTextTools::_EnumMText(CString& strText,GTextOprEnum oprEnum)
{
	CArray<int> nStartIndexArray;
	CArray<int> nEndIndexArray;
	MTextGroupIndex(nStartIndexArray,nEndIndexArray,strText);
	if(nStartIndexArray.GetCount()==0)
	{
		return _SubEnumMText(strText,oprEnum);
	}
	CString strResult=_T("");
	int nIndex0=nStartIndexArray.GetAt(0);
	if(nIndex0!=0)
	{
		CString str0=strText.Left(nIndex0);
		_SubEnumMText(str0,oprEnum);
		strResult+=str0;
	}
	for(int i=0;i<nStartIndexArray.GetCount();i++)
	{
		if(i!=0)
		{
			int nStartIndex1=nEndIndexArray.GetAt(i-1)+1;
		    int nEndIndex1=nStartIndexArray.GetAt(i)-1;
			int nLen=nEndIndex1-nStartIndex1+1;
			if(nLen>0)
			{
				//两个大括号之间的内容
				CString str3=strText.Mid(nStartIndex1,nEndIndex1-nStartIndex1+1);
				_SubEnumMText(str3,oprEnum);
				strResult+=str3;
			}
		}
		int nStartIndex=nStartIndexArray.GetAt(i);
		int nEndIndex=nEndIndexArray.GetAt(i);
		CString str1=strText.Mid(nStartIndex+1,(nEndIndex-1)-(nStartIndex+1)+1);
		_EnumMText(str1,oprEnum);
		strResult+=_T("{")+str1+_T("}");
	}
	int nIndexE=nEndIndexArray.GetAt(nEndIndexArray.GetCount()-1);
	int nLen=(strText.GetLength()-1)-(nIndexE+1)+1;
	if(nLen>0)
	{
		CString str2=strText.Right(nLen);
		_SubEnumMText(str2,oprEnum);
		strResult+=str2;
	}
	strText=strResult;
	return TRUE;
}
void GTextTools::MTextGroupIndex(CArray<int>& nStartIndexArray,CArray<int>& nEndIndexArray,CString& strText)
{
	if(strText.IsEmpty())
	{
		return;
	}
	int nStartIndex=strText.Find(_T('{'));
	if(nStartIndex<0)
	{
		return;
	}
	int nLeft=0;//左大括号
	int nRight=0;//右大括号
	for(int i=nStartIndex+1;i<strText.GetLength();i++)
	{
		TCHAR c=strText.GetAt(i);
		if(_T('{')==c)
		{
			nLeft++;
			if(nRight==nLeft)
			{
				nStartIndexArray.Add(i);
			}
		}
		else if(_T('}')==c)
		{
			nRight++;
		}
		if(nRight-nLeft==1)
		{
			nStartIndexArray.Add(nStartIndex);
			nEndIndexArray.Add(i);
		}
	}
	for(int i=nStartIndexArray.GetCount()-1;i>nEndIndexArray.GetCount();i--)
	{
		nStartIndexArray.RemoveAt(i);
	}
}
void GTextTools::MTextSubGroupIndex(CArray<int>& nStartIndexArray,CArray<int>& nEndIndexArray,CString& strSubText,int nStart)
{
	if(strSubText.IsEmpty())
	{
		return;
	}
	if(nStart>=strSubText.GetLength())
	{
		return;
	}
	int nStartIndex=strSubText.Find(_T('\\'),nStart);
	if(nStartIndex<0)
	{
		return;
	}
	if(nStartIndex==strSubText.GetLength()-1)
	{
		return;
	}
	TCHAR c=strSubText.GetAt(nStartIndex+1);
	if(c==_T('O')||c==_T('o')||c==_T('L')||c==_T('l')||
		c==_T('~')||c==_T('P'))
	{
		//第一类
		if(c==_T('O'))
		{
			//打开上划线
		}
		else if(c==_T('o'))
		{
			//关闭上划线
		}
		if(c==_T('L'))
		{
			//打开下划线
		}
		else if(c==_T('l'))
		{
			//关闭下划线
		}
		else if(c==_T('~'))
		{
			//插入不间断空格
		}
		else if(c==_T('P'))
		{
			//结束段落
		}
		nStartIndexArray.Add(nStartIndex);
		nEndIndexArray.Add(nStartIndex+1);
		MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nStartIndex+2);

	}
	if(c==_T('C')||c==_T('F')||c==_T('f')||c==_T('H')||
		c==_T('Q')||c==_T('W')||c==_T('A')||c==_T('S'))
	{
		int nEndIndex=strSubText.Find(_T(';'),nStartIndex+2);
		if(nEndIndex<nStartIndex)
		{
			MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nStartIndex+1);
		}
		else
		{
			if(c==_T('S'))
			{
				//堆叠 \、# 或 ^ 符号后的文字
				int nMidIndex=strSubText.FindOneOf(_T("\\#^"));
				if(nMidIndex<0||nMidIndex>=nEndIndex)
				{
					MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nEndIndex+1);
				}
				else
				{
					nStartIndexArray.Add(nStartIndex);
					nEndIndexArray.Add(nStartIndex+1);

					nStartIndexArray.Add(nMidIndex);
					nEndIndexArray.Add(nMidIndex);

					nStartIndexArray.Add(nEndIndex);
					nEndIndexArray.Add(nEndIndex);

					MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nEndIndex+1);
				}
			}
			else
			{
				//第二类
				if(c==_T('C'))
				{
					//修改为 指定的颜色
				}
				else if(c==_T('F')||c==_T('f'))
				{
					//修改为 指定的字体文件

				}
				else if(c==_T('H'))
				{
					//修改为 以图形单位表示的 指定文字高度 Autodesk \H2;AutoCAD
					//将文字高度修改为 当前样式文字高度的 数倍 Autodesk \H3x;AutoCAD
				}
				else if(c==_T('T'))
				{
					//调整字符之间的间距。有效值范围为字符间原始间距的 0.75 倍到字符间原始间距的 4 倍
				}
				else if(c==_T('Q'))
				{
					//修改倾斜角度
				}
				else if(c==_T('W'))
				{
					//修改宽度因子生成宽字
				}
				else if(c==_T('A'))
				{
					//设置对齐方式值，有效值为：0、1、2 （底端对正、居中对正、顶端对正）
				}
				nStartIndexArray.Add(nStartIndex);
				nEndIndexArray.Add(nEndIndex);
				MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nEndIndex+1);
			}
		}
	}
	else
	{
		MTextSubGroupIndex(nStartIndexArray,nEndIndexArray,strSubText,nStartIndex+1);
	}
}
//遍历多行文字纯内容
Acad::ErrorStatus GTextTools::EnumMText(CString& strText,GTextOprEnum oprEnum)
{
	ProtectMTextSpecial(strText);
	_EnumMText(strText,oprEnum);
	UnProtectMTextSpecial(strText);
	return Acad::eOk;
}
//////////////////////////多行文字///////////////////////////////////////
