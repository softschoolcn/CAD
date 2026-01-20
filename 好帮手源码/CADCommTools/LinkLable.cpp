//LinkLable.cpp
//作者:boitboy
//个人博客:http://www.boitboy.com
//功能:动态链接控件
//最后修改时间:2012/7/7
#include "StdAfx.h"
#include "LinkLable.h"
#define TOOLTIP_ID 1
IMPLEMENT_DYNAMIC (CLinkLable, CStatic)
CLinkLable::CLinkLable(void)
{
	m_hCursor       = NULL;//光标为空值
	m_crColour      = RGB(  0,   0, 238);   // Blue    
	m_crClickedColour   = RGB(  0,   0, 238);   // Purple    
	m_crHoverColour     = RGB(  0,   0, 238);    
	m_bOverControl      = FALSE;                // Cursor not yet over control    
	m_bClicked          = FALSE;                // Hasn't been visited yet.    
	m_bUnderline        = TRUE;                 // Underline the link?    
	m_bAdjustToFit      = TRUE;                 // Resize the window to fit the text?
	m_strTip=_T("");
	m_OnClickHandle=NULL;
}

CLinkLable::~CLinkLable(void)
{
	try
	{
	m_Font.DeleteObject();
	}
	catch(...)
	{

	}
}
BEGIN_MESSAGE_MAP(CLinkLable, CStatic)   
	//{{AFX_MSG_MAP(CLinkLable)
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP    
END_MESSAGE_MAP()
void CLinkLable::PreSubclassWindow()    
{   
	// We want to get mouse clicks via STN_CLICKED    
	DWORD dwStyle = GetStyle();   
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);   

	// Create the font
	LOGFONT lf; 
	if(GetFont()!=NULL)
	{
		GetFont()->GetLogFont(&lf);
	}
	lf.lfUnderline = m_bUnderline;   
	m_Font.CreateFontIndirect(&lf);   
	SetFont(&m_Font);

	PositionWindow();        // Adjust size of window to fit URL if necessary    
	SetDefaultCursor();      // Try and load up a "hand" cursor
	CRect rect;    
	GetClientRect(rect);   
	m_ToolTip.Create(this);   
	m_ToolTip.AddTool(this, m_strTip, rect, TOOLTIP_ID);

	CStatic::PreSubclassWindow();   
}
BOOL CLinkLable::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);   
	return CStatic::PreTranslateMessage(pMsg);
}
void CLinkLable::PositionWindow()   
{   
	if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit)    
		return;   

	// Get the current window position    
	CRect rect;   
	GetWindowRect(rect);   

	CWnd* pParent = GetParent();   
	if (pParent)   
		pParent->ScreenToClient(rect);   

	// Get the size of the window text    
	CString strWndText;   
	GetWindowText(strWndText);   

	CDC* pDC = GetDC();   
	CFont* pOldFont = pDC->SelectObject(&m_Font);   
	CSize Extent = pDC->GetTextExtent(strWndText);   
	pDC->SelectObject(pOldFont);   
	ReleaseDC(pDC);   

	// Get the text justification via the window style    
	DWORD dwStyle = GetStyle();   

	// Recalc the window size and position based on the text justification    
	if (dwStyle & SS_CENTERIMAGE)   
		rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);   
	else   
		rect.bottom = rect.top + Extent.cy;   

	if (dwStyle & SS_CENTER)      
		rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);   
	else if (dwStyle & SS_RIGHT)    
		rect.left  = rect.right - Extent.cx;   
	else // SS_LEFT = 0, so we can't test for it explicitly     
		rect.right = rect.left + Extent.cx;   

	// Move the window    
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);   
}
void CLinkLable::SetDefaultCursor()   
{   
	if (m_hCursor == NULL)    
	{   
		// 获取windows路径    
		CString strWndDir;   
		GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);   
		strWndDir.ReleaseBuffer();   

		strWndDir += _T("\\winhlp32.exe");   
		// 从winhlp32.exe获取一个手型的光标   
		HMODULE hModule = LoadLibrary(strWndDir);   
		if (hModule) {   
			HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));   
			if (hHandCursor)   
				m_hCursor = CopyCursor(hHandCursor);   
		}   
		FreeLibrary(hModule);   
	}   
}
void CLinkLable::OnMouseMove(UINT nFlags, CPoint point)    
{   
	CStatic::OnMouseMove(nFlags, point);   

	if (m_bOverControl)        // Cursor is currently over control    
	{   
		CRect rect;   
		GetClientRect(rect);   

		if (!rect.PtInRect(point))   
		{   
			m_bOverControl = FALSE;   
			ReleaseCapture();   
			RedrawWindow();   
			return;   
		}
	}   
	else                      // Cursor has just moved over control    
	{   
		CRect rect;   
		GetClientRect(rect);
		if (rect.PtInRect(point)) 
		{
			m_bOverControl = TRUE;   
			RedrawWindow();   
			SetCapture();  
		}
	}   
}
BOOL CLinkLable::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)    
{   
	if (m_hCursor)   
	{   
		::SetCursor(m_hCursor);   
		return TRUE;   
	}   
	return FALSE;   
}
HBRUSH CLinkLable::CtlColor(CDC* pDC, UINT nCtlColor)    
{   
	ASSERT(nCtlColor == CTLCOLOR_STATIC);   

	if (m_bOverControl)   
		pDC->SetTextColor(m_crHoverColour);   
	else if (m_bClicked)   
		pDC->SetTextColor(m_crClickedColour);   
	else   
		pDC->SetTextColor(m_crColour);   

	// transparent text.    
	pDC->SetBkMode(TRANSPARENT);   
	return (HBRUSH)GetStockObject(NULL_BRUSH);   
}
void CLinkLable::OnClicked()   
{   
	if(m_OnClickHandle!=NULL)
	{
		try
		{
			if(!m_OnClickHandle())
			{
				return;
			}
		}
		catch(...)
		{
			return;
		}
		
	}
	SetClicked();
} 
void CLinkLable::SetClicked(BOOL bClicked)    
{    
	m_bClicked = bClicked;    

	if (::IsWindow(GetSafeHwnd()))   
		Invalidate();    
}
void CLinkLable::SetCursor(HCURSOR hCursor)   
{    
	m_hCursor = hCursor;   
	if (m_hCursor == NULL)   
		SetDefaultCursor();   
}   

HCURSOR CLinkLable::GetCursor() const   
{   
	return m_hCursor;   
}   

void CLinkLable::SetUnderline(BOOL bUnderline /* = TRUE */)   
{   
	m_bUnderline = bUnderline;   

	if (::IsWindow(GetSafeHwnd()))   
	{   
		LOGFONT lf;   
		GetFont()->GetLogFont(&lf);   
		lf.lfUnderline = m_bUnderline;   

		m_Font.DeleteObject();   
		m_Font.CreateFontIndirect(&lf);   
		SetFont(&m_Font);   

		Invalidate();    
	}   
}   

BOOL CLinkLable::GetUnderline() const   
{    
	return m_bUnderline;    
}   

void CLinkLable::SetAutoSize(BOOL bAutoSize /* = TRUE */)   
{   
	m_bAdjustToFit = bAutoSize;   

	if (::IsWindow(GetSafeHwnd()))   
		PositionWindow();   
}   

BOOL CLinkLable::GetAutoSize() const   
{    
	return m_bAdjustToFit;    
}

void CLinkLable::SetColour(COLORREF crColour)
{
	if(crColour==m_crColour)
		return;
	m_crColour=crColour;
	if (m_bOverControl||m_bClicked)
		return;
	//刷新界面显示
	if (::IsWindow(m_hWnd))   
		Invalidate();
}

COLORREF CLinkLable::GetColour() const   
{    
	return m_crColour;    
}   
void CLinkLable::SetClickColor(COLORREF crClickedColour)
{
	if(m_crClickedColour==crClickedColour)
		return;
	if (m_bOverControl)
		return;
	if(!m_bClicked)
		return;
	m_crClickedColour=crClickedColour;
	//刷新界面显示
	if (::IsWindow(m_hWnd))   
		Invalidate();
}
COLORREF CLinkLable::GetClickedColour() const   
{   
	return m_crClickedColour;    
}   
void CLinkLable::SetHoverColour(COLORREF crHoverColour)
{
	if(m_crHoverColour==crHoverColour)
	{
		return;
	}
	if(!m_bOverControl)
		return;
	m_crHoverColour=crHoverColour;
	//刷新界面显示
	if (::IsWindow(m_hWnd))   
		Invalidate();
}
COLORREF CLinkLable::GetHoverColour() const   
{   
	return m_crHoverColour;   
} 
void CLinkLable::SetTip(CString Tip)
{
	if(m_strTip.Compare(Tip)==0)
		return;
	m_strTip=Tip;
	if (::IsWindow(GetSafeHwnd())) {   
		PositionWindow();   
		m_ToolTip.UpdateTipText(m_strTip, this, TOOLTIP_ID);   
	}

}
CString CLinkLable::GetTip() const
{
	return m_strTip;
}
void CLinkLable::SetOnClickHandle(OnClickHandle m_Handle)
{
	m_OnClickHandle=m_Handle;
}
//////////////////////////////////////////////分隔符:CHyperLinkLable///////////////////////////////////////////////////
extern LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
extern HINSTANCE GotoURL(LPCTSTR url, int showcmd);
CHyperLinkLable::CHyperLinkLable()
:CLinkLable()
{
	m_crColour      = RGB(  0,   0, 238);   // Blue    
	m_crClickedColour   = RGB( 85,  26, 139);   // Purple    
	m_crHoverColour     = ::GetSysColor(COLOR_HIGHLIGHT);
}
CHyperLinkLable::~CHyperLinkLable()
{

}
void CHyperLinkLable::SetURL(CString strURL)
{
	m_strURL=strURL;
	SetTip(m_strURL);

}
CString CHyperLinkLable::GetURL() const
{
	return m_strURL;
}
void CHyperLinkLable::OnClicked()
{
	int result = (int)GotoURL(m_strURL, SW_SHOW);   
	m_bVisited = (result > HINSTANCE_ERROR);
	if(!m_bVisited)
		return;  
	SetVisited();
}
void CHyperLinkLable::SetVisited(BOOL bVisited)    
{    
	m_bVisited = bVisited;
	SetClicked(bVisited);
	if (::IsWindow(GetSafeHwnd()))   
			Invalidate(); 
}
