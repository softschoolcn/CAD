#include "StdAfx.h"
#include "GPlotHelper.h"
double Glb_dPaperWidth;//上一次打印的时候图纸宽度
double Glb_dPaperHeight;//上一次打印的时候图纸高度
CString GPlotHelper::GetCanonicalName(AcDbPlotSettingsValidator* mPSV,AcDbPlotSettings* mPlotSettings,CString& media)
{
	AcArray<const ACHAR*> mediaList;
	mPSV->canonicalMediaNameList(mPlotSettings, mediaList);
	int len = mediaList.length();
	for (int i = 0; i < len; ++i)
	{
		const ACHAR* localMedia;
		mPSV->getLocaleMediaName(mPlotSettings, mediaList[i], localMedia);
		CString strMedia = media;
		CString strLocalMedia(localMedia);
		int index = strMedia.ReverseFind(' ');
		if (index > 0)
		{
			strMedia = strMedia.Left(index);
		}
		if (strLocalMedia.Find(strMedia) == 0)
		{
			return CString(mediaList[i]);
		}
	}
	return _T("");
}
bool GPlotHelper::GetPlotArea(GPlotItemEx& plotSetting, AcGePoint2d& minPnt, AcGePoint2d& maxPnt)
{
	AcGePoint3d pnt = plotSetting.m_ptMin;
	ads_point minPt = {pnt.x, pnt.y, pnt.z};
	pnt = plotSetting.m_ptMax;
	ads_point maxPt = {pnt.x, pnt.y, pnt.z};
	ads_point minPtDcs;
	ads_point maxPtDcs;

	//从wcs转到窗口坐标系，打印区域用的是窗口坐标系
	struct resbuf form, to;
	form.restype = RTSHORT;
	to.restype = RTSHORT;
	form.resval.rint = 1;
	to.resval.rint = 2;
	acedTrans(minPt, &form, &to, 0, minPtDcs);
	acedTrans(maxPt, &form, &to, 0, maxPtDcs);

	//计算最大和最小点
	double minX, minY, maxX, maxY;
	if (minPtDcs[0] < maxPtDcs[0])
	{
		minX = minPtDcs[0];
		maxX = maxPtDcs[0];
	}
	else
	{
		maxX = minPtDcs[0];
		minX = maxPtDcs[0];
	}
	if (minPtDcs[1] < maxPtDcs[1])
	{
		minY = minPtDcs[1];
		maxY = maxPtDcs[1];
	}
	else
	{
		maxY = minPtDcs[1];
		minY = maxPtDcs[1];
	}

	minPnt.set(minX, minY);
	maxPnt.set(maxX, maxY);
	return true;
}
bool GPlotHelper::SetupPlotSetting(GPlotItemEx& plotSetting,bool bModelSpace,bool bAllow90,AcDbPlotSettings::ShadePlotType spt)
{
	CString mError;                   //错误信息
	double m_dPaperWidth;//上一次打印的时候图纸宽度
	double m_dPaperHeight;//上一次打印的时候图纸高度
	AcDbPlotSettingsValidator* mPSV=NULL;  //打印配置设置类
	AcDbPlotSettings* mPlotSettings=NULL;  //打印配置类
	acDocManager->lockDocument(curDoc());
	mPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();

	AcDbLayoutManager* pLayoutMng = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId layoutBTRId = pLayoutMng->getActiveLayoutBTRId();
	AcDbBlockTableRecord* pLayoutBTR = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pLayoutBTR, layoutBTRId, AcDb::kForRead);
	pLayoutBTR->close();
	AcDbLayout* pLayout = NULL;
	es = acdbOpenObject((AcDbObject*&)mPlotSettings, pLayoutBTR->getLayoutId(), AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		mError.Format(_T("打开打印配置类时错误:%s"), acadErrorStatusText(es));
		acDocManager->unlockDocument(curDoc());
		return false;
	}
	mPSV->refreshLists(mPlotSettings);
	AcArray<const ACHAR*> deviceList;
	mPSV->plotDeviceList(deviceList);
	AcGePoint2d minPnt(plotSetting.m_ptMin.x,plotSetting.m_ptMin.y), maxPnt(plotSetting.m_ptMax.x,plotSetting.m_ptMax.y);
	GetPlotArea(plotSetting,minPnt,maxPnt);

	//设置打印机
	es = mPSV->setPlotCfgName(mPlotSettings, m_strDevice);
	if (es != Acad::eOk)
	{
		mError.Format(_T("设置打印设备名称时错误:%s"), acadErrorStatusText(es));
		mPlotSettings->close();
		acDocManager->unlockDocument(curDoc());
		return false;
	}
	//设置打印尺寸
	if (!plotSetting.m_size.IsEmpty())  //尺寸不为空，就设置尺寸
	{
		CString media = GetCanonicalName(mPSV,mPlotSettings,plotSetting.m_size);  //获取内部尺寸名称
		if (media.IsEmpty())
		{
			mError.Format(_T("%s没有找到尺寸%s的匹配的内部尺寸"), m_strDevice, plotSetting.m_size);
			mPlotSettings->close();
			acDocManager->unlockDocument(curDoc());
			return false;
		}
		else
		{
			es = mPSV->setCanonicalMediaName(mPlotSettings, media);
			if (es != Acad::eOk)
			{
				mError.Format(_T("%s设置尺寸%s时产生错误%s"), plotSetting.m_size, media, acadErrorStatusText(es));
				mPlotSettings->close();
				acDocManager->unlockDocument(curDoc());
				return false;
			}
		}
	}
	else
	{
		
		if(m_strDevice.CompareNoCase(_T("PostScript Level 2.pc3"))==0)
		{
			CString strSize=bModelSpace?_T("ISO A0 (841.00 x 1189.00 毫米)"):_T("ISO A4 (210.00 x 297.00 毫米)");
				if (fabs(maxPnt.x - minPnt.x) > fabs(maxPnt.y - minPnt.y))  //图纸是横着的
				{
					strSize=bModelSpace?_T("ISO A0 (1189.00 x 841.00 毫米)"):_T("ISO A4 (297.00 x 210.00 毫米)");
				}
				else
				{
					strSize=bModelSpace?_T("ISO A0 (841.00 x 1189.00 毫米)"):_T("ISO A4 (210.00 x 297.00 毫米)");
				}
			//打印成eps
			CString media = GetCanonicalName(mPSV,mPlotSettings,strSize);  //获取内部尺寸名称
			if (media.IsEmpty())
			{
				mError.Format(_T("%s没有找到尺寸%s的匹配的内部尺寸"), m_strDevice, plotSetting.m_size);
				mPlotSettings->close();
				acDocManager->unlockDocument(curDoc());
				return false;
			}
			else
			{
				es = mPSV->setCanonicalMediaName(mPlotSettings, media);
				if (es != Acad::eOk)
				{
					mError.Format(_T("%s设置尺寸%s时产生错误%s"), m_strDevice, media, acadErrorStatusText(es));
					mPlotSettings->close();
					acDocManager->unlockDocument(curDoc());
					return false;
				}
			}
		}
	}

	es = mPSV->setPlotPaperUnits(mPlotSettings, AcDbPlotSettings::kMillimeters);
	es = mPSV->setPlotOrigin(mPlotSettings, 0.0, 0.0);
		es=mPSV->setPlotWindowArea(mPlotSettings,
			minPnt.x, minPnt.y,
			maxPnt.x, maxPnt.y);
		double printableXmin, printableYmin, printableXmax, printableYmax;
		es=mPlotSettings->getPlotPaperMargins(printableXmin, printableYmin, printableXmax, printableYmax);
		if(es==Acad::eOk&&printableXmax-printableXmin>0&&printableYmax-printableYmin>0)
		{
			if (fabs(printableXmax - printableXmin) < fabs(printableYmax - printableYmin))  //打印尺寸是竖着的
			{
				if (fabs(maxPnt.x - minPnt.x) > fabs(maxPnt.y - minPnt.y))  //图纸是横着的，旋转
				{
					if(bAllow90)
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k90degrees);
					}
					else
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k0degrees);
					}
				}
				else
				{
					mPSV->setPlotRotation(mPlotSettings,AcDbPlotSettings::k0degrees);
				}
			}
			else  //打印尺寸是横着的
			{
				if (fabs(maxPnt.x - minPnt.x) < fabs(maxPnt.y - minPnt.y))  //图纸是竖着的，旋转
				{
					if(bAllow90)
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k90degrees);
					}
					else
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k0degrees);
					}
				}
				else
				{
					mPSV->setPlotRotation(mPlotSettings,AcDbPlotSettings::k0degrees);
				}
			}
		}
		else
		{
			mPlotSettings->getPlotPaperSize(m_dPaperWidth,m_dPaperHeight);
			if (m_dPaperWidth < m_dPaperHeight)  //打印尺寸是竖着的
			{
				if (fabs(maxPnt.x - minPnt.x) > fabs(maxPnt.y - minPnt.y))  //图纸是横着的，旋转
				{
					if(bAllow90)
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k90degrees);
					}
					else
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k0degrees);
					}
				}
				else
				{
					mPSV->setPlotRotation(mPlotSettings,AcDbPlotSettings::k0degrees);
				}
			}
			else  //打印尺寸是横着的
			{
				if (fabs(maxPnt.x - minPnt.x) < fabs(maxPnt.y - minPnt.y))  //图纸是竖着的，旋转
				{
					if(bAllow90)
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k90degrees);
					}
					else
					{
						mPSV->setPlotRotation(mPlotSettings, AcDbPlotSettings::k0degrees);
					}
				}
				else
				{
					mPSV->setPlotRotation(mPlotSettings,AcDbPlotSettings::k0degrees);
				}
			}

		}
	//设置打印类型，窗口还是界限
		es = mPSV->setPlotType(mPlotSettings, AcDbPlotSettings::kWindow);
	if (es != Acad::eOk)
	{
		mError.Format(_T("设置打印类型时错误:%s"), acadErrorStatusText(es));
		mPlotSettings->close();
		acDocManager->unlockDocument(curDoc());
		return false;
	}
	es=mPlotSettings->setShadePlot(spt);
	if (es != Acad::eOk)
	{
		mError.Format(_T("设置着色类型时错误:%s"), acadErrorStatusText(es));
		mPlotSettings->close();
		acDocManager->unlockDocument(curDoc());
		return false;
	}

	

	

	//设置打印样式表
	AcArray<const ACHAR*> styleList;
	mPSV->plotStyleSheetList(styleList);
	es = mPSV->setCurrentStyleSheet(mPlotSettings, plotSetting.m_style);
	mPlotSettings->setPlotPlotStyles(true);
	//if(!bModelSpace)
	//{
		//mPSV->setZoomToPaperOnUpdate(mPlotSettings,true);
		mPSV->setStdScaleType(mPlotSettings,AcDbPlotSettings::kScaleToFit);//布满图纸
		mPSV->setPlotCentered(mPlotSettings,true);//居中打印

	//}
	mPSV->setPlotCentered(mPlotSettings,true);
	mPlotSettings->getPlotPaperSize(m_dPaperWidth,m_dPaperHeight);
	Glb_dPaperWidth=m_dPaperWidth;
	Glb_dPaperHeight=m_dPaperHeight;

	mPlotSettings->close();
	acDocManager->unlockDocument(curDoc());

	return true;
}