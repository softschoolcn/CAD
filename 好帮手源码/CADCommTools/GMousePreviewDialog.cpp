// GMousePreviewDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GMousePreviewDialog.h"
extern CStringArray Glb_MousePreviewKeyArr;
extern CStringArray Glb_MousePreviewValueArr;
extern CArray<bool> Glb_MousePreviewOpenArr;
extern bool         Glb_MousePreviewAutoOpen;
extern bool m_bXDataDynViewOpen;

// GMousePreviewDialog 对话框

IMPLEMENT_DYNAMIC(GMousePreviewDialog, CDialog)

GMousePreviewDialog::GMousePreviewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GMousePreviewDialog::IDD, pParent)
{

}

GMousePreviewDialog::~GMousePreviewDialog()
{
}

void GMousePreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MOUSE,m_list);
	DDX_Control(pDX,IDC_CHECK_AUTOSTART,m_btnAutoStart);
}


BEGIN_MESSAGE_MAP(GMousePreviewDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, &GMousePreviewDialog::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &GMousePreviewDialog::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL, &GMousePreviewDialog::OnBnClickedButtonAllsel)
	ON_BN_CLICKED(IDC_BUTTON_FX, &GMousePreviewDialog::OnBnClickedButtonFx)
	ON_BN_CLICKED(IDC_BUTTON_ALLNOTSEL, &GMousePreviewDialog::OnBnClickedButtonAllnotsel)
END_MESSAGE_MAP()


// GMousePreviewDialog 消息处理程序
BOOL GMousePreviewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		int nIntem=m_list.InsertItem(m_list.GetItemCount(),Glb_MousePreviewValueArr.GetAt(i));
		if(Glb_MousePreviewOpenArr.GetAt(i))
		{
			m_list.SetCheck(nIntem,TRUE);
		}
	}
	if(Glb_MousePreviewAutoOpen)
	{
		m_btnAutoStart.SetCheck(TRUE);
	}
	if(m_bXDataDynViewOpen)
	{
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(_T("开始预览(*)"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(_T("停止预览(*)"));
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GMousePreviewDialog::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount=0;
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			Glb_MousePreviewOpenArr.SetAt(i,true);
			nCount++;
		}
		else
		{
			Glb_MousePreviewOpenArr.SetAt(i,false);
		}
	}
	if(nCount<=0)
	{
		AfxMessageBox(_T("您至少需要选择显示一个属性"));
		return;
	}
	m_bOpenPreview=true;
	if(m_btnAutoStart.GetCheck())
	{
		Glb_MousePreviewAutoOpen=true;
	}
	else
	{
		Glb_MousePreviewAutoOpen=false;
	}
	OnOK();

}

void GMousePreviewDialog::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount=0;
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			Glb_MousePreviewOpenArr.SetAt(i,true);
			nCount++;
		}
		else
		{
			Glb_MousePreviewOpenArr.SetAt(i,false);
		}
	}
	m_bOpenPreview=false;
	if(m_btnAutoStart.GetCheck())
	{
		Glb_MousePreviewAutoOpen=true;
	}
	else
	{
		Glb_MousePreviewAutoOpen=false;
	}
	OnOK();
	
}

void GMousePreviewDialog::OnBnClickedButtonAllsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}

void GMousePreviewDialog::OnBnClickedButtonFx()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		m_list.SetCheck(i,!m_list.GetCheck(i));
	}
}

void GMousePreviewDialog::OnBnClickedButtonAllnotsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<Glb_MousePreviewValueArr.GetCount();i++)
	{
		m_list.SetCheck(i,FALSE);
	}
}
