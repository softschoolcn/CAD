// GPurgeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GPurgeDialog.h"
#include "GxAfxFunction.h"
#include "GPurgeCommCommand.h"
// GPurgeDialog 对话框

IMPLEMENT_DYNAMIC(GPurgeDialog, CDialog)

GPurgeDialog::GPurgeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GPurgeDialog::IDD, pParent)
{
	m_nType=0;
}

GPurgeDialog::~GPurgeDialog()
{
}

void GPurgeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_FILES,m_list);
	DDX_Control(pDX,IDC_LIST_LOG,m_listLog);
	DDX_Control(pDX,IDC_TAB_PURGE,m_tab);
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
	ON_BN_CLICKED(IDC_BUTTON_LOG, &GPurgeDialog::OnBnClickedButtonLog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PURGE, &GPurgeDialog::OnTcnSelchangeTabPurge)
END_MESSAGE_MAP()


// GPurgeDialog 消息处理程序
BOOL GPurgeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nIndex=0;
	if(m_nType==0)
	{
	nIndex=m_comboTypes.AddString(_T("保持原有类型"));
	m_comboTypes.SetItemData(nIndex,(DWORD_PTR)AcDb::kDHL_Unknown);
	}
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
	int m_nDefaultSaveType=m_nType==0?AcDb::kDHL_Unknown:AcDb::kDHL_1800;
	int m_nSaveType=GUserDataRecord::getInstance()->GetProfileInt(_T("GPurgeDialog"),m_nType==0?_T("SaveType0"):_T("SaveType1"),m_nDefaultSaveType);
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
	m_list.InsertColumn(2,m_nType==0?_T("压缩比例"):_T("文件大小比例"),LVCFMT_LEFT,70);
	m_tab.InsertItem(0,_T("文件列表"));
	m_tab.InsertItem(1,m_nType==0?_T("清理日志"):_T("日志"));
	CArray<int> indexArray1;
	indexArray1.Add(IDC_STATIC_GROUP);
	indexArray1.Add(IDC_LIST_FILES);
	indexArray1.Add(IDC_BUTTON_PURGE);
	indexArray1.Add(IDC_BUTTON_FILE);
	indexArray1.Add(IDC_BUTTON_FLODER);
	indexArray1.Add(IDC_BUTTON_ALLSEL);
	indexArray1.Add(IDC_BUTTON_FX);
	indexArray1.Add(IDC_BUTTON_ALLNOTSEL);
	indexArray1.Add(IDC_BUTTON_DEL);
	indexArray1.Add(IDC_BUTTON_CLEAR);
	indexArray1.Add(IDC_BUTTON_LOG);
	indexArray1.Add(IDC_STATIC_SAVETYPE);
	indexArray1.Add(IDC_COMBO_SAVETYPE);

	for(int i=0;i<indexArray1.GetCount();i++)
	{
		int nIndex=indexArray1.GetAt(i);
		CWnd* pWnd=GetDlgItem(nIndex);
		if(pWnd==NULL)
		{
			continue;
		}
		pWnd->ShowWindow(SW_HIDE);
	}
	m_listLog.ShowWindow(SW_HIDE);
	m_tab.MoveWindow(15,15,865,445);
	m_listLog.MoveWindow(15+20,15+40,865-15-20-15,445-15-40-10);
	for(int i=0;i<indexArray1.GetCount();i++)
	{
		int nIndex=indexArray1.GetAt(i);
		MoveItem(nIndex,20,40);
		CWnd* pWnd=GetDlgItem(nIndex);
		if(pWnd==NULL)
		{
			continue;
		}
		pWnd->ShowWindow(SW_SHOW);
	}
	if(m_nType==1)
	{
		SetWindowText(_T("统一DWG文件版本"));
		GetDlgItem(IDC_BUTTON_PURGE)->SetWindowText(_T("开始统一"));
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GPurgeDialog::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
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
		if(AfxMessageBox(m_nType==0?_T("是否清理子目录"):_T("是否统一子目录文件版本"),MB_YESNO)==IDYES)
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
		AfxMessageBox(m_nType==0?_T("请先选择需要清理的文件"):_T("请先选择需要统一版本的文件"));
		return;
	}
	AcDb::AcDbDwgVersion ver=(AcDb::AcDbDwgVersion)m_comboTypes.GetItemData(m_comboTypes.GetCurSel());
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GPurgeDialog"),m_nType==0?_T("SaveType0"):_T("SaveType1"),ver);
	m_listLog.ResetContent();
	m_listLog.Invalidate();
	m_listLog.UpdateWindow();
	for(int i=0;i<strArr.GetCount();i++)
	{
		TCHAR lpTmpFile[MAX_PATH+1]={0};
		CString strTmp=NewGuid()+_T(".dwg");
		GetTempPath(MAX_PATH,lpTmpFile);
		PathAppend(lpTmpFile,strTmp);
		int nIndex=indexArr.GetAt(i);
		CString strFileName=strArr.GetAt(i);
		DWORD dSize1=GetFileSize(strFileName);
		TCHAR lpFileName[MAX_PATH+1]={0};
		lstrcpy(lpFileName,PathFindFileName(strFileName));
		AcDbDatabase* pDbTmp=new AcDbDatabase(false);
		try
		{
			Acad::ErrorStatus es=pDbTmp->readDwgFile(strFileName);
			if(es!=eOk)
			{
				AddLog(_T("读取DWG文件:%s，发生错误:%s"),lpFileName,acadErrorStatusText(es));
				m_list.SetItemText(nIndex,1,_T("读取文件失败"));
			}
			else
			{
				AddLog(m_nType==0?_T("开始清理文件,文件名:%s"):_T("开始统一文件版本,文件名:%s"),lpFileName);
				if(m_nType==0)
				{
					PurgeDatabase(pDbTmp,PurgeDialogMsgOutput,this);
				}
				es=SaveDatabase(pDbTmp,lpTmpFile,ver);
				if(es!=eOk)
				{
					AddLog(_T("保存DWG文件:%s，发生错误:%s"),lpFileName,acadErrorStatusText(es));
					m_list.SetItemText(nIndex,1,_T("保存文件失败"));
				}
				else
				{
					AddLog(_T("清理文件成功,文件名:%s"),lpFileName);
					m_list.SetItemText(nIndex,1,m_nType==0?_T("清理文件成功"):_T("统一版本成功"));
				}

			}

		}
		catch(...)
		{
			if(pDbTmp!=NULL)
			{
				delete pDbTmp;
				pDbTmp=NULL;
			}
		}
		if(pDbTmp!=NULL)
		{
			delete pDbTmp;
			pDbTmp=NULL;
		}
		if(PathFileExists(lpTmpFile))
		{
			CopyFile(lpTmpFile,strFileName,FALSE);
			DeleteFile(lpTmpFile);
		}
		
		DWORD dSize2=GetFileSize(strFileName);
		if(dSize1==0||dSize2==0)
		{
			m_list.SetItemText(nIndex,2,_T("100%"));
		}
		else
		{
			CString strValue;
			strValue.Format(_T("%5.2f"),(double)dSize2/(double)dSize1*100.0);
			m_list.SetItemText(nIndex,2,strValue+_T("%"));
		}
	}



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
		m_listLog.InsertString(m_listLog.GetCount(),strBuffer);
		m_listLog.Invalidate();
		m_listLog.UpdateWindow();
		GLog::WriteLine(strBuffer);
	}
	catch(...)
	{

	}
}
void GPurgeDialog::MsgOutput(CString strMsg)
{
	AddLog(strMsg);
}
void GPurgeDialog::OnBnClickedButtonLog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tab.SetCurSel(1);
	LRESULT rc;
	OnTcnSelchangeTabPurge(NULL,&rc);
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

void GPurgeDialog::OnTcnSelchangeTabPurge(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CArray<int> indexArray1;
	indexArray1.Add(IDC_STATIC_GROUP);
	indexArray1.Add(IDC_LIST_FILES);
	indexArray1.Add(IDC_BUTTON_PURGE);
	indexArray1.Add(IDC_BUTTON_FILE);
	indexArray1.Add(IDC_BUTTON_FLODER);
	indexArray1.Add(IDC_BUTTON_ALLSEL);
	indexArray1.Add(IDC_BUTTON_FX);
	indexArray1.Add(IDC_BUTTON_ALLNOTSEL);
	indexArray1.Add(IDC_BUTTON_DEL);
	indexArray1.Add(IDC_BUTTON_CLEAR);
	indexArray1.Add(IDC_BUTTON_LOG);
	indexArray1.Add(IDC_STATIC_SAVETYPE);
	indexArray1.Add(IDC_COMBO_SAVETYPE);
	int nCurSel = m_tab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		
		

		for(int i=0;i<indexArray1.GetCount();i++)
		{
			int nIndex=indexArray1.GetAt(i);
			CWnd* pWnd=GetDlgItem(nIndex);
			if(pWnd==NULL)
			{
				continue;
			}
			pWnd->ShowWindow(SW_SHOW);
		}
		m_listLog.ShowWindow(SW_HIDE);
		break;
	case 1:
		for(int i=0;i<indexArray1.GetCount();i++)
		{
			int nIndex=indexArray1.GetAt(i);
			CWnd* pWnd=GetDlgItem(nIndex);
			if(pWnd==NULL)
			{
				continue;
			}
			pWnd->ShowWindow(SW_HIDE);
		}
		m_listLog.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}
