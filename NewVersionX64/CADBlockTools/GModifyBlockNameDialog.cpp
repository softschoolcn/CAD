// GModifyBlockNameDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GModifyBlockNameDialog.h"
#include "BlockCommCommand.h"

// GModifyBlockNameDialog 对话框

IMPLEMENT_DYNAMIC(GModifyBlockNameDialog, CDialog)

GModifyBlockNameDialog::GModifyBlockNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GModifyBlockNameDialog::IDD, pParent)
{

}

GModifyBlockNameDialog::~GModifyBlockNameDialog()
{
}

void GModifyBlockNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GModifyBlockNameDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GModifyBlockNameDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GModifyBlockNameDialog 消息处理程序
BOOL GModifyBlockNameDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT1,m_strBlcokName);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GModifyBlockNameDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strNewBlcokName;
	GetDlgItemText(IDC_EDIT1,strNewBlcokName);
	CString str;
	if(m_strBlcokName.Compare(strNewBlcokName)!=0&&
		GStringHelper::IsStringInArray(m_strBlockNameArr,strNewBlcokName,false))
	{
		str.Format(_T("块名称\"%s\"已经存在，请您重新输入"),strNewBlcokName);
		AfxMessageBox(strNewBlcokName);
		return;
	}
	if(!IsValidBlockName(strNewBlcokName,str))
	{
		AfxMessageBox(str);
		return;
	}
	m_strBlcokName=strNewBlcokName;
	OnOK();
}
