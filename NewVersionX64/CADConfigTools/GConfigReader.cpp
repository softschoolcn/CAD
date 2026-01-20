#include "StdAfx.h"
#include "GConfigReader.h"
//读取XML节点的值
CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName)
{
	MSXML2::IXMLDOMNodePtr lpAttrNode = lpNode->attributes->getNamedItem(strName.GetBuffer());
	strName.ReleaseBuffer();
	if(lpAttrNode==NULL)
		return _T("");
	return (char*)lpAttrNode->text;
}
CString GLayerDefine::GetColorName()
{
	if(nColorIndex==0)
	{
		return _T("ByLayer");
	}
	else if(nColorIndex==1)
	{
		return _T("红");
	}
	else if(nColorIndex==2)
	{
		return _T("黄");
	}
	else if(nColorIndex==3)
	{
		return _T("绿");
	}
	else if(nColorIndex==4)
	{
		return _T("青");
	}
	else if(nColorIndex==5)
	{
		return _T("蓝");
	}
	else if(nColorIndex==6)
	{
		return _T("紫");
	}
	else if(nColorIndex==7)
	{
		return _T("白");
	}
	else if(nColorIndex==256)
	{
		return _T("ByBlock");
	}
	else if(nColorIndex>=8&&nColorIndex<=255)
	{
		CString strColor;
		strColor.Format(_T("%d"),nColorIndex);
		return strColor;
	}
	else
	{
		return _T("白");
	}
}
//读取定义好的图层
BOOL ReadDefinedLayer(CArray<GLayerDefine>& layers,LPCTSTR lpFile)
{
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpFile))
	{
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMNodeListPtr lpCmdNodeList=lpDocElement->getElementsByTagName(_T("Layer"));
	MSXML2::IXMLDOMNodePtr lpCmdNode = NULL;
	for ( ; (lpCmdNode = lpCmdNodeList->nextNode()) != NULL ; )
	{
		CString strName=GetNodeAttrValue(lpCmdNode,_T("Name"));
		CString strLayerName=GetNodeAttrValue(lpCmdNode,_T("LayerName"));
		CString strColor=GetNodeAttrValue(lpCmdNode,_T("Color"));
		CString strLineType=GetNodeAttrValue(lpCmdNode,_T("LineType"));
		for(int i=0;i<layers.GetCount();i++)
		{
			GLayerDefine& tmp=layers.GetAt(i);
			if(tmp.strLayerName.CompareNoCase(strLayerName)==0)
			{
				strColor.Format(_T("%d"),tmp.nColorIndex);
				strLineType=tmp.strLineType;
				break;
			}
		}
		int nColor=7;
		GStringHelper::IsValidInt(strColor,nColor);
		GLayerDefine ld;
		ld.strName=strName;
		ld.strLayerName=strLayerName;
		ld.nColorIndex=nColor;
		ld.strLineType=strLineType;
		layers.Add(ld);
	}
	return TRUE;
}
//读取定义好的图层
BOOL ReadDefinedLayer(CArray<GLayerDefine>& layers)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("UserConfig\\LayerDefine.xml"));
	CArray<GLayerDefine> layers1;
	ReadDefinedLayer(layers1,lpPath1);

	TCHAR lpPath2[MAX_PATH+1]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("Config\\LayerDefine.xml"));
	CArray<GLayerDefine> layers2;
	ReadDefinedLayer(layers2,lpPath2);

	layers.Append(layers1);

	for(int i=0;i<layers2.GetCount();i++)
	{
		bool bFind=false;
		for(int j=0;j<layers1.GetCount();j++)
		{
			if(layers1.GetAt(j).strName.CompareNoCase(layers2.GetAt(i).strName)==0)
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
		{
			layers.Add(layers2.GetAt(i));
		}
	}
	return TRUE;
}
