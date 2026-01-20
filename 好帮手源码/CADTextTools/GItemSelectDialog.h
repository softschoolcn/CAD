#pragma once
#include "resource.h"

// GItemSelectDialog 对话框

class GItemSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(GItemSelectDialog)

public:
	GItemSelectDialog(CStringArray& strArray,int nSelectIndex=0,CString strCaption=_T("选择项"),CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GItemSelectDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT };
	int m_nSelectIndex;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CStringArray m_strArray;
	
	CListBox m_list;
	CString m_strCaption;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
