#pragma once

class XdDbUtils
{
public:
	//转换AcDbPolyline到AcGeCurve2d
	static Acad::ErrorStatus convertPolylineToGeCurve2d(AcDbPolyline* pPoly, AcGeCurve2d*& pGeCurve);

	//转换AcDbPolyline到AcGeCurve3d
	static Acad::ErrorStatus convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve3d*& pGeCurve);

	//转换AcGeCurve2d到AcDbPolyline
	static Acad::ErrorStatus convertGeCurveToPolyline(AcGeCurve2d* pCurve, AcDbPolyline*& pResultPoly);

	//转换AcDbCurve到AcGeCurve3d
	static Acad::ErrorStatus convertDbCurveToGeCurve(AcDbCurve *pDbCurve,AcGeCurve3d *&pGeCurve);

	// convert AcDbLine to AcGeLineSeg3d
	static AcGeLineSeg3d* LineDb2GE(AcDbLine* pDbLine);

	// convert AcDbArc to AcGeCircArc3d
	static AcGeCircArc3d* ArcDb2Ge( AcDbArc* pDbArc);

	// convert AcDbCircle to AcGeCircArc3d
	static AcGeCircArc3d* CircleDb2Ge(AcDbCircle* pDbCircle);

	// convert AcDbSpline to AcGeNurbCurve3d
	static AcGeNurbCurve3d* SplineDb2Ge(AcDbSpline* pDbSpline);

	// convert AcDbEllipse to AcGeEllipArc3d
	static AcGeEllipArc3d* EllipseDb2Ge(AcDbEllipse* pDbEllise);

	//转换AcGeCompositeCurve3d到AcDbPolyline, This routine only called by GetRegionBoundaryPolyline
	static AcDbPolyline* convertGeCurveToPolyline(AcGeCompositeCurve3d* pCurve);

	//获取Region的边界PolyLines/Circles/Ellipses/Splines, 返回环的数目
	static int GetRegionBoundaryPolyline(AcDbRegion *pRegion, AcDbVoidPtrArray& pPolylines);

	static Acad::ErrorStatus Poly2dToLWPoly(AcDb2dPolyline *pPoly,AcDbPolyline *& pLwpoly);

	static void SSToIds(ads_name ssName,AcDbObjectIdArray &EnIds);
};

