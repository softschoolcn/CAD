// GCommandHideDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GCommandHideDialog.h"
#include <string>
using namespace std;
struct SMenuItem
{
	string name;
	string command;
	string reg;
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
	return item;
}
// GCommandHideDialog 对话框

IMPLEMENT_DYNAMIC(GCommandHideDialog, CDialog)

GCommandHideDialog::GCommandHideDialog(CWnd* pParent /*=NULL*/)
: CDialog(GCommandHideDialog::IDD, pParent)
{

}

GCommandHideDialog::~GCommandHideDialog()
{
}

void GCommandHideDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MAIN,m_list);
}


BEGIN_MESSAGE_MAP(GCommandHideDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QX, &GCommandHideDialog::OnBnClickedButtonQx)
	ON_BN_CLICKED(IDC_BUTTON_QBX, &GCommandHideDialog::OnBnClickedButtonQbx)
	ON_BN_CLICKED(IDC_BUTTON_FX, &GCommandHideDialog::OnBnClickedButtonFx)
END_MESSAGE_MAP()


// GCommandHideDialog 消息处理程序
//执行设置
BOOL GCommandHideDialog::ExcSet()
{
	CString str(_T(""));
	m_hideMenuArray.RemoveAll();
	int nNodeHide=0;
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetCheck(i))
		{
			nNodeHide++;
			continue;
		}
		m_hideMenuArray.Add(m_list.GetItemText(i,0));
		if(str.IsEmpty())
		{
			str=m_list.GetItemText(i,0);
		}
		else
		{
			str+=_T(";")+m_list.GetItemText(i,0);
		}
	}
	if(nNodeHide==0)
	{
		if(AfxMessageBox(_T("您隐藏了好帮手所有的命令，这将可能导致CAD好帮手运行出错，是否继续？"),MB_YESNO)!=IDYES)
		{
			return FALSE;
		}
	}
	GUserDataRecord::getInstance()->WriteProfileString(_T("MenuHide"),_T("HideMenus"),str);
	return TRUE;
}
void GCommandHideDialog::AddCommand(CString strName)
{
	int nIndex=m_list.InsertItem(m_list.GetItemCount(),strName);
	m_list.SetCheck(nIndex,!GStringHelper::IsStringInArray(m_hideMenuArray,strName));
}
bool GCommandHideDialog::ReadChildCommands(MSXML2::IXMLDOMNodePtr _lpMenuNode)
{
	MSXML2::IXMLDOMNodeListPtr lpMenuNodeList=_lpMenuNode->selectNodes(_T("MenuItem"));
	MSXML2::IXMLDOMNodePtr lpMenuNode = NULL;
	for ( ; (lpMenuNode = lpMenuNodeList->nextNode()) != NULL ; )
	{
		MenuItem item=ReadItem(lpMenuNode);
		if(strcmpi(item.name.c_str(),"--")==0)
		{
			//分隔符
			GLog::WriteLine(_T("增加分隔符"));
		}
		else if(item.command.length()==0)
		{
			//分组
			GLog::WriteLine(_T("增加分组:%s"),CString(item.name.c_str()));
			ReadChildCommands(lpMenuNode);

		}
		else
		{
			GLog::WriteLine(_T("增加命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
			//正常命令
			AddCommand(CString(item.name.c_str()));
		}

	}
	return true;
}
bool GCommandHideDialog::ReadCommands()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("config\\ScreenMenu.xml"));
	if(!PathFileExists(lpPath1))
	{
		GLog::WriteLine(_T("未找到菜单文件"));
		return true;
	}
	GLog::WriteLine(_T("加载菜单"));
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

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
	for ( ; (lpMenuNode = lpMenuNodeList->nextNode()) != NULL ; )
	{
		MenuItem item=ReadItem(lpMenuNode);
		if(strcmpi(item.name.c_str(),"--")==0)
		{
			//分隔符
			GLog::WriteLine(_T("增加分隔符"));
		}
		else if(item.command.length()==0)
		{
			//分组
			GLog::WriteLine(_T("增加分组:%s"),CString(item.name.c_str()));
			ReadChildCommands(lpMenuNode);

		}
		else
		{
			GLog::WriteLine(_T("增加命令:%s(%s)"),CString(item.name.c_str()),CString(item.command.c_str()));
			AddCommand(CString(item.name.c_str()));
		}

	}
	return true;
}
BOOL GCommandHideDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str=GUserDataRecord::getInstance()->GetProfileString(_T("MenuHide"),_T("HideMenus"),4096,_T(""));
	GStringHelper::Split(str,_T(";"),m_hideMenuArray,true);
	m_list.SetExtendedStyle(LVS_EX_CHECKBOXES);
	ReadCommands();
	return TRUE;
}
void GCommandHideDialog::OnBnClickedButtonQx()
{
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}

void GCommandHideDialog::OnBnClickedButtonQbx()
{
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,FALSE);
	}
}

void GCommandHideDialog::OnBnClickedButtonFx()
{
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetCheck(i,!m_list.GetCheck(i));
	}
}
