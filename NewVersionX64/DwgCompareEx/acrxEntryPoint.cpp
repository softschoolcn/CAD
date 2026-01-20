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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CmpOptionDialog.h"
#include "CmpConfig.h"
#include "CmpExcute.h"
#include "CommCommand.h"
#include "CmpUpdate.h"
#include "md5.h"
#include "des.h"
#include "ByteString.h"
#include "CmpReportDialog.h"
CmpUpdate Glb_CmpUpdateImp;
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
#define  MENU_TIMER 45
extern COLORREF ColorNew;
extern COLORREF ColorDelete;
extern COLORREF ColorModify;
extern COLORREF ColorSame;
CString CreateNewTmpFileName(CString strSourceFileName)
{
	OLECHAR szGuid[128] =OLESTR("");
	GUID g_guid;
	CoCreateGuid(&g_guid);	
	StringFromGUID2(g_guid,szGuid,sizeof(szGuid)/sizeof(OLECHAR));	
	USES_CONVERSION;
	CString strTmp;
	strTmp.Format(_T("%s"),W2T(szGuid));
	TCHAR lpPath[MAX_PATH+1]={0};//文件路径
	GetTempPath(MAX_PATH,lpPath);
	PathAppend(lpPath,_T("DwgCompare"));
	if(!PathFileExists(lpPath))
	{
		CreateDirectory(lpPath,NULL);
	}
	PathAppend(lpPath,strTmp);
	if(!PathFileExists(lpPath))
	{
		CreateDirectory(lpPath,NULL);
	}
	PathAppend(lpPath,PathFindFileName(strSourceFileName));
	return lpPath;
}
void CreateNewTmpFloder(CmpConfig& config)
{
	CString strTmp;
	OLECHAR szGuid[128] =OLESTR("");
	GUID g_guid;
	CoCreateGuid(&g_guid);	
	StringFromGUID2(g_guid,szGuid,sizeof(szGuid)/sizeof(OLECHAR));	
	USES_CONVERSION;
	strTmp.AppendFormat(_T("%s"),W2T(szGuid));
	TCHAR lpPath[MAX_PATH+1]={0};//文件路径
	GetTempPath(MAX_PATH,lpPath);
	PathAppend(lpPath,_T("DwgCompare"));
	if(!PathFileExists(lpPath))
	{
		CreateDirectory(lpPath,NULL);
	}
	PathAppend(lpPath,strTmp);
	if(!PathFileExists(lpPath))
	{
		CreateDirectory(lpPath,NULL);
	}
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("A原始文件"));
	if(!PathFileExists(lpPath1))
	{
		CreateDirectory(lpPath1,NULL);
	}
	config.m_strTmpFile1=lpPath1;

	TCHAR lpPath2[MAX_PATH+2]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("B比较文件"));
	if(!PathFileExists(lpPath2))
	{
		CreateDirectory(lpPath2,NULL);
	}
	config.m_strTmpFile2=lpPath2;
}
//枚举一个目录下的目录和文件列表
bool EnumDirectoryContent(CString strDirName , CString strFilter,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	strDirName.TrimLeft();
	strDirName.TrimRight();

	if(strDirName.GetLength() == 0)return false;

	arrSubDirNames.RemoveAll();
	arrSubFileNames.RemoveAll();

	CString			strDirList , strFileList , strDirFileList;
	CFileFind		finder;
	CString			strWildcard;

	if(strDirName.GetAt(strDirName.GetLength() - 1) == _T('\\') ||strDirName.GetAt(strDirName.GetLength() - 1) == _T('/'))
		strWildcard	= strDirName + strFilter;
	else
		strWildcard		= strDirName +"\\"+ strFilter;
	BOOL bWorking = finder.FindFile(strWildcard);

	if(!bWorking)return false;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())continue;
		if(finder.IsSystem())continue;
		if (finder.IsDirectory())
		{
			arrSubDirNames.Add(finder.GetFileName());
			continue;
		}

		arrSubFileNames.Add(finder.GetFileName());
	}
	finder.Close();

	return true;
}
bool EnumDirectoryContent(CString strDirName ,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	return EnumDirectoryContent(strDirName,_T("*.*"),arrSubDirNames,arrSubFileNames);
}
//拷贝文件夹下的.dwg文件
void CopyFiles(CStringArray& strFileArr,CString strTargetFloder,CString strSourceFloder,int nMode)
{
	if(nMode!=2)
	{
		CStringArray arrSubDirNames;
		CStringArray arrSubFileNames;
		EnumDirectoryContent(strSourceFloder,_T("*.dwg"),arrSubDirNames,arrSubFileNames);
		for(int i=0;i<arrSubFileNames.GetCount();i++)
		{
			TCHAR lpPathSource[MAX_PATH+1]={0};
			lstrcpy(lpPathSource,strSourceFloder);
			PathAppend(lpPathSource,arrSubFileNames.GetAt(i));
			TCHAR lpPathTarget[MAX_PATH+1]={0};
			lstrcpy(lpPathTarget,strTargetFloder);
			PathAppend(lpPathTarget,arrSubFileNames.GetAt(i));
			CopyFile(lpPathSource,lpPathTarget,FALSE);
			strFileArr.Add(lpPathTarget);
		}
		arrSubDirNames.RemoveAll();
		arrSubFileNames.RemoveAll();
		EnumDirectoryContent(strSourceFloder,_T("*.*"),arrSubDirNames,arrSubFileNames);
		for(int i=0;i<arrSubDirNames.GetCount();i++)
		{
			TCHAR lpPathSource[MAX_PATH+1]={0};
			lstrcpy(lpPathSource,strSourceFloder);
			PathAppend(lpPathSource,arrSubDirNames.GetAt(i));
			TCHAR lpPathTarget[MAX_PATH+1]={0};
			lstrcpy(lpPathTarget,strTargetFloder);
			PathAppend(lpPathTarget,arrSubDirNames.GetAt(i));
			if(!PathFileExists(lpPathTarget))
			{
				CreateDirectory(lpPathTarget,NULL);
			}
			CopyFiles(strFileArr,lpPathTarget,lpPathSource,nMode);
		}
	}
	else
	{
		CStringArray arrSubDirNames;
		CStringArray arrSubFileNames;
		EnumDirectoryContent(strSourceFloder,_T("*.dwg"),arrSubDirNames,arrSubFileNames);
		for(int i=0;i<arrSubFileNames.GetCount();i++)
		{
			TCHAR lpPathSource[MAX_PATH+1]={0};
			lstrcpy(lpPathSource,strSourceFloder);
			PathAppend(lpPathSource,arrSubFileNames.GetAt(i));
			strFileArr.Add(lpPathSource);
		}
		arrSubDirNames.RemoveAll();
		arrSubFileNames.RemoveAll();
		EnumDirectoryContent(strSourceFloder,_T("*.*"),arrSubDirNames,arrSubFileNames);
		for(int i=0;i<arrSubDirNames.GetCount();i++)
		{
			TCHAR lpPathSource[MAX_PATH+1]={0};
			lstrcpy(lpPathSource,strSourceFloder);
			PathAppend(lpPathSource,arrSubDirNames.GetAt(i));
			CopyFiles(strFileArr,strTargetFloder,lpPathSource,nMode);
		}
	}
}
COLORREF ToRGB(int nColorIndex)
{
	AcCmColor col;
	col.setColorIndex(nColorIndex);
	return RGB(col.red(),col.green(),col.blue());
}
//获取广告的md5
BOOL GetAdMD5(AcArray<byte>& byteArray)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\DwgCmpExAd.html"));
	CFile file;
	if(!file.Open(lpPath,CFile::modeRead|CFile::typeBinary))
	{
		return FALSE;
	}
	DWORD dLen=file.GetLength();
	if(dLen==0)
	{
		file.Close();
		return FALSE;
	}
	byte* bytes=new byte[dLen];
	file.Read(bytes,dLen);
	file.Close();
	MD5 md5(bytes,dLen);
	std::string str=md5.toString();
	for(int i=0;i<str.length();i++)
	{
		byteArray.append(str.at(i));
	}
	delete[] bytes;
	bytes=NULL;
	return TRUE;
}
BOOL GetVerifyString(CString& strVfy)
{
	AcArray<byte> byteArray;
	if(!GetAdMD5(byteArray))
	{
		return FALSE;
	}
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	CByteString bStr;
	strVfy=bStr.toString(byteArray);
	return TRUE;
}
BOOL VerifyAd()
{
	CString strVfy;
	if(!GetVerifyString(strVfy))
	{
		return FALSE;
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\DwgCmpExAd.ini"));
	TCHAR lpResult[128+1]={0};
	if(!GetPrivateProfileString(_T("System"),_T("VfyStr"),_T(""),lpResult,128,lpPath))
	{
		return FALSE;
	}
	if(strVfy.Compare(lpResult)==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
void vfyAd()
{
	CString strVfy;
	if(!GetVerifyString(strVfy))
	{
		return;
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\DwgCmpExAd.ini"));
	WritePrivateProfileString(_T("System"),_T("VfyStr"),strVfy,lpPath);
}
#include "afxmt.h"
CMutex m_mutex1;
//读取SDE数据
static UINT thread_ReadInfo(LPVOID pParamThread)
{
	m_mutex1.Lock();
	try
	{
		Glb_CmpUpdateImp.UpdateServerInfo();
		Glb_CmpUpdateImp.m_bNeedReadServer=false;
	}
	catch(...)
	{

	}
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	m_mutex1.Unlock();
	return 0;
}
CWinThread* pThread1=NULL;
void tzbj()
{
#ifndef CMP_SINGLE
	if(!CheckReg())
	{
		return;
	}
#endif
	ShowReportPanel(FALSE);
	CmpReportHelper::instance.Stop();
	GCmpResultInstance::instance.DeleteAllResult();
	CmpConfig config;
	if(!config.Read())
	{
		AfxMessageBox(_T("读取配置文件失败，请检查config\\CompareDwgConfig.xml"));
		return;
	}
	bool m_bReg=true;
	CAcModuleResourceOverride res;
	CmpOptionDialog optDlg(AfxGetApp()->GetMainWnd());
	optDlg.m_pConfig=&config;
	optDlg.m_bReg=m_bReg;
	int rc=optDlg.DoModal();
	if(rc!=IDOK)
	{
		return;
	}
	Glb_bOriginalFile=config.m_bSourceFile;
	Glb_bOriginalColor=Glb_bOriginalFile;
	Glb_nColorSame=config.m_nColorSame;
	Glb_nColorChange=config.m_nColorChange;
	Glb_nColorNew=config.m_nColorNew;
	Glb_nColorDelete=config.m_nColorDelete;
	//ColorNew=ToRGB(config.m_nColorNew);
	//ColorModify=ToRGB(config.m_nColorChange);
	//ColorDelete=ToRGB(config.m_nColorDelete);
	//ColorSame=ToRGB(config.m_nColorSame);
	config.Write();
	if(config.m_bEx)
	{
		//目录对比
		CArray<CmpResult> cmpResultArray;
		CreateNewTmpFloder(config);
		CStringArray strFileArr1;
		CopyFiles(strFileArr1,config.m_strTmpFile1,config.m_strFloder1,config.m_bSourceFile?2:config.m_nMode);
		CStringArray strFileArr2;
		CopyFiles(strFileArr2,config.m_strTmpFile2,config.m_strFloder2,config.m_bSourceFile?2:config.m_nMode);
		for(int i=0;i<strFileArr1.GetCount();i++)
		{
			TCHAR lpPath1[MAX_PATH+1]={0};
			lstrcpy(lpPath1,strFileArr1.GetAt(i));
			lstrcpy(lpPath1,PathFindFileName(lpPath1));
			bool bFind=false;
			for(int j=strFileArr2.GetCount()-1;j>=0;j--)
			{
				TCHAR lpPath2[MAX_PATH+1]={0};
				lstrcpy(lpPath2,strFileArr2.GetAt(j));
				lstrcpy(lpPath2,PathFindFileName(lpPath2));
				if(lstrcmpi(lpPath1,lpPath2)==0)
				{
					bFind=true;
					config.m_strTmpFile1=strFileArr1.GetAt(i);
					config.m_strTmpFile2=strFileArr2.GetAt(j);
					CmpExcute exc(&config);
					if(config.m_nMode!=2)
					{
						long diffCount=0;
						Acad::ErrorStatus es=exc.Excute(diffCount);
						if(es==eOk)
						{
							if(diffCount>0)
							{
								exc.m_Result.m_strFile1=strFileArr1.GetAt(i);
								exc.m_Result.m_strFile2=strFileArr2.GetAt(j);
								exc.m_Result.nType=1;
								cmpResultArray.Add(exc.m_Result);
							}
						}
					}
					else
					{
						exc.m_Result.nType=1;
						exc.m_Result.m_strFile1=strFileArr1.GetAt(i);
						exc.m_Result.m_strFile2=strFileArr2.GetAt(j);
						cmpResultArray.Add(exc.m_Result);
					}
					strFileArr2.RemoveAt(j);
					break;
				}
			}
			if(!bFind)
			{
				//删除的文件
				CmpResult rc;
				rc.nType=3;
				rc.m_strFile1=strFileArr1.GetAt(i);
				cmpResultArray.Add(rc);
			}
		}
		for(int k=0;k<strFileArr2.GetCount();k++)
		{
			//新增加的文件
			CmpResult rc;
			rc.nType=2;
			rc.m_strFile2=strFileArr2.GetAt(k);
			cmpResultArray.Add(rc);
		}
		if(cmpResultArray.GetCount()==0)
		{
			acutPrintf(_T("\n两个目录完全相同"));
			return;
		}
		for(int i=0;i<cmpResultArray.GetCount();i++)
		{
			CmpResult* pRC=new CmpResult(cmpResultArray.GetAt(i));
			GCmpResultInstance::instance.AppendResult(pRC);
		}
		//Glb_ViewReactor->Attach();
		ShowReportPanel(TRUE);
		SetReportPanelSizeMode(0);
		ShowCmpResultEx(config.m_nMode,m_bReg);
	}
	else
	{
		//文件对比
		config.m_strTmpFile1=config.m_bSourceFile||config.m_nMode==2?config.m_strFile1:CreateNewTmpFileName(config.m_strFile1);
		config.m_strTmpFile2=config.m_bSourceFile||config.m_nMode==2?config.m_strFile2:CreateNewTmpFileName(config.m_strFile2);
		if(config.m_nMode!=2&&(!config.m_bSourceFile))
		{
			CopyFile(config.m_strFile1,config.m_strTmpFile1,FALSE);
			CopyFile(config.m_strFile2,config.m_strTmpFile2,FALSE);
		}
		CmpExcute exc(&config);
		long diffCount=0;
		if(config.m_nMode!=2)
		{
			Acad::ErrorStatus es=exc.Excute(diffCount);
			if(es!=eOk)
			{
				return;
			}
			if(diffCount==0)
			{
				acutPrintf(_T("\n两张图纸完全相同"));
				return;
			}
		}
		acutPrintf(_T("\n共发现%d个差异"),diffCount);
		//Glb_ViewReactor->Attach();
		if(config.m_nMode!=2)
		{
			ShowReportPanel(TRUE);
		}
		SetReportPanelSizeMode(config.m_nMode!=2?0:1);
		exc.m_Result.nType=4;
		exc.m_Result.m_strFile1=config.m_strTmpFile1;
		exc.m_Result.m_strFile2=config.m_strTmpFile2;
		CmpResult* pRC=new CmpResult(exc.m_Result);
		pRC->m_pDoc1=config.m_pDoc1;
		pRC->m_pDoc2=config.m_pDoc2;
		GCmpResultInstance::instance.AppendResult(pRC);
		GCmpResultInstance::instance.SetCurrentResult(pRC);
		if(config.m_nMode!=2)
		{
			ShowCmpResult(config.m_nMode,m_bReg);
		}
		else
		{
			CmpReportHelper::instance.ShowCmpResult(config.m_nMode,m_bReg);
		}
	}
}
#ifdef _ARXDEBUG
void listEntClass()
{
	AcRxDictionaryIterator* pIter=acrxClassDictionary->newIterator(AcRx::kDictCollated);
	std::auto_ptr<AcRxDictionaryIterator> spIter(pIter);
	for (;!spIter->done();spIter->next())
	{
		AcRxObject* pObj=spIter->object();
		if(!pObj->isKindOf(AcRxClass::desc()))
		{
			continue;
		}
		AcRxClass* pClass=AcRxClass::cast(pObj);
		if(!pClass->isDerivedFrom(AcDbEntity::desc()))
		{
			continue;
		}
		CString strName=pClass->name();
		CString strDxfName=pClass->dxfName();
		CString strParName;
		CString strParDxfName;
		if(pClass->myParent()!=NULL)
		{
			strParName=pClass->myParent()->name();
			strParDxfName=pClass->myParent()->dxfName();
		}
		acutPrintf(_T("\nname=%s,dxfName=%s,parName=%s,parDxfName=%s"),strName,strDxfName,strParName,strParDxfName);
	}
}
#endif
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CDwgCompareApp : public AcRxArxApp {

public:
	CDwgCompareApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		// TODO: Add your initialization code here
		//图纸比较
		#ifdef CMP_SINGLE
		acedRegCmds->addCommand(_T("DwgCompareEx"),
			_T("tzbj"),
			_T("tzbj"),
			ACRX_CMD_MODAL|ACRX_CMD_SESSION,
			tzbj);
#else
		acedRegCmds->addCommand(_T("DwgCompareEx"),
			_T("cadgj_tzbj"),
			_T("cadgj_tzbj"),
			ACRX_CMD_MODAL|ACRX_CMD_SESSION,
			tzbj);	
#endif
#ifdef _ARXDEBUG
		acedRegCmds->addCommand(_T("DwgCompareEx"),
			_T("vfyAd"),
			_T("vfyAd"),
			0,
			vfyAd);
		acedRegCmds->addCommand(_T("DwgCompareEx"),
			_T("listEntClass"),
			_T("listEntClass"),
			0,
			listEntClass);
#endif
		AfxInitRichEdit2();

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		ShowReportPanel(FALSE);
		CmpReportHelper::instance.Stop();
		GCmpResultInstance::instance.DeleteAllResult();
		

		

		// TODO: Unload dependencies here
		acedRegCmds->removeGroup(_T("DwgCompareEx"));


		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CDwgCompareApp)

