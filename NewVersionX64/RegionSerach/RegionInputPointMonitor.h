// (C) Copyright 2001 by Autodesk, Inc. 
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
//////////////////////////////////////////////////////////////////////////////
//
// AcEdInputPointMonitor

#if !defined(ARX__ASDKIPM_H__19990505_132529)
#define ARX__ASDKIPM_H__19990505_132529

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "acedinpt.h"
#include "acdocman.h"

//将实体添加到数据库中
AcDbObjectId AddEntToDWGDatabase(AcDbEntity *pEnt);

class RegionInputPointMonitor : public AcEdInputPointMonitor
{
public:

	// Constructor / Destructor
	RegionInputPointMonitor(const bool storeInDocVars = true, AcApDocument* pDoc = curDoc());
	virtual ~RegionInputPointMonitor();

	virtual bool excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity,
	                                         int gsSelectionMark);
	// monitorInputPoint
    //
    // This callback is under the exact same conditions as AcEdInputPointFilter,
    // except that the point filter is invoked first, and can affect the
    // "computed point".
    //
    // The callback is made after the AutoCAD user input complex has
    // fully processed the point except for XYZ coordinate filtering,
    // AND after an app-defined input point filter has processed the point,
    // if one is registered.  After invoking all registered instances of
    // AcEdInputPointMonitor, AutoCAD will either
    // return the point to a caller, or accept the coordinate(s) being
    // prompted for and return for another point if XYZ coordinage filtering
    // is happening.
    //
    // Output Values
    //
    // Unlike AcEdInpoutPointFilter, this function cannot affect the point or
    // influence the prompting, and therefore has none of the Output Value
    // parameters of AcEdInputPointFilter.  However, see graphics output, which
    // is the same for monitors as for filters.
    //
    // Graphics Input/Output.
    //
    // Same as AcEdInputPointFilter
    //
    // Input Values
    //
    // Same as AcEdInputPointFilter, except where noted.
    //
    // Return Status:  Acad::eOk should always be returned.
    //
    virtual Acad::ErrorStatus monitorInputPoint(
 
          // Output
          // If changedTooltipStr is kTrue
          // then newTooltipString has the new tooltip string in it.
          //
          bool&                             appendToTooltipStr,
          TCHAR*&                            additionalTooltipString,

          // Input/Output
          //
          AcGiViewportDraw*                   drawContext,

          // Input parameters:
          //
          AcApDocument*                       document,
          bool                                pointComputed,
          int                                 history,
          const AcGePoint3d&                  lastPoint,
          const AcGePoint3d&                  rawPoint,
          const AcGePoint3d&                  grippedPoint,
          const AcGePoint3d&                  cartesianSnappedPoint,
          const AcGePoint3d&                  osnappedPoint,
          AcDb::OsnapMask                     osnapMask,
          const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
          AcDb::OsnapMask                     osnapOverrides,
          const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
          const AcArray<AcDbObjectId>&        apertureEntities,
          const AcArray< AcDbObjectIdArray, AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& 
                                              nestedApertureEntities,
          const AcArray<int>&                 gsSelectionMark,
          const AcArray<AcDbObjectId>&        keyPointEntities,
          const AcArray< AcDbObjectIdArray, AcArrayObjectCopyReallocator< AcDbObjectIdArray > >&
                                              nestedKeyPointEntities,
          const AcArray<int>&                 keyPointGsSelectionMark,
          const AcArray<AcGeCurve3d*>&        alignmentPaths,
          const AcGePoint3d&                  computedPoint,
          const TCHAR*                         tooltipString);


private:
	// Pointer to the document this instance belongs to.
	AcApDocument* m_pDoc;

	// 记录临时生成的闭合曲线
	AcDbObjectIdArray m_objIdArray;

	// 保存上一次鼠标位置所在的面域
	AcDbRegion* m_pOldHilightReg;

	// 保存当前鼠标位置所在的面域
	AcDbRegion* m_pHilightReg;

	// 所有需要参与计算的面域
	AcDbVoidPtrArray m_RegionArray;

public:
	// 设置所有需要参与计算的面域
	void SetRegionArray(AcDbVoidPtrArray& regionArray) 
	{
		m_RegionArray.setLogicalLength(0); 
		m_RegionArray.append(regionArray); 
	}

	// 获取当前鼠标位置所在的面域
	AcDbRegion* GetHilightReg() 
	{
		return m_pHilightReg; 
	}
};



#endif // !defined(ARX__ASDKIPM_H__19990505_132529)