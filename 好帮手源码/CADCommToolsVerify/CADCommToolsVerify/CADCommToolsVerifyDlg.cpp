// CADCommToolsVerifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CADCommToolsVerify.h"
#include "CADCommToolsVerifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCADCommToolsVerifyDlg 对话框




CCADCommToolsVerifyDlg::CCADCommToolsVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCADCommToolsVerifyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCADCommToolsVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCADCommToolsVerifyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCADCommToolsVerifyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCADCommToolsVerifyDlg 消息处理程序

BOOL CCADCommToolsVerifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCADCommToolsVerifyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCADCommToolsVerifyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCADCommToolsVerifyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//获取广告的md5
BOOL GetAdMD5(CString strSubPath,CArray<byte>& byteArray)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=NULL;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,strSubPath);
	PathAppend(lpPath,_T("CADCommTools.arx"));
	CFile file;
	if(!file.Open(lpPath,CFile::modeRead|CFile::typeBinary))
	{
		return FALSE;
	}
	DWORD dLen=file.GetLength();
	if(dLen==0)
	{
		file.Close();
		return FALSE;
	}
	byte* bytes=new byte[dLen];
	file.Read(bytes,dLen);
	file.Close();
	MD5 md5(bytes,dLen);
	std::string str=md5.toString();
	for(int i=0;i<str.length();i++)
	{
		byteArray.Add(str.at(i));
	}
	delete[] bytes;
	bytes=NULL;
	return TRUE;
}
BOOL GetVerifyString(CString strSubPath,CString& strVfy)
{
	CArray<byte> byteArray;
	if(!GetAdMD5(strSubPath,byteArray))
	{
		return FALSE;
	}
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	CByteString bStr;
	strVfy=bStr.toString(byteArray);
	return TRUE;
}
void vfyAd(CString strSubPath)
{
	CString strVfy;
	if(!GetVerifyString(strSubPath,strVfy))
	{
		return;
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=NULL;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,strSubPath);
	PathAppend(lpPath,_T("CADCommTools.ini"));
	WritePrivateProfileString(_T("System"),_T("VfyStr"),strVfy,lpPath);
}
void CCADCommToolsVerifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	CStringArray strArr;
	strArr.Add(_T("sys16"));
	strArr.Add(_T("sys16.0"));
	strArr.Add(_T("sys17"));
	strArr.Add(_T("sys18"));
	strArr.Add(_T("sys19"));
	strArr.Add(_T("sys20"));
	strArr.Add(_T("sys18(X64)"));
	strArr.Add(_T("sys19(X64)"));
	strArr.Add(_T("sys20(X64)"));
	for(int i=0;i<strArr.GetCount();i++)
	{
		vfyAd(strArr.GetAt(i));
	}
}
