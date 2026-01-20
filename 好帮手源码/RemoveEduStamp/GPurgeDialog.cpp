// GPurgeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GPurgeDialog.h"
// GPurgeDialog 对话框
CString BrowseForFolder( HWND hWnd, LPCTSTR pszInitDir /*= NULL*/)
{
	TCHAR szTitle[] = _T("选择一个文件夹"); 
	TCHAR  szDisplayName[MAX_PATH] = _T("");
	TCHAR  szPath[MAX_PATH] = _T("");
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.lpszTitle = szTitle;
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE|BIF_EDITBOX;
	bi.lpfn=NULL;
	bi.lParam = 0;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
	if( pItemIDList )
	{
		SHGetPathFromIDList(pItemIDList,szPath) ;

		IMalloc *pMalloc;
		if( SHGetMalloc( &pMalloc ) != NOERROR )
		{
			TRACE( "Failed to get pointer to shells task allocator" ) ;
			return szPath;
		}
		pMalloc->Free( pItemIDList );
		if( pMalloc )
			pMalloc->Release();
	}

	return szPath;
}
/*!
@zhangshl 生成GUID
*/
CString NewGuid()
{
	OLECHAR szGuid[128] =OLESTR("");
	GUID g_guid;
	CoCreateGuid(&g_guid);	
	StringFromGUID2(g_guid,szGuid,sizeof(szGuid)/sizeof(OLECHAR));
	USES_CONVERSION;
	CString m_GuidPrt =W2T(szGuid);
	return m_GuidPrt;
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
DWORD GetFileSize(LPCTSTR lpFile)
{
	DWORD dSize=0;
	try
	{
		CStdioFile file(lpFile, CFile::modeRead);
		dSize=file.GetLength();
		file.Close();
	}
	catch (CFileException* pe) 
	{
		dSize=0;
		pe->Delete();
	}
	return dSize;
}
//功能，保存CAD数据库
//pDb,数据库
//lpFileName,文件名称，如果为NULL，表示按原有的文件名称保存
//ver,保存的DWG的版本号
//pSecParams,加密参数
Acad::ErrorStatus SaveDatabase(AcDbDatabase* pDb,const TCHAR* lpFileName,AcDb::AcDbDwgVersion ver,const SecurityParams* pSecParams)
{
	Acad::ErrorStatus es=Acad::eOk;
	if(pDb==NULL)
	{
		return Acad::eNullHandle;
	}
	TCHAR fileName[MAX_PATH+1]={0};
	if(lpFileName!=NULL)
	{
		lstrcpy(fileName,lpFileName);
	}
	else
	{
		const TCHAR* lpTmp=NULL;
		es=pDb->getFilename(lpTmp);
		if(es!=Acad::eOk)
		{
			return es;
		}
		lstrcpy(fileName,lpTmp);
	}
	if(lstrcmpi(PathFindExtension(fileName),_T(".dwg"))!=0)
	{
		return Acad::eInvalidFileExtension;
	}
	AcDb::AcDbDwgVersion inner_ver=ver;
	if(inner_ver==AcDb::kDHL_Unknown)
	{
		inner_ver=pDb->lastSavedAsVersion();
	}
	if(inner_ver==AcDb::kDHL_MC0_0)
	{
		//文件没有保存过,且未设置版本
#if _MSC_VER>=1400
		inner_ver=AcDb::kDHL_1021;//2007 final
#else
		inner_ver=AcDb::kDHL_1800;//2004 final
#endif
	}
	if(inner_ver==AcDb::kDHL_1500||inner_ver==AcDb::kDHL_1015)
	{
		//2000
		return acdbSaveAs2000(pDb,fileName);
	}
	else if(inner_ver==AcDb::kDHL_1800||inner_ver==AcDb::kDHL_1800a)
	{
		//2004
#if _MSC_VER>=1400
		return acdbSaveAs2004(pDb,fileName);
#elif _MSC_VER>=1300
		return pDb->saveAs(fileName,pSecParams);
#else
		return Acad::eInvalidInput;
#endif
	}
#if _MSC_VER>=1400
	else if(inner_ver==AcDb::kDHL_2100a||inner_ver==AcDb::kDHL_1021)
	{
		//2007
#if _MSC_VER>=1500
		return pDb->saveAs(fileName,false,inner_ver);
#elif _MSC_VER>=1400
		return pDb->saveAs(fileName,pSecParams);
#else
		return Acad::eInvalidInput;
#endif
	}
#endif
#if _MSC_VER>=1500
	else if(inner_ver==AcDb::kDHL_1024)
	{
		//2010
		return pDb->saveAs(fileName,false,inner_ver);
	}

#endif
#if _MSC_VER>=1600
	else if(inner_ver==AcDb::kDHL_1027)
	{
		//2013
		return pDb->saveAs(fileName,false,inner_ver);
	}
#endif
	else
	{
		return Acad::eInvalidInput;
	}

}
IMPLEMENT_DYNAMIC(GPurgeDialog, CDialog)

GPurgeDialog::GPurgeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GPurgeDialog::IDD, pParent)
{
}

GPurgeDialog::~GPurgeDialog()
{
}

void GPurgeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_FILES,m_list);
	DDX_Control(pDX,IDC_COMBO_SAVETYPE,m_comboTypes);
}


BEGIN_MESSAGE_MAP(GPurgeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &GPurgeDialog::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_FLODER, &GPurgeDialog::OnBnClickedButtonFloder)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL, &GPurgeDialog::OnBnClickedButtonAllsel)
	ON_BN_CLICKED(IDC_BUTTON_FX, &GPurgeDialog::OnBnClickedButtonFx)
	ON_BN_CLICKED(IDC_BUTTON_ALLNOTSEL, &GPurgeDialog::OnBnClickedButtonAllnotsel)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &GPurgeDialog::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &GPurgeDialog::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_PURGE, &GPurgeDialog::OnBnClickedButtonPurge)
END_MESSAGE_MAP()


// GPurgeDialog 消息处理程序
BOOL GPurgeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nIndex=0;
	nIndex=m_comboTypes.AddString(_T("保持原有类型"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_Unknown);
	nIndex=m_comboTypes.AddString(_T("AutoCAD 2000/LT2000 图形"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_1500);
	nIndex=m_comboTypes.AddString(_T("AutoCAD 2004/LT2004 图形"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_1800);
#if _MSC_VER>=1400
	nIndex=m_comboTypes.AddString(_T("AutoCAD 2007/LT2007 图形"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_1021);
#endif
	#if _MSC_VER>=1500
	nIndex=m_comboTypes.AddString(_T("AutoCAD 2010/LT2010 图形"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_1024);
#endif
	#if _MSC_VER>=1600
	nIndex=m_comboTypes.AddString(_T("AutoCAD 2013/LT2013 图形"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_1024);
#endif
	int m_nDefaultSaveType=AcDb::kDHL_Unknown;
	int m_nSaveType=AcDb::kDHL_Unknown;
	bool bFind=false;
	for(int i=0;i<m_comboTypes.GetCount();i++)
	{
		if(m_comboTypes.GetItemData(i)==m_nSaveType)
		{
			bFind=true;
			m_comboTypes.SetCurSel(i);
			break;
		}
	}
	if(!bFind)
	{
		for(int i=0;i<m_comboTypes.GetCount();i++)
		{
			if(m_comboTypes.GetItemData(i)==m_nDefaultSaveType)
			{
				bFind=true;
				m_comboTypes.SetCurSel(i);
				break;
			}
		}
		
	}
	if(!bFind)
	{
		m_comboTypes.SetCurSel(0);
	}
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_list.InsertColumn(0,_T("文件"),LVCFMT_LEFT,530);
	m_list.InsertColumn(1,_T("状态"),LVCFMT_LEFT,90);
	m_list.InsertColumn(2,_T("文件大小比例"),LVCFMT_LEFT,70);
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GPurgeDialog::OnBnClickedButtonFile()
{
	const TCHAR* filea = NULL;
	const TCHAR* dlgname = _T("选择DWG文件"); 
	struct resbuf* result = acutNewRb(RTSTR);
	int rc=acedGetFileNavDialog(filea, NULL, _T("dwg"), dlgname, 32|4096, &result);
	if(rc!=RTNORM)
	{
		acutRelRb(result);
		return;
	}
	struct resbuf* pTmp=result;
	int nIngore=0;
	int nCount=0;
	while(pTmp)
	{
		CString lpDwgFile=CString(pTmp->resval.rstring);
		if(AddFile(lpDwgFile))
		{
			nCount++;
		}
		else
		{
			nIngore++;
		}
		pTmp=pTmp->rbnext;
	}
	acutRelRb(result);
	CString strInfo;
	if(nIngore==0)
	{
		strInfo.Format(_T("成功添加:%d个文件"),nCount);
	}
	else if(nCount==0)
	{
		strInfo.Format(_T("%d个文件都已经在列表中了，无需重复添加"),nIngore);
	}
	else
	{
		strInfo.Format(_T("成功添加:%d个文件,忽略%d个重复文件"),nCount,nIngore);
	}
	AfxMessageBox(strInfo);
	/*
	// TODO: 在此添加控件通知处理程序代码
	CAcModuleResourceOverride resOverride;
	// TODO: 在此添加控件通知处理程序代码
	TCHAR lpDwgFile[MAX_PATH]={0};
	CFileDialog fileDlg(true, _T("*.dwg"), NULL, 
		OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
		_T("dwg文件 (*.dwg)|*.dwg||"));
	//最多可以打开100个文件
	fileDlg.m_ofn.nMaxFile = 512 * MAX_PATH;
	if(fileDlg.m_ofn.lpstrFile!=NULL)
	{
#if _WIN64
		delete fileDlg.m_ofn.lpstrFile;
#else
		delete[] fileDlg.m_ofn.lpstrFile;
#endif
		fileDlg.m_ofn.lpstrFile=NULL;
	}
	fileDlg.m_ofn.lpstrFile = new TCHAR[fileDlg.m_ofn.nMaxFile];  
	ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);

	int rc=fileDlg.DoModal();
	if (rc != IDOK)
	{
		return;
	}
	POSITION pos=fileDlg.GetStartPosition();
	int nIngore=0;
	int nCount=0;
	while(pos)
	{
		CString lpDwgFile=fileDlg.GetNextPathName(pos);
		if(AddFile(lpDwgFile))
		{
			nCount++;
		}
		else
		{
			nIngore++;
		}

	}
	CString strInfo;
	if(nIngore==0)
	{
		strInfo.Format(_T("成功添加:%d个文件"),nCount);
	}
	else if(nCount==0)
	{
		strInfo.Format(_T("%d个文件都已经在列表中了，无需重复添加"),nIngore);
	}
	else
	{
		strInfo.Format(_T("成功添加:%d个文件,忽略%d个重复文件"),nCount,nIngore);
	}
	AfxMessageBox(strInfo);
	*/
}

void GPurgeDialog::OnBnClickedButtonFloder()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFloder=BrowseForFolder(this->GetSafeHwnd(),NULL);
	if(strFloder.IsEmpty())
	{
		return;
	}
	CStringArray arrSubDirNames,arrSubFileNames;
	EnumDirectoryContent(strFloder,arrSubDirNames,arrSubFileNames);
	int nIngore=0;
	int nCount=0;
	for(int i=0;i<arrSubFileNames.GetCount();i++)
	{
		TCHAR lpFile[MAX_PATH+1]={0};
		lstrcpy(lpFile,strFloder);
		PathAppend(lpFile,arrSubFileNames.GetAt(i));
		if(lstrcmpi(PathFindExtension(lpFile),_T(".dwg"))!=0)
		{
			continue;
		}
		if(AddFile(lpFile))
		{
			nCount++;
		}
		else
		{
			nIngore++;
		}
	}
	if(arrSubDirNames.GetCount()>0)
	{
		if(AfxMessageBox(_T("是否添加子目录"),MB_YESNO)==IDYES)
		{
			for(int i=0;i<arrSubDirNames.GetCount();i++)
			{
				TCHAR lpFile[MAX_PATH+1]={0};
				lstrcpy(lpFile,strFloder);
				PathAppend(lpFile,arrSubDirNames.GetAt(i));
				AddFloder(lpFile,nIngore,nCount);
			}
		}
	}
	CString strInfo;
	if(nIngore==0)
	{
		strInfo.Format(_T("成功添加:%d个文件"),nCount);
	}
	else if(nCount==0)
	{
		strInfo.Format(_T("%d个文件都已经在列表中了，无需重复添加"),nIngore);
	}
	else
	{
		strInfo.Format(_T("成功添加:%d个文件,忽略%d个重复文件"),nCount,nIngore);
	}
	AfxMessageBox(strInfo);
}
void GPurgeDialog::AddFloder(const CString strFloder,int& nIngore,int& nCount)
{
	CStringArray arrSubDirNames,arrSubFileNames;
	EnumDirectoryContent(strFloder,arrSubDirNames,arrSubFileNames);
	for(int i=0;i<arrSubFileNames.GetCount();i++)
	{
		TCHAR lpFile[MAX_PATH+1]={0};
		lstrcpy(lpFile,strFloder);
		PathAppend(lpFile,arrSubFileNames.GetAt(i));
		if(lstrcmpi(PathFindExtension(lpFile),_T(".dwg"))!=0)
		{
			continue;
		}
		if(AddFile(lpFile))
		{
			nCount++;
		}
		else
		{
			nIngore++;
		}
	}
	if(arrSubDirNames.GetCount()>0)
	{
		for(int i=0;i<arrSubDirNames.GetCount();i++)
		{
			TCHAR lpFile[MAX_PATH+1]={0};
			lstrcpy(lpFile,strFloder);
			PathAppend(lpFile,arrSubDirNames.GetAt(i));
			AddFloder(lpFile,nIngore,nCount);
		}
	}
}
bool GPurgeDialog::AddFile(const CString strFile)
{
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		CString strFile1=m_list.GetItemText(i,0);
		if(strFile.CompareNoCase(strFile1)==0)
		{
			m_list.SetCheck(i,TRUE);
			return false;
		}
	}
	int nIndex=m_list.InsertItem(m_list.GetItemCount(),strFile);
	m_list.SetItemText(nIndex,1,_T("未处理"));
	m_list.SetCheck(nIndex,TRUE);
	return true;
}

void GPurgeDialog::OnBnClickedButtonAllsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}

void GPurgeDialog::OnBnClickedButtonFx()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,!m_list.GetCheck(i));
	}
}

void GPurgeDialog::OnBnClickedButtonAllnotsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,FALSE);
	}
}

void GPurgeDialog::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	while(pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		m_list.DeleteItem(nItem);
	}
}

void GPurgeDialog::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
}
#include "GCommand.h"
void GPurgeDialog::OnBnClickedButtonPurge()
{
	// TODO: 在此添加控件通知处理程序代码
	CStringArray strArr;
	CArray<int> indexArr;
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		if(!m_list.GetCheck(i))
		{
			continue;
		}
		strArr.Add(m_list.GetItemText(i,0));
		indexArr.Add(i);
	}
	if(strArr.GetCount()==0)
	{
		AfxMessageBox(_T("请先选择需要去除教育版戳记的文件"));
		return;
	}
	m_strFileArray.RemoveAll();
	m_strFileArray.Append(strArr);
	OnOK();
	return;
}
void GPurgeDialog::AddLog(CString fmt, ...)
{
	try
	{
		CString strBuffer;

		va_list marker;
		va_start(marker, fmt);
		strBuffer.FormatV(fmt, marker);
		va_end(marker);
	}
	catch(...)
	{

	}
}
void GPurgeDialog::MsgOutput(CString strMsg)
{
	AddLog(strMsg);
}
void PurgeDialogMsgOutput(CString strMsg,void* pArg)
{
	if(pArg==NULL)
	{
		return;
	}
	GPurgeDialog* pDlg=(GPurgeDialog*)pArg;
	if(pDlg==NULL)
		return;
	pDlg->MsgOutput(strMsg);
}
//移动控件
void GPurgeDialog::MoveItem(int ItemID,int x,int y)
{
	CRect rect;
	GetDlgItem(ItemID)->GetWindowRect(rect);
	ScreenToClient(&rect);
	rect.left+=x;
	rect.right+=x;
	rect.top+=y;
	rect.bottom+=y;
	GetDlgItem(ItemID)->MoveWindow(&rect);
}
