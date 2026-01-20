#pragma once
#include "resource.h"
// GTextEditorDialog 对话框

class GTextEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(GTextEditorDialog)

public:
	GTextEditorDialog(AcDbObjectIdArray& ids,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GTextEditorDialog();
	CGridCtrl mGrid;  //属性表格
	AcDbObjectIdArray mIds;
	AcDbObjectIdArray mTextStyleIds;
	CStringArray mTextStyleNames;
	virtual BOOL OnInitDialog();
	

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	afx_msg void OnBnClickedCheck1();
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};
