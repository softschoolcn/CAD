#pragma once
#include "resource.h"

// GDimTextDialog 对话框

class GDimTextDialog : public CDialog
{
	DECLARE_DYNAMIC(GDimTextDialog)

public:
	GDimTextDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GDimTextDialog();
	virtual BOOL OnInitDialog();
	CString strText;

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTDIM };
	void ReadTxtFile(LPCTSTR lpFile, CStringArray& array);
	void WriteTxtFile(LPCTSTR lpFile, const CStringArray& array);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	TCHAR m_lpFile[MAX_PATH+1];
	CListCtrl m_list;
	CEdit m_wndEdit;
	void UpdateTxtFile();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
};
