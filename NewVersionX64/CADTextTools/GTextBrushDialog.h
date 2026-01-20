#pragma once
#include "resource.h"

// GTextBrushDialog 对话框
struct GTextBrushState
{
	GTextBrushState();
	~GTextBrushState();
	bool bContent;//内容
	bool bColor;//颜色
	bool bFontHeight;//文字高度
	bool bAngle;//角度
	bool bAlign;//对齐
	bool bLayer;//图层
	bool bScale;//宽高比
	bool bStyle;//文字样式
	long getState() const;
	void setState(long nState);
private:
	void Reset();

};

class GTextBrushDialog : public CDialog
{
	DECLARE_DYNAMIC(GTextBrushDialog)

public:
	GTextBrushDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GTextBrushDialog();
	virtual BOOL OnInitDialog();
	GTextBrushState m_state;

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTBRUSH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CArray<int> m_itemIds;
	CArray<bool*> m_bools;
	void InitIdAndBools();//初始化ID和bool的对应关系
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedOk();
};
