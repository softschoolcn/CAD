#pragma once

#include "resource.h"
#include "GWndEx.h"
// GJoinDwgDialog 对话框
//图纸合并参数
struct GJoinDwgParam
{
	GJoinDwgParam():m_dDis(0.0)
	{
	}
	GJoinDwgParam(const GJoinDwgParam& src)
	{
		m_fileArray.RemoveAll();
		m_fileArray.Append(src.m_fileArray);
		m_nPL=src.m_nPL;
		m_dDis=src.m_dDis;
		m_dDisHor=src.m_dDisHor;
		m_dDisVer=src.m_dDisVer;
		m_nNum=src.m_nNum;
	}
	GJoinDwgParam& operator=(const GJoinDwgParam& src)
	{
		if(this==&src)
		{
			return *this;
		}
		m_fileArray.RemoveAll();
		m_fileArray.Append(src.m_fileArray);
		m_nPL=src.m_nPL;
		m_dDis=src.m_dDis;
		m_dDisHor=src.m_dDisHor;
		m_dDisVer=src.m_dDisVer;
		m_nNum=src.m_nNum;
		return *this;
	}
	CStringArray m_fileArray;
	int          m_nPL;//排列方式0:横排 1:竖排 2:混合排列
	double       m_dDis;//间距
	double       m_dDisHor;//水平间距
	double       m_dDisVer;//竖直间距
	int          m_nNum;//单排图纸数量
};
class GJoinDwgDialog : public CDialog
{
	DECLARE_DYNAMIC(GJoinDwgDialog)

public:
	GJoinDwgDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GJoinDwgDialog();
	virtual BOOL OnInitDialog();

	GJoinDwgParam m_param;

// 对话框数据
	enum { IDD = IDD_DIALOG_JOINDWG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool AddFile(const CString strFile);
	void AddFloder(const CString strFloder,int& nIngore,int& nCount);
	CListBox m_list;
	CComboBox m_comBox;
	//不可编辑控件置灰
	void EnableEdits();

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedButtonFile();
public:
	afx_msg void OnBnClickedButtonFloder();
public:
	afx_msg void OnBnClickedButtonClear();
public:
	afx_msg void OnBnClickedButtonDelete();
public:
	afx_msg void OnBnClickedButtonUp();
public:
	afx_msg void OnBnClickedButtonDown();
public:
	afx_msg void OnBnClickedButtonTop();
public:
	afx_msg void OnBnClickedButtonBottom();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedRadioHor();
public:
	afx_msg void OnBnClickedRadioVer();
public:
	afx_msg void OnBnClickedRadioHv();
public:
	afx_msg void OnCbnSelchangeComboPage();
};
