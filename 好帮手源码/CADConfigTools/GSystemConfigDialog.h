#pragma once
#include "resource.h"
#include "GMainConfigDialog.h"
#include "GLayerConfigDialog.h"
#include "GDimConfigDialog.h"
#include "GCommandAliasDlg.h"
#include "GCommandHideDialog.h"
// GSystemConfigDialog 对话框

class GSystemConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(GSystemConfigDialog)

public:
	GSystemConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GSystemConfigDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CTabCtrl m_tab;
	GMainConfigDialog m_MainDialog;
	GLayerConfigDialog m_LayerDialog;
	GDimConfigDialog   m_DimConfigDialog;
	GCommandAliasDlg   m_CommandAliasDlg;//命令别名
	GCommandHideDialog m_CommandHideDialog;//命令隐藏


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
};
