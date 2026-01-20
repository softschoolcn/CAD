#pragma once
#include "resource.h"

// GBarCodeDialog 对话框

class GBarCodeDialog : public CDialog
{
	DECLARE_DYNAMIC(GBarCodeDialog)

public:
	GBarCodeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GBarCodeDialog();
	virtual BOOL OnInitDialog();
	CString m_strBarCode;
	bool    m_bText;

// 对话框数据
	enum { IDD = IDD_DIALOG_BARCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
