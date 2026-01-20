#pragma once

#include "resource.h"
#include "GFindExt.h"
#include "GFindParam.h"
#include "GWndEx.h"
#include "GFindTypes.h"
#include "GFindResultItem.h"
#if _MSC_VER<=1300
   #ifndef ACAD_2004
      #include "dbtable.h"
   #endif
#endif
// GFindDialog 对话框

class GFindDialog : public CAcUiDialog
{
	DECLARE_DYNAMIC(GFindDialog)

public:
	GFindDialog(CWnd* pParent = NULL,HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~GFindDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIND };
	virtual BOOL OnInitDialog();
	GFindExt            m_ext;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CGridCtrl mGrid;  //属性表格
	CAcUiSelectButton	m_btnEnt;//选择
	CComboBox           m_comboExt;//范围
	GFindParam          m_findParam;
	GFindTypes          m_types;
	CArray<GFindResultItem> m_resultItems;
	CStringArray        entTypeNameArray;
	CStringArray        entTypeAliasArray;
	void FindInFloder(CString strPath);
	void Find(CString strFile);
	void Find(CString strFile,AcDbDatabase* pDb);
	void Find(CString strFile,AcDbBlockTableRecord* pBTR);
	void Find(CString strFile,AcDbObjectIdArray& ids);
	void Find(CString strFile,AcDbObjectId id);
	#ifndef ACAD_2004
	void Find(CString strFile,AcDbTable* pTable);
#endif
	BOOL ReadTypeAlias();
	CString GetTypeAlias(const CString strName) const;
	CString GetTypeName(const CString strAlias) const;
	AcDbObjectIdArray m_blockIds;
	void UpdateGrid();
	/*!
     @brief 清空结果表格
     */
	void ClearGrid();

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnBnClickedButtonFindtype();
public:
	afx_msg void OnBnClickedButtonSelectent();
public:
	afx_msg void OnBnClickedButtonFloder();
public:
	afx_msg void OnBnClickedButtonFile();
public:
	afx_msg void OnCbnSelchangeComboExt();
public:
	afx_msg void OnBnClickedButtonFind();
public:
	afx_msg void OnBnClickedCheckFullmatch();
public:
	afx_msg void OnBnClickedCheckTpf();
public:
	afx_msg void OnBnClickedButtonHelp();
};
