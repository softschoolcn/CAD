#pragma once
#include "resource.h"

// GTextDialog 对话框

class GTextDialog : public CDialog
{
	DECLARE_DYNAMIC(GTextDialog)

public:
	GTextDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GTextDialog();
	CString m_strText;
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
