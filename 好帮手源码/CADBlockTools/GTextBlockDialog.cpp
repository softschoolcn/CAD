// GTextBlockDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GTextBlockDialog.h"


// GTextBlockDialog 对话框

IMPLEMENT_DYNAMIC(GTextBlockDialog, CDialog)

GTextBlockDialog::GTextBlockDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GTextBlockDialog::IDD, pParent),m_nTextMode(3)
{

}

GTextBlockDialog::~GTextBlockDialog()
{
}

void GTextBlockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GTextBlockDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GTextBlockDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GTextBlockDialog 消息处理程序
BOOL GTextBlockDialog::OnInitDialog()
{
	BOOL rc=CDialog::OnInitDialog();
	m_nTextMode=GUserDataRecord::getInstance()->GetProfileInt(_T("GTextBlockDialog"),_T("TextMode"),3);
	if(m_nTextMode<1||m_nTextMode>3)
	{
		m_nTextMode=3;
	}
	SetItemCheck(IDC_RADIO_SM,TRUE);
	if(m_nTextMode==1)
	{
		SetItemCheck(IDC_RADIO_S,TRUE);
	}
	else if(m_nTextMode==2)
	{
		SetItemCheck(IDC_RADIO_M,TRUE);
	}
	CString strX=GUserDataRecord::getInstance()->GetProfileString(_T("GTextBlockDialog"),_T("X"),128,_T("0"));
	if(GStringHelper::IsValidDouble(strX,m_dX))
	{
		SetDlgItemText(IDC_EDIT_X,strX);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_X,_T("0"));
	}
	CString strY=GUserDataRecord::getInstance()->GetProfileString(_T("GTextBlockDialog"),_T("Y"),128,_T("0"));
	if(GStringHelper::IsValidDouble(strY,m_dY))
	{
		SetDlgItemText(IDC_EDIT_Y,strY);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_Y,_T("0"));
	}
	return rc;
}
void GTextBlockDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_RADIO_S))
	{
		m_nTextMode=1;
	}
	else if(GetItemCheck(IDC_RADIO_M))
	{
		m_nTextMode=2;
	}
	else
	{
		m_nTextMode=3;
	}
	CString strX;
	GetDlgItemText(IDC_EDIT_X,strX);
	if(!GStringHelper::IsValidDouble(strX,m_dX))
	{
		AfxMessageBox(_T("请输入合理的X值"));
		return;
	}
	CString strY;
	GetDlgItemText(IDC_EDIT_Y,strY);
	if(!GStringHelper::IsValidDouble(strY,m_dY))
	{
		AfxMessageBox(_T("请输入合理的Y值"));
		return;
	}
	OnOK();
}
