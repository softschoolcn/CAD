// GItemSelectDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GItemSelectDialog.h"


// GItemSelectDialog 对话框

IMPLEMENT_DYNAMIC(GItemSelectDialog, CDialog)

GItemSelectDialog::GItemSelectDialog(CStringArray& strArray,
									 int nSelectIndex,
									 CString strCaption,
									 CWnd* pParent /*=NULL*/)
	: CDialog(GItemSelectDialog::IDD, pParent)
{
	m_strArray.Append(strArray);
	m_nSelectIndex=nSelectIndex;
	m_strCaption=strCaption;
}

GItemSelectDialog::~GItemSelectDialog()
{
}

void GItemSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_ITEMS,m_list);
}


BEGIN_MESSAGE_MAP(GItemSelectDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GItemSelectDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GItemSelectDialog 消息处理程序
BOOL GItemSelectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int i=0;i<m_strArray.GetCount();i++)
	{
		int nIndex=m_list.AddString(m_strArray.GetAt(i));
		if(i==m_nSelectIndex)
		{
			m_list.SetCurSel(nIndex);
		}
	}
	if(m_list.GetCurSel()<0)
	{
		m_list.SetCurSel(0);
	}
	SetWindowText(m_strCaption);
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GItemSelectDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nSelectIndex=m_list.GetCurSel();
	OnOK();
}
