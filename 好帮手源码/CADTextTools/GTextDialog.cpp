// GTextDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GTextDialog.h"


// GTextDialog 对话框

IMPLEMENT_DYNAMIC(GTextDialog, CDialog)

GTextDialog::GTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GTextDialog::IDD, pParent)
{

}

GTextDialog::~GTextDialog()
{
}

void GTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GTextDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GTextDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GTextDialog 消息处理程序

void GTextDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1,m_strText);
	OnOK();
}
BOOL GTextDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT1,m_strText);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
