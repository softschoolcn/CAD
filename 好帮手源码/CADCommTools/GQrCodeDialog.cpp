// GQrCodeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GQrCodeDialog.h"

// GQrCodeDialog 对话框

IMPLEMENT_DYNAMIC(GQrCodeDialog, CDialog)

GQrCodeDialog::GQrCodeDialog(CWnd* pParent /*=NULL*/)
: CDialog(GQrCodeDialog::IDD, pParent)
{

}

GQrCodeDialog::~GQrCodeDialog()
{
}

void GQrCodeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GQrCodeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GQrCodeDialog::OnBnClickedOk)
END_MESSAGE_MAP()

void WriteBlobFile(LPCTSTR lpFile, const CString strText)
{
	try
	{
		CFile file(lpFile, CFile::modeCreate | CFile::modeWrite|CFile::typeBinary);
		char* szBuf=NULL;
		GStringHelper::ConvertToLPCSTR(szBuf,strText);
		file.Write(szBuf,strlen(szBuf));
		free(szBuf);
		file.Close();
	}
	catch (CFileException* pe) 
	{
		pe->Delete();
	}
}
void ReadBlobFile(LPCTSTR lpFile, CString& strText)
{
	try
	{
		CFile file(lpFile, CFile::modeRead|CFile::typeBinary);
		int nLen = file.GetLength();
		if(nLen == 0)
		{
			file.Close(); 
			return;
		}
		char* strBuf = new char[nLen+1];
		memset(strBuf,0,sizeof(char)*(nLen+1));
		file.Read(strBuf,nLen);
		file.Close();
		strText = strBuf;
		delete [] strBuf;
		file.Close();
	}
	catch (CFileException* pe) 
	{
		pe->Delete();
	}
}
// GQrCodeDialog 消息处理程序
BOOL GQrCodeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	TCHAR m_lpFile[MAX_PATH+1]={0};
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("QrCode.txt"));
	if(PathFileExists(m_lpFile))
	{
		ReadBlobFile(m_lpFile,m_strText);
	}
	else
	{
		m_strText=GlobalSystemValue::instance.getSoftName();
	}
	SetDlgItemText(IDC_EDIT1,m_strText);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GQrCodeDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItemText(IDC_EDIT1,strText);
	if(strText.IsEmpty())
	{
		AfxMessageBox(_T("请输入二维码内容"));
		return;
	}
	m_strText=strText;
	std::string str=GStringHelper::ConvertToLPCSTR(m_strText);
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	TCHAR m_lpFile[MAX_PATH+1]={0};
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	if(!PathFileExists(m_lpFile))
	{
		CreateDirectory(m_lpFile,NULL);
	}
	PathAppend(m_lpFile,_T("QrCode.txt"));
	WriteBlobFile(m_lpFile,m_strText);
	OnOK();
}
