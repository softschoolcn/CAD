#pragma once
#include "resource.h"
#include "GWndEx.h"
// GEntAlignDialog 对话框

class GEntAlignDialog : public CAcUiDialog
{
	DECLARE_DYNAMIC(GEntAlignDialog)

public:
	GEntAlignDialog(AcDbObjectIdArray& m_ids,CWnd* pParent = NULL,HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~GEntAlignDialog();
	virtual BOOL OnInitDialog();
	AcDbObjectIdArray   ids;//实体数组
	AcGePoint3dArray    pts;//偏移数组
	AcDbExtents         m_selExt;
	void MoveBack();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//应用
	Acad::ErrorStatus Apply();
	CAcUiSelectButton	m_btnPt;
	CAcUiSelectButton	m_btnEnt;

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedCheck3();
public:
	afx_msg void OnBnClickedCheck4();
public:
	afx_msg void OnBnClickedCheck5();
public:
	afx_msg void OnBnClickedCheck6();
public:
	afx_msg void OnBnClickedCheck7();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedOk();
};
