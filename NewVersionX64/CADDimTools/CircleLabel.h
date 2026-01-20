#pragma once
#include "SpcoordLabel.h"
//////////////////////////////////////////////////////////////////////////
// Ô²»¡±ê×¢
class CCircleLabel :
	public CSpcoordLabel
{
public:
	CCircleLabel(void);
	virtual ~CCircleLabel(void);

public:
	BOOL CreatCircleLabel(ads_point StartPt, ads_point EndPt, double Bulge);
};