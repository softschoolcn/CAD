#include "StdAfx.h"
#include "GStringMatch.h"

GStringMatch::GStringMatch(void)
{
	m_bIngoreCase=true;
}

GStringMatch::~GStringMatch(void)
{
}
//是否匹配
bool GStringMatch::IsMatch(CString str,CString strMatch)
{
	if(str.IsEmpty())
	{
		return false;
	}
	if(strMatch.IsEmpty())
	{
		return false;
	}
	if(m_bIngoreCase)
	{
		//忽略大小写
		str.MakeUpper();
		strMatch.MakeUpper();
	}
	CStringArray strMatchArray;
	CString strTmp=_T("");
	for(int i=0;i<strMatch.GetLength();i++)
	{
		TCHAR c=strMatch.GetAt(i);
		if(c==_T('*'))
		{
			if(!strTmp.IsEmpty())
			{
				strMatchArray.Add(strTmp);
				strTmp=_T("");
			}
			if(strMatchArray.GetCount()==0)
			{
				strMatchArray.Add(_T("*"));
			}
			else
			{
				if(strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("*"))!=0&&
					strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("?"))!=0)
				{
					//两个连续的*号没有意义
					//*?也是没有意义的
					strMatchArray.Add(_T("*"));
				}
			}
		}
		else if(c==_T('?'))
		{
			if(!strTmp.IsEmpty())
			{
				strMatchArray.Add(strTmp);
				strTmp=_T("");
			}
			if(strMatchArray.GetCount()==0)
			{
				strMatchArray.Add(_T("?"));
			}
			else
			{
				if(strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("*"))!=0&&
					strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("?"))!=0)
				{
					strMatchArray.Add(_T("?"));
				}
				else if(strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("*"))==0)
				{
					//*?其实相当于?
					strMatchArray.SetAt(strMatchArray.GetCount()-1,_T("?"));
				}
				else if(strMatchArray.GetAt(strMatchArray.GetCount()-1).Compare(_T("?"))==0)
				{
					//??相当于?
				}
			}
		}
		else
		{
			strTmp+=c;
			if(i==strMatch.GetLength()-1)
			{
				strMatchArray.Add(strTmp);
			}
		}
	}
	if(strMatchArray.GetCount()==0)
	{
		return false;
	}
	return IsMatch(str,strMatchArray);
}
bool GStringMatch::IsMatch(CString str,CStringArray& strMatchArray)
{
	if(strMatchArray.GetCount()==0)
	{
		if(str.IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	CString strMatch0=strMatchArray.GetAt(0);
	if(strMatch0.CompareNoCase(_T("*"))==0)
	{
		if(strMatchArray.GetCount()==1)
		{
			if(str.IsEmpty())
			{
				return false;
			}
			else
			{
			return true;
			}
		}
		CString strMatch1=strMatchArray.GetAt(1);
		int nIndex=str.Find(strMatch1);
		if(nIndex<0)
		{
			return false;
		}
		if(nIndex==0)
		{
			//*不能匹配空内容
			return false;
		}
		int nLen=str.GetLength()-(nIndex+strMatch1.GetLength());
		str=nLen>0?str.Right(nLen):_T("");
		strMatchArray.RemoveAt(0);
		strMatchArray.RemoveAt(0);
		return IsMatch(str,strMatchArray);

	}
	else if(strMatch0.CompareNoCase(_T("?"))==0)
	{
		if(strMatchArray.GetCount()==1)
		{
			return true;
		}
		CString strMatch1=strMatchArray.GetAt(1);
		int nIndex=str.Find(strMatch1);
		if(nIndex<0)
		{
			return false;
		}
		int nLen=str.GetLength()-(nIndex+strMatch1.GetLength());
		str=nLen>0?str.Right(nLen):_T("");
		strMatchArray.RemoveAt(0);
		strMatchArray.RemoveAt(0);
		return IsMatch(str,strMatchArray);
	}
	else
	{
		if(str.GetLength()<strMatch0.GetLength())
		{
			return false;
		}
		CString str0=str.Left(strMatch0.GetLength());
		if(str0.Compare(strMatch0)!=0)
		{
			return false;
		}
		int nLen=str.GetLength()-strMatch0.GetLength();
		str=nLen>0?str.Right(nLen):_T("");
		strMatchArray.RemoveAt(0);
		return IsMatch(str,strMatchArray);
	}
}
