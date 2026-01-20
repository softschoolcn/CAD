// CmpOptionDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CmpOptionDialog.h"
#include "CmpUpdate.h"
#include "GDocListDialog.h"
// CmpOptionDialog 对话框
extern CmpUpdate Glb_CmpUpdateImp;
IMPLEMENT_DYNAMIC(CmpOptionDialog, CDialog)
LONG GetRegKeyX(HKEY key, LPCTSTR subkey, LPTSTR retdata)   
{   
	HKEY hkey;   
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);   

	if (retval == ERROR_SUCCESS) {   
		long datasize = MAX_PATH;   
		TCHAR data[MAX_PATH];   
		RegQueryValue(hkey, NULL, data, &datasize);   
		lstrcpy(retdata,data);   
		RegCloseKey(hkey);   
	}   

	return retval;   
}
bool OnClickUpdate()
{
	TCHAR key[MAX_PATH + MAX_PATH];   

	// First try ShellExecute()    
	HINSTANCE result = ShellExecute(NULL, _T("open"), Glb_CmpUpdateImp.m_ServerInfo.m_strURL, NULL,NULL, SW_SHOW);   

	// If it failed, get the .htm regkey and lookup the program    
	if ((UINT)result == HINSTANCE_ERROR) {   

		if (GetRegKeyX(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {   
			lstrcat(key, _T("\\shell\\open\\command"));   

			if (GetRegKeyX(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {   
				TCHAR *pos;   
				pos = _tcsstr(key, _T("\"%1\""));   
				if (pos == NULL) {                     // No quotes found 
#ifdef _UNICODE
					pos = wcsstr(key, _T("%1"));       // Check for %1, without quotes
#else
					pos = strstr(key, _T("%1"));       // Check for %1, without quotes
#endif

					if (pos == NULL)                   // No parameter at all...    
						pos = key+lstrlen(key)-1;   
					else   
						*pos = '\0';                   // Remove the parameter    
				}   
				else   
					*pos = '\0';                       // Remove the parameter    

				lstrcat(pos, _T(" "));   
				lstrcat(pos, Glb_CmpUpdateImp.m_ServerInfo.m_strURL);   
				result = (HINSTANCE) WinExec((LPCSTR)key,SW_SHOW);   
			}   
		}   
	} 
	return true;
}

CmpOptionDialog::CmpOptionDialog(CWnd* pParent /*=NULL*/)
: CDialog(CmpOptionDialog::IDD, pParent)
{
	m_pConfig=NULL;
	m_bReg=false;
	m_nRootNodeCount=0;
}

CmpOptionDialog::~CmpOptionDialog()
{
}

void CmpOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TREE1,m_wndTree);
	DDX_Control(pDX,IDC_LIST1,m_wndList);
	DDX_Control(pDX,IDC_COMBO1,m_ColorComboBoxSame);
	DDX_Control(pDX,IDC_COMBO2,m_ColorComboBoxChange);
	DDX_Control(pDX,IDC_COMBO3,m_ColorComboBoxNew);
	DDX_Control(pDX,IDC_COMBO4,m_ColorComboBoxDelete);
	DDX_Control(pDX,IDC_RADIO_POSTION,m_ButtonPostion);
	DDX_Control(pDX,IDC_RADIO_HANDLE,m_ButtonHandle);
	DDX_Control(pDX,IDC_RADIO_LOOKONLY,m_ButtonLookOnly);
	DDX_Control(pDX,IDC_RADIO_CMPFILE,m_ButtonCmpFile);
	DDX_Control(pDX,IDC_RADIO_CMPFLODER,m_ButtonCmpFloder);
	DDX_Control(pDX,IDC_CHECK_SOURCEFILE,m_ButtonSourceFile);
	DDX_Control(pDX,IDC_CHECK1,m_ButtonCloseLayer);
	DDX_Control(pDX,IDC_CHECK2,m_ButtonLockLayer);
	DDX_Control(pDX,IDC_EDIT3,m_EditIngoreLayer);
}


BEGIN_MESSAGE_MAP(CmpOptionDialog, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CmpOptionDialog::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CmpOptionDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmpOptionDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CmpOptionDialog::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_POSTION, &CmpOptionDialog::OnBnClickedRadioPostion)
	ON_BN_CLICKED(IDC_RADIO_HANDLE, &CmpOptionDialog::OnBnClickedRadioHandle)
	ON_BN_CLICKED(IDC_RADIO_LOOKONLY, &CmpOptionDialog::OnBnClickedRadioLookonly)
	ON_BN_CLICKED(IDC_RADIO_CMPFILE, &CmpOptionDialog::OnBnClickedRadioCmpfile)
	ON_BN_CLICKED(IDC_RADIO_CMPFLODER, &CmpOptionDialog::OnBnClickedRadioCmpfloder)
	ON_BN_CLICKED(IDC_BUTTON_DOC1, &CmpOptionDialog::OnBnClickedButtonDoc1)
	ON_BN_CLICKED(IDC_BUTTON_DOC2, &CmpOptionDialog::OnBnClickedButtonDoc2)
END_MESSAGE_MAP()


// CmpOptionDialog 消息处理程序
BOOL CmpOptionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_bReg)
	{
		m_ButtonCmpFloder.EnableWindow(TRUE);
		m_ButtonPostion.EnableWindow(TRUE);
		m_ButtonLookOnly.EnableWindow(TRUE);
		m_ButtonSourceFile.EnableWindow(TRUE);
	}
	if(m_pConfig->m_nMode==1)
	{
		m_ButtonHandle.SetCheck(TRUE);
	}
	else if(m_pConfig->m_nMode==0)
	{
		m_ButtonPostion.SetCheck(TRUE);
	}
	else
	{
		m_ButtonLookOnly.SetCheck(TRUE);
	}
	int nIndex=m_ColorComboBoxSame.FindItemByColorIndex(m_pConfig->m_nColorSame);
	if(nIndex<0)
	{
		nIndex=m_ColorComboBoxSame.AddColorToMRU(m_pConfig->m_nColorSame);
	}
	m_ColorComboBoxSame.SetCurSel(nIndex);

	nIndex=m_ColorComboBoxChange.FindItemByColorIndex(m_pConfig->m_nColorChange);
	if(nIndex<0)
	{
		nIndex=m_ColorComboBoxChange.AddColorToMRU(m_pConfig->m_nColorChange);
	}
	m_ColorComboBoxChange.SetCurSel(nIndex);

	nIndex=m_ColorComboBoxNew.FindItemByColorIndex(m_pConfig->m_nColorNew);
	if(nIndex<0)
	{
		nIndex=m_ColorComboBoxNew.AddColorToMRU(m_pConfig->m_nColorNew);
	}
	m_ColorComboBoxNew.SetCurSel(nIndex);

	nIndex=m_ColorComboBoxDelete.FindItemByColorIndex(m_pConfig->m_nColorDelete);
	if(nIndex<0)
	{
		nIndex=m_ColorComboBoxDelete.AddColorToMRU(m_pConfig->m_nColorDelete);
	}
	m_ColorComboBoxDelete.SetCurSel(nIndex);

	m_ButtonCloseLayer.SetCheck(m_pConfig->m_bIngoreCloseLayer);
	m_ButtonLockLayer.SetCheck(m_pConfig->m_bIngoreLockLayer);
	m_EditIngoreLayer.SetWindowText(m_pConfig->m_strIngoreLayer);

	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle()|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT);
	m_wndList.ModifyStyle(TVS_CHECKBOXES,0);
	m_wndList.ModifyStyle(0,TVS_CHECKBOXES);

	CBitmap bmpImageState;
	bmpImageState.LoadBitmap(IDB_THREESTATE);
	m_ImageList.Create(13, 13, ILC_COLOR8, 0, 3);
	m_ImageList.Add(&bmpImageState,RGB(0,0,0));
	m_wndTree.SetImageList(&m_ImageList,TVSIL_STATE);

	m_wndList.InsertColumn(0,_T("名称"),LVCFMT_LEFT,170);
	for(long i=0;i<m_pConfig->m_nCmpTypeCount;i++)
	{
		CmpType* pType=m_pConfig->m_cmpTypes[i];
		if(!pType->m_strParentClassName.IsEmpty())
		{
			continue;
		}
		m_nRootNodeCount++;
		HTREEITEM hItem =m_wndTree.InsertItem(pType->m_strName,TVI_ROOT);
		m_wndTree.SetItemData(hItem,(DWORD_PTR)(pType));
		m_wndTree.SetCheck(hItem,pType->m_bActive);
		ExpandNode(hItem);
		m_wndTree.Expand(hItem,TVE_EXPAND);
	}
	m_wndTree.Invalidate();
	m_wndTree.UpdateWindow();
	if(m_pConfig->m_bEx)
	{
		m_ButtonCmpFloder.SetCheck(TRUE);
		SetItemEnable(IDC_BUTTON_DOC1,FALSE);
	SetItemEnable(IDC_BUTTON_DOC2,FALSE);
	}
	else
	{
		m_ButtonCmpFile.SetCheck(TRUE);
	}
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_pConfig->m_bEx?m_pConfig->m_strFloder1:m_pConfig->m_strFile1);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(m_pConfig->m_bEx?m_pConfig->m_strFloder2:m_pConfig->m_strFile2);
	if(m_pConfig->m_bEx)
	{
		SetDlgItemText(IDC_STATIC_CMPFILES,_T("比较目录"));
		SetDlgItemText(IDC_STATIC_FILE1,_T("目录1"));
		SetDlgItemText(IDC_STATIC_FILE2,_T("目录2"));
	}
	if(m_pConfig->m_bSourceFile)
	{
		m_ButtonSourceFile.SetCheck(TRUE);
	}
#ifndef CMP_SINGLE
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
#endif
	return TRUE;
}
void CmpOptionDialog::ExpandNode(HTREEITEM hItem)
{
	CmpType* pType=(CmpType*)m_wndTree.GetItemData(hItem);
	if(pType==NULL)
		return;
	for(long i=0;i<m_pConfig->m_nCmpTypeCount;i++)
	{
		CmpType* pType1=m_pConfig->m_cmpTypes[i];
		if(pType1->m_strParentClassName.CompareNoCase(pType->m_strClassName)!=0)
		{
			continue;
		}
		HTREEITEM hItem1 =m_wndTree.InsertItem(pType1->m_strName,hItem);
		m_wndTree.SetItemData(hItem1,(DWORD_PTR)(pType1));
		m_wndTree.SetCheck(hItem1,pType1->m_bActive);
		ExpandNode(hItem1);
	}

}
int CmpOptionDialog::GetChildNodeCount(HTREEITEM hItem)
{
	int nCount=0;
	CmpType* pType=(CmpType*)m_wndTree.GetItemData(hItem);
	if(pType==NULL)
		return nCount;
	for(long i=0;i<m_pConfig->m_nCmpTypeCount;i++)
	{
		CmpType* pType1=m_pConfig->m_cmpTypes[i];
		if(pType1->m_strParentClassName.CompareNoCase(pType->m_strClassName)!=0)
		{
			continue;
		}
		nCount++;
	}
	return nCount;
}
void CmpOptionDialog::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	ShowTypes(pNMTreeView->itemNew.hItem);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
void CmpOptionDialog::ShowTypes(HTREEITEM hItem)
{
	CmpType* pType=(CmpType*)m_wndTree.GetItemData(hItem);
	if(pType==NULL)
		return;
	SaveTypes();
	m_wndList.DeleteAllItems();
	for(long i=0;i<pType->m_nCmpItemsCount;i++)
	{
		CmpItem* pItem=&pType->m_cmpItems[i];
		int nId=m_wndList.InsertItem(m_wndList.GetItemCount(),pItem->m_strName);
		m_wndList.SetCheck(nId,pItem->m_bActive);
		m_wndList.SetItemData(nId,(DWORD_PTR)(pItem));
	}

}
void CmpOptionDialog::SaveTypes()
{
	for(long i=0;i<m_wndList.GetItemCount();i++)
	{
		CmpItem* pItem=(CmpItem*)m_wndList.GetItemData(i);
		if(pItem==NULL)
			continue;
		pItem->m_bActive=m_wndList.GetCheck(i);
	}
}
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
void CmpOptionDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_pConfig->m_bEx)
	{
		struct resbuf *result = acutNewRb(RTSTR);
		//2^5 Inhibits display of the alert box, which warns that a file exists when a new file of the same name is opened
		if (acedGetFileNavDialog(_T("选择图纸1"),m_shrPath, _T("dwg"),NULL, 2<<5, &result)!=RTNORM)
			return;
		m_shrPath =result->resval.rstring;
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_shrPath);
		acutRelRb(result);
		SetItemEnable(IDC_EDIT1,TRUE);
		m_pConfig->m_pDoc1=NULL;
		if(m_pConfig->m_pDoc1==NULL&&
			m_pConfig->m_pDoc2==NULL)
		{
			SetItemEnable(IDC_CHECK_SOURCEFILE,TRUE);
		}
	}
	else
	{
		CString strFloder=BrowseForFolder(GetSafeHwnd(),NULL);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(strFloder);
	}

}

void CmpOptionDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_pConfig->m_bEx)
	{
		struct resbuf *result = acutNewRb(RTSTR);
		//2^5 Inhibits display of the alert box, which warns that a file exists when a new file of the same name is opened
		if (acedGetFileNavDialog(_T("选择图纸2"),m_shrPath, _T("dwg"),NULL, 2<<5, &result)!=RTNORM)
			return;
		m_shrPath =result->resval.rstring;
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(m_shrPath);
		acutRelRb(result);
		SetItemEnable(IDC_EDIT2,TRUE);
		m_pConfig->m_pDoc2=NULL;
		if(m_pConfig->m_pDoc1==NULL&&
			m_pConfig->m_pDoc2==NULL)
		{
			SetItemEnable(IDC_CHECK_SOURCEFILE,TRUE);
		}
	}
	else
	{
		CString strFloder=BrowseForFolder(GetSafeHwnd(),NULL);
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(strFloder);
	}

}

void CmpOptionDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveTypes();
	m_pConfig->m_bIngoreCloseLayer=m_ButtonCloseLayer.GetCheck();
	m_pConfig->m_bIngoreLockLayer=m_ButtonLockLayer.GetCheck();
	m_EditIngoreLayer.GetWindowText(m_pConfig->m_strIngoreLayer);
	int nCount=0;
	EnumCheckState(TVI_ROOT,nCount,FALSE);
	if(!m_pConfig->m_bEx)
	{
		if(m_pConfig->m_pDoc1==NULL)
		{
			((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(m_shrPath);
			m_pConfig->m_strFile1=m_shrPath;
			if(!m_pConfig->m_strFile1.IsEmpty())
				m_pConfig->m_strFile1.Trim();
			if (m_pConfig->m_strFile1.IsEmpty())
			{
				MessageBox(_T("图纸1，不能为空！"));
				return;
			}
			if(!PathFileExists(m_pConfig->m_strFile1))
			{
				MessageBox(_T("图纸1不存在"));
				return;
			}
		}
		if(m_pConfig->m_pDoc2==NULL)
		{
			((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(m_shrPath);
			m_pConfig->m_strFile2=m_shrPath;
			if(!m_pConfig->m_strFile2.IsEmpty())
				m_pConfig->m_strFile2.Trim();
			if (m_pConfig->m_strFile2.IsEmpty())
			{
				MessageBox(_T("图纸2，不能为空！"));
				return;
			}
			if(!PathFileExists(m_pConfig->m_strFile2))
			{
				MessageBox(_T("图纸2不存在"));
				return;
			}
		}
		CString strFile1=m_pConfig->m_pDoc1==NULL?m_pConfig->m_strFile1:m_pConfig->m_pDoc1->fileName();
		CString strFile2=m_pConfig->m_pDoc2==NULL?m_pConfig->m_strFile2:m_pConfig->m_pDoc2->fileName();
		if(strFile1.CompareNoCase(strFile2)==0)
		{
			MessageBox(_T("两张图纸不可以相同"));
			return;
		}



		m_pConfig->m_nColorSame=m_ColorComboBoxSame.GetCurrentItemColorIndex();
		m_pConfig->m_nColorChange=m_ColorComboBoxChange.GetCurrentItemColorIndex();
		m_pConfig->m_nColorNew=m_ColorComboBoxNew.GetCurrentItemColorIndex();
		m_pConfig->m_nColorDelete=m_ColorComboBoxDelete.GetCurrentItemColorIndex();



	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(m_shrPath);
		m_pConfig->m_strFloder1=m_shrPath;
		if(!m_pConfig->m_strFloder1.IsEmpty())
			m_pConfig->m_strFloder1.Trim();

		((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(m_shrPath);
		m_pConfig->m_strFloder2=m_shrPath;
		if(!m_pConfig->m_strFloder2.IsEmpty())
			m_pConfig->m_strFloder2.Trim();

		if (m_pConfig->m_strFloder1.IsEmpty())
		{
			MessageBox(_T("文件夹1，不能为空！"));
			return;
		}
		m_pConfig->m_nColorSame=m_ColorComboBoxSame.GetCurrentItemColorIndex();
		m_pConfig->m_nColorChange=m_ColorComboBoxChange.GetCurrentItemColorIndex();
		m_pConfig->m_nColorNew=m_ColorComboBoxNew.GetCurrentItemColorIndex();
		m_pConfig->m_nColorDelete=m_ColorComboBoxDelete.GetCurrentItemColorIndex();
		if(!PathFileExists(m_pConfig->m_strFloder1))
		{
			MessageBox(_T("文件夹1不存在"));
			return;
		}
		if (m_pConfig->m_strFloder2.IsEmpty())
		{
			MessageBox(_T("文件夹2，不能为空！"));
			return;
		}
		if(!PathFileExists(m_pConfig->m_strFloder2))
		{
			MessageBox(_T("文件夹2不存在"));
			return;
		}
		if(m_pConfig->m_strFloder1.CompareNoCase(m_pConfig->m_strFloder2)==0)
		{
			MessageBox(_T("两个文件夹不可以相同"));
			return;
		}
	}
	if(m_ButtonHandle.GetCheck())
	{
		m_pConfig->m_nMode=1;
	}
	else if(m_ButtonPostion.GetCheck())
	{
		m_pConfig->m_nMode=0;
	}
	else if(m_ButtonLookOnly.GetCheck())
	{
		m_pConfig->m_nMode=2;
	}
	if(m_bReg)
	{
		m_pConfig->m_bSourceFile=m_ButtonSourceFile.GetCheck();
	}
	if(!m_bReg)
	{
		//非注册版本
		//不能进行目录比较
		if(m_pConfig->m_bEx)
		{
			AfxMessageBox(_T("未注册版本不支持文件夹比较，请您先注册"));
			return;
		}
		if(m_pConfig->m_nMode==2)
		{
			AfxMessageBox(_T("未注册版本不支持对比查看，请您先注册"));
			return;
		}
		if(m_pConfig->m_nMode==0)
		{
			AfxMessageBox(_T("未注册版本位置模式，请您先注册"));
			return;
		}
	}
	OnOK();
}

void CmpOptionDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
	int nCount=0;
	if(!EnumCheckState(TVI_ROOT,nCount,TRUE))
	{
		return;
	}
	if(nCount!=m_nRootNodeCount)
	{
		return;
	}
	GLog::WriteLine(_T("节点选中状态同步成功"));
	m_wndTree.Invalidate();
	m_wndTree.UpdateWindow();
	KillTimer(nIDEvent);
}
bool CmpOptionDialog::EnumCheckState(HTREEITEM hParent,int& nCount,BOOL bWrite)
{
	nCount=0;
	HTREEITEM hChildItem =m_wndTree.GetChildItem(hParent);
	while (hChildItem != NULL)
	{
		CmpType* pType=(CmpType*)m_wndTree.GetItemData(hChildItem);
		if(pType!=NULL)
		{
			if(bWrite)
			{
				m_wndTree.SetCheck(hChildItem,pType->m_bActive);
				if(m_wndTree.GetCheck(hChildItem)!=pType->m_bActive)
				{
					return false;
				}
				else
				{
					nCount++;
				}
			}
			else
			{
				pType->m_bActive=m_wndTree.GetCheck(hChildItem);
			}

		}
		int nCount1=0;
		bool rc=EnumCheckState(hChildItem,nCount1,bWrite);
		if(!rc)
		{
			return rc;
		}
		int nCount2=GetChildNodeCount(hChildItem);
		if(nCount1!=nCount2)
		{
			return false;
		}

		hChildItem = m_wndTree.GetNextItem(hChildItem, TVGN_NEXT);
	}
	return true;
}
void CmpOptionDialog::OnDestroy()
{
	CDialog::OnDestroy();
	KillTimer(1);

	// TODO: 在此处添加消息处理程序代码
}

void CmpOptionDialog::OnBnClickedRadioPostion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ButtonPostion.SetCheck(TRUE);
	m_ButtonHandle.SetCheck(FALSE);
	m_ButtonLookOnly.SetCheck(FALSE);
}

void CmpOptionDialog::OnBnClickedRadioHandle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ButtonPostion.SetCheck(FALSE);
	m_ButtonHandle.SetCheck(TRUE);
	m_ButtonLookOnly.SetCheck(FALSE);
}

void CmpOptionDialog::OnBnClickedRadioLookonly()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ButtonPostion.SetCheck(FALSE);
	m_ButtonHandle.SetCheck(FALSE);
	m_ButtonLookOnly.SetCheck(TRUE);
}

void CmpOptionDialog::OnBnClickedRadioCmpfile()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pConfig->m_bEx=false;
	m_ButtonCmpFile.SetCheck(TRUE);
	m_ButtonCmpFloder.SetCheck(FALSE);
	SetDlgItemText(IDC_STATIC_CMPFILES,_T("比较文件"));
	SetDlgItemText(IDC_STATIC_FILE1,_T("文件1"));
	SetDlgItemText(IDC_STATIC_FILE2,_T("文件2"));
	SetDlgItemText(IDC_EDIT1,m_pConfig->m_strFile1);
	SetDlgItemText(IDC_EDIT2,m_pConfig->m_strFile2);
	m_pConfig->m_pDoc1=NULL;
	m_pConfig->m_pDoc2=NULL;
	SetItemEnable(IDC_BUTTON_DOC1,TRUE);
	SetItemEnable(IDC_BUTTON_DOC2,TRUE);
}

void CmpOptionDialog::OnBnClickedRadioCmpfloder()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pConfig->m_bEx=true;
	m_ButtonCmpFile.SetCheck(FALSE);
	m_ButtonCmpFloder.SetCheck(TRUE);
	SetDlgItemText(IDC_STATIC_CMPFILES,_T("比较文件夹"));
	SetDlgItemText(IDC_STATIC_FILE1,_T("文件夹1"));
	SetDlgItemText(IDC_STATIC_FILE2,_T("文件夹2"));
	SetDlgItemText(IDC_EDIT1,m_pConfig->m_strFloder1);
	SetDlgItemText(IDC_EDIT2,m_pConfig->m_strFloder2);
	SetItemEnable(IDC_CHECK_SOURCEFILE,TRUE);
	m_pConfig->m_pDoc1=NULL;
	m_pConfig->m_pDoc2=NULL;
	SetItemEnable(IDC_BUTTON_DOC1,FALSE);
	SetItemEnable(IDC_BUTTON_DOC2,FALSE);
}

void CmpOptionDialog::OnBnClickedButtonDoc1()
{
	//选择文档1
	CAcModuleResourceOverride res;
	GDocListDialog dlg(this);
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	m_pConfig->m_bEx=false;
	m_pConfig->m_pDoc1=dlg.m_pDoc;
	SetDlgItemText(IDC_EDIT1,m_pConfig->m_pDoc1->fileName());
	SetItemEnable(IDC_EDIT1,FALSE);
	SetItemEnable(IDC_CHECK_SOURCEFILE,FALSE);
	m_ButtonSourceFile.SetCheck(TRUE);

}

void CmpOptionDialog::OnBnClickedButtonDoc2()
{
	//选择文档2
	CAcModuleResourceOverride res;
	GDocListDialog dlg(this);
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	m_pConfig->m_bEx=false;
	m_pConfig->m_pDoc2=dlg.m_pDoc;
	SetDlgItemText(IDC_EDIT2,m_pConfig->m_pDoc2->fileName());
	SetItemEnable(IDC_EDIT2,FALSE);
	SetItemEnable(IDC_CHECK_SOURCEFILE,FALSE);
	m_ButtonSourceFile.SetCheck(TRUE);
}
