// GridCellCheck.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// 23 Jul 2001 - Complete rewrite
// 13 Mar 2004 - GetCellExtent and GetCheckPlacement fixed by Yogurt
//             - Read-only now honoured - Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "GridCellColor.h"
const COLORREF IndexedColors[] = {
	RGB(0, 0, 0),
	RGB(255, 0, 0),
	RGB(255, 255, 0),
	RGB(0, 255, 0),
	RGB(0, 255, 255),
	RGB(0, 0, 255),
	RGB(255, 0, 255),
	RGB(255, 255, 255),
	RGB(65, 65, 65),
	RGB(128, 128, 128),
	RGB(255, 0, 0),
	RGB(255, 170, 170),
	RGB(189, 0, 0),
	RGB(189, 126, 126),
	RGB(129, 0, 0),
	RGB(129, 86, 86),
	RGB(104, 0, 0),
	RGB(104, 69, 69),
	RGB(79, 0, 0),
	RGB(79, 53, 53),
	RGB(255, 63, 0),
	RGB(255, 191, 170),
	RGB(189, 46, 0),
	RGB(189, 141, 126),
	RGB(129, 31, 0),
	RGB(129, 96, 86),
	RGB(104, 25, 0),
	RGB(104, 78, 69),
	RGB(79, 19, 0),
	RGB(79, 59, 53),
	RGB(255, 127, 0),
	RGB(255, 212, 170),
	RGB(189, 94, 0),
	RGB(189, 157, 126),
	RGB(129, 64, 0),
	RGB(129, 107, 86),
	RGB(104, 52, 0),
	RGB(104, 86, 69),
	RGB(79, 39, 0),
	RGB(79, 66, 53),
	RGB(255, 191, 0),
	RGB(255, 234, 170),
	RGB(189, 141, 0),
	RGB(189, 173, 126),
	RGB(129, 96, 0),
	RGB(129, 118, 86),
	RGB(104, 78, 0),
	RGB(104, 95, 69),
	RGB(79, 59, 0),
	RGB(79, 73, 53),
	RGB(255, 255, 0),
	RGB(255, 255, 170),
	RGB(189, 189, 0),
	RGB(189, 189, 126),
	RGB(129, 129, 0),
	RGB(129, 129, 86),
	RGB(104, 104, 0),
	RGB(104, 104, 69),
	RGB(79, 79, 0),
	RGB(79, 79, 53),
	RGB(191, 255, 0),
	RGB(234, 255, 170),
	RGB(141, 189, 0),
	RGB(173, 189, 126),
	RGB(96, 129, 0),
	RGB(118, 129, 86),
	RGB(78, 104, 0),
	RGB(95, 104, 69),
	RGB(59, 79, 0),
	RGB(73, 79, 53),
	RGB(127, 255, 0),
	RGB(212, 255, 170),
	RGB(94, 189, 0),
	RGB(157, 189, 126),
	RGB(64, 129, 0),
	RGB(107, 129, 86),
	RGB(52, 104, 0),
	RGB(86, 104, 69),
	RGB(39, 79, 0),
	RGB(66, 79, 53),
	RGB(63, 255, 0),
	RGB(191, 255, 170),
	RGB(46, 189, 0),
	RGB(141, 189, 126),
	RGB(31, 129, 0),
	RGB(96, 129, 86),
	RGB(25, 104, 0),
	RGB(78, 104, 69),
	RGB(19, 79, 0),
	RGB(59, 79, 53),
	RGB(0, 255, 0),
	RGB(170, 255, 170),
	RGB(0, 189, 0),
	RGB(126, 189, 126),
	RGB(0, 129, 0),
	RGB(86, 129, 86),
	RGB(0, 104, 0),
	RGB(69, 104, 69),
	RGB(0, 79, 0),
	RGB(53, 79, 53),
	RGB(0, 255, 63),
	RGB(170, 255, 191),
	RGB(0, 189, 46),
	RGB(126, 189, 141),
	RGB(0, 129, 31),
	RGB(86, 129, 96),
	RGB(0, 104, 25),
	RGB(69, 104, 78),
	RGB(0, 79, 19),
	RGB(53, 79, 59),
	RGB(0, 255, 127),
	RGB(170, 255, 212),
	RGB(0, 189, 94),
	RGB(126, 189, 157),
	RGB(0, 129, 64),
	RGB(86, 129, 107),
	RGB(0, 104, 52),
	RGB(69, 104, 86),
	RGB(0, 79, 39),
	RGB(53, 79, 66),
	RGB(0, 255, 191),
	RGB(170, 255, 234),
	RGB(0, 189, 141),
	RGB(126, 189, 173),
	RGB(0, 129, 96),
	RGB(86, 129, 118),
	RGB(0, 104, 78),
	RGB(69, 104, 95),
	RGB(0, 79, 59),
	RGB(53, 79, 73),
	RGB(0, 255, 255),
	RGB(170, 255, 255),
	RGB(0, 189, 189),
	RGB(126, 189, 189),
	RGB(0, 129, 129),
	RGB(86, 129, 129),
	RGB(0, 104, 104),
	RGB(69, 104, 104),
	RGB(0, 79, 79),
	RGB(53, 79, 79),
	RGB(0, 191, 255),
	RGB(170, 234, 255),
	RGB(0, 141, 189),
	RGB(126, 173, 189),
	RGB(0, 96, 129),
	RGB(86, 118, 129),
	RGB(0, 78, 104),
	RGB(69, 95, 104),
	RGB(0, 59, 79),
	RGB(53, 73, 79),
	RGB(0, 127, 255),
	RGB(170, 212, 255),
	RGB(0, 94, 189),
	RGB(126, 157, 189),
	RGB(0, 64, 129),
	RGB(86, 107, 129),
	RGB(0, 52, 104),
	RGB(69, 86, 104),
	RGB(0, 39, 79),
	RGB(53, 66, 79),
	RGB(0, 63, 255),
	RGB(170, 191, 255),
	RGB(0, 46, 189),
	RGB(126, 141, 189),
	RGB(0, 31, 129),
	RGB(86, 96, 129),
	RGB(0, 25, 104),
	RGB(69, 78, 104),
	RGB(0, 19, 79),
	RGB(53, 59, 79),
	RGB(0, 0, 255),
	RGB(170, 170, 255),
	RGB(0, 0, 189),
	RGB(126, 126, 189),
	RGB(0, 0, 129),
	RGB(86, 86, 129),
	RGB(0, 0, 104),
	RGB(69, 69, 104),
	RGB(0, 0, 79),
	RGB(53, 53, 79),
	RGB(63, 0, 255),
	RGB(191, 170, 255),
	RGB(46, 0, 189),
	RGB(141, 126, 189),
	RGB(31, 0, 129),
	RGB(96, 86, 129),
	RGB(25, 0, 104),
	RGB(78, 69, 104),
	RGB(19, 0, 79),
	RGB(59, 53, 79),
	RGB(127, 0, 255),
	RGB(212, 170, 255),
	RGB(94, 0, 189),
	RGB(157, 126, 189),
	RGB(64, 0, 129),
	RGB(107, 86, 129),
	RGB(52, 0, 104),
	RGB(86, 69, 104),
	RGB(39, 0, 79),
	RGB(66, 53, 79),
	RGB(191, 0, 255),
	RGB(234, 170, 255),
	RGB(141, 0, 189),
	RGB(173, 126, 189),
	RGB(96, 0, 129),
	RGB(118, 86, 129),
	RGB(78, 0, 104),
	RGB(95, 69, 104),
	RGB(59, 0, 79),
	RGB(73, 53, 79),
	RGB(255, 0, 255),
	RGB(255, 170, 255),
	RGB(189, 0, 189),
	RGB(189, 126, 189),
	RGB(129, 0, 129),
	RGB(129, 86, 129),
	RGB(104, 0, 104),
	RGB(104, 69, 104),
	RGB(79, 0, 79),
	RGB(79, 53, 79),
	RGB(255, 0, 191),
	RGB(255, 170, 234),
	RGB(189, 0, 141),
	RGB(189, 126, 173),
	RGB(129, 0, 96),
	RGB(129, 86, 118),
	RGB(104, 0, 78),
	RGB(104, 69, 95),
	RGB(79, 0, 59),
	RGB(79, 53, 73),
	RGB(255, 0, 127),
	RGB(255, 170, 212),
	RGB(189, 0, 94),
	RGB(189, 126, 157),
	RGB(129, 0, 64),
	RGB(129, 86, 107),
	RGB(104, 0, 52),
	RGB(104, 69, 86),
	RGB(79, 0, 39),
	RGB(79, 53, 66),
	RGB(255, 0, 63),
	RGB(255, 170, 191),
	RGB(189, 0, 46),
	RGB(189, 126, 141),
	RGB(129, 0, 31),
	RGB(129, 86, 96),
	RGB(104, 0, 25),
	RGB(104, 69, 78),
	RGB(79, 0, 19),
	RGB(79, 53, 59),
	RGB(51, 51, 51),
	RGB(80, 80, 80),
	RGB(105, 105, 105),
	RGB(130, 130, 130),
	RGB(190, 190, 190),
	RGB(255, 255, 255)
};
CString GetIndexColorName(int nColorIndex)
{
	if(nColorIndex==0)
	{
		return _T("ByLayer");
	}
	else if(nColorIndex==1)
	{
		return _T("ºì");
	}
	else if(nColorIndex==2)
	{
		return _T("»Æ");
	}
	else if(nColorIndex==3)
	{
		return _T("ÂÌ");
	}
	else if(nColorIndex==4)
	{
		return _T("Çà");
	}
	else if(nColorIndex==5)
	{
		return _T("À¶");
	}
	else if(nColorIndex==6)
	{
		return _T("×Ï");
	}
	else if(nColorIndex==7)
	{
		return _T("°×");
	}
	else if(nColorIndex==256)
	{
		return _T("ByBlock");
	}
	else if(nColorIndex>=8&&nColorIndex<=255)
	{
		CString strColor;
		strColor.Format(_T("%d"),nColorIndex);
		return strColor;
	}
	else
	{
		return _T("°×");
	}
}
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellColor, CGridCell)

CGridCellColor::CGridCellColor() : CGridCell()
{
    //m_Rect.IsRectNull();
}

CSize CGridCellColor::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellColor::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;
    }
    return bResult;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellColor::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    CRect CheckRect = GetCheckPlacement();
    rect.left = CheckRect.right;

    // enough room to draw?
    // if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

    // Do the draw 
    //pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
	//    (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);
	LPARAM lp=GetGrid()->GetItemData(nRow,nCol);
	int nColorIndex=7;
	if(lp!=NULL)
	{
		nColorIndex=(int)lp;
	}
	if(nColorIndex<1||nColorIndex>255)
	{
		nColorIndex=7;
	}
	CBrush brush(IndexedColors[nColorIndex]);
	pDC->FillRect(GetCheckPlacement(),&brush);

    // }
#endif
    return bResult;
}

void CGridCellColor::OnClick(CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);	
    //if (!GetGrid()->IsCellEditable(cell))		
    //    return;
	LPARAM lp=GetGrid()->GetItemData(cell.row,cell.col);
	int nColorIndex=7;
	if(lp!=NULL)
	{
		nColorIndex=(int)lp;
	}
	if(acedSetColorDialog(nColorIndex,false,256)!=Adesk::kTrue)
	{
		return;
	}
	GetGrid()->SetItemData(cell.row,cell.col,(LPARAM)nColorIndex);
	GetGrid()->SetItemText(cell.row,cell.col,GetIndexColorName(nColorIndex));
	GetGrid()->InvalidateRect(m_Rect);
	//EndEdit();
	// Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = NULL;
    dispinfo.hdr.idFrom   = 0;
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = cell.row;
    dispinfo.item.col     = cell.col;
    dispinfo.item.strText  = GetIndexColorName(nColorIndex);
    dispinfo.item.lParam  = (LPARAM) NULL;
	CWnd *pOwner = GetGrid()->GetOwner();
	if(pOwner!=NULL)
	pOwner->SendMessage(WM_NOTIFY, 0, (LPARAM)&dispinfo );
	//GetGrid()->SendMessageToParent(cell.row,cell.col,GVN_ENDLABELEDIT);
        

}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellColor::GetCheckPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect + CSize(GetMargin(), GetMargin());
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	/* for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    */

    if (m_Rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = m_Rect.top + (m_Rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}