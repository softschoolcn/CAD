#pragma once

class GColorButton:public CButton
{
	DECLARE_DYNAMIC(GColorButton)
public:
	GColorButton(void);
public:
	~GColorButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	COLORREF GetFillColor(){return m_FillColor;}//获取填充框颜色
	void SetFillColor(const COLORREF color){m_FillColor=color;}//设置填充框颜色
private:
	COLORREF m_FillColor;//填充框颜色
};
