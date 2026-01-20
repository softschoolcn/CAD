#pragma once

#include "resource.h"
#include "LinkLable.h"
// GRegDialog 对话框

class GRegDialog : public CDialog
{
	DECLARE_DYNAMIC(GRegDialog)

public:
	GRegDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GRegDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_REG };
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CHyperLinkLable m_link;
	CTabCtrl        m_tab;
	CGridCtrl mGrid;  //属性表格
	void ShowTabSel(int nSel);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
};
