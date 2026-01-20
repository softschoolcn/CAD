#pragma once
//图层定义
struct GLayerDefine
{
	CString strName;//图层含义
	CString strLayerName;//图层名称
	int     nColorIndex;//颜色
	CString strLineType;
	CString GetColorName();
};
//读取定义好的图层
BOOL ReadDefinedLayer(CArray<GLayerDefine>& layers,LPCTSTR lpFile);
//读取定义好的图层
BOOL ReadDefinedLayer(CArray<GLayerDefine>& layers);
