#include "stdafx.h"



BOOL GetAllLine(const CString &strLayer,AcDbObjectIdArray& dbObjIdArr);
void CopyAllLine(AcDbObjectIdArray& dbObjIdArr,AcDbObjectIdArray &arrObjIdOut, CString strLayer);

//创建直线
void CreateFromPoints(AcDbObject*pdbObj,
					  AcDbObjectIdArray &dbIdArray,
					  AcGePoint3dArray &gePoi3dArr,
					  AcDbObjectIdArray &dbIdRemove);

AcDbObjectId CreateDistLine(AcGePoint3d &gePoi3d,
							AcGePoint3d&gePoi3d1);

//去除所有的端点
void RemoveAllVertPoint(AcDbObject*pdbObj,AcGePoint3dArray&insertPoints);

void CreateCrossLine(AcDbObjectIdArray&dbObjIdArrOut,
					 AcDbObjectIdArray&dbObjIdArrOutNew);

//创建临时的线。
void CreateTempLine(AcDbObject*pdbObj,
					AcDbObjectIdArray& entity,//in
					AcDbObjectIdArray &dbIdArray,//out
					AcDbObjectIdArray &dbIdRemove);
	
void SortByDistance(AcGePoint3d &stPt,
					AcGePoint3dArray&gePoi3dArr);//,
					//AcGePoint3dArray&gePoi3dArrSort);


void Bubble(AcGePoint3d &stPt,
			AcGePoint3dArray &gePoi3dArr,
			int n);

void CreateArcForPoints(AcDbObject*pdbObj,
						AcDbObjectIdArray &dbIdArray,
						AcGePoint3dArray &gePoi3dArr,
						AcDbObjectIdArray &dbIdRemove);
BOOL GetArcStAndEdPt(AcDbArc*pdbArc,AcGePoint3d &geStPt,AcGePoint3d &geEdPt);

void BubbleByAngle(double dStAngle,
				   AcArray<double> &AngArr,
				   int i);

void SortByAngle(double dStAngle,AcArray<double> &AngleArr
				 );

void RemoveEntity(AcDbObjectIdArray&dbObjIdArrOutNew);


void Check2dPoint(AcDbObject *pDbO,AcDbDatabase *pDbDatab=acdbHostApplicationServices()->workingDatabase());


CString GetCurrentLayerName();