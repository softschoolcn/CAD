#pragma once
#include "resource.h"

// GLayerConfigDialog 对话框

class GLayerConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(GLayerConfigDialog)

public:
	GLayerConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GLayerConfigDialog();
	virtual BOOL OnInitDialog();
	//执行设置
	BOOL ExcSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_LAYER };
	CGridCtrl mGrid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};
