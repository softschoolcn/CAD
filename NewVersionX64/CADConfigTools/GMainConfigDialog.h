#pragma once
#include "resource.h"
#include "..\\CADGlobalTools\\GWndEx.h"
// GMainConfigDialog 对话框

class GMainConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(GMainConfigDialog)

public:
	GMainConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GMainConfigDialog();
	virtual BOOL OnInitDialog();
	//执行设置
	BOOL ExcSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void SelectScale(int nId,int nScale);

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
};
