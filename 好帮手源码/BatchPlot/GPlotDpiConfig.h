#pragma once

//转换DPI设置,DPI越大,转换越清晰，但是转换出来的文件也越大
struct GPlotDpiConfigItem
{
	float print_xdpi;
	float print_ydpi;
	GPlotDpiConfigItem()
	{
		print_xdpi=300;
		print_ydpi=300;
	}
	GPlotDpiConfigItem(const float xdpi,const float ydpi)
	{
		print_xdpi=xdpi;
		print_ydpi=ydpi;
	}
};
//转换DPI设置
struct GPlotDpiConfig
{
	GPlotDpiConfigItem PdfDpiConfig;
	GPlotDpiConfigItem TifDpiConfig;
	GPlotDpiConfigItem JpgDpiConfig;
	BOOL Read(GPlotDpiConfigItem& item,const GPlotDpiConfigItem defaultItem,LPCTSTR lpPath,LPCTSTR strSection);
	BOOL Read();
};
