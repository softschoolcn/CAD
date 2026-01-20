// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//------ StdAfx.cpp : source file that includes just the standard includes
//------  StdAfx.pch will be the pre-compiled header
//------  StdAfx.obj will contain the pre-compiled type information
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//读取XML节点的值
CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName)
{
	MSXML2::IXMLDOMNodePtr lpAttrNode = lpNode->attributes->getNamedItem(strName.GetBuffer());
	strName.ReleaseBuffer();
	if(lpAttrNode==NULL)
		return _T("");
	return (char*)lpAttrNode->text;
}
CString ReadRegKey()
{
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("Licence.ini"));
	if(PathFileExists(m_lpFile))
	{
		TCHAR lpResult[1024+1]={0};
		if(GetPrivateProfileString(_T("System"),_T("RegKey"),_T(""),lpResult,1024,m_lpFile))
		{
			return CString(lpResult);
		}
	}
	return _T("");
}
BOOL RegCodeBack(CString& regCode,CString& strId,CString& strDate)
{
	CByteString byteStr;
	BYTE* data=NULL;
	long byteLen;
	if(!byteStr.toByte(data,byteLen,regCode))
	{
		return FALSE;
	}
	AcArray<byte> byteArray;
	for(int i=0;i<byteLen;i++)
	{
		byteArray.append(data[i]);
	}
	delete data;
	GDES::TransformData(byteArray,FALSE);
	GDES::TransformData(byteArray,FALSE);
	GDES::TransformData(byteArray,FALSE);
	GDES::TransformData(byteArray,FALSE);
	strId=_T("");
	AcArray<byte> byteDates;
	for(int i=0;i<8;i++)
	{
		byteDates.append(byteArray.at(3*i));
		strId+=(TCHAR)byteArray.at(3*i+1);
		strId+=(TCHAR)byteArray.at(3*i+2);
	}

	GDES::TransformData(byteDates,FALSE);
	GDES::TransformData(byteDates,FALSE);
	GDES::TransformData(byteDates,FALSE);
	GDES::TransformData(byteDates,FALSE);
	strDate=_T("");
	for(int i=0;i<8;i++)
	{
		TCHAR c=(TCHAR)byteDates.at(i);
		if(c<_T('0')||c>_T('9'))
			return FALSE;
		strDate+=(TCHAR)byteDates.at(i);
	}
	return TRUE;
}
BOOL CHECK_REG_BOOL(CString lpDevDate)
{
	TCHAR m_lpFile[MAX_PATH+1]={0};
	CString _strDevDate=lpDevDate;
	CString regCode=ReadRegKey();
	if(!regCode.IsEmpty())
	{
		CString strId;
		CString strDate;
		RegCodeBack(regCode,strId,strDate);
		GComputerId id;
		CString strId2=id.GetComputerId();
		if((!strDate.IsEmpty())&&
			(strId.Compare(strId2)==0))
		{
			Glb_strRegDate=strDate;
		}
		if(!Glb_strRegDate.IsEmpty())
		{
			Glb_bReg=TRUE;
		}
		else
		{
			Glb_bReg=FALSE;
		}
	}
	if((!Glb_bReg))
	{
		if(AfxMessageBox(_T("您的软件未注册，是否现在注册"),MB_YESNO)!=IDYES)
		{
			return FALSE;
		}
		CAcModuleResourceOverride res;
		GRegDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
		dlg.DoModal();
		if(!Glb_bReg)
		{
			return FALSE;
		}
	}
	if(Glb_strRegDate.CompareNoCase(lpDevDate)<0)
	{
		if(AfxMessageBox(_T("您的软件已注册，但是注册码已过期\r\n您需要申请新的注册码才能使用\r\n是否现在申请新的注册码？"),MB_YESNO)!=IDYES)\
		{
			return FALSE;
		}
		CAcModuleResourceOverride res;
		GRegDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
		dlg.DoModal();
		if(!Glb_bReg)
		{
			return FALSE;
		}
		if(Glb_strRegDate.CompareNoCase(lpDevDate)<0)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CHECK_REG_BOOL()
{
	CTime tm = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%.4d%.2d%.2d"),tm.GetYear(),tm.GetMonth(),tm.GetDay());
	return CHECK_REG_BOOL(strTime);
}
BOOL CheckReg()
{
	return CHECK_REG_BOOL();
}
