#pragma once

#include "resource.h"
#include "..\\CADGlobalTools\\GWndEx.h"
// GTextBlockDialog 对话框

class GTextBlockDialog : public CDialog
{
	DECLARE_DYNAMIC(GTextBlockDialog)

public:
	GTextBlockDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GTextBlockDialog();
public:
	int    m_nTextMode;//1:单行文字 2:多行文字 3:单行、多行
	double m_dX;
	double m_dY;
	BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTBLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedOk();
};
