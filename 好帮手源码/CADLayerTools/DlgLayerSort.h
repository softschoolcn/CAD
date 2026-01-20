#pragma once
#include "resource.h"
#ifndef ACAD_2004
// CDlgLayerSort 对话框

class CDlgLayerSort : public CDialog
{
	DECLARE_DYNAMIC(CDlgLayerSort)

public:
	         CDlgLayerSort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLayerSort();

	virtual BOOL OnInitDialog( );
	        BOOL DisplayLayerOrder(AcDbObjectIdArray &LyIdArray, CUIntArray* iRepeats);

// 对话框数据
	enum { IDD = IDD_DLG_LYSORT };

public:
	CDragListBox m_listly;
	CButton m_ApplyBtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void ApplyLayerOrder(void);
	bool RefreshList(void);
	void DisplayEntorder();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnLbnSelchangeListLy();
	afx_msg void OnBnClickedCancel();
};
#endif
