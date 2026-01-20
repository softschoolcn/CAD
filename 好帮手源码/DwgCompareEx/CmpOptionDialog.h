#pragma once
#include "resource.h"
#include "CmpConfig.h"
#include "LinkLable.h"
#include "..\\CADGlobalTools\\GWndEx.h"
// CmpOptionDialog 对话框

class CmpOptionDialog : public CDialog
{
	DECLARE_DYNAMIC(CmpOptionDialog)

public:
	CmpOptionDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CmpOptionDialog();
	virtual BOOL OnInitDialog();
	CmpConfig* m_pConfig;
	bool       m_bReg;//是否已经注册

// 对话框数据
	enum { IDD = IDD_DIALOG_OPTION };
	bool EnumCheckState(HTREEITEM hParent,int& nCount,BOOL bWrite=TRUE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
private:
	CTreeCtrl		m_wndTree;
	CListCtrl       m_wndList;
	CImageList m_ImageList;
	CAcUiColorComboBox m_ColorComboBoxSame;
	CAcUiColorComboBox m_ColorComboBoxChange;
	CAcUiColorComboBox m_ColorComboBoxNew;
	CAcUiColorComboBox m_ColorComboBoxDelete;
	CString m_shrPath;
	CButton m_ButtonPostion;
	CButton m_ButtonHandle;
	CButton m_ButtonLookOnly;
	CButton m_ButtonCmpFile;
	CButton m_ButtonCmpFloder;
	CButton m_ButtonSourceFile;
	CButton m_ButtonCloseLayer;
	CButton m_ButtonLockLayer;
	CEdit   m_EditIngoreLayer;
	CString m_strTmp1,m_strTmp2;//记录切换过程中的文件或者目录
	void ExpandNode(HTREEITEM hItem);
	int GetChildNodeCount(HTREEITEM hItem);
	int m_nRootNodeCount;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowTypes(HTREEITEM hItem);
	void SaveTypes();
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadioPostion();
	afx_msg void OnBnClickedRadioHandle();
	afx_msg void OnBnClickedRadioLookonly();
	afx_msg void OnBnClickedRadioCmpfile();
	afx_msg void OnBnClickedRadioCmpfloder();
public:
public:
	afx_msg void OnBnClickedButtonDoc1();
public:
	afx_msg void OnBnClickedButtonDoc2();
};
