#pragma once
#include "Resource.h"

// GPurgeDialog 对话框

class GPurgeDialog : public CDialog
{
	DECLARE_DYNAMIC(GPurgeDialog)

public:
	GPurgeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GPurgeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PURGE };
	void MsgOutput(CString strMsg);
	int m_nType;//0:清理窗口（默认）,1:统一版本窗口

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_list;
	CListBox  m_listLog;
	CTabCtrl  m_tab;
	CComboBox m_comboTypes;
	void AddLog(CString fmt, ...);
	
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonFloder();
	void AddFloder(const CString strFloer,int& nIngore,int& nCount);
	bool AddFile(const CString strFile);
	afx_msg void OnBnClickedButtonAllsel();
	afx_msg void OnBnClickedButtonFx();
	afx_msg void OnBnClickedButtonAllnotsel();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonPurge();
	afx_msg void OnBnClickedButtonLog();
	void MoveItem(int ItemID,int x,int y);
	afx_msg void OnTcnSelchangeTabPurge(NMHDR *pNMHDR, LRESULT *pResult);
};
void PurgeDialogMsgOutput(CString strMsg,void* pArg);
