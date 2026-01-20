#include "Stdafx.h"
#include "GMenu.h"
#include "AcCommFunction.h"
extern BOOL    Glb_bReg;
extern CString Glb_strRegDate;
extern CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName);
CStringArray m_hideMenuArray;
#if _MSC_VER>=1700
//#pragma comment(lib, "acad21.lib")
#elif _MSC_VER>=1600
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys19(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys19(X64)\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys19\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys19\\CADGlobalTools.lib")
#endif
#elif _MSC_VER>=1500
#ifdef _WIN64
#pragma comment(lib, "..\\Release\\sys18(X64)\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys18(X64)\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys18\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys18\\CADGlobalTools.lib")
#endif
#elif _MSC_VER>=1400
#pragma comment(lib, "..\\Release\\sys17\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys17\\CADGlobalTools.lib")
#else
#ifdef ACAD_2004 
#pragma comment(lib, "..\\Release\\sys16.0\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys16.0\\CADGlobalTools.lib")
#else
#pragma comment(lib, "..\\Release\\sys16\\GGridCtrl.lib")
#pragma comment(lib, "..\\Release\\sys16\\CADGlobalTools.lib")
#endif
#endif
namespace GMenu
{
	/*!
	@brief 菜单项
	*/
	struct SMenuItem
	{
		string name;
		string command;
		string reg;
		string tip;
		bool IsEnable()
		{
			if(reg.empty())
			{
				return true;
			}
			CString strReg(reg.c_str());
			if(!Glb_bReg)
			{
				return false;
			}
			if(Glb_strRegDate.CompareNoCase(strReg)<0)
			{
				GLog::WriteLine(_T("%s>%s"),strReg,Glb_strRegDate);
				return false;
			}
			return true;
		}
	};
	typedef struct SMenuItem MenuItem;
	static UINT CodePage=CP_THREAD_ACP;
	MenuItem ReadItem(MSXML2::IXMLDOMNodePtr lpMenuNode)
	{
		MenuItem item;
		MSXML2::IXMLDOMNodePtr lpNameNode = lpMenuNode->attributes->getNamedItem("text");
		item.name=lpNameNode!=NULL?string((const char*)lpNameNode->text):"";
		MSXML2::IXMLDOMNodePtr lpRegNode = lpMenuNode->attributes->getNamedItem("reg");
		item.reg=lpRegNode!=NULL?string((const char*)lpRegNode->text):"";
		MSXML2::IXMLDOMNodePtr lpCommandNode = lpMenuNode->attributes->getNamedItem("Cmd");
		if(lpCommandNode!=NULL)
		{
			item.command=string((const char*)lpCommandNode->text);
			if(item.command.length()!=0)
			{
				if(item.command.at(item.command.length()-1)!=' ')
				{
					item.command+=" ";
				}
			}
		}
		else
		{
			item.command="";
		}
		MSXML2::IXMLDOMNodePtr lpTipNode = lpMenuNode->attributes->getNamedItem("tip");
		if(lpTipNode!=NULL)
		{
			item.tip=string((const char*)lpTipNode->text);
		}
		else
		{
			item.tip="";
		}
		return item;
	}
	int ValidChildMenuCount(MSXML2::IXMLDOMNodePtr _lpMenuNode)
	{
		int nCount=0;
		MSXML2::IXMLDOMNodeListPtr lpMenuNodeList=_lpMenuNode->selectNodes(_T("MenuItem"));
		MSXML2::IXMLDOMNodePtr lpMenuNode = NULL;
		for ( ; (lpMenuNode = lpMenuNodeList->nextNode()) != NULL ; )
		{
			MenuItem item=ReadItem(lpMenuNode);
			if(strcmpi(item.name.c_str(),"-")==0)
			{
				//分隔符
			}
			else if(item.command.length()==0)
			{
				//分组
				if(ValidChildMenuCount(lpMenuNode)>0)
				{
					nCount++;
				}
			}
			else
			{
				//正常命令
				CString strName=CString(item.name.c_str());
				strName.Trim();
				if(GStringHelper::IsStringInArray(m_hideMenuArray,strName))
				{
					continue;
				}
				nCount++;
			}

		}
		return nCount;
	}
	//同级下面有效的节点
	int ValidNextMenuCount(MSXML2::IXMLDOMNodePtr _lpMenuNode)
	{
		int nCount=0;
		MSXML2::IXMLDOMNodePtr lpMenuNode = _lpMenuNode;
		for ( ; (lpMenuNode = lpMenuNode->nextSibling) != NULL ; )
		{
			MenuItem item=ReadItem(lpMenuNode);
			if(strcmpi(item.name.c_str(),"-")==0)
			{
				//分隔符
			}
			else if(item.command.length()==0)
			{
				//分组
				if(ValidChildMenuCount(lpMenuNode)>0)
				{
					nCount++;
				}
			}
			else
			{
				//正常命令
				CString strName=CString(item.name.c_str());
				strName.Trim();
				if(GStringHelper::IsStringInArray(m_hideMenuArray,strName))
				{
					continue;
				}
				nCount++;
			}

		}
		return nCount;
	}
	void AddChildMenu(IAcadPopupMenu* pPopUpMenu,MSXML2::IXMLDOMNodePtr _lpMenuNode)
	{
		MSXML2::IXMLDOMNodeListPtr lpMenuNodeList=_lpMenuNode->selectNodes(_T("MenuItem"));
		MSXML2::IXMLDOMNodePtr lpMenuNode = NULL;
		VARIANT index1;
		VariantInit(&index1);
		V_VT(&index1) = VT_I4;
		int nValue=0;
		bool bAllowSeparator=false;
		for ( ; (lpMenuNode = lpMenuNodeList->nextNode()) != NULL ; )
		{
			V_I4(&index1) = nValue;
			MenuItem item=ReadItem(lpMenuNode);
			if(strcmpi(item.name.c_str(),"--")==0)
			{
				if(ValidNextMenuCount(lpMenuNode)==0)
				{
					GLog::WriteLine(_T("隐藏分隔符，下面无有效节点"));
					continue;
				}
				if(!bAllowSeparator)
				{
					GLog::WriteLine(_T("隐藏分隔符，不允许第一个或最后一个是分隔符，不允许两个分隔符连在一起"));
					continue;
				}
				bAllowSeparator=false;
				//分隔符
				GLog::WriteLine(_T("增加分隔符"));
				IAcadPopupMenuItem* pMenuItem=NULL;
				pPopUpMenu->AddSeparator(index1,&pMenuItem);
				nValue++;
			}
			else if(item.command.length()==0)
			{
				if(ValidChildMenuCount(lpMenuNode)==0)
				{
					GLog::WriteLine(_T("隐藏分组:%s，因其下所有子项隐藏"),CString(item.name.c_str()));
					continue;
				}
				bAllowSeparator=true;
				//分组
				GLog::WriteLine(_T("增加分组:%s"),CString(item.name.c_str()));
				IAcadPopupMenu* pMenu=NULL;
				WCHAR wstrMenuItemName[256];
				MultiByteToWideChar(CodePage, 0, item.name.c_str(), -1, wstrMenuItemName, 256); 
				HRESULT hr=pPopUpMenu->AddSubMenu(index1,wstrMenuItemName,&pMenu);
				if(FAILED(hr))
				{
					GLog::WriteLine(_T("增加分组%s失败:返回值0x%x"),CString(item.name.c_str()),hr);
				}
				else
				{
					nValue++;
					AddChildMenu(pMenu,lpMenuNode);
				}

			}
			else
			{
				CString strName=CString(item.name.c_str());
				strName.Trim();
				if(GStringHelper::IsStringInArray(m_hideMenuArray,strName))
				{
					GLog::WriteLine(_T("隐藏命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
					continue;
				}
				bAllowSeparator=true;
				GLog::WriteLine(_T("增加命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
				//正常命令
				WCHAR wstrMenuItemName[256];
				MultiByteToWideChar(CodePage, 0, item.name.c_str(), -1, wstrMenuItemName, 256); 
				WCHAR wstrMenuItemMacro[256];
				MultiByteToWideChar(CodePage, 0, item.command.c_str(), -1, wstrMenuItemMacro, 256);
				WCHAR wstrMenuItemTip[256];
				MultiByteToWideChar(CodePage, 0, item.tip.c_str(), -1, wstrMenuItemTip, 256);
				IAcadPopupMenuItem* pMenuItem;
				HRESULT hr=pPopUpMenu->AddMenuItem(index1,wstrMenuItemName,wstrMenuItemMacro,&pMenuItem);
				if(FAILED(hr))
				{
					GLog::WriteLine(_T("增加命令%s(%s)失败:返回值0x%x"),CString(item.name.c_str()),CString(item.command.c_str()),hr);
				}
				else
				{
					if(!item.IsEnable())
					{
						pMenuItem->put_Enable(VARIANT_FALSE);
						pMenuItem->put_HelpString(L"您需要注册软件或更新注册码才可以使用当前功能");
					}
					else
					{
						pMenuItem->put_HelpString(wstrMenuItemTip);
					}
					nValue++;
				}
			}

		}
	}
	bool AddMenu()
	{
		m_hideMenuArray.RemoveAll();
		CString str=GUserDataRecord::getInstance()->GetProfileString(_T("MenuHide"),_T("HideMenus"),4096,_T(""));
		GStringHelper::Split(str,_T(";"),m_hideMenuArray,true);
		IAcadApplication* pAcad;
		IAcadMenuBar* pMenuBar;
		IAcadMenuGroups* pMenuGroups;
		IAcadMenuGroup* pMenuGroup;
		IAcadPopupMenus* pPopUpMenus;
		IAcadPopupMenu* pPopUpMenu;
		IAcadPopupMenuItem* pPopUpMenuItem;

		HRESULT hr = NOERROR;
		LPUNKNOWN pUnk = NULL;
		LPDISPATCH pAcadDisp = NULL;
		pAcadDisp = acedGetAcadWinApp()->GetIDispatch(TRUE);
		if (pAcadDisp == NULL)
		{
			AfxMessageBox(_T("加载菜单时错误:找不到ACAD主程序"));
			return false;
		}
		pAcad = (IAcadApplication*)pAcadDisp;
		if (FAILED(hr))
		{
			AfxMessageBox(_T("加载菜单时错误:查询ACAD接口失败"));
			return false;
		}

		pAcad->put_Visible(true);
		pAcad->get_MenuBar(&pMenuBar);
		pAcad->get_MenuGroups(&pMenuGroups);
		pAcad->Release();
		long numberOfMenus;
		pMenuBar->get_Count(&numberOfMenus);
		pMenuBar->Release();

		VARIANT index;
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;
		pMenuGroups->Item(index, &pMenuGroup);
		pMenuGroups->Release();
		pMenuGroup->get_Menus(&pPopUpMenus);
		pMenuGroup->Release();
		WCHAR wstrMenuName[256];
		if(GlobalSystemValue::instance.IsHaoBangshou())
		{
			MultiByteToWideChar(CP_ACP, 0, "好帮手(&B)", -1, wstrMenuName, 256);
		}
		else if(GlobalSystemValue::instance.IsZhongXianCAD())
		{
			MultiByteToWideChar(CP_ACP, 0, "中线CAD(&B)", -1, wstrMenuName, 256);
		}
		else if(GlobalSystemValue::instance.IsXiaoZhuShou())
		{
			MultiByteToWideChar(CP_ACP, 0, "小助手(&B)", -1, wstrMenuName, 256);
		}
		pPopUpMenus->Add(wstrMenuName, &pPopUpMenu);
		if (pPopUpMenu == NULL)
		{
			return true;
		}//增加菜单
		TCHAR lpPath[MAX_PATH+1]={0};
		HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
		if(hModule==NULL)
		{
			hModule=_hdllInstance;
		}
		GetModuleFileName(hModule,lpPath,MAX_PATH);
		PathRemoveFileSpec(lpPath);
		PathRemoveFileSpec(lpPath);
		TCHAR lpPath1[MAX_PATH+1]={0};
		lstrcpy(lpPath1,lpPath);
		PathAppend(lpPath1,_T("config\\ScreenMenu.xml"));
		if(!PathFileExists(lpPath1))
		{
			GLog::WriteLine(_T("未找到菜单文件"));
			pPopUpMenu->Release();
			return true;
		}
		GLog::WriteLine(_T("加载菜单"));
		MSXML2::IXMLDOMDocumentPtr lpDocument;
		hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

		if ( FAILED(hr) ) 
		{
			_com_raise_error(hr);
			return true;
		}
		//加载XML文件
		if(!lpDocument->load(lpPath1))
		{
			GLog::WriteLine(_T("读取菜单文件出错"));
			return true;
		}
		MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
		hr=lpDocument->get_documentElement(&lpDocElement);
		if ( FAILED(hr) ) 
		{
			GLog::WriteLine(_T("读取菜单文件出错,未找到根节点"));
			_com_raise_error(hr);
			return true;
		}
		MSXML2::IXMLDOMNodePtr lpMenus=lpDocElement->selectSingleNode(_T("Menus"));
		if(lpMenus==NULL)
		{
			GLog::WriteLine(_T("读取菜单文件出错,未找到Menus节点"));
			return true;
		}
		MSXML2::IXMLDOMNodeListPtr _lpMenuList=lpMenus->selectNodes(_T("Menu"));

		MSXML2::IXMLDOMNodePtr _lpMenu = NULL;
		for ( ; (_lpMenu = _lpMenuList->nextNode()) != NULL ; )
		{
			CString strId=GetNodeAttrValue(_lpMenu,_T("ID"));
			if(strId!=_T("ID_Cadgj_Com_Menu"))
			{
				continue;
			}
			break;
		}
		if(_lpMenu==NULL)
		{
			GLog::WriteLine(_T("读取菜单文件出错,未找到ID_GhCad_Menu节点"));
			return true;
		}
		MSXML2::IXMLDOMNodeListPtr lpMenuNodeList=_lpMenu->selectNodes(_T("MenuItem"));
		MSXML2::IXMLDOMNodePtr lpMenuNode = NULL;
		VARIANT index1;
		VariantInit(&index1);
		V_VT(&index1) = VT_I4;
		int nValue=0;
		bool bAllowSeparator=false;
		for ( ; (lpMenuNode = lpMenuNodeList->nextNode()) != NULL ; )
		{
			V_I4(&index1) = nValue;
			MenuItem item=ReadItem(lpMenuNode);
			if(strcmpi(item.name.c_str(),"--")==0)
			{
				//分隔符
				if(ValidNextMenuCount(lpMenuNode)==0)
				{
					GLog::WriteLine(_T("隐藏分隔符，下面无有效节点"));
					continue;
				}
				if(!bAllowSeparator)
				{
					GLog::WriteLine(_T("隐藏分隔符，不允许第一个或最后一个是分隔符，不允许两个分隔符连在一起"));
					continue;
				}
				bAllowSeparator=false;
				GLog::WriteLine(_T("增加分隔符"));
				IAcadPopupMenuItem* pMenuItem=NULL;
				pPopUpMenu->AddSeparator(index1,&pMenuItem);
				nValue++;
			}
			else if(item.command.length()==0)
			{
				if(ValidChildMenuCount(lpMenuNode)==0)
				{
					GLog::WriteLine(_T("隐藏分组:%s，因其下所有子项隐藏"),CString(item.name.c_str()));
					continue;
				}
				//分组
				bAllowSeparator=true;
				GLog::WriteLine(_T("增加分组:%s"),CString(item.name.c_str()));
				IAcadPopupMenu* pMenu=NULL;
				WCHAR wstrMenuItemName[256];
				MultiByteToWideChar(CodePage, 0, item.name.c_str(), -1, wstrMenuItemName, 256); 
				HRESULT hr=pPopUpMenu->AddSubMenu(index1,wstrMenuItemName,&pMenu);
				if(FAILED(hr))
				{
					GLog::WriteLine(_T("增加分组%s失败:返回值0x%x"),CString(item.name.c_str()),hr);
				}
				else
				{
					nValue++;
					AddChildMenu(pMenu,lpMenuNode);
				}

			}
			else
			{
				CString strName=CString(item.name.c_str());
				strName.Trim();
				//GLog::WriteLine(_T("判断命令是否需要隐藏:%s(%s)"),strName,CString(item.command.c_str()));
				if(GStringHelper::IsStringInArray(m_hideMenuArray,strName))
				{
					GLog::WriteLine(_T("隐藏命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
					continue;
				}
				bAllowSeparator=true;
				GLog::WriteLine(_T("增加命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
				//正常命令
				WCHAR wstrMenuItemName[256];
				MultiByteToWideChar(CodePage, 0, item.name.c_str(), -1, wstrMenuItemName, 256); 
				WCHAR wstrMenuItemMacro[256];
				MultiByteToWideChar(CodePage, 0, item.command.c_str(), -1, wstrMenuItemMacro, 256);
				WCHAR wstrMenuItemTip[256];
				MultiByteToWideChar(CodePage, 0, item.tip.c_str(), -1, wstrMenuItemTip, 256);
				IAcadPopupMenuItem* pMenuItem;
				HRESULT hr=pPopUpMenu->AddMenuItem(index1,wstrMenuItemName,wstrMenuItemMacro,&pMenuItem);
				if(FAILED(hr))
				{
					GLog::WriteLine(_T("增加命令%s(%s)失败:返回值0x%x"),CString(item.name.c_str()),CString(item.command.c_str()),hr);
				}
				else
				{
					if(!item.IsEnable())
					{
						pMenuItem->put_Enable(VARIANT_FALSE);
						pMenuItem->put_HelpString(L"您需要注册软件或更新注册码才可以使用当前功能");
					}
					else
					{
						pMenuItem->put_HelpString(wstrMenuItemTip);
					}
					nValue++;
				}
			}

		}
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = numberOfMenus;
		pPopUpMenu->InsertInMenuBar(index);
		pPopUpMenu->Release();
		return true;
	}
}
