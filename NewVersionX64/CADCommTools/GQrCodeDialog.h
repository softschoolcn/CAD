#pragma once
#include "Resource.h"

// GQrCodeDialog 对话框

class GQrCodeDialog : public CDialog
{
	DECLARE_DYNAMIC(GQrCodeDialog)

public:
	GQrCodeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GQrCodeDialog();
	virtual BOOL OnInitDialog();
	CString m_strText;

// 对话框数据
	enum { IDD = IDD_DIALOG_QRCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
