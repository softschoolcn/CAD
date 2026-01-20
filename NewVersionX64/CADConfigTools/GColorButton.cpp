#include "StdAfx.h"
#include "GColorButton.h"
IMPLEMENT_DYNAMIC(GColorButton, CGridCell)
GColorButton::GColorButton(void)
{
	m_FillColor=RGB(255,0,0);
}

GColorButton::~GColorButton(void)
{
}
void GColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	UINT uStyle = DFCS_BUTTONPUSH;
	// If drawing selected, add the pushed style to DrawFrameControl.
   if (lpDIS->itemState & ODS_SELECTED)
      uStyle |= DFCS_PUSHED;

   // Draw the button frame.
   ::DrawFrameControl(lpDIS->hDC, &lpDIS->rcItem, 
      DFC_BUTTON, uStyle);
	RECT rc=lpDIS->rcItem;
	int nHeight=abs(rc.top-rc.bottom);
	int nWidth=abs(rc.right-rc.left);
	nHeight/=4;

	RECT rc1=rc;
	rc1.left+=nHeight;
	rc1.right=rc1.left+3*nHeight;

	rc1.top=rc.top+nHeight;
	rc1.bottom=rc.bottom-nHeight;
	


	lpDIS->rcItem.left+=nHeight*4;
	// Get the button's text.
   CString strText;
   GetWindowText(strText);
   ::DrawText(lpDIS->hDC, strText, strText.GetLength(), 
      &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::FillRect(lpDIS->hDC,&rc1,CreateSolidBrush(m_FillColor));
}
