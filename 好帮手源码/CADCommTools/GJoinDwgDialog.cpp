// GJoinDwgDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GJoinDwgDialog.h"


// GJoinDwgDialog 对话框

IMPLEMENT_DYNAMIC(GJoinDwgDialog, CDialog)

GJoinDwgDialog::GJoinDwgDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GJoinDwgDialog::IDD, pParent)
{

}

GJoinDwgDialog::~GJoinDwgDialog()
{
}

void GJoinDwgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MAIN,m_list);
	DDX_Control(pDX,IDC_COMBO_PAGE,m_comBox);
}


BEGIN_MESSAGE_MAP(GJoinDwgDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &GJoinDwgDialog::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_FLODER, &GJoinDwgDialog::OnBnClickedButtonFloder)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &GJoinDwgDialog::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &GJoinDwgDialog::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UP, &GJoinDwgDialog::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &GJoinDwgDialog::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_TOP, &GJoinDwgDialog::OnBnClickedButtonTop)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM, &GJoinDwgDialog::OnBnClickedButtonBottom)
	ON_BN_CLICKED(IDOK, &GJoinDwgDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_HOR, &GJoinDwgDialog::OnBnClickedRadioHor)
	ON_BN_CLICKED(IDC_RADIO_VER, &GJoinDwgDialog::OnBnClickedRadioVer)
	ON_BN_CLICKED(IDC_RADIO_HV, &GJoinDwgDialog::OnBnClickedRadioHv)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE, &GJoinDwgDialog::OnCbnSelchangeComboPage)
END_MESSAGE_MAP()


// GJoinDwgDialog 消息处理程序

void GJoinDwgDialog::OnBnClickedButtonFile()
{
	const TCHAR* filea = NULL;
	const TCHAR* dlgname = _T("选择DWG文件"); 
	struct resbuf* result = acutNewRb(RTSTR);
	int rc=acedGetFileNavDialog(filea, NULL, _T("dwg"), dlgname, 1|32|4096, &result);
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
}
bool GJoinDwgDialog::AddFile(const CString strFile)
{
	for(int i=0;i<m_list.GetCount();i++)
	{
		CString strFile1;
		m_list.GetText(i,strFile1);
		if(strFile.CompareNoCase(strFile1)==0)
		{
			return false;
		}
	}
	int nIndex=m_list.AddString(strFile);
	return true;
}
void GJoinDwgDialog::OnBnClickedButtonFloder()
{
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
		if(AfxMessageBox(_T("是否添加子目录文件"),MB_YESNO)==IDYES)
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
void GJoinDwgDialog::AddFloder(const CString strFloder,int& nIngore,int& nCount)
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
void GJoinDwgDialog::OnBnClickedButtonClear()
{
	m_list.ResetContent();
}

void GJoinDwgDialog::OnBnClickedButtonDelete()
{
	if(m_list.GetSelCount()==0)
	{
		return;
	}
	int nSel=m_list.GetCurSel();
	if(nSel<0)
	{
		return;
	}
	m_list.DeleteString(nSel);
}

void GJoinDwgDialog::OnBnClickedButtonUp()
{
	if(m_list.GetSelCount()==0)
	{
		return;
	}
	int nSel=m_list.GetCurSel();
	if(nSel<=0)
	{
		return;
	}
	CString strTmp;
	m_list.GetText(nSel,strTmp);
	m_list.DeleteString(nSel);
	m_list.InsertString(nSel-1,strTmp);
	m_list.SetCurSel(nSel-1);
}

void GJoinDwgDialog::OnBnClickedButtonDown()
{
	if(m_list.GetSelCount()==0)
	{
		return;
	}
	int nSel=m_list.GetCurSel();
	if(nSel<0||nSel==m_list.GetCount()-1)
	{
		return;
	}
	CString strTmp;
	m_list.GetText(nSel,strTmp);
	m_list.DeleteString(nSel);
	m_list.InsertString(nSel+1,strTmp);
	m_list.SetCurSel(nSel+1);
}

void GJoinDwgDialog::OnBnClickedButtonTop()
{
	if(m_list.GetSelCount()==0)
	{
		return;
	}
	int nSel=m_list.GetCurSel();
	if(nSel<=0)
	{
		return;
	}
	CString strTmp;
	m_list.GetText(nSel,strTmp);
	m_list.DeleteString(nSel);
	m_list.InsertString(0,strTmp);
	m_list.SetCurSel(0);
}

void GJoinDwgDialog::OnBnClickedButtonBottom()
{
	if(m_list.GetSelCount()==0)
	{
		return;
	}
	int nSel=m_list.GetCurSel();
	if(nSel<0||nSel==m_list.GetCount()-1)
	{
		return;
	}
	CString strTmp;
	m_list.GetText(nSel,strTmp);
	m_list.DeleteString(nSel);
	m_list.InsertString(m_list.GetCount(),strTmp);
	m_list.SetCurSel(m_list.GetCount()-1);
}

void GJoinDwgDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_param.m_fileArray.RemoveAll();
	for(int i=0;i<m_list.GetCount();i++)
	{
		CString strFile1;
		m_list.GetText(i,strFile1);
		m_param.m_fileArray.Add(strFile1);
	}
	CString strPL;
	if(GetItemCheck(IDC_RADIO_HV))
	{
		//混合排列
		strPL=_T("HV");
		m_param.m_nPL=2;
		CString strHORDIS;
		GetDlgItemText(IDC_EDIT_HOR,strHORDIS);
		if(!GStringHelper::IsValidDouble(strHORDIS,m_param.m_dDisHor)||m_param.m_dDisHor<0)
		{
			AfxMessageBox(_T("请输入有效水平间距"));
			return;
		}
		GUserDataRecord::getInstance()->WriteProfileString(_T("GJoinDwgDialog"),_T("HORDIS"),strHORDIS);
		

		CString strVERDIS;
		GetDlgItemText(IDC_EDIT_VER,strVERDIS);
		if(!GStringHelper::IsValidDouble(strVERDIS,m_param.m_dDisVer)||m_param.m_dDisVer<0)
		{
			AfxMessageBox(_T("请输入有效垂直间距"));
			return;
		}
		GUserDataRecord::getInstance()->WriteProfileString(_T("GJoinDwgDialog"),_T("VERDIS"),strVERDIS);
		

		CString strNUM;
		GetDlgItemText(IDC_EDIT_NUM,strNUM);
		if(!GStringHelper::IsValidInt(strNUM,m_param.m_nNum)||m_param.m_nNum<=1)
		{
			AfxMessageBox(_T("请输入有效每行图纸数量"));
			return;
		}
		GUserDataRecord::getInstance()->WriteProfileString(_T("GJoinDwgDialog"),_T("NUM"),strNUM);
		
	}
	else
	{
		if(GetItemCheck(IDC_RADIO_VER))
		{
			//竖直排列
			strPL=_T("VER");
			m_param.m_nPL=1;
		}
		else
		{
			//水平排列
			strPL=_T("HOR");
			m_param.m_nPL=0;
		}
		CString strDIS;
		GetDlgItemText(IDC_EDIT_DIS,strDIS);
		if(!GStringHelper::IsValidDouble(strDIS,m_param.m_dDis)||m_param.m_dDis<0)
		{
			AfxMessageBox(_T("请输入有效间距"));
			return;
		}
		GUserDataRecord::getInstance()->WriteProfileString(_T("GJoinDwgDialog"),_T("DIS"),strDIS);
		

	}
	GUserDataRecord::getInstance()->WriteProfileString(_T("GJoinDwgDialog"),_T("PL"),strPL);
	OnOK();
}

BOOL GJoinDwgDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strPL=GUserDataRecord::getInstance()->GetProfileString(_T("GJoinDwgDialog"),_T("PL"),20,_T("HOR"));
	if(strPL.CompareNoCase(_T("HV"))==0)
	{
		//混合排列
		SetItemCheck(IDC_RADIO_HV,TRUE);
	}
	else if(strPL.CompareNoCase(_T("VER"))==0)
	{
		//竖直排列
		SetItemCheck(IDC_RADIO_VER,TRUE);
	}
	else
	{
		//水平排列
		SetItemCheck(IDC_RADIO_HOR,TRUE);
	}

	CString strDIS=GUserDataRecord::getInstance()->GetProfileString(_T("GJoinDwgDialog"),_T("DIS"),20,_T("10"));
	SetDlgItemText(IDC_EDIT_DIS,strDIS);

	CString strHORDIS=GUserDataRecord::getInstance()->GetProfileString(_T("GJoinDwgDialog"),_T("HORDIS"),20,_T("297"));
	SetDlgItemText(IDC_EDIT_HOR,strHORDIS);

	CString strVERDIS=GUserDataRecord::getInstance()->GetProfileString(_T("GJoinDwgDialog"),_T("VERDIS"),20,_T("210"));
	SetDlgItemText(IDC_EDIT_VER,strVERDIS);

	CString strNUM=GUserDataRecord::getInstance()->GetProfileString(_T("GJoinDwgDialog"),_T("NUM"),20,_T("10"));
	SetDlgItemText(IDC_EDIT_NUM,strNUM);
	EnableEdits();

	m_comBox.AddString(_T("A0横向 1189*841mm"));
	m_comBox.AddString(_T("A0竖向 841*1189mm"));

	m_comBox.AddString(_T("A1横向 841*594mm"));
	m_comBox.AddString(_T("A1竖向 594*841mm"));

	m_comBox.AddString(_T("A2横向 594*420mm"));
	m_comBox.AddString(_T("A2竖向 420*594mm"));

	m_comBox.AddString(_T("A3横向 420*297mm"));
	m_comBox.AddString(_T("A3竖向 297*420mm"));

	m_comBox.AddString(_T("A4横向 297*210mm"));
	m_comBox.AddString(_T("A4竖向 210*297mm"));

	m_comBox.SetCurSel(8);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());

	return TRUE;

}
//不可编辑控件置灰
void GJoinDwgDialog::EnableEdits()
{
	if(GetItemCheck(IDC_RADIO_HV))
	{
		SetItemEnable(IDC_EDIT_DIS,FALSE);
		
		SetItemEnable(IDC_EDIT_HOR,TRUE);
		SetItemEnable(IDC_EDIT_VER,TRUE);
		SetItemEnable(IDC_EDIT_NUM,TRUE);
		SetItemEnable(IDC_COMBO_PAGE,TRUE);
	}
	else
	{
		SetItemEnable(IDC_EDIT_DIS,TRUE);
		
		SetItemEnable(IDC_EDIT_HOR,FALSE);
		SetItemEnable(IDC_EDIT_VER,FALSE);
		SetItemEnable(IDC_EDIT_NUM,FALSE);
		SetItemEnable(IDC_COMBO_PAGE,FALSE);
	}
}

void GJoinDwgDialog::OnBnClickedRadioHor()
{
	EnableEdits();
}

void GJoinDwgDialog::OnBnClickedRadioVer()
{
	EnableEdits();
}

void GJoinDwgDialog::OnBnClickedRadioHv()
{
	EnableEdits();
}

void GJoinDwgDialog::OnCbnSelchangeComboPage()
{
	// TODO: 在此添加控件通知处理程序代码
	double dWidths[10]={1189,841,841,594,594,420,420,297,297,210};
	double dHeights[10]={841,1189,594,841,420,594,297,420,210,297};
	int nSel=m_comBox.GetCurSel();
	if(nSel<0)
	{
		return;
	}
	CString strWidth;
	strWidth.Format(_T("%d"),(int)dWidths[nSel]);
	CString strHeight;
	strHeight.Format(_T("%d"),(int)dHeights[nSel]);
	SetDlgItemText(IDC_EDIT_HOR,strWidth);
	SetDlgItemText(IDC_EDIT_VER,strHeight);
}
