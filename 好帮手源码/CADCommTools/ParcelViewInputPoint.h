

#if !defined(ARX__DYNVIEWINPUTPOINT_H__20041110_131242)
#define ARX__DYNVIEWINPUTPOINT_H__20041110_131242

#if _MSC_VER > 1000
#pragma once
#endif 

#include "acedinpt.h"
#include "acdocman.h"
// 动态显示HYDYNICVIEW扩展数据的信息
class AsdkDynViewInputPoint : public AcEdInputPointMonitor
{
public:

	// Constructor / Destructor
	AsdkDynViewInputPoint();
	virtual ~AsdkDynViewInputPoint();

	//{{AFX_ARX_METHODS(AsdkDynViewInputPoint)
	virtual bool excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity,
		int gsSelectionMark);
	//}}AFX_ARX_METHODS
	#if _MSC_VER<1500
    Acad::ErrorStatus monitorInputPoint(bool& appendToTooltipStr,
		TCHAR*& additionalTooltipString,
		AcGiViewportDraw* drawContext,
		AcApDocument* document,
		bool pointComputed,
		int history,
		const AcGePoint3d& lastPoint,
		const AcGePoint3d& rawPoint,
		const AcGePoint3d& grippedPoint,
		const AcGePoint3d& cartesianSnappedPoint,
		const AcGePoint3d& osnappedPoint,
		AcDb::OsnapMask osnapMask,
		const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
		AcDb::OsnapMask osnapOverrides,
		const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
		const AcArray<AcDbObjectId>& apertureEntities,
		const AcArray< AcDbObjectIdArray,
		AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedApertureEntities,
		const AcArray<int>& gsSelectionMark,
		const AcArray<AcDbObjectId>& keyPointEntities,
		const AcArray< AcDbObjectIdArray,
		AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedKeyPointEntities,
		const AcArray<int>& keyPointGsSelectionMark,
		const AcArray<AcGeCurve3d*>& alignmentPaths,
		const AcGePoint3d& computedPoint,
		const TCHAR* tooltipString);
#else
	virtual Acad::ErrorStatus monitorInputPoint(const AcEdInputPoint& input, AcEdInputPointMonitorResult& output);
#endif
private:
	AcDbObjectIdArray g_HighObjIds;
	AcApDocument* m_pDoc;
	CString strTip;
	CStringArray        entTypeNameArray;
	CStringArray        entTypeAliasArray;
	CString GetTypeAlias(const CString strName) const;
};

#endif 