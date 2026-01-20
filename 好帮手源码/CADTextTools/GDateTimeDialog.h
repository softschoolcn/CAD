#pragma once
#include "resource.h"
#include "GWndEx.h"
// GDateTimeDialog 对话框

class GDateTimeDialog : public CDialog
{
	DECLARE_DYNAMIC(GDateTimeDialog)

public:
	GDateTimeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GDateTimeDialog();
	virtual BOOL OnInitDialog();
	CTime     m_time;
	CString   m_str;
// 对话框数据
	enum { IDD = IDD_DIALOG_DATETIME };
	CWNDEX_MEMBERS()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	void UpdateDateTime(BOOL updateTime=FALSE);
public:
	afx_msg void OnBnClickedCheck3();
public:
	afx_msg void OnBnClickedCheck7();
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio5();
public:
	afx_msg void OnBnClickedRadio7();
public:
	afx_msg void OnBnClickedRadio8();
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnBnClickedRadio6();
public:
	afx_msg void OnBnClickedRadio9();
public:
	afx_msg void OnBnClickedOk();
};
