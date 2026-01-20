#include "StdAfx.h"
#include "CmpConfig.h"
extern CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName);
void SplitString(CString string,char marker,CStringArray  &stringArray)
{
	if(string.IsEmpty())return ;

	CString tmp;
	int nIndex=string.Find(marker);
	while(nIndex!=-1)
	{
		tmp=string.Left(nIndex);
		stringArray.Add(tmp);
		string=string.Right(string.GetLength()-nIndex-1);
		nIndex=string.Find(marker);
	}
	stringArray.Add(string);
}
void CombineArrayToString(const CStringArray& array,CString &string,CString linkSymbol)
{
	int n=array.GetSize();
	//zhangshl BUG修改
	if(n==0)
	{
		string +=_T("");
		return;
	}
	//zhangshl BUG修改
	for(int i=0;i<n-1;i++)
	{
		string += array.GetAt(i);
		string += linkSymbol;
	}
	string += array.GetAt(n-1);
}
CString CombineArrayToString(const CStringArray& array,CString linkSymbol)
{
	CString tmp;
	CombineArrayToString(array,tmp,linkSymbol);
	return tmp;
}
BOOL IsValidLong(const CString value,long &lValue)
{
	bool bNegative=false;//是否是负数
	CString tmpValue=value;
	try
	{
		tmpValue.Trim();
	}
	catch(...)
	{

	}
	int nLen=tmpValue.GetLength();
	if(nLen==0)
		return FALSE;
	TCHAR c=tmpValue.GetAt(0);
	if(c=='-')
	{
		//负数
		bNegative=true;
		if(nLen==1)
		{
			//只有一个负号是不合理的
			return FALSE;
		}
		tmpValue=tmpValue.Right(nLen-1);
		nLen--;
	}
	long lTmpValue=0L;
	c=tmpValue.GetAt(0);
	if(c=='0')
	{
		if(nLen==1)
		{
			lValue=0L;
			return TRUE;
		}
		else
		{
			//第一位是0并且后面还有其它数字认为是不合理的
			return FALSE;
		}
	}
	else if(c>='1'&&c<='9')
	{
		lTmpValue+=lTmpValue*10+c-'0';
		for(int i=1;i<nLen;i++)
		{
			c=tmpValue.GetAt(i);
			if(c>='0'&&c<='9')
			{
				lTmpValue=lTmpValue*10+(c-'0');
			}
			else
			{
				//出现了非数字认为是不合理的
				return FALSE;
			}
		}
	}
	else
	{
		//出现了非数字认为是不合理的
		return FALSE;
	}
	lValue=bNegative?-lTmpValue:lTmpValue;
	return TRUE;
}
CmpConfig::CmpConfig(void):m_pDoc1(NULL),m_pDoc2(NULL)
{
	m_nMode=1;
	m_nColorSame=251;
	m_nColorChange=2;//黄色表示改变
	m_nColorDelete=1;//红色表示删除
	m_nColorNew=3;//绿色表示新增加
	m_strFile1=_T("");
	m_strFile2=_T("");
	m_nCmpTypeCount=0;
	m_bEx=false;
	m_bSourceFile=false;
	m_bIngoreCloseLayer=true;
	m_bIngoreLockLayer=false;
	m_strIngoreLayer=_T("");
	for(int i=0;i<1024;i++)
	{
		m_cmpTypes[i]=NULL;
	}

}

CmpConfig::~CmpConfig(void)
{
	for(int i=0;i<1024;i++)
	{
		if(m_cmpTypes[i]!=NULL)
		{
			delete m_cmpTypes[i];
			m_cmpTypes[i]=NULL;
		}
	}
}
void CmpConfig::UpdateLockLayers()
{
	m_strIngoreLayerArray.RemoveAll();
	if(m_strIngoreLayer.IsEmpty())
	{
		return;
	}
	SplitString(m_strIngoreLayer,';',m_strIngoreLayerArray);

}
BOOL CmpConfig::Read()
{
	m_nCmpTypeCount=0;
	for(int i=0;i<1024;i++)
	{
		if(m_cmpTypes[i]!=NULL)
		{
			delete m_cmpTypes[i];
			m_cmpTypes[i]=NULL;
		}
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgConfig.xml"));
	if(!PathFileExists(lpPath))
		return FALSE;
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	//加载XML文件
	if(!lpDocument->load(lpPath))
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
	MSXML2::IXMLDOMNodePtr lpCmpConfigNode=lpDocElement->selectSingleNode(_T("CmpConfig"));
	if(lpCmpConfigNode!=NULL)
	{
		CString strMode=GetNodeAttrValue(lpCmpConfigNode,_T("Mode"));
		if(strMode.CompareNoCase(_T("0"))==0)
		{
			m_nMode=0;
		}
		else if(strMode.CompareNoCase(_T("1"))==0)
		{
			m_nMode=1;
		}
		else
		{
			m_nMode=2;
		}
		CString strCmpEx=GetNodeAttrValue(lpCmpConfigNode,_T("CmpEx"));
		if(strCmpEx.CompareNoCase(_T("true"))==0)
		{
			m_bEx=true;
		}
		else
		{
			m_bEx=false;
		}
		CString strSourceFile=GetNodeAttrValue(lpCmpConfigNode,_T("SourceFile"));
		if(strSourceFile.CompareNoCase(_T("true"))==0)
		{
			m_bSourceFile=true;
		}
		else
		{
			m_bSourceFile=false;
		}
		CString strIngoreCloseLayer=GetNodeAttrValue(lpCmpConfigNode,_T("IngoreCloseLayer"));
		if(strIngoreCloseLayer.CompareNoCase(_T("true"))==0)
		{
			m_bIngoreCloseLayer=true;
		}
		else
		{
			m_bIngoreCloseLayer=false;
		}

		CString strIngoreLockLayer=GetNodeAttrValue(lpCmpConfigNode,_T("IngoreLockLayer"));
		if(strIngoreLockLayer.CompareNoCase(_T("true"))==0)
		{
			m_bIngoreLockLayer=true;
		}
		else
		{
			m_bIngoreLockLayer=false;
		}
		m_strIngoreLayer=GetNodeAttrValue(lpCmpConfigNode,_T("IngoreLayer"));
		UpdateLockLayers();
	}
	MSXML2::IXMLDOMNodePtr lpColorNode=lpDocElement->selectSingleNode(_T("CmpColorConfig"));
	if(lpColorNode!=NULL)
	{
		long nValue=0;
		if(IsValidLong(GetNodeAttrValue(lpColorNode,_T("Same")),nValue))
		{
			m_nColorSame=nValue;
		}
		if(IsValidLong(GetNodeAttrValue(lpColorNode,_T("Change")),nValue))
		{
			m_nColorChange=nValue;
		}
		if(IsValidLong(GetNodeAttrValue(lpColorNode,_T("New")),nValue))
		{
			m_nColorNew=nValue;
		}
		if(IsValidLong(GetNodeAttrValue(lpColorNode,_T("Delete")),nValue))
		{
			m_nColorDelete=nValue;
		}
	}
	MSXML2::IXMLDOMNodePtr lpCmpFileNode=lpDocElement->selectSingleNode(_T("CmpFile"));
	if(lpCmpFileNode!=NULL)
	{
		m_strFile1=GetNodeAttrValue(lpCmpFileNode,_T("File1"));
		m_strFile2=GetNodeAttrValue(lpCmpFileNode,_T("File2"));
	}
	else
	{
		m_strFile1=_T("");
		m_strFile2=_T("");
	}
	MSXML2::IXMLDOMNodePtr lpCmpFloderNode=lpDocElement->selectSingleNode(_T("CmpFloder"));
	if(lpCmpFloderNode!=NULL)
	{
		m_strFloder1=GetNodeAttrValue(lpCmpFloderNode,_T("Floder1"));
		m_strFloder2=GetNodeAttrValue(lpCmpFloderNode,_T("Floder2"));
	}
	else
	{
		m_strFloder1=_T("");
		m_strFloder2=_T("");
	}
	MSXML2::IXMLDOMNodeListPtr lpCmpTypeNodeList=lpDocElement->getElementsByTagName(_T("CmpType"));
	if(lpCmpTypeNodeList==NULL)
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr lpCmpTypeNode = NULL;

	for ( ; (lpCmpTypeNode = lpCmpTypeNodeList->nextNode()) != NULL ; )
	{
		if(m_nCmpTypeCount>=1024)
		{
			break;
		}
		m_cmpTypes[m_nCmpTypeCount]=new CmpType();
		if(!ReadType(m_cmpTypes[m_nCmpTypeCount],lpCmpTypeNode))
			continue;
		m_nCmpTypeCount++;
	}
	FindParentTypes();
	return TRUE;


}
BOOL CmpConfig::Write()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("config\\CompareDwgConfig.xml"));
	if(!PathFileExists(lpPath))
		return FALSE;
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return FALSE;
	}
	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='GB2312'"));
	lpDocument->appendChild(lpInstruction);
	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("Root")));
	lpDocument->appendChild(lpRoot);
	CString strTmp;
	MSXML2::IXMLDOMElementPtr lpCmpConfigNode = lpDocument->createElement(_T("CmpConfig"));
	strTmp.Format(_T("%d"),m_nMode);
	lpCmpConfigNode->setAttribute(_T("Mode"),_variant_t(strTmp));
	lpCmpConfigNode->setAttribute(_T("CmpEx"),_variant_t(m_bEx?_T("true"):_T("false")));
	lpCmpConfigNode->setAttribute(_T("SourceFile"),_variant_t(m_bSourceFile?_T("true"):_T("false")));
	lpCmpConfigNode->setAttribute(_T("IngoreCloseLayer"),_variant_t(m_bIngoreCloseLayer?_T("true"):_T("false")));
	lpCmpConfigNode->setAttribute(_T("IngoreLockLayer"),_variant_t(m_bIngoreLockLayer?_T("true"):_T("false")));
	lpCmpConfigNode->setAttribute(_T("IngoreLayer"),_variant_t(m_strIngoreLayer));
	UpdateLockLayers();
	lpRoot->appendChild(lpCmpConfigNode);

	MSXML2::IXMLDOMElementPtr lpColorNode = lpDocument->createElement(_T("CmpColorConfig"));
	strTmp.Format(_T("%d"),m_nColorSame);
	lpColorNode->setAttribute(_T("Same"), _variant_t(strTmp));
	strTmp.Format(_T("%d"),m_nColorChange);
	lpColorNode->setAttribute(_T("Change"), _variant_t(strTmp));
	strTmp.Format(_T("%d"),m_nColorNew);
	lpColorNode->setAttribute(_T("New"), _variant_t(strTmp));
	strTmp.Format(_T("%d"),m_nColorDelete);
	lpColorNode->setAttribute(_T("Delete"), _variant_t(strTmp));
	lpRoot->appendChild(lpColorNode);


	MSXML2::IXMLDOMElementPtr lpCmpFileNode=lpDocument->createElement(_T("CmpFile"));
	lpCmpFileNode->setAttribute(_T("File1"),_variant_t(m_strFile1));
	lpCmpFileNode->setAttribute(_T("File2"),_variant_t(m_strFile2));
	lpRoot->appendChild(lpCmpFileNode);

	MSXML2::IXMLDOMElementPtr lpCmpFloderNode=lpDocument->createElement(_T("CmpFloder"));
	lpCmpFloderNode->setAttribute(_T("Floder1"),_variant_t(m_strFloder1));
	lpCmpFloderNode->setAttribute(_T("Floder2"),_variant_t(m_strFloder2));
	lpRoot->appendChild(lpCmpFloderNode);

	for(long i=0;i<m_nCmpTypeCount;i++)
	{
		CmpType* mType=m_cmpTypes[i];
		MSXML2::IXMLDOMElementPtr lpCmpTypeNode=lpDocument->createElement(_T("CmpType"));
		WriteType(mType,lpCmpTypeNode);
		lpRoot->appendChild(lpCmpTypeNode);
	}


	lpDocument->save(lpPath);
	return FALSE;
}
BOOL CmpConfig::WriteType(CmpType* type,MSXML2::IXMLDOMElementPtr lpCmpTypeNode)
{
	lpCmpTypeNode->setAttribute(_T("Name"),_variant_t(type->m_strName));
	lpCmpTypeNode->setAttribute(_T("ClassName"),_variant_t(type->m_strClassName));
	lpCmpTypeNode->setAttribute(_T("ParentClassName"),_variant_t(type->m_strParentClassName));
	lpCmpTypeNode->setAttribute(_T("Active"),_variant_t(type->m_bActive?_T("TRUE"):_T("FALSE")));
	for(long i=0;i<type->m_nCmpItemsCount;i++)
	{
		CmpItem& item=type->m_cmpItems[i];
		MSXML2::IXMLDOMElementPtr lpCmpItemNode=lpCmpTypeNode->GetownerDocument()->createElement(_T("CmpItem"));
		WriteItem(item,lpCmpItemNode);
		lpCmpTypeNode->appendChild(lpCmpItemNode);
	}
	if(type->m_IngoreDxfCodes.GetCount()>0)
	{
		CString strDXF;
		for(long i=0;i<type->m_IngoreDxfCodes.GetCount();i++)
		{
			if(i!=0)
			{
				strDXF.Append(_T(","));
			}
			strDXF.AppendFormat(_T("%d"),type->m_IngoreDxfCodes.GetAt(i));
		}
		lpCmpTypeNode->setAttribute(_T("IngoreDxfCodes"),_variant_t(strDXF));
	}
	return TRUE;
}
BOOL CmpConfig::ReadType(CmpType* type,MSXML2::IXMLDOMNodePtr lpCmpTypeNode)
{
	MSXML2::IXMLDOMNodePtr lpNameNode = lpCmpTypeNode->attributes->getNamedItem(_T("Name"));
	if(lpNameNode==NULL)
		return FALSE;
	CString strName = (char*)lpNameNode->text;
	if(strName.IsEmpty())
		return FALSE;

	MSXML2::IXMLDOMNodePtr lpClassNameNode = lpCmpTypeNode->attributes->getNamedItem(_T("ClassName"));
	if(lpClassNameNode==NULL)
		return FALSE;
	CString strClassName = (char*)lpClassNameNode->text;
	if(strClassName.IsEmpty())
		return FALSE;

	MSXML2::IXMLDOMNodePtr lpParentClassNameNode = lpCmpTypeNode->attributes->getNamedItem(_T("ParentClassName"));
	if(lpParentClassNameNode==NULL)
		return FALSE;
	CString strParentClassName = (char*)lpParentClassNameNode->text;

	BOOL bActive=TRUE;

	MSXML2::IXMLDOMNodePtr lpActiveNode = lpCmpTypeNode->attributes->getNamedItem(_T("Active"));
	if(lpActiveNode!=NULL)
	{
		CString strActive = CString((const char*)lpActiveNode->text);
		if(strActive.CompareNoCase(_T("TRUE"))==0)
		{
			bActive=TRUE;
		}
		else
		{
			bActive=FALSE;
		}
	}
	MSXML2::IXMLDOMNodePtr lpIngoreDxfCodesNode = lpCmpTypeNode->attributes->getNamedItem(_T("IngoreDxfCodes"));
	if(lpIngoreDxfCodesNode!=NULL)
	{
		type->m_IngoreDxfCodes.RemoveAll();
		CString strDxfCodes = (char*)lpIngoreDxfCodesNode->text;
		CStringArray  stringArray;
		SplitString(strDxfCodes,',',stringArray);
		for(long i=0;i<stringArray.GetCount();i++)
		{
			long nDxfCode=0;
			if(IsValidLong(stringArray.GetAt(i),nDxfCode))
			{
				type->m_IngoreDxfCodes.Add(nDxfCode);
			}
		}
	}

	type->m_bActive=bActive;
	type->m_strName=strName;
	type->m_strClassName=strClassName;
	type->m_strParentClassName=strParentClassName;
	MSXML2::IXMLDOMNodeListPtr lpCmpItemNodeList=lpCmpTypeNode->selectNodes(_T("CmpItem"));
	if(lpCmpItemNodeList==NULL)
	{
		return TRUE;
	}
	MSXML2::IXMLDOMNodePtr lpCmpItemNode = NULL;

	for ( ; (lpCmpItemNode = lpCmpItemNodeList->nextNode()) != NULL ; )
	{
		if(type->m_nCmpItemsCount>=128)
		{
			break;
		}
		if(!ReadItem(type->m_cmpItems[type->m_nCmpItemsCount],lpCmpItemNode))
		{
			continue;
		}
		type->m_nCmpItemsCount++;
	}
	return TRUE;

}
BOOL CmpConfig::ReadItem(CmpItem& item,MSXML2::IXMLDOMNodePtr lpCmpItemNode)
{
	MSXML2::IXMLDOMNodePtr lpNameNode = lpCmpItemNode->attributes->getNamedItem(_T("Name"));
	if(lpNameNode==NULL)
		return FALSE;
	CString strName = (char*)lpNameNode->text;
	if(strName.IsEmpty())
		return FALSE;

	MSXML2::IXMLDOMNodePtr lpDxfCodesNode = lpCmpItemNode->attributes->getNamedItem(_T("DxfCodes"));
	if(lpDxfCodesNode==NULL)
		return FALSE;
	item.m_dxfCodes.RemoveAll();
	CString strDxfCodes = (char*)lpDxfCodesNode->text;
	CStringArray  stringArray;
	SplitString(strDxfCodes,',',stringArray);
	for(long i=0;i<stringArray.GetCount();i++)
	{
		long nDxfCode=0;
		if(IsValidLong(stringArray.GetAt(i),nDxfCode))
		{
			item.m_dxfCodes.Add(nDxfCode);
		}
	}
	if(item.m_dxfCodes.GetCount()==0)
		return FALSE;



	BOOL bActive=TRUE;

	MSXML2::IXMLDOMNodePtr lpActiveNode = lpCmpItemNode->attributes->getNamedItem(_T("Active"));
	if(lpActiveNode!=NULL)
	{
		CString strActive = (char*)lpActiveNode->text;
		if(strActive.CompareNoCase(_T("TRUE"))==0)
		{
			bActive=TRUE;
		}
		else
		{
			bActive=FALSE;
		}
	}

	BOOL bOnlyOne=TRUE;

	MSXML2::IXMLDOMNodePtr lpOnlyOneNode = lpCmpItemNode->attributes->getNamedItem(_T("OnlyOne"));
	if(lpActiveNode!=NULL)
	{
		CString strOnlyOne = (char*)lpOnlyOneNode->text;
		if(strOnlyOne.CompareNoCase(_T("TRUE"))==0)
		{
			bOnlyOne=TRUE;
		}
		else
		{
			bOnlyOne=FALSE;
		}
	}

	BOOL bIngoreOrder=FALSE;

	MSXML2::IXMLDOMNodePtr lpIngoreOrderNode = lpCmpItemNode->attributes->getNamedItem(_T("IngoreOrder"));
	if(lpIngoreOrderNode!=NULL)
	{
		CString strIngoreOrder = (char*)lpIngoreOrderNode->text;
		if(strIngoreOrder.CompareNoCase(_T("TRUE"))==0)
		{
			bIngoreOrder=TRUE;
		}
		else
		{
			bIngoreOrder=FALSE;
		}
	}
	BOOL bIngoreNoCase=FALSE;
	MSXML2::IXMLDOMNodePtr lpIngoreNoCaseNode = lpCmpItemNode->attributes->getNamedItem(_T("IngoreNoCase"));
	if(lpIngoreNoCaseNode!=NULL)
	{
		CString strIngoreNoCaseNode = (char*)lpIngoreNoCaseNode->text;
		if(strIngoreNoCaseNode.CompareNoCase(_T("TRUE"))==0)
		{
			bIngoreNoCase=TRUE;
		}
		else
		{
			bIngoreNoCase=FALSE;
		}
	}
	item.m_bIngoreNoCase=bIngoreNoCase;
	item.m_bActive=bActive;
	item.m_bIngoreOrder=bIngoreOrder;
	item.m_bOnlyOne=bOnlyOne;
	item.m_strName=strName;
	return TRUE;

}
BOOL CmpConfig::WriteItem(CmpItem& item,MSXML2::IXMLDOMElementPtr lpCmpItemNode)
{
	lpCmpItemNode->setAttribute(_T("Name"),_variant_t(item.m_strName));
	lpCmpItemNode->setAttribute(_T("Active"),_variant_t(item.m_bActive?_T("TRUE"):_T("FALSE")));
	lpCmpItemNode->setAttribute(_T("OnlyOne"),_variant_t(item.m_bOnlyOne?_T("TRUE"):_T("FALSE")));
	lpCmpItemNode->setAttribute(_T("IngoreOrder"),_variant_t(item.m_bIngoreOrder?_T("TRUE"):_T("FALSE")));
	if(item.m_bIngoreNoCase)
	{
		lpCmpItemNode->setAttribute(_T("IngoreNoCase"),_variant_t(_T("TRUE")));
	}
	CString strDXF;
	for(long i=0;i<item.m_dxfCodes.GetCount();i++)
	{
		if(i!=0)
		{
			strDXF.Append(_T(","));
		}
		strDXF.AppendFormat(_T("%d"),item.m_dxfCodes.GetAt(i));

	}
	lpCmpItemNode->setAttribute(_T("DxfCodes"),_variant_t(strDXF));
	return TRUE;
}
void CmpConfig::FindParentTypes()
{
	for(long i=m_nCmpTypeCount-1;i>=1;i--)
	{
		CmpType* type1=m_cmpTypes[i];
		for(long j=i-1;j>=0;j--)
		{
			CmpType* type2=m_cmpTypes[j];
			if(type1->m_strParentClassName.Compare(type2->m_strClassName)==0)
			{
				type1->m_pParType=type2;
				break;
			}
		}
	}
}
