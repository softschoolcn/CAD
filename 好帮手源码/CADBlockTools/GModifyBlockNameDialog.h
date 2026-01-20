#pragma once
#include "resource.h"
// GModifyBlockNameDialog 对话框

class GModifyBlockNameDialog : public CDialog
{
	DECLARE_DYNAMIC(GModifyBlockNameDialog)

public:
	GModifyBlockNameDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GModifyBlockNameDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_BLOCKNAME };
	CString m_strBlcokName;
	CStringArray m_strBlockNameArr;
	virtual BOOL OnInitDialog();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
