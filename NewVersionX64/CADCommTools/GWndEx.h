#pragma once
#define CWNDEX_MEMBERS()\
	BOOL GetItemCheck(int nId){\
	CButton* pButton = (CButton*)GetDlgItem(nId);\
	if(pButton==NULL)return ERROR;\
	return pButton->GetCheck();}\
	BOOL SetItemCheck(int nId,BOOL bCheck=TRUE){\
	CButton* pButton = (CButton*)GetDlgItem(nId);\
	if(pButton==NULL)return ERROR;\
	pButton->SetCheck(bCheck);\
	return TRUE;}\
	BOOL GetItemEnable(int nId){\
	CWnd* pWnd = GetDlgItem(nId);\
	if(pWnd==NULL)return ERROR;\
	return pWnd->IsWindowEnabled();}\
	BOOL SetItemEnable(int nId,BOOL bEnable=TRUE){\
	CWnd* pWnd = GetDlgItem(nId);\
	if(pWnd==NULL)return ERROR;\
	pWnd->EnableWindow(bEnable);\
	return TRUE;}\
	BOOL GetItemVisable(int nId){\
	CWnd* pWnd = GetDlgItem(nId);\
	if(pWnd==NULL)return ERROR;\
	return pWnd->IsWindowVisible();}\
	BOOL SelectComboxItem(CComboBox* pCombo,const CString strText,BOOL bIngoreNoCase=TRUE){\
	CString strTmp;\
	for(int i=0;i<pCombo->GetCount();i++){\
	pCombo->GetLBText(i,strTmp);\
	bool rc=bIngoreNoCase?strTmp.CompareNoCase(strText)==0:strTmp.Compare(strText)==0;\
	if(!rc)continue;\
	if(i==pCombo->GetCurSel()) return TRUE;\
	pCombo->SetCurSel(i); return TRUE;}\
	return FALSE;};\
    BOOL FindItemInCombox(CComboBox* pCombo,const CString strText,BOOL bIngoreNoCase=TRUE){\
	CString strTmp;\
	for(int i=0;i<pCombo->GetCount();i++){\
	pCombo->GetLBText(i,strTmp);\
	bool rc=bIngoreNoCase?strTmp.CompareNoCase(strText)==0:strTmp.Compare(strText)==0;\
	if(!rc)continue;\
	return i;}\
	return -1;};\
	BOOL SelectComboxItem(int nId,const CString strText,BOOL bIngoreNoCase=TRUE){\
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nId);\
	if(pCombo==NULL)return ERROR;\
	return SelectComboxItem(pCombo,strText,bIngoreNoCase);     }\
	BOOL FindItemInCombox(int nId,const CString strText,BOOL bIngoreNoCase=TRUE){\
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nId);\
	if(pCombo==NULL)return -2;\
	return FindItemInCombox(pCombo,strText,bIngoreNoCase);     }\
	//定义了CWnd类的一些扩展方法,可以和任何从CWnd类继承的类一起使用
//一般方法返回值为BOOL类型
//TRUE:表示执行成功，或者返回值为TRUE
//ERROR:发生错误，一般情况下为指定ID的对象不存在或类型不正确