#pragma once
#include "GsPreviewCtrl.h"
#include "resource.h"
#include "GColorButton.h"
#include "..\\CADGlobalTools\\GWndEx.h"
// GDimConfigDialog 对话框

class GDimConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(GDimConfigDialog)

public:
	GDimConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GDimConfigDialog();
	virtual BOOL OnInitDialog();
	//执行设置
	BOOL ExcSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_DIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CGsPreviewCtrl mPreviewCtrl;
	GColorButton   m_ColorButton;
	AcDbAlignedDimension* m_pDim;
	int   m_nColorIndex;
	Acad::ErrorStatus InitPreview();
	void InitDimPreview();
	void ZoomAndUpdate();
	void UpdateDim();
	void UpdateFonts();
	bool GetActiveViewPortInfo(ads_real& height, ads_real& width,
                                          AcGePoint3d& target, AcGeVector3d& viewDir,
                                          ads_real& viewTwist, bool getViewCenter);
	Acad::ErrorStatus CreateNewDim();
	void RoolbackUserSet();
	void RoolbackUserComboSet(int nId,CString strKey,CString strDefault);

	void RecordUserSet();
	void RecordUserComboSet(int nId,CString strKey);

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnCbnSelchangeComboArrow();
public:
	afx_msg void OnCbnSelchangeComboLccta();
public:
	afx_msg void OnCbnSelchangeComboTextheight();
public:
	afx_msg void OnCbnSelchangeComboDimexe();
public:
	afx_msg void OnCbnSelchangeComboDimex0();
public:
	afx_msg void OnBnClickedButtonColor();
public:
	afx_msg void OnCbnSelchangeComboXsd();
public:
	afx_msg void OnBnClickedButtonUpdatefonts();
};
