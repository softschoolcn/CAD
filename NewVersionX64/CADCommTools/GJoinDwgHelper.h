#pragma once
#include "GJoinDwgDialog.h"
class GJoinDwgHelper
{
public:
	GJoinDwgHelper(void);
public:
	~GJoinDwgHelper(void);
public:
	Acad::ErrorStatus Join();
	Acad::ErrorStatus Join(AcDbDatabase* pTargetDb,const CString strFile);
	Acad::ErrorStatus Join(AcDbDatabase* pTargetDb,AcDbDatabase* pDb);
	GJoinDwgParam m_param;
	double        m_dMaxY;
private:
	AcGePoint3d m_insertPt;
};
