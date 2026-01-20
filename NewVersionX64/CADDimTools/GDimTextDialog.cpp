// GDimTextDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GDimTextDialog.h"


// GDimTextDialog 对话框

IMPLEMENT_DYNAMIC(GDimTextDialog, CDialog)

GDimTextDialog::GDimTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GDimTextDialog::IDD, pParent)
{

}

GDimTextDialog::~GDimTextDialog()
{
}

void GDimTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST2,m_list);
	DDX_Control(pDX,IDC_EDIT1,m_wndEdit);
}
BOOL GDimTextDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetModuleFileName(_hdllInstance,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig\\习惯标注用语.txt"));
	CStringArray strTextArray;
	if(PathFileExists(m_lpFile))
	ReadTxtFile(m_lpFile,strTextArray);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("序号"), LVCFMT_LEFT,50);
	m_list.InsertColumn(1, _T("习惯用语"), LVCFMT_LEFT,400);
	for (int i=0; i<strTextArray.GetCount(); i++) 
	{
		CString strPos;
		strPos.Format(_T("%d"), i+1);
		int nIndex = m_list.InsertItem(i, strPos);
		m_list.SetItemText(nIndex, 1, strTextArray.GetAt(i));
	}

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}

BEGIN_MESSAGE_MAP(GDimTextDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &GDimTextDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &GDimTextDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &GDimTextDialog::OnBnClickedButton4)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &GDimTextDialog::OnNMDblclkList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &GDimTextDialog::OnNMClickList2)
	ON_BN_CLICKED(IDOK, &GDimTextDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GDimTextDialog::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &GDimTextDialog::OnLvnItemchangedList2)
END_MESSAGE_MAP()


// GDimTextDialog 消息处理程序
void GDimTextDialog::ReadTxtFile(LPCTSTR lpFile, CStringArray& array)
{
	CString str;  

	try
	{
		CStdioFile file(lpFile, CFile::modeRead);
		while(file.ReadString(str))  
		{
			array.Add(str);
		}
		file.Close();
	}
	catch (CFileException* pe) 
	{
		CString strErr;
		strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
		MessageBox(strErr);
		pe->Delete();
	}
}

void GDimTextDialog::WriteTxtFile(LPCTSTR lpFile, const CStringArray& array)
{
	try
	{
		CStdioFile file(lpFile, CFile::modeCreate | CFile::modeWrite);

		for (int i=0; i<array.GetSize(); i++) 
		{
			file.WriteString(array.GetAt(i));
			file.WriteString(_T("\n"));
		}
		file.Close();
	}
	catch (CFileException* pe) 
	{
		CString strErr;
		strErr.Format(_T("读取文件发生异常, 错误号：%d"), pe->m_cause);
		MessageBox(strErr);
		pe->Delete();
	}
}
void GDimTextDialog::UpdateTxtFile()
{
	CStringArray textArray;
	for (int i=0; i<m_list.GetItemCount(); i++) 
	{
		textArray.Add(m_list.GetItemText(i,1));
	}
	WriteTxtFile(m_lpFile, textArray);
}
void GDimTextDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//增加
	CString strIdiom;
	m_wndEdit.GetWindowText(strIdiom);

	if (strIdiom.IsEmpty())
		return;

	int nCount = m_list.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nCount+1);
	int nIndex = m_list.InsertItem(nCount, strIndex);
	m_list.SetItemText(nIndex, 1, strIdiom);

	// 更新文件
	UpdateTxtFile();
}

void GDimTextDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//修改
	CString strIdiom;
	m_wndEdit.GetWindowText(strIdiom);

	if (strIdiom.IsEmpty())
		return;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int nSelected = m_list.GetNextSelectedItem(pos);

	m_list.SetItemText(nSelected, 1, strIdiom);

	// 更新文件
	UpdateTxtFile();
}

void GDimTextDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//删除
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int nSelected = m_list.GetNextSelectedItem(pos);

	m_list.DeleteItem(nSelected);

	// 更新文件
	UpdateTxtFile();
}

void GDimTextDialog::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	// 获取选中行
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	//获取ObjectId
	int nItem = m_list.GetNextSelectedItem(pos);
	CString strIdiom=m_list.GetItemText(nItem,1);
	m_wndEdit.SetWindowText(strIdiom);
	strText=strIdiom;
	if(!strText.IsEmpty())
	OnOK();
}

void GDimTextDialog::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	//获取ObjectId
	int nItem = m_list.GetNextSelectedItem(pos);
	CString strIdiom=m_list.GetItemText(nItem,1);
	m_wndEdit.SetWindowText(strIdiom);
}

void GDimTextDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIdiom;
	m_wndEdit.GetWindowText(strIdiom);

	if (strIdiom.IsEmpty())
	{
		AfxMessageBox(_T("请输入标注文字"));
		return;
	}
	strText=strIdiom;
	OnOK();
}

void GDimTextDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void GDimTextDialog::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
