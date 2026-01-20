// GBlockDefineDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GBlockDefineDialog.h"
#include "BlockCommCommand.h"

// GBlockDefineDialog 对话框

IMPLEMENT_DYNAMIC(GBlockDefineDialog, CDialog)

GBlockDefineDialog::GBlockDefineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GBlockDefineDialog::IDD, pParent)
{
	m_nHor=0;
	m_nVer=0;
	m_bOrigin=false;
}

GBlockDefineDialog::~GBlockDefineDialog()
{
}

void GBlockDefineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GBlockDefineDialog, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &GBlockDefineDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &GBlockDefineDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &GBlockDefineDialog::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO10, &GBlockDefineDialog::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO7, &GBlockDefineDialog::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &GBlockDefineDialog::OnBnClickedRadio8)
	ON_BN_CLICKED(IDOK, &GBlockDefineDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GBlockDefineDialog 消息处理程序

void GBlockDefineDialog::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,TRUE);
	SetItemCheck(IDC_RADIO2,FALSE);
	SetItemCheck(IDC_RADIO5,FALSE);
	m_nHor=0;
}

void GBlockDefineDialog::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO2,TRUE);
	SetItemCheck(IDC_RADIO5,FALSE);
	m_nHor=1;
}

void GBlockDefineDialog::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO2,FALSE);
	SetItemCheck(IDC_RADIO5,TRUE);
	m_nHor=2;
}

void GBlockDefineDialog::OnBnClickedRadio10()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO7,FALSE);
	SetItemCheck(IDC_RADIO8,FALSE);
	SetItemCheck(IDC_RADIO10,TRUE);
	m_nVer=0;
}

void GBlockDefineDialog::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO10,FALSE);
	SetItemCheck(IDC_RADIO8,FALSE);
	SetItemCheck(IDC_RADIO7,TRUE);
	m_nVer=1;
}

void GBlockDefineDialog::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO10,FALSE);
	SetItemCheck(IDC_RADIO7,FALSE);
	SetItemCheck(IDC_RADIO8,TRUE);
	m_nVer=2;
}
BOOL GBlockDefineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nHor=GUserDataRecord::getInstance()->GetProfileInt(_T("GBlockDefineDialog"),_T("Hor"),0);
	m_nVer=GUserDataRecord::getInstance()->GetProfileInt(_T("GBlockDefineDialog"),_T("Ver"),0);
	if(m_nHor==1)
	{
		SetItemCheck(IDC_RADIO2,TRUE);
	}
	else if(m_nHor==2)
	{
		SetItemCheck(IDC_RADIO5,TRUE);
	}
	else
	{
		SetItemCheck(IDC_RADIO1,TRUE);
	}
	if(m_nVer==1)
	{
		SetItemCheck(IDC_RADIO7,TRUE);
	}
	else if(m_nVer==2)
	{
		SetItemCheck(IDC_RADIO8,TRUE);
	}
	else
	{
		SetItemCheck(IDC_RADIO10,TRUE);
	}
	CString strOrigin=GUserDataRecord::getInstance()->GetProfileString(_T("GBlockDefineDialog"),_T("Origin"),128,_T("False"));
	if(strOrigin.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK6,true);
	}
	else
	{
		SetItemCheck(IDC_CHECK6,false);
	}
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}

void GBlockDefineDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strNewBlcokName;
	GetDlgItemText(IDC_EDIT1,strNewBlcokName);
	CString str;
	if(!IsValidBlockName(strNewBlcokName,str))
	{
		AfxMessageBox(str);
		return;
	}
	if(GStringHelper::IsStringInArray(m_strBlockNameArr,strNewBlcokName,false))
	{
		str.Format(_T("块名称\"%s\"已经存在，是否覆盖？"),strNewBlcokName);
		if(AfxMessageBox(strNewBlcokName,MB_YESNO)!=IDYES)
		{
			return;
		}
		
	}
	
	m_strBlcokName=strNewBlcokName;
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GBlockDefineDialog"),_T("Hor"),m_nHor);
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GBlockDefineDialog"),_T("Ver"),m_nVer);
	if(GetItemCheck(IDC_CHECK6))
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBlockDefineDialog"),_T("Origin"),_T("True"));
		m_bOrigin=true;
	}
	else
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBlockDefineDialog"),_T("Origin"),_T("False"));
		m_bOrigin=false;
	}
	OnOK();
}
