#pragma once
#include "resource.h"

// GCommandHideDialog 对话框

class GCommandHideDialog : public CDialog
{
	DECLARE_DYNAMIC(GCommandHideDialog)

public:
	GCommandHideDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCommandHideDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMMANHIDE };
	//执行设置
	BOOL ExcSet();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CListCtrl m_list;
	bool ReadCommands();
	bool ReadChildCommands(MSXML2::IXMLDOMNodePtr lpMenuNode);
	void AddCommand(CString strName);
	CStringArray m_hideMenuArray;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonQx();
public:
	afx_msg void OnBnClickedButtonQbx();
public:
	afx_msg void OnBnClickedButtonFx();
};
