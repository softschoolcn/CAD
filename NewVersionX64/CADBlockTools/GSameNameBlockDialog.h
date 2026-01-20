#pragma once
#include "resource.h"
#include "..\\CADGlobalTools\\GWndEx.h"
// GSameNameBlockDialog 对话框

class GSameNameBlockDialog : public CDialog
{
	DECLARE_DYNAMIC(GSameNameBlockDialog)

public:
	GSameNameBlockDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GSameNameBlockDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SAMENAMEBLOCK };
public:
	bool m_bSameHandle;//相同处理
	int  m_nMode;//1:不生成块 2:生成相同的块 3:生成不同名称的块

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedOk();
};
