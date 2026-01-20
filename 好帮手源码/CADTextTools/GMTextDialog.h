#pragma once
#include "resource.h"

// GMTextDialog 对话框

class GMTextDialog : public CDialog
{
	DECLARE_DYNAMIC(GMTextDialog)

public:
	GMTextDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GMTextDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_MTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
