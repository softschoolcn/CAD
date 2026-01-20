#pragma once
#include "resource.h"
#include "GWndEx.h"
// GExportDimensionDialog 对话框

class GExportDimensionDialog : public CDialog
{
	DECLARE_DYNAMIC(GExportDimensionDialog)

public:
	GExportDimensionDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GExportDimensionDialog();
	virtual BOOL OnInitDialog();
	CStringArray m_strNameArray;
	CArray<AcRxClass*> m_classArray;
	bool m_bAllEnt;

// 对话框数据
	enum { IDD = IDD_DIALOG_EXPORTDIMENSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CListCtrl m_list;
	


	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAllsel();
	afx_msg void OnBnClickedButtonFx();
	afx_msg void OnBnClickedButtonAllnotsel();
};
