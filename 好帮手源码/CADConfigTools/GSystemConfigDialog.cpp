// GSystemConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GSystemConfigDialog.h"


// GSystemConfigDialog 对话框

IMPLEMENT_DYNAMIC(GSystemConfigDialog, CDialog)

GSystemConfigDialog::GSystemConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GSystemConfigDialog::IDD, pParent)
{

}

GSystemConfigDialog::~GSystemConfigDialog()
{
}

void GSystemConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TAB_MAIN,m_tab);
}


BEGIN_MESSAGE_MAP(GSystemConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GSystemConfigDialog::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &GSystemConfigDialog::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// GSystemConfigDialog 消息处理程序
BOOL GSystemConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());

	CAcModuleResourceOverride resOverride;

	m_tab.InsertItem(0,_T("系统"));
	m_tab.InsertItem(1,_T("图层"));
	m_tab.InsertItem(2,_T("尺寸标注"));
	m_tab.InsertItem(3,_T("隐藏命令"));
	m_tab.InsertItem(4,_T("命令别名"));

	m_MainDialog.Create(GMainConfigDialog::IDD,&m_tab);
	m_LayerDialog.Create(GLayerConfigDialog::IDD,&m_tab);
	m_DimConfigDialog.Create(GDimConfigDialog::IDD,&m_tab);
	m_CommandAliasDlg.Create(GCommandAliasDlg::IDD,&m_tab);
	m_CommandHideDialog.Create(GCommandHideDialog::IDD,&m_tab);

	CRect rs; 
	m_tab.GetClientRect(rs); 
	rs.top+=30; 
	rs.bottom-=4; 
	rs.left+=4; 
	rs.right-=4;

	m_MainDialog.MoveWindow(rs);
	m_LayerDialog.MoveWindow(rs);
	m_DimConfigDialog.MoveWindow(rs);
	m_CommandAliasDlg.MoveWindow(rs);
	m_CommandHideDialog.MoveWindow(rs);

	m_MainDialog.ShowWindow(SW_NORMAL);
	m_LayerDialog.ShowWindow(SW_HIDE);
	m_DimConfigDialog.ShowWindow(SW_HIDE);
	m_CommandAliasDlg.ShowWindow(SW_HIDE);
	m_CommandHideDialog.ShowWindow(SW_HIDE);
	return TRUE;
}
void GSystemConfigDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_tab.GetCurSel();
	if(nSel==0)
	{
		if(!m_MainDialog.ExcSet())
		{
			return;
		}
		else
		{
			AfxMessageBox(_T("设置成功，部分效果可能要重启CAD后才会生效"));
		}
	}
	else if(nSel==1)
	{
		if(!m_LayerDialog.ExcSet())
		{
			return;
		}
		else
		{
			AfxMessageBox(_T("设置成功"));
		}
	}
	else if(nSel==2)
	{
		if(!m_DimConfigDialog.ExcSet())
		{
			return;
		}
		else
		{
			AfxMessageBox(_T("设置成功"));
		}
	}
	else if(nSel==3)
	{
		if(!m_CommandHideDialog.ExcSet())
		{
			return;
		}
		else
		{
			AfxMessageBox(_T("设置成功，隐藏的命令在下次CAD启动后将不再显示"));
		}
	}
	else if(nSel==4)
	{
		if(!m_CommandAliasDlg.ExcSet())
		{
			return;
		}
		else
		{
			AfxMessageBox(_T("设置成功，命令别名将在下次CAD启动后生效"));
		}
	}
	OnOK();
}

void GSystemConfigDialog::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSel=m_tab.GetCurSel();
	switch(nSel)
	{
	case 0:
		m_MainDialog.ShowWindow(SW_NORMAL);
	    m_LayerDialog.ShowWindow(SW_HIDE);
		m_DimConfigDialog.ShowWindow(SW_HIDE);
		m_CommandAliasDlg.ShowWindow(SW_HIDE);
		m_CommandHideDialog.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_MainDialog.ShowWindow(SW_HIDE);
	    m_LayerDialog.ShowWindow(SW_NORMAL);
		m_DimConfigDialog.ShowWindow(SW_HIDE);
		m_CommandAliasDlg.ShowWindow(SW_HIDE);
		m_CommandHideDialog.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_MainDialog.ShowWindow(SW_HIDE);
		m_LayerDialog.ShowWindow(SW_HIDE);
		m_DimConfigDialog.ShowWindow(SW_NORMAL);
		m_CommandAliasDlg.ShowWindow(SW_HIDE);
		m_CommandHideDialog.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_MainDialog.ShowWindow(SW_HIDE);
		m_LayerDialog.ShowWindow(SW_HIDE);
		m_DimConfigDialog.ShowWindow(SW_HIDE);
		m_CommandAliasDlg.ShowWindow(SW_HIDE);
		m_CommandHideDialog.ShowWindow(SW_NORMAL);
		break;
	case 4:
		m_MainDialog.ShowWindow(SW_HIDE);
		m_LayerDialog.ShowWindow(SW_HIDE);
		m_DimConfigDialog.ShowWindow(SW_HIDE);
		m_CommandAliasDlg.ShowWindow(SW_NORMAL);
		m_CommandHideDialog.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
}
