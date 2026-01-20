#include "StdAfx.h"
#include "GPlotDpiConfig.h"
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
/////////////////////////////////////////////////PlotDpiConfig//////////////////////////////////////////////////////
BOOL GPlotDpiConfig::Read(GPlotDpiConfigItem& item,const GPlotDpiConfigItem defaultItem,LPCTSTR lpPath,LPCTSTR strSection)
{
	CString strValue;
	::GetPrivateProfileString(strSection,_T("x"),_T(""),strValue.GetBuffer(128+1),128,lpPath);
	strValue.ReleaseBuffer();
	if(!strValue.IsEmpty())
	{
		double dValue=0.0;
		if(GStringHelper::IsValidDouble(strValue,dValue))
		{
			if(dValue>=10.0&&dValue<=10000)
			{
				item.print_xdpi=(float)dValue;
			}
			else
			{
				item.print_xdpi=defaultItem.print_xdpi;
			}
		}
		else
		{
			item.print_xdpi=defaultItem.print_xdpi;
		}
	}
	else
	{
		item.print_xdpi=defaultItem.print_xdpi;
	}

	::GetPrivateProfileString(strSection,_T("y"),_T(""),strValue.GetBuffer(128+1),128,lpPath);
	strValue.ReleaseBuffer();
	if(!strValue.IsEmpty())
	{
		double dValue=0.0;
		if(GStringHelper::IsValidDouble(strValue,dValue))
		{
			if(dValue>=10.0&&dValue<=10000)
			{
				item.print_ydpi=(float)dValue;
			}
			else
			{
				item.print_ydpi=defaultItem.print_ydpi;
			}
		}
		else
		{
			item.print_ydpi=defaultItem.print_ydpi;
		}
	}
	else
	{
		item.print_ydpi=defaultItem.print_ydpi;
	}
	return TRUE;
}
BOOL GPlotDpiConfig::Read()
{
	TCHAR lpPath[MAX_PATH]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("UserConfig\\plotdpi.ini"));
	Read(PdfDpiConfig,GPlotDpiConfigItem(720,720),lpPath,_T("pdf"));
	Read(PdfDpiConfig,GPlotDpiConfigItem(204,196),lpPath,_T("tif"));
	Read(JpgDpiConfig,GPlotDpiConfigItem(300,300),lpPath,_T("jpg"));
	return TRUE;
}
