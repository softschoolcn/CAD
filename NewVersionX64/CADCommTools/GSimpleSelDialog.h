#pragma once
#include "Resource.h"

// GSimpleSelDialog 对话框

class GSimpleSelDialog : public CDialog
{
	DECLARE_DYNAMIC(GSimpleSelDialog)

public:
	GSimpleSelDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GSimpleSelDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SIMPLESEL };
	AcDbObjectIdArray layerIdArray;
	CStringArray      layerNameArray;
	AcArray<AcRxClass*> entTypeArray;
	struct resbuf* selFilter;
	CStringArray        entTypeNameArray;
	CStringArray        entTypeAliasArray;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL ReadTypeAlias();
	CString GetTypeAlias(const CString strName) const;
	CString GetTypeName(const CString strAlias) const;
	CButton   m_checkAllLayer;
	CButton   m_checkAllType;
	CButton   m_btnAllLayer;
	CButton   m_btnAllType;
	CButton   m_btnAllLayerNot;
	CButton   m_btnAllTypeNot;
	CButton   m_btnLayerFX;
	CButton   m_btnTypeFX;
	CListCtrl m_listLayer;
	CListCtrl m_listType;
	CButton   m_radioSel;
	CButton   m_radioFX;
	BOOL BuildSelFilter();
	void SaveLastSelect();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLayerall();
public:
	afx_msg void OnBnClickedButtonLayerallnot();
public:
	afx_msg void OnBnClickedButtonLayerfx();
public:
	afx_msg void OnBnClickedButtonTypeall();
public:
	afx_msg void OnBnClickedButtonTypeallnot();
public:
	afx_msg void OnBnClickedButtonTypefx();
public:
	afx_msg void OnBnClickedCheckAlllayer();
public:
	afx_msg void OnBnClickedCheckAlltype();
public:
	afx_msg void OnBnClickedOk();
};
