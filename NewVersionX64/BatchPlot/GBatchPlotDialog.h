#pragma once

#include "resource.h"
// GBatchPlotDialog 对话框
#include "GPlotDefine.h"
#include "GWndEx.h"
class GBatchPlotDialog : public CAcUiDialog
{
	DECLARE_DYNAMIC(GBatchPlotDialog)

public:
	GBatchPlotDialog(CWnd* pParent = NULL,HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~GBatchPlotDialog();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_BATCHPLOT };
	CString m_strDevice;
	CArray<GPlotItemEx> m_plotItemExs;
	bool m_bPlotToFile;//打印到文件
	CString m_strPath;//保存文件路径
	CString m_strExt;
	bool m_bJoinPDF;//合并PDF
	bool m_bDelMidPdf;//删除中间PDF
	bool m_bAllow90;//允许旋转90度

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CGridCtrl mGrid;  //属性表格
	CArray<GPlotItem> m_plotItems;
	GFrameDefine m_frame;
	void InitDevice();
	void ChangeType();
	/*!
     @brief 清空结果表格
     */
	void ClearGrid();
	/*!
     @brief 更新表格显示
     */
    void UpdateGrid();
	//更新图框定义
	void UpdateFrameDefine();
	//更新图框
	bool UpdateFrames(bool bAlert);
	//从文件目录中获取所有需要打印的项
	void GetPlotItems(CString strPath);
	//从文件中获取所有需要打印的项
	void GetPlotItemsFromFile(CString strFile);
	void GetPlotItemsFromFile(CString strFile,AcDbDatabase* pDb);
	void CheckPlotFile();
	bool SaveExt();
	bool ReadExt();

	DECLARE_MESSAGE_MAP()
	CWNDEX_MEMBERS()
public:
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	afx_msg void OnBnClickedButtonRefursh();
public:
	afx_msg void OnCbnSelchangeComboPloter();
public:
	afx_msg void OnBnClickedButtonBrowserfloder();
public:
	afx_msg void OnBnClickedCheckUnisize();
public:
	afx_msg void OnBnClickedCheckUnistyle();
public:
	afx_msg void OnBnClickedCheckUninum();
public:
	afx_msg void OnBnClickedCheckPlotfile();
public:
	afx_msg void OnBnClickedButtonPath();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedRadioBlockref();
public:
	afx_msg void OnBnClickedRadioBhx();
public:
	afx_msg void OnBnClickedRadioLrtb();
public:
	afx_msg void OnBnClickedRadioTblr();
public:
	afx_msg void OnBnClickedButtonSelent();
};
