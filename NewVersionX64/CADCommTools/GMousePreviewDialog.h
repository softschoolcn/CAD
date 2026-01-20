#pragma once

#include "resource.h"
// GMousePreviewDialog 对话框

class GMousePreviewDialog : public CDialog
{
	DECLARE_DYNAMIC(GMousePreviewDialog)

public:
	GMousePreviewDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GMousePreviewDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOUSEPREVIEW };
	bool m_bOpenPreview;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CListCtrl m_list;
	CButton   m_btnAutoStart;
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
public:
	afx_msg void OnBnClickedButtonStop();
public:
	afx_msg void OnBnClickedButtonAllsel();
public:
	afx_msg void OnBnClickedButtonFx();
public:
	afx_msg void OnBnClickedButtonAllnotsel();
};
