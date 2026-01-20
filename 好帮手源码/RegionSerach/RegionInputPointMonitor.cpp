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

#include "StdAfx.h"

#include "XdDbUtils.h"



AcDbVoidPtrArray m_RegionArray;
AcDbRegion *m_pHilightReg = NULL;

BOOL PointInRegion(AcDbRegion *pRegion, AcGePoint3d Pt) 
{ 
	if (pRegion == NULL) 
		return FALSE; 

	AcBrBrep brep; 
	if (brep.set(*pRegion) != AcBr::eOk) 
		return FALSE; 

	AcBr::Relation relation; 
	AcBr::ErrorStatus esbr = brep.getPointRelationToBrep(Pt, relation); 
	if (esbr != AcBr::eOk) 
		return FALSE; 

	if ((relation == AcBr::kBoundary) || 
		(relation == AcBr::kInside))
	{
		return TRUE; 
	}

	return FALSE;
}


//将实体添加到数据库中
AcDbObjectId AddEntToDWGDatabase(AcDbEntity *pEnt)
{
	Acad::ErrorStatus es;
	AcDbBlockTable *pBlockTable;
	es = acdbCurDwg()->getSymbolTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	es = pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);
	pBlockTable->close();

	if (es != eOk)
		return AcDbObjectId::kNull;

	AcDbObjectId ObjId = NULL;
	es = pBlockTableRecord->appendAcDbEntity(ObjId,pEnt);

	pBlockTableRecord->close();

	return ObjId;
}

RegionInputPointMonitor::RegionInputPointMonitor(const bool storeInDocVars, AcApDocument* pDoc)
{
	if (storeInDocVars) {

		m_pDoc = pDoc;
		m_pDoc->inputPointManager()->addPointMonitor(this);
	} 
	else
		m_pDoc = NULL;

	m_pHilightReg = NULL;
	m_pOldHilightReg = NULL;
}

RegionInputPointMonitor::~RegionInputPointMonitor()
{
	if (m_pDoc)
		m_pDoc->inputPointManager()->removePointMonitor(this);

	if (m_objIdArray.length() > 0)
	{
		for(int i=0; i<m_objIdArray.logicalLength(); i++)
		{
			AcDbObjectId objId = m_objIdArray.at(i);
			AcDbEntity * pEnt;

			Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);
			if (es == Acad::eOk)
			{
				pEnt->erase();
				pEnt->close();
			}
		}
	}
}

bool
RegionInputPointMonitor::excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity,
	                                   int gsSelectionMark)
{
  return true;
}
Acad::ErrorStatus RegionInputPointMonitor::monitorInputPoint(
 
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
          const TCHAR*                         tooltipString)
	 {
		 m_pHilightReg = NULL;

	Acad::ErrorStatus es;

	if (pointComputed && m_RegionArray.length() > 0)
	{
		for (int i=0; i<m_RegionArray.logicalLength();i++) 
		{
			AcDbRegion *ppReg = (AcDbRegion*)m_RegionArray.at(i);

			if (PointInRegion(ppReg, rawPoint))
			{
				m_pHilightReg = ppReg;
				break;
			}
		}

		if (NULL == m_pHilightReg)
			m_pHilightReg = (AcDbRegion*)m_RegionArray.at(m_RegionArray.logicalLength()-1);

		if(m_pHilightReg->isNull())
		{
			return Acad::eOk;
		}

		if(m_pHilightReg != m_pOldHilightReg)
		{
			if (m_objIdArray.length() > 0)
			{
				for(int i=0; i<m_objIdArray.logicalLength(); i++)
				{
					AcDbObjectId objId = m_objIdArray.at(i);
					AcDbEntity * pEnt;

					es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);
					if (es == Acad::eOk)
					{
						pEnt->erase();
						pEnt->close();
					}
				}
			}

			AcDbVoidPtrArray pPolylines;

			XdDbUtils::GetRegionBoundaryPolyline(m_pHilightReg, pPolylines);

			for (int i=0; i<pPolylines.logicalLength(); i++) 
			{
				AcDbEntity* pEnt = (AcDbEntity*)pPolylines.at(i);
				pEnt->setColorIndex(1);

				AcDbObjectId objId = AddEntToDWGDatabase(pEnt);

				pEnt->close();

				m_objIdArray.append(objId);

				AcDbHatch* pHatch = new AcDbHatch();

				// Set hatch plane
				AcGeVector3d normal(0.0, 0.0, 1.0);
				pHatch->setNormal(normal);
				pHatch->setElevation(0.0);

				pHatch->setAssociative(Adesk::kTrue);
				pHatch->setPattern(AcDbHatch::kPreDefined,_T("SOLID"));
				pHatch->setHatchStyle(AcDbHatch::kNormal);//kOuter
				pHatch->setColorIndex(4);

				AcDbObjectIdArray dbObjIds;
				dbObjIds.append(objId);
				pHatch->appendLoop(AcDbHatch::kDefault, dbObjIds);

				// Elaborate hatch lines
				pHatch->evaluateHatch();

				// Post hatch entity to database
				AcDbObjectId objIdHatch = AddEntToDWGDatabase(pHatch);
				pHatch->close();

				m_objIdArray.append(objIdHatch);
			}

			m_pOldHilightReg = m_pHilightReg;
		}
	}

	return Acad::eOk;
	 }

