#pragma once

#include "resource.h"
#include "GWndEx.h"
// GFindTypeDialog 对话框

class GFindTypeDialog : public CDialog
{
	DECLARE_DYNAMIC(GFindTypeDialog)

public:
	GFindTypeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GFindTypeDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FINDTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedOk();
};
