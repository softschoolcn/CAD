// GFindTypeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GFindTypeDialog.h"
#include "GFindTypes.h"

// GFindTypeDialog 对话框

IMPLEMENT_DYNAMIC(GFindTypeDialog, CDialog)
GFindTypeDialog::GFindTypeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GFindTypeDialog::IDD, pParent)
{
}

GFindTypeDialog::~GFindTypeDialog()
{
}

void GFindTypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GFindTypeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GFindTypeDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GFindTypeDialog 消息处理程序
BOOL GFindTypeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	GFindTypes types;
	types.Read();
	SetItemCheck(IDC_CHECK1,types.m_bAttr);
	SetItemCheck(IDC_CHECK2,types.m_bDim);
	SetItemCheck(IDC_CHECK4,types.m_bText);
	SetItemCheck(IDC_CHECK3,types.m_bTable);
	SetItemCheck(IDC_CHECK5,types.m_bLinkDesc);
	SetItemCheck(IDC_CHECK6,types.m_bLink);
	SetItemCheck(IDC_CHECK7,types.m_bBlock);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GFindTypeDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GFindTypes types;
	types.m_bAttr=GetItemCheck(IDC_CHECK1);
	types.m_bDim=GetItemCheck(IDC_CHECK2);
	types.m_bText=GetItemCheck(IDC_CHECK4);
	types.m_bTable=GetItemCheck(IDC_CHECK3);
	types.m_bLinkDesc=GetItemCheck(IDC_CHECK5);
	types.m_bLink=GetItemCheck(IDC_CHECK6);
	types.m_bBlock=GetItemCheck(IDC_CHECK7);
	if(types.GetCount()==0)
	{
		AfxMessageBox(_T("至少需要选择一种实体类型"));
		return;
	}
	types.Write();
	OnOK();
}
