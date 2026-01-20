#include "stdafx.h"
#include "gplotsettings.h"
#include "dbents.h"

GPlotSettings* GPlotSettings::mInstance = NULL;
GPlotSettings::GPlotSettings(void)
{
}

GPlotSettings::~GPlotSettings(void)
{
}

GPlotSettings* GPlotSettings::GetInstance()
{
    if (mInstance == NULL)
    {
        mInstance = new GPlotSettings();
    }
    return mInstance;
}

Acad::ErrorStatus GPlotSettings::Init()
{
    acDocManager->lockDocument(curDoc());
    mPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
    AcDbLayoutManager* pLayoutMng = acdbHostApplicationServices()->layoutManager();
    AcDbObjectId layoutBTRId = pLayoutMng->getActiveLayoutBTRId();
    AcDbBlockTableRecord* pLayoutBTR = NULL;
    Acad::ErrorStatus es = acdbOpenObject(pLayoutBTR, layoutBTRId, AcDb::kForRead);
    pLayoutBTR->close();
    AcDbLayout* pLayout = NULL;
    es = acdbOpenObject((AcDbObject*&)mPlotSettings, pLayoutBTR->getLayoutId(), AcDb::kForRead);
    mPlotSettings->close();
    acDocManager->unlockDocument(curDoc());
    return es;
}

Acad::ErrorStatus GPlotSettings::RefreshDevices(CStringArray& devices)
{
    AcArray<const ACHAR*> deviceList;
    Acad::ErrorStatus es = mPSV->plotDeviceList(deviceList);
    int len = deviceList.length();
    for (int i = 0; i < len; ++i)
    {
        devices.Add(deviceList[i]);
    }
    return es;
}

Acad::ErrorStatus GPlotSettings::SetDevice(CString& device)
{
    acDocManager->lockDocument(curDoc());
    AcDbObjectId id = mPlotSettings->id();
    Acad::ErrorStatus es;
    es = acdbOpenAcDbObject((AcDbObject*&)mPlotSettings, id, AcDb::kForWrite);
    if (es != Acad::eOk)
    {
        return es;
    }
    es = mPSV->setPlotCfgName(mPlotSettings, device);
    if (es != Acad::eOk)
    {
        mPlotSettings->close();
        acDocManager->unlockDocument(curDoc());
        return es;
    }
    mPSV->refreshLists(mPlotSettings);
    mPlotSettings->close();
    acDocManager->unlockDocument(curDoc());
    return es;
}

Acad::ErrorStatus GPlotSettings::RefreshInterMedias(CStringArray& medias)
{
	Acad::ErrorStatus es;
	AcArray<const ACHAR*> mediaNameList;
	es = mPSV->canonicalMediaNameList(mPlotSettings, mediaNameList);
	int len = mediaNameList.length();
	for (int i = 0; i < len; ++i)
	{
		medias.Add(mediaNameList[i]);
	}
	return es;
}

Acad::ErrorStatus GPlotSettings::RefreshMedias(CStringArray& medias)
{
    Acad::ErrorStatus es;
    AcArray<const ACHAR*> mediaNameList;
    es = mPSV->canonicalMediaNameList(mPlotSettings, mediaNameList);
	if(es!=eOk)
	{
		acutPrintf(_T("\nGPlotSettings::RefreshMedias:%s"),acadErrorStatusText(es));
	}
    int len = mediaNameList.length();
    for (int i = 0; i < len; ++i)
    {
        const ACHAR* localMedia;
        CString canonicalMedia = mediaNameList[i];
        mPSV->getLocaleMediaName(mPlotSettings, canonicalMedia, localMedia);
        medias.Add(localMedia);
    }
    return es;
}
Acad::ErrorStatus GPlotSettings::RefreshMedias(CStringArray& medias,CStringArray& innerMedias,CArray<double>& mediaWidths,CArray<double>& mediaHeights)
{
    acDocManager->lockDocument(curDoc());
    AcDbObjectId id = mPlotSettings->id();
    Acad::ErrorStatus es;
    es = acdbOpenAcDbObject((AcDbObject*&)mPlotSettings, id, AcDb::kForWrite);
	if (es != Acad::eOk)
    {
		acDocManager->unlockDocument(curDoc());
        return es;
    }
	double dWidth=0.0;
	double dHeight=0.0;
    AcArray<const ACHAR*> mediaNameList;
    es = mPSV->canonicalMediaNameList(mPlotSettings, mediaNameList);
	if(es!=eOk)
	{
		acutPrintf(_T("\nGPlotSettings::RefreshMedias:%s"),acadErrorStatusText(es));
		mPlotSettings->close();
		acDocManager->unlockDocument(curDoc());
		return es;
	}
    int len = mediaNameList.length();
    for (int i = 0; i < len; ++i)
    {
        const ACHAR* localMedia;
        CString canonicalMedia = mediaNameList[i];
        es=mPSV->getLocaleMediaName(mPlotSettings, canonicalMedia, localMedia);
		if(es!=Acad::eOk)
		{
			continue;
		}
		es=mPSV->setCanonicalMediaName(mPlotSettings,canonicalMedia);
		if(es!=Acad::eOk)
		{
			continue;
		}
		es=mPlotSettings->getPlotPaperSize(dWidth,dHeight);
		if(es!=Acad::eOk)
		{
			continue;
		}
		medias.Add(localMedia);
		innerMedias.Add(canonicalMedia);
		mediaWidths.Add(dWidth);
		mediaHeights.Add(dHeight);
    }
	mPlotSettings->close();
	acDocManager->unlockDocument(curDoc());
	return Acad::eOk;
}

Acad::ErrorStatus GPlotSettings::RefreshStyles(CStringArray& styles)
{
    Acad::ErrorStatus es;
    AcArray<const ACHAR*> styleList;
    es = mPSV->plotStyleSheetList(styleList);
    int len = styleList.length();
    for (int i = 0; i < len; ++i)
    {
        styles.Add(styleList[i]);
    }
    return es;
}

CString GPlotSettings::GetDefaultMedia()
{
	Acad::ErrorStatus es;
	const ACHAR* name;
	es = mPlotSettings->getCanonicalMediaName(name);
	if (es != Acad::eOk)
	{
		return _T("");
	}
	const ACHAR* localName;
	es = mPSV->getLocaleMediaName(mPlotSettings, name, localName);
	if(es!=Acad::eOk)
	{
		return _T("");
	}
	return CString(localName);
}