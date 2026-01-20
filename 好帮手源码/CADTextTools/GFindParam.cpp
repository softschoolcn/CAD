#include "Stdafx.h"
#include "GFindParam.h"
bool GFindParam::IsMatch(CString str)
{
	CString strFindTmp=strFind;
	CString strTmp=str;
	if(m_bCase)
	{
		strFindTmp.MakeUpper();
		strTmp.MakeUpper();
	}
	if(m_bFullMatch)
	{
		if(strTmp.Compare(strFindTmp)==0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(m_bTPF)
		{
			GStringMatch match;
			match.m_bIngoreCase=false;
			if(match.IsMatch(strTmp,strFindTmp))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return strTmp.Find(strFindTmp)>=0;
		}
	}
	return true;
}
void GFindParam::Read()
{
	m_bCase=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindParam"),_T("Case"),false);
	m_bFullMatch=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindParam"),_T("FullMatch"),false);
	m_bTPF=GUserDataRecord::getInstance()->GetProfileBool(_T("GFindParam"),_T("TPF"),false);
	if(m_bFullMatch)
	{
		m_bTPF=false;
	}
	strFind=GUserDataRecord::getInstance()->GetProfileString(_T("GFindParam"),_T("Find"),256,_T(""));
}
void GFindParam::Write()
{
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindParam"),_T("Case"),m_bCase);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindParam"),_T("FullMatch"),m_bFullMatch);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GFindParam"),_T("TPF"),m_bTPF);
	GUserDataRecord::getInstance()->WriteProfileString(_T("GFindParam"),_T("Find"),strFind);
}