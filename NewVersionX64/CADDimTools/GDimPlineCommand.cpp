#include "StdAfx.h"
#include "CircleLabel.h"
//bTZ:是否是天正
void CreatLabelPlineAllPt(bool bTZ)
{
	double dPrecision = 0.000001;

	ads_name ssName;

	struct resbuf * preb = acutBuildList(RTDXF0, _T("LWPOLYLINE"), 0);
	TCHAR* promptPtrs[2]={_T("\n选择需要标注的多段线:"),_T("\n移除需要标注的多段线:")};

	if (acedSSGet(_T(":$:S"), promptPtrs, NULL, preb, ssName) != RTNORM)
	{
		acedSSFree(ssName);
		acutRelRb(preb);
		return;
	}
	acutRelRb(preb);

	ads_name ssTemp;

	acedSSName(ssName, 0, ssTemp);

	AcDbObjectId dbObjId;

	acdbGetObjectId(dbObjId, ssTemp);

	acedSSFree(ssName);

	AcDbPolyline * pPline = NULL;

	if (Acad::eOk != acdbOpenObject(pPline, dbObjId, AcDb::kForRead))
	{
		return;
	}

	pPline->close();

	int nCount = pPline->numVerts();

	CCircleLabel temp;
	if(bTZ)
	{
		temp.m_bExchangeXY=true;
		temp.m_dScale=1E-3;
	}

	for (int i = 0; i < nCount; i++)
	{
		AcGePoint3d pt3d, pt3dE;

		if (Acad::eOk != pPline->getPointAt(i, pt3d))
		{
			continue;
		}

		ads_point ptSet, ptSetE;

		ptSet[0] = pt3d.x;
		ptSet[1] = pt3d.y;
		ptSet[2] = pt3d.z;

		temp.CreateCoordLabel(ptSet);

		if (Adesk::kTrue == pPline->hasBulges())
		{
			if (i == nCount - 1)
			{
				if (Acad::eOk != pPline->getPointAt(0, pt3dE))
				{
					continue;
				}
			}
			else
			{
				if (Acad::eOk != pPline->getPointAt(i + 1, pt3dE))
				{
					continue;
				}
			}

			ptSetE[0] = pt3dE.x;
			ptSetE[1] = pt3dE.y;
			ptSetE[2] = pt3dE.z;

			double Bulge = 0;

			if (Acad::eOk != pPline->getBulgeAt(i, Bulge))
			{
				continue;
			}

			if (Bulge < -dPrecision || Bulge > dPrecision)
			{
				temp.CreatCircleLabel(ptSet, ptSetE, Bulge);
			}
		}
	}
}

void CreatLabelPlineAllPt()
{
	if(!CheckReg())
	{
		return;
	}
	CreatLabelPlineAllPt(false);
}
void CreatLabelPlineAllPtTZ()
{
	if(!CheckReg())
	{
		return;
	}
	CreatLabelPlineAllPt(false);
}