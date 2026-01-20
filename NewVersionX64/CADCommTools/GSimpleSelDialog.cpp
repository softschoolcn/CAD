// GSimpleSelDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GSimpleSelDialog.h"

// GSimpleSelDialog 对话框

IMPLEMENT_DYNAMIC(GSimpleSelDialog, CDialog)
GSimpleSelDialog::GSimpleSelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GSimpleSelDialog::IDD, pParent)
{
	selFilter=NULL;
}

GSimpleSelDialog::~GSimpleSelDialog()
{
}

void GSimpleSelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_LAYER,m_listLayer);
	DDX_Control(pDX,IDC_LIST_TYPE,m_listType);
	DDX_Control(pDX,IDC_CHECK_ALLLAYER,m_checkAllLayer);
	DDX_Control(pDX,IDC_CHECK_ALLTYPE,m_checkAllType);
	DDX_Control(pDX,IDC_BUTTON_LAYERALL,m_btnAllLayer);
	DDX_Control(pDX,IDC_BUTTON_LAYERALLNOT,m_btnAllLayerNot);
	DDX_Control(pDX,IDC_BUTTON_LAYERFX,m_btnLayerFX);
	DDX_Control(pDX,IDC_BUTTON_TYPEALL,m_btnAllType);
	DDX_Control(pDX,IDC_BUTTON_TYPEALLNOT,m_btnAllTypeNot);
	DDX_Control(pDX,IDC_BUTTON_TYPEFX,m_btnTypeFX);
	DDX_Control(pDX,IDC_RADIO_SEL,m_radioSel);
	DDX_Control(pDX,IDC_RADIO_FX,m_radioFX);

}


BEGIN_MESSAGE_MAP(GSimpleSelDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LAYERALL, &GSimpleSelDialog::OnBnClickedButtonLayerall)
	ON_BN_CLICKED(IDC_BUTTON_LAYERALLNOT, &GSimpleSelDialog::OnBnClickedButtonLayerallnot)
	ON_BN_CLICKED(IDC_BUTTON_LAYERFX, &GSimpleSelDialog::OnBnClickedButtonLayerfx)
	ON_BN_CLICKED(IDC_BUTTON_TYPEALL, &GSimpleSelDialog::OnBnClickedButtonTypeall)
	ON_BN_CLICKED(IDC_BUTTON_TYPEALLNOT, &GSimpleSelDialog::OnBnClickedButtonTypeallnot)
	ON_BN_CLICKED(IDC_BUTTON_TYPEFX, &GSimpleSelDialog::OnBnClickedButtonTypefx)
	ON_BN_CLICKED(IDC_CHECK_ALLLAYER, &GSimpleSelDialog::OnBnClickedCheckAlllayer)
	ON_BN_CLICKED(IDC_CHECK_ALLTYPE, &GSimpleSelDialog::OnBnClickedCheckAlltype)
	ON_BN_CLICKED(IDOK, &GSimpleSelDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GSimpleSelDialog 消息处理程序
BOOL GSimpleSelDialog::OnInitDialog()
{
	BOOL rc=CDialog::OnInitDialog();
	ReadTypeAlias();
	m_listLayer.SetExtendedStyle(m_listLayer.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	m_listType.SetExtendedStyle(m_listType.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	CString strTmp;
	strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GSimpleSelDialog"),_T("AllLayer"),128,_T("False"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		m_checkAllLayer.SetCheck(TRUE);
		OnBnClickedCheckAlllayer();
	}
	for(int i=0;i<layerNameArray.GetCount();i++)
	{
		int nItem=m_listLayer.InsertItem(i,layerNameArray.GetAt(i));
		strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GSimpleSelDialog"),_T("LAYER_")+layerNameArray.GetAt(i),128,_T("True"));
		if(strTmp.CompareNoCase(_T("True"))==0)
		{
			m_listLayer.SetCheck(nItem,TRUE);
		}
		else
		{
			m_listLayer.SetCheck(nItem,FALSE);
		}

	}
	strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GSimpleSelDialog"),_T("AllType"),128,_T("True"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		m_checkAllType.SetCheck(TRUE);
		OnBnClickedCheckAlltype();
	}
	for(int i=0;i<entTypeArray.logicalLength();i++)
	{
		AcRxClass* pClass=entTypeArray.at(i);
		CString strDxf=pClass->dxfName();
		CString strAlias=GetTypeAlias(pClass->name());
		int nItem=m_listType.InsertItem(i,strAlias);
		m_listType.SetItemData(nItem,(DWORD_PTR)pClass);
		strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GSimpleSelDialog"),_T("TYPE_")+strDxf,128,_T("True"));
		if(strTmp.CompareNoCase(_T("True"))==0)
		{
			m_listType.SetCheck(nItem,TRUE);
		}
		else
		{
			m_listType.SetCheck(nItem,FALSE);
		}

	}
	CString strTmp1=GUserDataRecord::getInstance()->GetProfileString(_T("GSimpleSelDialog"),_T("Select"),128,_T("True"));
	if(strTmp1.CompareNoCase(_T("True"))==0)
	{
		m_radioSel.SetCheck(TRUE);
	}
	else
	{
		m_radioFX.SetCheck(TRUE);
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return rc;
}
void GSimpleSelDialog::OnBnClickedButtonLayerall()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listLayer.GetItemCount();i++)
	{
		m_listLayer.SetCheck(i,TRUE);
	}
}

void GSimpleSelDialog::OnBnClickedButtonLayerallnot()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listLayer.GetItemCount();i++)
	{
		m_listLayer.SetCheck(i,FALSE);
	}
}

void GSimpleSelDialog::OnBnClickedButtonLayerfx()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listLayer.GetItemCount();i++)
	{
		m_listLayer.SetCheck(i,!m_listLayer.GetCheck(i));
	}
}

void GSimpleSelDialog::OnBnClickedButtonTypeall()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listType.GetItemCount();i++)
	{
		m_listType.SetCheck(i,TRUE);
	}
}

void GSimpleSelDialog::OnBnClickedButtonTypeallnot()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listType.GetItemCount();i++)
	{
		m_listType.SetCheck(i,FALSE);
	}
}

void GSimpleSelDialog::OnBnClickedButtonTypefx()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_listType.GetItemCount();i++)
	{
		m_listType.SetCheck(i,!m_listType.GetCheck(i));
	}
}

void GSimpleSelDialog::OnBnClickedCheckAlllayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listLayer.EnableWindow(!m_checkAllLayer.GetCheck());
	m_btnAllLayer.EnableWindow(!m_checkAllLayer.GetCheck());
	m_btnAllLayerNot.EnableWindow(!m_checkAllLayer.GetCheck());
	m_btnLayerFX.EnableWindow(!m_checkAllLayer.GetCheck());
}

void GSimpleSelDialog::OnBnClickedCheckAlltype()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listType.EnableWindow(!m_checkAllType.GetCheck());
	m_btnAllType.EnableWindow(!m_checkAllType.GetCheck());
	m_btnAllTypeNot.EnableWindow(!m_checkAllType.GetCheck());
	m_btnTypeFX.EnableWindow(!m_checkAllType.GetCheck());
}
BOOL GSimpleSelDialog::BuildSelFilter()
{
	if(selFilter!=NULL)
	{
		acutRelRb(selFilter);
	}
	selFilter=NULL;
	if(m_checkAllType.GetCheck()&&m_checkAllLayer.GetCheck())
	{
		AfxMessageBox(_T("不支持同时选中所有图层和所有类型"));
		return FALSE;
	}
	CString Type=_T("");
	if(!m_checkAllType.GetCheck())
	{
		for(int i=0;i<m_listType.GetItemCount();i++)
		{
			if(!m_listType.GetCheck(i))
			{
				continue;
			}
			AcRxClass* pClass=(AcRxClass*)m_listType.GetItemData(i);
			CString strType=m_listType.GetItemText(i,0);
			CString strDxf=pClass->dxfName();
			if(Type.IsEmpty())
			{
				Type=strDxf;
			}
			else
			{
				if(Type.Find(strDxf)<0)
				{
					Type+=_T(",")+strDxf;
				}
				
			}
		}
		if(Type.IsEmpty())
		{
			AfxMessageBox(_T("请至少选择一种实体类型"));
			acutRelRb(selFilter);
			selFilter=NULL;
			return FALSE;
		}
	}
	CString Layer=_T("");
	if(!m_checkAllLayer.GetCheck())
	{
		for(int i=0;i<m_listLayer.GetItemCount();i++)
		{
			if(!m_listLayer.GetCheck(i))
				continue;
			CString strLayer=m_listLayer.GetItemText(i,0);
			if(Layer.IsEmpty())
			{
				Layer=strLayer;
			}
			else
			{
				Layer+=_T(",")+strLayer;
			}
		}
		if(Layer.IsEmpty())
		{
			AfxMessageBox(_T("请至少选择一种实体类型"));
			return FALSE;
		}
	}
	if(m_radioSel.GetCheck())
	{
		//选择
		if(m_checkAllLayer.GetCheck()&&(!m_checkAllType.GetCheck()))
		{
			//所有图层+选定类型
			if(Type.Find(_T("INSERT")) >= 0)
			{
				selFilter=acutBuildList(RTDXF0,Type,100,_T("AcDbBlockReference"),0);
			}
			else
			{
				selFilter=acutBuildList(RTDXF0,Type,0);
			}
			
		}
		else if(m_checkAllType.GetCheck()&&(!m_checkAllLayer.GetCheck()))
		{
			selFilter=acutBuildList(8,Layer,0);
		}
		else
		{
			if(Type.Find(_T("INSERT")) >= 0)
			{
				selFilter=acutBuildList(8,Layer,RTDXF0,Type,100,_T("AcDbBlockReference"),0);
			}
			else
			{
				selFilter=acutBuildList(8,Layer,RTDXF0,Type,0);
			}
		}
	}
	else
	{
		//反选
		if(m_checkAllLayer.GetCheck()&&(!m_checkAllType.GetCheck()))
		{
			//所有图层+选定类型
			if(Type.Find(_T("INSERT")) >= 0)
			{
				selFilter=acutBuildList(-4,_T("<NOT"),RTDXF0,Type,100,_T("AcDbBlockReference"),-4,_T("NOT>"),0);
			}
			else
			{
				selFilter=acutBuildList(-4,_T("<NOT"),RTDXF0,Type,-4,_T("NOT>"),0);
			}

		}
		else if(m_checkAllType.GetCheck()&&(!m_checkAllLayer.GetCheck()))
		{
			selFilter=acutBuildList(-4,_T("<NOT"),8,Layer,-4,_T("NOT>"),0);
		}
		else
		{
			if(Type.Find(_T("INSERT")) >= 0)
			{
				selFilter=acutBuildList(-4,_T("<NOT"),8,Layer,RTDXF0,Type,100,_T("AcDbBlockReference"),-4,_T("NOT>"),0);
			}
			else
			{
				selFilter=acutBuildList(-4,_T("<NOT"),8,Layer,RTDXF0,Type,-4,_T("NOT>"),0);
			}
		}
	}
	return TRUE;
}

void GSimpleSelDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!BuildSelFilter())
	{
		return;
	}
	SaveLastSelect();
	OnOK();
}
void GSimpleSelDialog::SaveLastSelect()
{
	if(m_checkAllLayer.GetCheck())
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("AllLayer"),_T("True"));
	}
	else
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("AllLayer"),_T("False"));
		for(int i=0;i<m_listLayer.GetItemCount();i++)
		{
			CString strLayer=m_listLayer.GetItemText(i,0);
			BOOL bCheck=m_listLayer.GetCheck(i);
			GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("LAYER_")+strLayer,bCheck?_T("True"):_T("False"));
		}
	}
	if(m_checkAllType.GetCheck())
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("AllType"),_T("True"));
	}
	else
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("AllType"),_T("False"));
		for(int i=0;i<m_listType.GetItemCount();i++)
		{
			BOOL bCheck=m_listType.GetCheck(i);
			AcRxClass* pClass=(AcRxClass*)m_listType.GetItemData(i);
			CString strDxf=pClass->dxfName();
			GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("TYPE_")+strDxf,bCheck?_T("True"):_T("False"));

		}
	}
	GUserDataRecord::getInstance()->WriteProfileString(_T("GSimpleSelDialog"),_T("Select"),m_radioSel.GetCheck()?_T("True"):_T("False"));
}
BOOL GSimpleSelDialog::ReadTypeAlias()
{
	TCHAR m_lpFile[MAX_PATH+1]={0};
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("Config\\TypeAlias.xml"));
	// 装载XML字符串
	if (!lpDocument->load(m_lpFile))
	{

		OutputDebugString(_T("LoadXML failed!"));

		return FALSE;
	}

	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	lpDocument->get_documentElement(&lpDocElement);

	// 解析Types
	MSXML2::IXMLDOMNodePtr lpTypes = lpDocElement->selectSingleNode(_bstr_t(_T("Types")));

	if (NULL == lpTypes)
		return FALSE;

	// 解析参数列表
	MSXML2::IXMLDOMNodeListPtr lpNodeList = lpTypes->GetchildNodes();

	MSXML2::IXMLDOMNodePtr lpParam = NULL;

	for ( ; (lpParam = lpNodeList->nextNode()) != NULL ; )
	{
		MSXML2::IXMLDOMNodePtr lpNameNode = lpParam->attributes->getNamedItem(_T("Name"));
		CString strName = (char*)lpNameNode->text;

		MSXML2::IXMLDOMNodePtr lpValueNode = lpParam->attributes->getNamedItem(_T("Alias"));
		CString strValue = (char*)lpValueNode->text;
		if(strName.IsEmpty()||strValue.IsEmpty())
		{
			continue;
		}
		entTypeNameArray.Add(strName);
		entTypeAliasArray.Add(strValue);
	}

	return TRUE;
}
CString GSimpleSelDialog::GetTypeAlias(const CString strName) const
{
	for(int i=0;i<entTypeNameArray.GetCount();i++)
	{
		if(strName.CompareNoCase(entTypeNameArray.GetAt(i))==0)
		{
			return entTypeAliasArray.GetAt(i);
		}
	}
	return strName;
}
CString GSimpleSelDialog::GetTypeName(const CString strAlias) const
{
	for(int i=0;i<entTypeNameArray.GetCount();i++)
	{
		if(strAlias.CompareNoCase(entTypeAliasArray.GetAt(i))==0)
		{
			return entTypeNameArray.GetAt(i);
		}
	}
	return strAlias;
}
