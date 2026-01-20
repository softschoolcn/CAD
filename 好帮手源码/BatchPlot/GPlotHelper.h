#pragma once
#include "GPlotDefine.h"
struct GPlotHelper
{
public:
	//bAllow90,允许90度旋转打印
	bool SetupPlotSetting(GPlotItemEx& plotSetting,bool bModelSpace,bool bAllow90,AcDbPlotSettings::ShadePlotType spt=AcDbPlotSettings::kAsDisplayed);
	CString m_strDevice;
private:
	CString GetCanonicalName(AcDbPlotSettingsValidator* mPSV,AcDbPlotSettings* mPlotSettings,CString& media);
	bool GetPlotArea(GPlotItemEx& plotSetting, AcGePoint2d& minPnt, AcGePoint2d& maxPnt);
};