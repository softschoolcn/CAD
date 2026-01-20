#include "StdAfx.h"
#include "GFindTypes.h"

GFindTypes::GFindTypes(void)
{
	m_bAttr=false;
	m_bDim=false;
	m_bText=false;
	m_bTable=false;
	m_bLinkDesc=false;
	m_bLink=false;
	m_bBlock=false;
}

GFindTypes::~GFindTypes(void)
{
}
void GFindTypes::Read()
{
	m_bAttr=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Attr"),true);
	m_bDim=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Dim"),true);
	m_bText=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Text"),true);
	m_bTable=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Table"),true);
	m_bLinkDesc=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("LinkDesc"),true);
	m_bLink=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Link"),true);
	m_bBlock=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindTypes"),_T("Block"),false);
}
void GFindTypes::Write()
{
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Attr"),m_bAttr);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Dim"),m_bDim);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Text"),m_bText);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Table"),m_bTable);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("LinkDesc"),m_bLinkDesc);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Link"),m_bLink);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindTypes"),_T("Block"),m_bBlock);
}
int GFindTypes::GetCount()
{
	int nCount=0;
	if(m_bAttr)
	{
		nCount++;
	}
	if(m_bDim)
	{
		nCount++;
	}
	if(m_bText)
	{
		nCount++;
	}
	if(m_bTable)
	{
		nCount++;
	}
	if(m_bLinkDesc)
	{
		nCount++;
	}
	if(m_bLink)
	{
		nCount++;
	}
	return nCount;

}
CString GFindTypes::GetTypeDesc(int nType)
{
	if(nType==0)
	{
		return _T("属性");
	}
	else if(nType==1)
	{
		return _T("标注");
	}
	else if(nType==2)
	{
		return _T("文字");
	}
	else if(nType==3)
	{
		return _T("表格文字");
	}
	else if(nType==4)
	{
		return _T("超链接说明");
	}
	else if(nType==5)
	{
		return _T("超链接");
	}
	else
	{
		return _T("未知类型");
	}
}
