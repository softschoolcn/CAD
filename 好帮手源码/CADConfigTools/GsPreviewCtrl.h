//
// (C) Copyright 2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

#if !defined(AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_)
#define AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GsPreviewCtrl.h : header file
//
#include "acgi.h"
#include "AcGsManager.h"
#include <math.h>
#include "gs.h"
// Simple lighweight drawable to do orbit glyph
//
/////////////////////////////////////////////////////////////////////////////
// CGsPreviewCtrl window

class CGsPreviewCtrl : public CStatic
{

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGsPreviewCtrl)
	//}}AFX_VIRTUAL


public:
    CGsPreviewCtrl() : CStatic(), 
                   mpManager(NULL), mpFactory(NULL), mpDevice(NULL), mpView(NULL), 
                   mpModel(NULL),mbModelCreated(false),mbPanning(false),mbOrbiting(false) {} 
    
    virtual ~CGsPreviewCtrl() {clearAll();}
    void init(HMODULE hRes, bool bCreateModel = true);
    void erasePreview();
    void clearAll();

    AcGsView*   view()      { return mpView; }
    AcGsDevice* device()    { return mpDevice; }
    AcGsModel*  model()     { return mpModel; }
    void setModel(AcGsModel* pModel);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGsPreviewCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	HCURSOR mhSavedCursor;
	HCURSOR mhPanCursor;
    HCURSOR mhCrossCursor;
    HCURSOR mhOrbitCursor;
    AcGsManager         *mpManager;
#if _MSC_VER<1700
    AcGsClassFactory    *mpFactory;
#else
	AcGsGraphicsKernel    *mpFactory;
#endif
    AcGsDevice          *mpDevice;
    AcGsView            *mpView;
    AcGsModel           *mpModel;
    AcGsModel           *mpGhostModel;
    bool mbModelCreated;
    bool mbPanning;
    bool mbOrbiting;
    CPoint mStartPt;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_)
