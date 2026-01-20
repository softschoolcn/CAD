// GRegDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GRegDialog.h"
#include "GComputerId.h"
#include "DES.h"
#include "ByteString.h"
extern BOOL    Glb_bReg;
extern CString Glb_strRegDate;
struct GSpecialRegItem
{
	CString m_strName;
	CString m_strKey;//必须为八位
};
CArray<GSpecialRegItem> Glb_SpecialRegItemArray;
// GRegDialog 对话框
CString _GetSpecialRegName(CString strKey)
{
	for(int i=0;i<Glb_SpecialRegItemArray.GetCount();i++)
	{
		GSpecialRegItem& item=Glb_SpecialRegItemArray.GetAt(i);
		if(item.m_strKey.Compare(strKey)==0)
		{
			return item.m_strName;
		}
	}
	return _T("未知功能");
}
BOOL _CheckSpecialReg(CString strKey,CString strId)
{
	if(strId.IsEmpty())
	{
		GComputerId id;
		strId=id.GetComputerId();
	}
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("Licence.ini"));
	if(PathFileExists(m_lpFile))
	{
		TCHAR lpResult[1024+1]={0};
		if(GetPrivateProfileString(_T("System"),strKey,_T(""),lpResult,1024,m_lpFile))
		{
			CString strRegKey=lpResult;
			CByteString byteStr;
			BYTE* data=NULL;
			long byteLen=0;
			byteStr.toByte(data,byteLen,strRegKey);
			AcArray<byte> byteArray;
			for(int i=0;i<byteLen;i++)
			{
				byteArray.append(data[i]);
			}
			delete[] data;
			data=NULL;
			if(byteArray.logicalLength()!=24)
			{
				return FALSE;
			}
			GDES::TransformData(byteArray,FALSE);
			GDES::TransformData(byteArray,FALSE);
			GDES::TransformData(byteArray,FALSE);
			GDES::TransformData(byteArray,FALSE);
			AcArray<byte> byteDates;
			CString strComputerId;
			for(int i=0;i<8;i++)
			{
				byteDates.append(byteArray.at(3*i));
				strComputerId+=(TCHAR)byteArray.at(3*i+1);
				strComputerId+=(TCHAR)byteArray.at(3*i+2);
			}
			if(strComputerId.Compare(strId)!=0)
			{
				return FALSE;
			}
			GDES::TransformData(byteDates,FALSE);
			GDES::TransformData(byteDates,FALSE);
			GDES::TransformData(byteDates,FALSE);
			GDES::TransformData(byteDates,FALSE);
			CString strDate;
			for(int i=0;i<byteDates.logicalLength();i++)
			{
				strDate+=(TCHAR)byteDates.at(i);
			}
			if(strDate.Compare(strKey)!=0)
			{
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}
void InitSpecialRegItemArray()
{
	GSpecialRegItem item;
	item.m_strName=_T("文件夹对比");
	item.m_strKey=_T("DwgCmpEx");
	Glb_SpecialRegItemArray.Add(item);
}
IMPLEMENT_DYNAMIC(GRegDialog, CDialog)

GRegDialog::GRegDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GRegDialog::IDD, pParent)
{

}

GRegDialog::~GRegDialog()
{
}

void GRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_LINK,m_link);
	DDX_Control(pDX,IDC_TAB_MAIN,m_tab);
	DDX_Control(pDX, IDC_GRID, mGrid);
}


BEGIN_MESSAGE_MAP(GRegDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GRegDialog::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &GRegDialog::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()

// GRegDialog 消息处理程序
BOOL GRegDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0,_T("软件注册"));
	//m_tab.InsertItem(1,_T("专业功能注册"));
	CRect rc;
	GetClientRect(rc);
	CRect rc2;
	rc2.top=rc.top+10;
	rc2.bottom=rc.bottom -50;
	rc2.left=rc.left+10;
	rc2.right=rc.right-10;
	m_tab.MoveWindow(rc2);

	rc2.top+=25;
	rc2.bottom-=10;
	rc2.left+=10;
	rc2.right-=10;
	mGrid.MoveWindow(rc2);
	GComputerId id;
	CString strId=id.GetComputerId();
	SetDlgItemText(IDC_EDIT_ID,strId);
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("Licence.ini"));
	if(PathFileExists(m_lpFile))
	{
		TCHAR lpResult[1024+1]={0};
		if(GetPrivateProfileString(_T("System"),_T("RegKey"),_T(""),lpResult,1024,m_lpFile))
		{
			SetDlgItemText(IDC_EDIT_REGKEY,lpResult);
			CString strTmp;
			VALID_REG_KEY(strTmp,lpResult)
			if(!strTmp.IsEmpty())
			{
				CString strText;
				strText.Format(_T("您的授权状态为:已注册(%s),感谢您对本软件的支持"),strTmp);
				SetDlgItemText(IDC_STATIC_TIP,strText);
			}
		}


	}
	m_link.SetURL(GlobalSystemValue::instance.getRegWebSite());
	CGridCell* pCell = new CGridCell();
	mGrid.SetDefaultCellType(pCell->GetRuntimeClass());
	int nCol=0;
	nCol=mGrid.InsertColumn(_T("     命令名称     "));
	mGrid.SetColumnWidth(nCol,120);
	nCol=mGrid.InsertColumn(_T("注册码"));
	mGrid.SetColumnWidth(nCol,350);
	nCol=mGrid.InsertColumn(_T("注册状态"));
	mGrid.SetColumnWidth(nCol,150);
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetFixedColumnCount(1);
	mGrid.AutoSizeColumn(0);
	mGrid.ShowWindow(SW_HIDE);
	for(int i=0;i<Glb_SpecialRegItemArray.GetCount();i++)
	{
		GSpecialRegItem& item=Glb_SpecialRegItemArray.GetAt(i);
		int row = mGrid.InsertRow(item.m_strName);  //插入命令名称
		mGrid.SetItemState(row,2,mGrid.GetItemState(row,1)|GVIS_READONLY);
		TCHAR lpResult[1024+1]={0};
		if(GetPrivateProfileString(_T("System"),item.m_strKey,_T(""),lpResult,1024,m_lpFile))
		{
			mGrid.SetItemText(row,1,lpResult);
		}
		if(_CheckSpecialReg(item.m_strKey,_T("")))
		{
			mGrid.SetItemText(row,2,_T("已注册"));
		}
		else
		{
			mGrid.SetItemText(row,2,_T("未注册"));
		}
	}
	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}

void GRegDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strId;
	GetDlgItemText(IDC_EDIT_ID,strId);
	if(lstrlen(strId)!=16)
	{
		AfxMessageBox(_T("非常抱歉，您的机器计算机器码出现未知错误，如您需要使用注册功能，请您联系作者"));
		return;
	}
	CString strKey;
	GetDlgItemText(IDC_EDIT_REGKEY,strKey);
	CString strRegDate=_T("");
	VALID_REG_KEY(strRegDate,strKey);
	if(strRegDate.IsEmpty())
	{
		return;
	}
	Glb_bReg=TRUE;
	Glb_strRegDate=strRegDate;
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	if(!PathFileExists(m_lpFile))
	{
		CreateDirectory(m_lpFile,NULL);
	}
	PathAppend(m_lpFile,_T("Licence.ini"));
	WritePrivateProfileString(_T("System"),_T("RegKey"),strKey,m_lpFile);
	CStringArray strRegSpecialRegItemArray;
	for(int i=0;i<Glb_SpecialRegItemArray.GetCount();i++)
	{
		GSpecialRegItem& item=Glb_SpecialRegItemArray.GetAt(i);
		WritePrivateProfileString(_T("System"),item.m_strKey,mGrid.GetItemText(i+1,1),m_lpFile);
		if(_CheckSpecialReg(item.m_strKey,_T("")))
		{
			strRegSpecialRegItemArray.Add(item.m_strName);
		}
	}
	CString strText;
	strText.Format(_T("注册成功(%s)，感谢您对本软件的支持"),Glb_strRegDate);
	if(strRegSpecialRegItemArray.GetCount()>0)
	{
		strText+=_T("\r\n您同时还可以使用软件提供的以下专业功能");
		for(int i=0;i<strRegSpecialRegItemArray.GetCount();i++)
		{
			CString strTmp;
			strTmp.Format(_T("\r\n%d：%s"),i+1,strRegSpecialRegItemArray.GetAt(i));
			strText+=strTmp;
		}
	}
	//AfxMessageBox(strText);
	OnOK();
}

void GRegDialog::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCurSel = m_tab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		ShowTabSel(0);
		break;
	case 1:
		ShowTabSel(1);
		break;
	default:
		break;
	}
}
void GRegDialog::ShowTabSel(int nSel)
{
	CArray<int> ids;
	ids.Add(IDC_STATIC1);
	ids.Add(IDC_STATIC2);
	ids.Add(IDC_STATIC3);
	ids.Add(IDC_EDIT_ID);
	ids.Add(IDC_EDIT_REGKEY);
	ids.Add(IDC_STATIC_TIP);
	ids.Add(IDC_STATIC_LINK);
	for(int i=0;i<ids.GetCount();i++)
	{
		GetDlgItem(ids.GetAt(i))->ShowWindow(nSel==0?SW_SHOW:SW_HIDE);
	}
	mGrid.ShowWindow(nSel==1?SW_SHOW:SW_HIDE);

}
