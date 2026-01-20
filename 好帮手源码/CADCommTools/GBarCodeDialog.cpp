// GBarCodeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GBarCodeDialog.h"


// GBarCodeDialog 对话框

IMPLEMENT_DYNAMIC(GBarCodeDialog, CDialog)

GBarCodeDialog::GBarCodeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GBarCodeDialog::IDD, pParent)
{

}

GBarCodeDialog::~GBarCodeDialog()
{
}

void GBarCodeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GBarCodeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GBarCodeDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GBarCodeDialog 消息处理程序
BOOL GBarCodeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strText=GUserDataRecord::getInstance()->GetProfileString(_T("GBarCodeDialog"),_T("BarCode"),128,GlobalSystemValue::instance.getWebSite());
	CString strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GBarCodeDialog"),_T("Text"),128,_T("True"));
	SetDlgItemText(IDC_EDIT1,strText);
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		CButton* pBtn=(CButton*)GetDlgItem(IDC_RADIO2);
		pBtn->SetCheck(TRUE);
	}
	else
	{
		CButton* pBtn=(CButton*)GetDlgItem(IDC_RADIO1);
		pBtn->SetCheck(TRUE);
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GBarCodeDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1,m_strBarCode);
	if(m_strBarCode.IsEmpty())
	{
		AfxMessageBox(_T("请输入条形码"));
		return;
	}
	CButton* pBtn=(CButton*)GetDlgItem(IDC_RADIO2);
	if(pBtn->GetCheck())
	{
		m_bText=true;
	}
	else
	{
		m_bText=false;
	}
	GUserDataRecord::getInstance()->WriteProfileString(_T("GBarCodeDialog"),_T("BarCode"),m_strBarCode);
	GUserDataRecord::getInstance()->WriteProfileString(_T("GBarCodeDialog"),_T("Text"),m_bText?_T("True"):_T("False"));
	OnOK();
}
