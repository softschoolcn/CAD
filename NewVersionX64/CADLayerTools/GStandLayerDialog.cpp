// GStandLayerDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GStandLayerDialog.h"


// GStandLayerDialog 对话框

IMPLEMENT_DYNAMIC(GStandLayerDialog, CDialog)

GStandLayerDialog::GStandLayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GStandLayerDialog::IDD, pParent)
{

}

GStandLayerDialog::~GStandLayerDialog()
{
}

void GStandLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GStandLayerDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GStandLayerDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GStandLayerDialog 消息处理程序
BOOL GStandLayerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CArray<int> ids;
	ids.Add(IDC_COMBO1);
	ids.Add(IDC_COMBO2);
	ids.Add(IDC_COMBO3);
	for(int i=0;i<ids.GetCount();i++)
	{
		CString strType;
		CString strDefault;
		if(i==0)
		{
			strType=_T("Text");
			strDefault=_T("Text");
		}
		else if(i==1)
		{
			strType=_T("Dim");
			strDefault=_T("Dim");
		}
		else
		{
			strType=_T("Hatch");
			strDefault=_T("Hat");
		}
		CComboBox* pCombo=(CComboBox*)GetDlgItem(ids.GetAt(i));
		pCombo->AddString(_T("保持原有图层不变"));
		CString strValue=GUserDataRecord::getInstance()->GetProfileString(_T("GStandLayerDialog"),strType,256,strDefault);
		for(int j=0;j<m_strLayerNameArr.GetCount();j++)
		{
			int nIndex=pCombo->AddString(m_strLayerNameArr.GetAt(j));
			if(strValue.CompareNoCase(m_strLayerNameArr.GetAt(j))==0)
			{
				pCombo->SetCurSel(nIndex);
			}
		}
		if(pCombo->GetCurSel()<0)
		{
			pCombo->SetCurSel(0);
		}
		
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GStandLayerDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CArray<int> ids;
	ids.Add(IDC_COMBO1);
	ids.Add(IDC_COMBO2);
	ids.Add(IDC_COMBO3);
	for(int i=0;i<ids.GetCount();i++)
	{
		CString strType;
		if(i==0)
		{
			strType=_T("Text");
		}
		else if(i==1)
		{
			strType=_T("Dim");
		}
		else
		{
			strType=_T("Hatch");
		}
		CComboBox* pCombo=(CComboBox*)GetDlgItem(ids.GetAt(i));
		int nSel=pCombo->GetCurSel();
		nSel-=1;//去掉"保持原有图层不变"
		m_selectLayerIndexArr.Add(nSel);
		if(nSel>=0)
		{
			GUserDataRecord::getInstance()->WriteProfileString(_T("GStandLayerDialog"),strType,m_strLayerNameArr.GetAt(i));
		}
		
	}
	OnOK();
}
