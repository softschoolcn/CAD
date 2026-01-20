// GSameNameBlockDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GSameNameBlockDialog.h"


// GSameNameBlockDialog 对话框

IMPLEMENT_DYNAMIC(GSameNameBlockDialog, CDialog)

GSameNameBlockDialog::GSameNameBlockDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GSameNameBlockDialog::IDD, pParent),m_bSameHandle(false),m_nMode(1)
{

}

GSameNameBlockDialog::~GSameNameBlockDialog()
{
}

void GSameNameBlockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GSameNameBlockDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GSameNameBlockDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GSameNameBlockDialog 消息处理程序
BOOL GSameNameBlockDialog::OnInitDialog()
{
	BOOL rc=CDialog::OnInitDialog();
	m_bSameHandle=GUserDataRecord::getInstance()->GetProfileBool(_T("GSameNameBlockDialog"),_T("SameHandle"),false);
	SetItemCheck(IDC_CHECK1,m_bSameHandle);
	m_nMode=GUserDataRecord::getInstance()->GetProfileInt(_T("GSameNameBlockDialog"),_T("Mode"),1);
	if(m_nMode==2)
	{
		SetItemCheck(IDC_RADIO_S);
	}
	else if(m_nMode==3)
	{
		SetItemCheck(IDC_RADIO_M);
	}
	else
	{
		SetItemCheck(IDC_RADIO_SM);
	}
	return rc;
}
void GSameNameBlockDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSameHandle=GetItemCheck(IDC_CHECK1);
	if(GetItemCheck(IDC_RADIO_S))
	{
		m_nMode=2;
	}
	else if(GetItemCheck(IDC_RADIO_M))
	{
		m_nMode=3;
	}
	else
	{
		m_nMode=1;
	}
	OnOK();
}
