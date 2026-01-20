#include "StdAfx.h"
#include "GMTextFun.h"
//返回TRUE，已经拆分完毕，不需要再分了
//否则需要再分
BOOL _SplitMText(CStringArray& strTextArr,CString& strText)
{
	if(strText.IsEmpty())
	{
		return TRUE;
	}
	int nStartIndex=strText.Find(_T('{'));
	if(nStartIndex<0)
	{
		strTextArr.Add(strText);
		return TRUE;
	}
	int nLeft=0;//左大括号
	int nRight=0;//右大括号
	for(int i=nStartIndex+1;i<strText.GetLength();i++)
	{
		TCHAR c=strText.GetAt(i);
		if(_T('{')==c)
		{
			nLeft++;
		}
		else if(_T('}')==c)
		{
			nRight++;
		}
		if(nRight-nLeft==1)
		{
			if(i==strText.GetLength()-1&&nStartIndex==0)
			{
				strText=strText.Mid(1,strText.GetLength()-2);
				return _SplitMText(strTextArr,strText);
			}
			//找到匹配的大括号
			if(nStartIndex>0)
			{
				strTextArr.Add(strText.Left(nStartIndex));
			}
			strTextArr.Add(strText.Mid(nStartIndex+1,(i-1)-(nStartIndex+1)+1));
			if(i==strText.GetLength()-1)
			{
				strText.Empty();
				return TRUE;
			}
			strText=strText.Right(strText.GetLength()-(i+1));
			return FALSE;
		}
	}
	strTextArr.Add(strText);
	return TRUE;
}
void SplitMText(CStringArray& strTextArr,CString strText)
{
	CString strTmp=strText;
	while(!_SplitMText(strTextArr,strTmp))
	{

	}
}
void SplitMText(CStringArray& strTextArr,CArray<int>&  parentIndexArray,int parentIndex,CString strText)
{
	CStringArray strTmpArr;
	SplitMText(strTmpArr,strText);
	if(0==strTmpArr.GetCount()||1==strTmpArr.GetCount())
	{
		return;
	}
	for(int i=0;i<strTmpArr.GetCount();i++)
	{
		int nIndex=strTextArr.Add(strTmpArr.GetAt(i));
		parentIndexArray.Add(parentIndex);//顶层的
		SplitMText(strTextArr,parentIndexArray,nIndex,strTmpArr.GetAt(i));
	}
}
BOOL _SplitSubMText(CStringArray& strSubTextArray,CArray<int>&  nSubTypeArray,CString& strSubText)
{
	if(strSubText.IsEmpty())
	{
		return TRUE;
	}
	int nStartIndex=strSubText.Find(_T('\\'));
	if(nStartIndex<0)
	{
		strSubTextArray.Add(strSubText);
		nSubTypeArray.Add(0);
		return TRUE;
	}
	if(nStartIndex==strSubText.GetLength()-1)
	{
		strSubTextArray.Add(strSubText);
		nSubTypeArray.Add(0);
		return TRUE;
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
		if(nStartIndex!=0)
		{
			strSubTextArray.Add(strSubText.Left(nStartIndex));
			nSubTypeArray.Add(0);
		}
		strSubTextArray.Add(strSubText.Mid(nStartIndex,2));
		nSubTypeArray.Add(1);
		if(nStartIndex+1!=strSubText.GetLength()-1)
		{
			strSubText=strSubText.Right((strSubText.GetLength()-1)-(nStartIndex+2)+1);
			return FALSE;
		}
		else
		{
			return TRUE;
		}

	}
	if(nStartIndex+1>=strSubText.GetLength()-1)
	{
		strSubTextArray.Add(strSubText);
		nSubTypeArray.Add(0);
		return TRUE;
	}
	int nEndIndex=strSubText.Find(_T(';'),nStartIndex+2);
	if(nEndIndex<nStartIndex)
	{
		strSubTextArray.Add(strSubText);
		nSubTypeArray.Add(0);
		return TRUE;
	}
	if(c==_T('C')||c==_T('F')||c==_T('f')||c==_T('H')||
		c==_T('Q')||c==_T('W')||c==_T('A'))
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
		if(nStartIndex!=0)
		{
			strSubTextArray.Add(strSubText.Left(nStartIndex));
			nSubTypeArray.Add(0);
		}
		strSubTextArray.Add(strSubText.Mid(nStartIndex,nEndIndex-nStartIndex+1));
		nSubTypeArray.Add(1);
		if(nEndIndex!=strSubText.GetLength()-1)
		{
			strSubText=strSubText.Right((strSubText.GetLength()-1)-(nEndIndex+1)+1);
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	//第三类，只有一种最复杂
	else if(c==_T('S'))
	{
		//堆叠 \、# 或 ^ 符号后的文字
		if(nStartIndex!=0)
		{
			strSubTextArray.Add(strSubText.Left(nStartIndex));
			nSubTypeArray.Add(0);
		}
		strSubTextArray.Add(strSubText.Mid(nStartIndex,nEndIndex-nStartIndex+1));
		nSubTypeArray.Add(1);
		if(nEndIndex!=strSubText.GetLength()-1)
		{
			strSubText=strSubText.Right((strSubText.GetLength()-1)-(nEndIndex+1)+1);
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		strSubTextArray.Add(strSubText);
		nSubTypeArray.Add(0);
		return TRUE;
	}


	return FALSE;

}
void SplitSubMText(CStringArray& strSubTextArray,CArray<int>&  nSubTypeArray,CString strSubText)
{
	CString strTmp=strSubText;
	while(!_SplitSubMText(strSubTextArray,nSubTypeArray,strTmp))
	{

	}
}
void MTextItemArray::updateChildren()
{
	ProtectSpecial(item.str);
	children.RemoveAll();
	CStringArray strTmpArr;
	SplitMText(strTmpArr,item.str);
	if(0==strTmpArr.GetCount()||1==strTmpArr.GetCount())
	{
		UnProtectSpecial(item.str);
		CStringArray strSubTextArray;
		CArray<int>  nSubTypeArray;
		SplitSubMText(strSubTextArray,nSubTypeArray,1==strTmpArr.GetCount()?strTmpArr.GetAt(0):item.str);
		for(int i=0;i<strSubTextArray.GetCount();i++)
		{
			MTextItemArray tmp;
			tmp.item.nType=nSubTypeArray.GetAt(i);
			tmp.item.str=strSubTextArray.GetAt(i);
			UnProtectSpecial(tmp.item.str);
			children.Add(tmp);
		}
	}
	else
	{
		UnProtectSpecial(item.str);
		for(int i=0;i<strTmpArr.GetCount();i++)
		{
			MTextItemArray tmp;
			tmp.item.nType=-1;
			tmp.item.str=strTmpArr.GetAt(i);
			tmp.updateChildren();
			children.Add(tmp);
		}
	}
}
CString MTextItemArray::getText()
{
	if(children.GetCount()>0)
	{
		CString strText=_T("");
		for(int i=0;i<children.GetCount();i++)
		{
			MTextItemArray* pTmp=children.GetData()+i;
			strText+=pTmp->getText();
		}
		return strText;
	}
	else
	{
		if(item.nType==0)
		{
			return item.str;
		}
		else if(item.nType==1)
		{
			if(item.str.GetLength()<2)
			{
				return _T("");
			}
			else
			{
				TCHAR c=item.str.GetAt(1);
				if(c==_T('P'))
				{
					return _T("\r\n");
				}
				else if(c==_T('~'))
				{
					return _T(" ");
				}
				else if(c==_T('S'))
				{
					CString strTmp=item.str;
					strTmp.SetAt(0,_T('0'));
					int nIndex1=strTmp.FindOneOf(_T("\\#^"));
					if(nIndex1<0)
						return _T("");
					if(item.str.GetAt(item.str.GetLength()-1)!=_T(';'))
					{
						return _T("");
					}
					CString str1=item.str.Mid(3,nIndex1-1-3+1);
					CString str2=item.str.Mid(nIndex1+1,item.str.GetLength()-2-(nIndex1+1)+1);
					return str1+str2;
				}
				else
				{
					return _T("");
				}
			}
		}
		else
		{
			return _T("");
		}
	}
}
CString MTextItemArray::getTextCAD()
{
	if(children.GetCount()>0)
	{
		CString strText=_T("");
		for(int i=0;i<children.GetCount();i++)
		{
			MTextItemArray* pTmp=children.GetData()+i;
			strText+=pTmp->getText();
		}
		return strText;
	}
	else
	{
		if(item.nType==0)
		{
			return item.str;
		}
		else if(item.nType==1)
		{
			if(item.str.GetLength()<2)
			{
				return _T("");
			}
			else
			{
				TCHAR c=item.str.GetAt(1);
				if(c==_T('P'))
				{
					return _T("\\P");
				}
				else if(c==_T('~'))
				{
					return _T("\\~");
				}
				else if(c==_T('S'))
				{
					CString strTmp=item.str;
					strTmp.SetAt(0,_T('0'));
					int nIndex1=strTmp.FindOneOf(_T("\\#^"));
					if(nIndex1<0)
						return _T("");
					if(item.str.GetAt(item.str.GetLength()-1)!=_T(';'))
					{
						return _T("");
					}
					CString str1=item.str.Mid(3,nIndex1-1-3+1);
					CString str2=item.str.Mid(nIndex1+1,item.str.GetLength()-2-(nIndex1+1)+1);
					return str1+str2;
				}
				else
				{
					return _T("");
				}
			}
		}
		else
		{
			return _T("");
		}
	}
}
//保护特殊字符
void ProtectSpecial(CString& strText)
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
}
//恢复保护特殊字符
void UnProtectSpecial(CString& strText)
{
	CString strTmp1;
	strTmp1.Format(_T("%c"),(TCHAR)1);
	CString strTmp2;
	strTmp2.Format(_T("%c"),(TCHAR)2);
	CString strTmp3;
	strTmp3.Format(_T("%c"),(TCHAR)3);
	strText.Replace(strTmp1,_T("\\"));
	strText.Replace(strTmp2,_T("{"));
	strText.Replace(strTmp3,_T("}"));
}

