// GTextBrushDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GTextBrushDialog.h"
#include "GlobalFunction.h"
GTextBrushState::GTextBrushState()
{
	Reset();
}
GTextBrushState::~GTextBrushState()
{

}
long GTextBrushState::getState() const
{
	long m_nState=0L;
	if(bContent)
	{
		m_nState|=0x1;
	}
	if(bColor)
	{
		m_nState|=0x2;
	}
	if(bFontHeight)
	{
		m_nState|=0x4;
	}
	if(bAngle)
	{
		m_nState|=0x8;
	}
	if(bAlign)
	{
		m_nState|=0x10;
	}
	if(bLayer)
	{
		m_nState|=0x20;
	}
	if(bScale)
	{
		m_nState|=0x40;
	}
	if(bStyle)
	{
		m_nState|=0x80;
	}
	return m_nState;
}
void GTextBrushState::setState(long nState)
{
	bContent=IsXor(nState,0x1);
	bColor=IsXor(nState,0x2);
	bFontHeight=IsXor(nState,0x4);
	bAngle=IsXor(nState,0x8);
	bAlign=IsXor(nState,0x10);
	bLayer=IsXor(nState,0x20);
	bScale=IsXor(nState,0x40);
	bStyle=IsXor(nState,0x80);

}
void GTextBrushState::Reset()
{
	bContent=false;
	bColor=false;
	bFontHeight=false;
	bAngle=false;
	bAlign=false;
	bLayer=false;
	bScale=false;
	bStyle=false;
}

// GTextBrushDialog 对话框

IMPLEMENT_DYNAMIC(GTextBrushDialog, CDialog)

GTextBrushDialog::GTextBrushDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GTextBrushDialog::IDD, pParent)
{

}

GTextBrushDialog::~GTextBrushDialog()
{
}

void GTextBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GTextBrushDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &GTextBrushDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &GTextBrushDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &GTextBrushDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDOK, &GTextBrushDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GTextBrushDialog 消息处理程序
BOOL GTextBrushDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_state.setState(GUserDataRecord::getInstance()->GetProfileInt(_T("GTextBrushDialog"),_T("State"),0x80));
	InitIdAndBools();
	for(int i=0;i<m_itemIds.GetCount();i++)
	{
		int nId=m_itemIds.GetAt(i);
		CWnd* pWnd=GetDlgItem(nId);
		if(pWnd==NULL)
		{
			continue;
		}
		CButton* pBtn=(CButton*)pWnd;
		if(pBtn==NULL)
		{
			continue;
		}
		pBtn->SetCheck((*m_bools.GetAt(i))?TRUE:FALSE);

	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
//初始化ID和bool的对应关系
void GTextBrushDialog::InitIdAndBools()
{
	m_itemIds.Add(IDC_CHECK1);
	m_bools.Add(&m_state.bContent);
	m_itemIds.Add(IDC_CHECK2);
	m_bools.Add(&m_state.bColor);
	m_itemIds.Add(IDC_CHECK3);
	m_bools.Add(&m_state.bFontHeight);
	m_itemIds.Add(IDC_CHECK4);
	m_bools.Add(&m_state.bAngle);
	m_itemIds.Add(IDC_CHECK5);
	m_bools.Add(&m_state.bAlign);
	m_itemIds.Add(IDC_CHECK6);
	m_bools.Add(&m_state.bLayer);
	//m_itemIds.Add(IDC_CHECK7);
	//m_bools.Add(&m_state.bScale);
	m_itemIds.Add(IDC_CHECK8);
	m_bools.Add(&m_state.bStyle);
}
void GTextBrushDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//全选
	for(int i=0;i<m_itemIds.GetCount();i++)
	{
		CWnd* pWnd=GetDlgItem(m_itemIds.GetAt(i));
		if(pWnd==NULL)
		{
			continue;
		}
		CButton* pBtn=(CButton*)pWnd;
		if(pBtn==NULL)
		{
			continue;
		}
		pBtn->SetCheck(TRUE);
	}
}

void GTextBrushDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//反选
	for(int i=0;i<m_itemIds.GetCount();i++)
	{
		CWnd* pWnd=GetDlgItem(m_itemIds.GetAt(i));
		if(pWnd==NULL)
		{
			continue;
		}
		CButton* pBtn=(CButton*)pWnd;
		if(pBtn==NULL)
		{
			continue;
		}
		pBtn->SetCheck(!pBtn->GetCheck());
	}
}

void GTextBrushDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//全不选
	for(int i=0;i<m_itemIds.GetCount();i++)
	{
		CWnd* pWnd=GetDlgItem(m_itemIds.GetAt(i));
		if(pWnd==NULL)
		{
			continue;
		}
		CButton* pBtn=(CButton*)pWnd;
		if(pBtn==NULL)
		{
			continue;
		}
		pBtn->SetCheck(FALSE);
	}
}

void GTextBrushDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_itemIds.GetCount();i++)
	{
		int nId=m_itemIds.GetAt(i);
		CWnd* pWnd=GetDlgItem(nId);
		if(pWnd==NULL)
		{
			continue;
		}
		CButton* pBtn=(CButton*)pWnd;
		if(pBtn==NULL)
		{
			continue;
		}
		*m_bools.GetAt(i)=pBtn->GetCheck();

	}
	long lState=m_state.getState();
	if(lState==0)
	{
		AfxMessageBox(_T("请您至少选择一个属性"));
		return;
	}
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GTextBrushDialog"),_T("State"),lState);
	OnOK();
}
