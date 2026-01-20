// GExportDimensionDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GExportDimensionDialog.h"


// GExportDimensionDialog 对话框

IMPLEMENT_DYNAMIC(GExportDimensionDialog, CDialog)

GExportDimensionDialog::GExportDimensionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GExportDimensionDialog::IDD, pParent)
{

}

GExportDimensionDialog::~GExportDimensionDialog()
{
}

void GExportDimensionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MOUSE,m_list);
}


BEGIN_MESSAGE_MAP(GExportDimensionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GExportDimensionDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GExportDimensionDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ALLSEL, &GExportDimensionDialog::OnBnClickedButtonAllsel)
	ON_BN_CLICKED(IDC_BUTTON_FX, &GExportDimensionDialog::OnBnClickedButtonFx)
	ON_BN_CLICKED(IDC_BUTTON_ALLNOTSEL, &GExportDimensionDialog::OnBnClickedButtonAllnotsel)
END_MESSAGE_MAP()


// GExportDimensionDialog 消息处理程序
BOOL GExportDimensionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	m_strNameArray.Add(_T("对齐标注"));
	m_classArray.Add(AcDbAlignedDimension::desc());

	//m_strNameArray.Add(_T("线性标注"));
	//m_classArray.Add(AcDbAlignedDimension::desc());

	m_strNameArray.Add(_T("转角标注"));
	m_classArray.Add(AcDbRotatedDimension::desc());

	m_strNameArray.Add(_T("半径标注"));
	m_classArray.Add(AcDbRadialDimension::desc());

	m_strNameArray.Add(_T("直径标注"));
	m_classArray.Add(AcDbDiametricDimension::desc());

	m_strNameArray.Add(_T("角度标注"));
	m_classArray.Add(AcDb3PointAngularDimension::desc());

	m_strNameArray.Add(_T("坐标标注"));
	m_classArray.Add(AcDbOrdinateDimension::desc());

	CString strValue;
	for(int i=0;i<m_strNameArray.GetCount();i++)
	{
		int nIntem=m_list.InsertItem(m_list.GetItemCount(),m_strNameArray.GetAt(i));
		m_list.SetItemData(nIntem,(DWORD_PTR)m_classArray.GetAt(i));
		strValue=GUserDataRecord::getInstance()->GetProfileString(_T("GExportDimensionDialog"),m_classArray.GetAt(i)->name(),256,_T("True"));
		if(strValue.CompareNoCase(_T("True"))==0)
		{
			m_list.SetCheck(nIntem,TRUE);
		}
	}
	strValue=GUserDataRecord::getInstance()->GetProfileString(_T("GExportDimensionDialog"),_T("AllEnt"),256,_T("True"));
	if(strValue.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_EXPORTALL,TRUE);
	}
	else
	{
		SetItemCheck(IDC_CHECK_EXPORTALL,FALSE);
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GExportDimensionDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CStringArray m_strNewNameArray;
	CArray<AcRxClass*> m_classNewArray;
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			GUserDataRecord::getInstance()->WriteProfileString(_T("GExportDimensionDialog"),m_classArray.GetAt(i)->name(),_T("True"));
			m_strNewNameArray.Add(m_strNameArray.GetAt(i));
			m_classNewArray.Add(m_classArray.GetAt(i));
		}
		else
		{
			GUserDataRecord::getInstance()->WriteProfileString(_T("GExportDimensionDialog"),m_classArray.GetAt(i)->name(),_T("False"));
		}
	}
	if(m_strNewNameArray.GetCount()==0)
	{
		AfxMessageBox(_T("您至少需要选择一种类型的标注"));
		return;
	}
	m_strNameArray.RemoveAll();
	m_classArray.RemoveAll();

	m_strNameArray.Append(m_strNewNameArray);
	m_classArray.Append(m_classNewArray);
	if(GetItemCheck(IDC_CHECK_EXPORTALL))
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GExportDimensionDialog"),_T("AllEnt"),_T("True"));
		m_bAllEnt=true;
	}
	else
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GExportDimensionDialog"),_T("AllEnt"),_T("False"));
		m_bAllEnt=false;
	}

	OnOK();
}

void GExportDimensionDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void GExportDimensionDialog::OnBnClickedButtonAllsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}

void GExportDimensionDialog::OnBnClickedButtonFx()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,!m_list.GetCheck(i));
	}
}

void GExportDimensionDialog::OnBnClickedButtonAllnotsel()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,FALSE);
	}
}
