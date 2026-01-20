#pragma once
#include "StdAfx.h"

class AcDbPlotSettingsValidator;
class AcDbPlotSettings;
class AcDbLayout;

class GPlotSettings
{
public:
    GPlotSettings(void);
    ~GPlotSettings(void);

    /*!
     @brief
     */
    static GPlotSettings* GetInstance();

    /*!
     @brief
     */
    Acad::ErrorStatus Init();

    /*!
     @brief
     */
    Acad::ErrorStatus RefreshDevices(CStringArray& devices);

    /*!
     @brief
     */
    Acad::ErrorStatus SetDevice(CString& device);

    /*!
     @brief
     */
    Acad::ErrorStatus RefreshMedias(CStringArray& medias);
	Acad::ErrorStatus RefreshMedias(CStringArray& medias,CStringArray& innerMedias,CArray<double>& mediaWidths,CArray<double>& mediaHeights);

    /*!
     @brief
     */
    Acad::ErrorStatus RefreshStyles(CStringArray& styles);

	/*!
	 @brief 得到默认打印尺寸
	 */
	CString GetDefaultMedia();

	/*!
	 @brief 得到打印尺寸的内部名称，主要用于测试
	 */
	Acad::ErrorStatus RefreshInterMedias(CStringArray& medias);

private:
    static GPlotSettings* mInstance;
    AcDbPlotSettingsValidator* mPSV;  //打印配置设置类
    AcDbPlotSettings* mPlotSettings;  //打印配置类
    AcDbLayout* mLayout;              //布局
};
