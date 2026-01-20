#pragma once
#include "resource.h"

// GDocListDialog 对话框

class GDocListDialog : public CDialog
{
	DECLARE_DYNAMIC(GDocListDialog)

public:
	GDocListDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GDocListDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOCLIST };
	AcApDocument* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CListBox m_list;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
