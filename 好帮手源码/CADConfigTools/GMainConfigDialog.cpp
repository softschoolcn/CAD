// GMainConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GMainConfigDialog.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "ConfigToolsHeaders.h"
// GMainConfigDialog 对话框

IMPLEMENT_DYNAMIC(GMainConfigDialog, CDialog)

GMainConfigDialog::GMainConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GMainConfigDialog::IDD, pParent)
{

}

GMainConfigDialog::~GMainConfigDialog()
{
}

void GMainConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GMainConfigDialog, CDialog)
END_MESSAGE_MAP()


// GMainConfigDialog 消息处理程序
//执行设置
BOOL GMainConfigDialog::ExcSet()
{
	bool bScreenMenu=GetItemCheck(IDC_CHECK_SCREENMENU);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("ScreenMenu"),_T("Show"),bScreenMenu);
	bool bCADMenu=GetItemCheck(IDC_CHECK_CADMENU);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("CADMenu"),_T("Show"),bCADMenu);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HTBL);
	if((!bScreenMenu)&&(!bCADMenu))
	{
		AfxMessageBox(_T("不允许同时关闭CAD菜单和屏幕菜单"));
		return FALSE;
	}
	CString strHTBL;
	pCombo->GetWindowText(strHTBL);
	int nHTBL=1;
	int nCTBL=1;
	if(!GStringHelper::IsValidInt(strHTBL,nHTBL))
	{
		AfxMessageBox(_T("请输入有效的绘图比例"));
		return FALSE;
	}
	CString strCTBL;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CTBL);
	pCombo->GetWindowText(strCTBL);
	if(!GStringHelper::IsValidInt(strCTBL,nCTBL))
	{
		AfxMessageBox(_T("请输入有效的出图比例"));
		return FALSE;
	}
	Acad::ErrorStatus es=GCADUtils::SetScale(nHTBL,nCTBL);
	if(es!=Acad::eOk)
	{
		CString strInfo;
		strInfo.Format(_T("设置比例失败:%s"),acadErrorStatusTextChs(es));
		AfxMessageBox(strInfo);
		return FALSE;
	}
	if(GetItemCheck(IDC_CHECK_HTBL))
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GMainConfigDialog"),_T("HTBL"),strHTBL);
	}
	if(GetItemCheck(IDC_CHECK_CTBL))
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GMainConfigDialog"),_T("CTBL"),strCTBL);
	}
	GLockDocument _lock;
	AcDbObjectId   rcId;
	UpdateDimStyleByConfig(rcId);
	return TRUE;
}
BOOL GMainConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_HTBL);
	if (pCombo != NULL)
	{
		pCombo->AddString(_T("1"));
		pCombo->AddString(_T("5"));
		pCombo->AddString(_T("10"));
		pCombo->AddString(_T("20"));
		pCombo->AddString(_T("25"));
		pCombo->AddString(_T("50"));
		pCombo->AddString(_T("75"));
		pCombo->AddString(_T("100"));
		pCombo->AddString(_T("150"));
		pCombo->AddString(_T("200"));
		pCombo->AddString(_T("300"));
		pCombo->AddString(_T("400"));
		pCombo->AddString(_T("500"));
		pCombo->SetCurSel(0);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CTBL);
	if (pCombo != NULL)
	{
		pCombo->AddString(_T("1"));
		pCombo->AddString(_T("5"));
		pCombo->AddString(_T("10"));
		pCombo->AddString(_T("20"));
		pCombo->AddString(_T("25"));
		pCombo->AddString(_T("50"));
		pCombo->AddString(_T("75"));
		pCombo->AddString(_T("100"));
		pCombo->AddString(_T("150"));
		pCombo->AddString(_T("200"));
		pCombo->AddString(_T("300"));
		pCombo->AddString(_T("400"));
		pCombo->AddString(_T("500"));
		pCombo->SetCurSel(0);
	}
	AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase();
	int nHTBL=1;
	int nCTBL=1;
	GCADUtils::GetScale(nHTBL,nCTBL);
	SelectScale(IDC_COMBO_HTBL,nHTBL);
	SelectScale(IDC_COMBO_CTBL,nCTBL);
	bool bScreenMenu=GUserDataRecord::getInstance()->GetProfileBool(_T("ScreenMenu"),_T("Show"),true);
	bool bCADMenu=GUserDataRecord::getInstance()->GetProfileBool(_T("CADMenu"),_T("Show"),true);
	SetItemCheck(IDC_CHECK_SCREENMENU,bScreenMenu);
	SetItemCheck(IDC_CHECK_CADMENU,bCADMenu);
	return TRUE;
}
void GMainConfigDialog::SelectScale(int nId,int nScale)
{
	CString strScale;
	strScale.Format(_T("%d"),nScale);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nId);
	for(int i=0;i<pCombo->GetCount();i++)
	{
		CString strText;
		pCombo->GetLBText(i,strText);
		if(strScale.CompareNoCase(strText)==0)
		{
			pCombo->SetCurSel(i);
			return;
		}
	}
	int nIndex=pCombo->AddString(strScale);
	pCombo->SetCurSel(nIndex);

}