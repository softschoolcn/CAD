// GDocListDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GDocListDialog.h"


// GDocListDialog 对话框

IMPLEMENT_DYNAMIC(GDocListDialog, CDialog)

GDocListDialog::GDocListDialog(CWnd* pParent /*=NULL*/)
: CDialog(GDocListDialog::IDD, pParent),m_pDoc(NULL)
{

}

GDocListDialog::~GDocListDialog()
{
}

void GDocListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_DOC,m_list);
}


BEGIN_MESSAGE_MAP(GDocListDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GDocListDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GDocListDialog 消息处理程序
BOOL GDocListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step())
	{
		int nIndex=m_list.AddString(pIter->document()->fileName());
		m_list.SetItemData(nIndex,(DWORD_PTR)pIter->document());
		if(nIndex==0)
		{
			m_list.SetCurSel(nIndex);
		}
	}
	delete pIter;
	pIter=NULL;
	#ifndef CMP_SINGLE
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
#endif
	return TRUE;
}
void GDocListDialog::OnBnClickedOk()
{
	m_pDoc=(AcApDocument*)m_list.GetItemData(m_list.GetCurSel());
	OnOK();
}
