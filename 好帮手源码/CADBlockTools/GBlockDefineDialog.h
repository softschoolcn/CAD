#pragma once
#include "resource.h"
#include "..\\CADGlobalTools\\GWndEx.h"

// GBlockDefineDialog 对话框

class GBlockDefineDialog : public CDialog
{
	DECLARE_DYNAMIC(GBlockDefineDialog)

public:
	GBlockDefineDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GBlockDefineDialog();
	CString m_strBlcokName;
	CStringArray m_strBlockNameArr;
	int m_nHor;//水平,0:左,1:中,2:右
	int m_nVer;//竖直,0:下,1:中,2:上
	bool m_bOrigin;//移动到原点
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_BLOCKDEFINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio5();
public:
	afx_msg void OnBnClickedRadio10();
public:
	afx_msg void OnBnClickedRadio7();
public:
	afx_msg void OnBnClickedRadio8();
public:
	afx_msg void OnBnClickedOk();
};
