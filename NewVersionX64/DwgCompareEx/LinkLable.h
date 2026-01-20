//LinkLable.h
//作者:boitboy
//个人博客:http://www.boitboy.com
//功能:动态链接控件
//最后修改时间:2012/7/7
#pragma once
#include "afxwin.h"
typedef bool (*OnClickHandle)();
class CLinkLable :
	public CStatic
{
	DECLARE_DYNAMIC(CLinkLable)
public:
	CLinkLable(void);
	~CLinkLable(void);
public:
	void SetCursor(HCURSOR hCursor);//设置光标 
	HCURSOR GetCursor() const; //获取光标

	void SetUnderline(BOOL bUnderline = TRUE);//设置是否使用下划线 
	BOOL GetUnderline() const; //获取是否使用下划线

	void SetAutoSize(BOOL bAutoSize = TRUE); //设置是否根据调整控件大小以适应文字
	BOOL GetAutoSize() const;//获取是否根据调整控件大小以适应文字

	void SetColour(COLORREF crColour);//设置控件普通情况下的颜色
	COLORREF GetColour() const;//获取控件普通情况下的颜色

	void SetClickColor(COLORREF crClickedColour);//设置控件被点击过后的颜色
	COLORREF GetClickedColour() const;//获取控件被点击过后的颜色

	void SetHoverColour(COLORREF crHoverColour);//设置鼠标晃过控件时的颜色
	COLORREF GetHoverColour() const;//获取鼠标晃过控件时的颜色

	void SetTip(CString Tip);//设置提示文字
	CString GetTip() const;////获取提示文字

	void SetOnClickHandle(OnClickHandle m_Handle);
public: 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected: 
	//方法
	virtual void PreSubclassWindow();
	void PositionWindow(); 
	void SetDefaultCursor();
	void SetClicked(BOOL bClicked=TRUE);
protected:
	//变量
	CFont    m_Font;//字体
	BOOL     m_bUnderline;//下划线
	BOOL     m_bAdjustToFit;//调整至适应文字大小
	HCURSOR  m_hCursor;//光标
	BOOL     m_bOverControl;//鼠标晃过控件
	COLORREF m_crColour;//控件普通状态下的颜色
	COLORREF m_crClickedColour;//按过一次后控件的颜色
	COLORREF m_crHoverColour;//鼠标晃过控件的时候控件的颜色
	BOOL     m_bClicked;//是否被点击过了
	CToolTipCtrl m_ToolTip;//ToolTip控件
	CString m_strTip;//提示
	OnClickHandle m_OnClickHandle;//被按下执行函数
protected:
	//消息处理
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg virtual void OnClicked();
	DECLARE_MESSAGE_MAP()
};
class CHyperLinkLable:
	public CLinkLable
{
public:
	CHyperLinkLable();
	~CHyperLinkLable();
public:
	void SetURL(CString strURL); 
	CString GetURL() const;
protected:
	CString  m_strURL;//网页地址
	BOOL     m_bVisited;
	virtual void OnClicked();
	void SetVisited(BOOL bVisited=TRUE);
private:
	CLinkLable::SetTip;//方法在此类里被禁用
	CLinkLable::GetTip;//方法在此类里被禁用
	CLinkLable::SetOnClickHandle;//方法在此类里被禁用

};
