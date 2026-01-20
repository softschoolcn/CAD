#pragma once
#include "Resource.h"

// GStandLayerDialog 对话框

class GStandLayerDialog : public CDialog
{
	DECLARE_DYNAMIC(GStandLayerDialog)

public:
	GStandLayerDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GStandLayerDialog();
	virtual BOOL OnInitDialog();
	CStringArray m_strLayerNameArr;

	CArray<int>  m_selectLayerIndexArr;


// 对话框数据
	enum { IDD = IDD_DIALOG_LAYERNORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
