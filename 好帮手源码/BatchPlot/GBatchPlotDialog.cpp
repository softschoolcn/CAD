// GBatchPlotDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GBatchPlotDialog.h"
#include "GPlotSettings.h"
#include "GFrameBorderHelper.h"
#include "GLockDocument.h"
//枚举一个目录下的目录和文件列表
bool EnumDirectoryContent(CString strDirName , CString strFilter,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	strDirName.TrimLeft();
	strDirName.TrimRight();

	if(strDirName.GetLength() == 0)return false;

	arrSubDirNames.RemoveAll();
	arrSubFileNames.RemoveAll();

	CString			strDirList , strFileList , strDirFileList;
	CFileFind		finder;
	CString			strWildcard;

	if(strDirName.GetAt(strDirName.GetLength() - 1) == _T('\\') ||strDirName.GetAt(strDirName.GetLength() - 1) == _T('/'))
		strWildcard	= strDirName + strFilter;
	else
		strWildcard		= strDirName +"\\"+ strFilter;
	BOOL bWorking = finder.FindFile(strWildcard);

	if(!bWorking)return false;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())continue;
		if(finder.IsSystem())continue;
		if (finder.IsDirectory())
		{
			arrSubDirNames.Add(finder.GetFileName());
			continue;
		}

		arrSubFileNames.Add(finder.GetFileName());
	}
	finder.Close();

	return true;
}
bool EnumDirectoryContent(CString strDirName ,
						  CStringArray &arrSubDirNames , 
						  CStringArray &arrSubFileNames)
{
	return EnumDirectoryContent(strDirName,_T("*.*"),arrSubDirNames,arrSubFileNames);
}
// GBatchPlotDialog 对话框

IMPLEMENT_DYNAMIC(GBatchPlotDialog, CAcUiDialog)
GBatchPlotDialog::GBatchPlotDialog(CWnd* pParent /*=NULL*/,HINSTANCE hInstance)
: CAcUiDialog(GBatchPlotDialog::IDD, pParent,hInstance)
{
	GPlotSettings::GetInstance()->Init();
}

GBatchPlotDialog::~GBatchPlotDialog()
{
}

void GBatchPlotDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRID,mGrid);
}


BEGIN_MESSAGE_MAP(GBatchPlotDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REFURSH, &GBatchPlotDialog::OnBnClickedButtonRefursh)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOTER, &GBatchPlotDialog::OnCbnSelchangeComboPloter)
	ON_BN_CLICKED(IDC_BUTTON_BROWSERFLODER, &GBatchPlotDialog::OnBnClickedButtonBrowserfloder)
	ON_BN_CLICKED(IDC_CHECK_UNISIZE, &GBatchPlotDialog::OnBnClickedCheckUnisize)
	ON_BN_CLICKED(IDC_CHECK_UNISTYLE, &GBatchPlotDialog::OnBnClickedCheckUnistyle)
	ON_BN_CLICKED(IDC_CHECK_UNINUM, &GBatchPlotDialog::OnBnClickedCheckUninum)
	ON_BN_CLICKED(IDC_CHECK_PLOTFILE, &GBatchPlotDialog::OnBnClickedCheckPlotfile)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &GBatchPlotDialog::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDOK, &GBatchPlotDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_BLOCKREF, &GBatchPlotDialog::OnBnClickedRadioBlockref)
	ON_BN_CLICKED(IDC_RADIO_BHX, &GBatchPlotDialog::OnBnClickedRadioBhx)
	ON_BN_CLICKED(IDC_RADIO_LRTB, &GBatchPlotDialog::OnBnClickedRadioLrtb)
	ON_BN_CLICKED(IDC_RADIO_TBLR, &GBatchPlotDialog::OnBnClickedRadioTblr)
	ON_BN_CLICKED(IDC_BUTTON_SELENT, &GBatchPlotDialog::OnBnClickedButtonSelent)
END_MESSAGE_MAP()


// GBatchPlotDialog 消息处理程序
BOOL GBatchPlotDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	mGrid.InsertColumn(_T("图纸名称"));
	mGrid.InsertColumn(_T("图框名称"));
	mGrid.InsertColumn(_T("打印尺寸"));
	mGrid.InsertColumn(_T("样式表"));
	mGrid.InsertColumn(_T("打印份数"));
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnWidthPercent(0, 28);
	mGrid.SetColumnWidthPercent(1, 25);
	mGrid.SetColumnWidthPercent(2, 22);
	mGrid.SetColumnWidthPercent(3, 15);
	mGrid.SetColumnWidthPercent(4, 8);
	InitDevice();
	UpdateFrameDefine();
	UpdateFrames(false);
	UpdateGrid();
	CheckPlotFile();
	ReadExt();

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GBatchPlotDialog::InitDevice()
{
	CStringArray devices;    //打印机列表
	GPlotSettings::GetInstance()->RefreshDevices(devices);
	CString firstArchivedevice(_T(""));

	CString strDevice=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("Device"),256,_T(""));
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pCombo != NULL)
	{
		int len = devices.GetCount();
		int nIndex=0;
		for (int i = 0; i < len; i++)
		{
			nIndex=pCombo->AddString(devices[i]);
			if(strDevice.CompareNoCase(devices[i])==0)
			{
				pCombo->SetCurSel(nIndex);
			}
		}
	}
	if(pCombo->GetCurSel()<0)
	{
		pCombo->SetCurSel(0);
	}
	CString device;
	CComboBox* pPloter = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pPloter != NULL)
	{
		int index = pPloter->GetCurSel();
		if(index>=0)
		{
			pPloter->GetLBText(index, device);
			GPlotSettings::GetInstance()->SetDevice(device);
		}
	}
}
/*!
@brief 清空结果表格
*/
void GBatchPlotDialog::ClearGrid()
{
	for (int nRow = mGrid.GetRowCount(); nRow >= 1; nRow--)
		mGrid.DeleteRow(nRow);
}
/*!
@brief 更新表格显示
*/
void GBatchPlotDialog::UpdateGrid()
{
	GPlotSettings* pSetting = GPlotSettings::GetInstance();

	CStringArray medias;  //打印尺寸列表
	pSetting->RefreshMedias(medias);

	CStringArray styles;     //样式表列表
	pSetting->RefreshStyles(styles);

	for(int i=0;i<m_plotItems.GetCount();i++)
	{
		GPlotItem plotItem=m_plotItems.GetAt(i);
		CString strName=_T("当前图纸");
		if(!plotItem.m_strFile.IsEmpty())
		{
		TCHAR lpFile[MAX_PATH+1]={0};
		lstrcpy(lpFile,plotItem.m_strFile);
		strName=PathFindFileName(lpFile);
		}
		int row = mGrid.InsertRow(strName);
		mGrid.SetItemText(row,1,plotItem.m_strBlockName);
		mGrid.SetRowHeight(row,24);
		mGrid.SetCellType(row,0,RUNTIME_CLASS(CGridCellCheck));
		CGridCellBase* pCell=mGrid.GetCell(row,0);
		CGridCellCheck* pCheckCell=static_cast<CGridCellCheck*>(pCell);
		if(pCheckCell!=NULL)
		{
			pCheckCell->SetCheck(TRUE);
		}
		mGrid.SetCellType(row, 2, RUNTIME_CLASS(CGridCellCombo));
		mGrid.SetCellType(row, 3, RUNTIME_CLASS(CGridCellCombo));
		mGrid.SetCellType(row, 4, RUNTIME_CLASS(CGridCellNumeric));

		CGridCellCombo* pMedia = (CGridCellCombo*)mGrid.GetCell(row, 2);
		pMedia->SetOptions(medias);
		CString mediaName = pSetting->GetDefaultMedia();
		if(mediaName.IsEmpty()&&medias.GetCount()>0)
		{
			mediaName=medias.GetAt(0);
		}
		pMedia->SetText(mediaName);
		mGrid.SetCell(row, 2, pMedia);

		CGridCellCombo* pStyle = (CGridCellCombo*)mGrid.GetCell(row, 3);
		pStyle->SetOptions(styles);
		pStyle->SetText(_T("monochrome.ctb"));

		mGrid.SetCell(row, 3, pStyle);

		CGridCellNumeric* pCopies = (CGridCellNumeric*)mGrid.GetCell(row, 4);
		pCopies->SetNumber(1);
		mGrid.SetCell(row, 4, pCopies);
	}

	mGrid.Invalidate();
}
//更新图框定义
void GBatchPlotDialog::UpdateFrameDefine()
{
	CString strFrameBref=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameBref"),256,_T("True"));
	CString strFrameNameCheck=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameNameCheck"),256,_T("True"));
	CString strFrameName=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameName"),256,_T("*TK*"));
	CString strFrameLayerCheck=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameLayerCheck"),256,_T("False"));
	CString strFrameLayer=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameLayer"),256,_T(""));
	
	//图块是块参照
	SetItemCheck(IDC_RADIO_BLOCKREF,strFrameBref.CompareNoCase(_T("True"))==0?TRUE:FALSE);
	
	//图框名称
	if(strFrameNameCheck.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_FRAMENAME,TRUE);
	}
	SetDlgItemText(IDC_EDIT_FRAMENAME,strFrameName);

	//图框图层
	if(strFrameLayerCheck.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_FRAMELAYER,TRUE);
	}
	SetDlgItemText(IDC_EDIT_FRMAELAYER,strFrameLayer);

	CString strFrameBHX=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("FrameBHX"),256,_T("True"));
	//图框是闭合线
	if(!GetItemCheck(IDC_RADIO_BLOCKREF))
	{
		SetItemCheck(IDC_RADIO_BHX,strFrameBHX.CompareNoCase(_T("True"))==0?TRUE:FALSE);
	}
	//闭合线图册
	CString strBHXLayer=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("BHXLayer"),256,_T(""));
	SetDlgItemText(IDC_EDIT_BHXLAYER,strBHXLayer);
	CString strBHXLayerCheck=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("BHXLayerCheck"),256,_T("False"));
	if(strBHXLayerCheck.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_BHXLAYER,TRUE);
	}

	//闭合线多段线
	CString strBHXPlineCheck=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("BHXPlineCheck"),256,_T("True"));
	if(strBHXPlineCheck.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_PLINE,TRUE);
	}

	//闭合线直线
	CString strBHXLineCheck=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("BHXLineCheck"),256,_T("False"));
	if(strBHXLineCheck.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK_LINE,TRUE);
	}

	CString strLRTB=GUserDataRecord::getInstance()->GetProfileString(_T("GBatchPlotDialog"),_T("LRTB"),256,_T("True"));
	if(strLRTB.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_RADIO_LRTB,TRUE);
	}
	else
	{
		SetItemCheck(IDC_RADIO_TBLR,TRUE);
	}
	ChangeType();

	SetItemCheck(IDC_CHECK_JOINPDF,GUserDataRecord::getInstance()->GetProfileBool(_T("GBatchPlotDialog"),_T("JOINPDF"),false));
	SetItemCheck(IDC_CHECK_DELMIDPDF,GUserDataRecord::getInstance()->GetProfileBool(_T("GBatchPlotDialog"),_T("DELMIDPDF"),false));
	SetItemCheck(IDC_CHECK_ALLOW90,GUserDataRecord::getInstance()->GetProfileBool(_T("GBatchPlotDialog"),_T("ALLOW90"),true));
}
//更新图框
bool GBatchPlotDialog::UpdateFrames(bool bAlert)
{
	GFrameDefine define;
	if(GetItemCheck(IDC_RADIO_LRTB))
	{
		define.m_nOrder=0;
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("LRTB"),_T("True"));
	}
	else
	{
		define.m_nOrder=1;
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("LRTB"),_T("False"));
	}
	if(GetItemCheck(IDC_RADIO_BLOCKREF))
	{
		define.m_bBREF=TRUE;
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameBref"),_T("True"));
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameBHX"),_T("False"));
		if(GetItemCheck(IDC_CHECK_FRAMENAME))
		{
			define.m_bFrameNameCheck=TRUE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameNameCheck"),_T("True"));
		}
		else
		{
			define.m_bFrameNameCheck=FALSE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameNameCheck"),_T("False"));
		}
		GetDlgItemText(IDC_EDIT_FRAMENAME,define.m_strFrameName);
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameName"),define.m_strFrameName);
		if(GetItemCheck(IDC_CHECK_FRAMELAYER))
		{
			define.m_bFrameLayerCheck=TRUE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameLayerCheck"),_T("True"));
		}
		else
		{
			define.m_bFrameLayerCheck=FALSE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameLayerCheck"),_T("False"));
		}
		GetDlgItemText(IDC_EDIT_FRMAELAYER,define.m_strFrameLayer);
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameLayer"),define.m_strFrameLayer);
		if((!define.m_bFrameLayerCheck)&&(!define.m_bFrameNameCheck))
		{
			if(bAlert)
				AfxMessageBox(_T("请至少设定图框名称或图框图层中的一个"));
			return false;
		}
		if(define.m_bFrameNameCheck)
		{
			if(define.m_strFrameName.IsEmpty())
			{
				if(bAlert)
				{
					AfxMessageBox(_T("请输入图框名称"));
				}
				return false;
			}
		}
		if(define.m_bFrameLayerCheck)
		{
			if(define.m_strFrameLayer.IsEmpty())
			{
				if(bAlert)
				{
					AfxMessageBox(_T("请输入图层名称"));
				}
				return false;
			}
		}
	}
	else
	{
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameBref"),_T("False"));
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("FrameBHX"),_T("True"));
		define.m_bBREF=FALSE;
		GetDlgItemText(IDC_EDIT_BHXLAYER,define.m_strBHXLayer);
		GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXLayer"),define.m_strBHXLayer);
		if(GetItemCheck(IDC_CHECK_BHXLAYER))
		{
			define.m_bBHXLayer=TRUE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXLayerCheck"),_T("True"));
		}
		else
		{
			define.m_bBHXLayer=FALSE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXLayerCheck"),_T("False"));
		}
		if(GetItemCheck(IDC_CHECK_PLINE))
		{
			define.m_bPLine=TRUE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXPlineCheck"),_T("True"));
		}
		else
		{
			define.m_bPLine=FALSE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXPlineCheck"),_T("False"));
		}
		if(GetItemCheck(IDC_CHECK_LINE))
		{
			define.m_bLine=TRUE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXLineCheck"),_T("True"));
		}
		else
		{
			define.m_bLine=FALSE;
			GUserDataRecord::getInstance()->WriteProfileString(_T("GBatchPlotDialog"),_T("BHXLineCheck"),_T("False"));
		}

		if(define.m_bBHXLayer)
		{
			if(define.m_strBHXLayer.IsEmpty())
			{
				if(bAlert)
				{
					AfxMessageBox(_T("请输入图层名称"));
				}
				return false;
			}
		}
		if(define.m_bPLine||define.m_bLine)
		{
		}
		else
		{
			if(bAlert)
			{
				AfxMessageBox(_T("请至少选择多段线和直线中的一个"));
			}
			return false;
		}

	}
	CString strPath;
	GetDlgItemText(IDC_EDIT_FLODER,strPath);
	m_plotItems.RemoveAll();
	m_frame=define;
	if(strPath.IsEmpty())
	{
		GFrameBorderHelper helper;
		helper.m_frame=define;
		helper.GetPlotItems(m_plotItems);
		return true;
	}
	GLockDocument _Lock;
	GetPlotItems(strPath);
	return true;
}
void GBatchPlotDialog::GetPlotItems(CString strPath)
{
	CStringArray arrSubDirNames;
	CStringArray arrSubFileNames;
	EnumDirectoryContent(strPath,arrSubDirNames,arrSubFileNames);
	for(int i=0;i<arrSubDirNames.GetCount();i++)
	{
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,strPath);
		PathAppend(lpPath,arrSubDirNames.GetAt(i));
		GetPlotItems(lpPath);
	}
	for(int i=0;i<arrSubFileNames.GetCount();i++)
	{
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,strPath);

		PathAppend(lpPath,arrSubFileNames.GetAt(i));
		if(lstrcmpi(PathFindExtension(lpPath),_T(".dwg"))==0)
		{
			GetPlotItemsFromFile(lpPath);
		}
	}
}
//从文件中获取所有需要打印的项
void GBatchPlotDialog::GetPlotItemsFromFile(CString strFileName)
{
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step())
	{
		if (strFileName.CompareNoCase(pIter->document()->fileName()) == 0)
		{
			//已经打开的文件
			GetPlotItemsFromFile(strFileName,pIter->document()->database());
			delete pIter;
			return;
		}
	}
	delete pIter;
	AcDbDatabase* pDbTmp=new AcDbDatabase(false);
	try
	{
		Acad::ErrorStatus es=pDbTmp->readDwgFile(strFileName);
		if(es!=eOk)
		{
			acutPrintf(_T("\n读取DWG文件:%s，发生错误:%s\n"),strFileName,acadErrorStatusText(es));
		}
		else
		{
			GetPlotItemsFromFile(strFileName,pDbTmp);
		}

	}
	catch(...)
	{
		if(pDbTmp!=NULL)
		{
			delete pDbTmp;
			pDbTmp=NULL;
		}
	}
	if(pDbTmp!=NULL)
	{
		delete pDbTmp;
		pDbTmp=NULL;
	}
}
void GBatchPlotDialog::GetPlotItemsFromFile(CString strFile,AcDbDatabase* pDb)
{
	GFrameBorderHelper helper;
	helper.m_frame=m_frame;
	helper.m_strFileName=strFile;
	helper.GetPlotItems(m_plotItems,pDb);
}
void GBatchPlotDialog::OnBnClickedButtonRefursh()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearGrid();
	UpdateFrames(true);
	UpdateGrid();
}
void GBatchPlotDialog::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	int row = pItem->iRow;
	int column = pItem->iColumn; //得到当前行、列
	if (row > 0 && column > 1 && column < 4)  //不是固定行且是打印尺寸列和打印样式表列
	{
		CGridCellCombo* pCombo = (CGridCellCombo*)mGrid.GetCell(row, column);
		CCellID cellId(row, column);
		pCombo->Click(cellId);
	}
}

void GBatchPlotDialog::OnCbnSelchangeComboPloter()
{
	// TODO: 在此添加控件通知处理程序代码
	CString device;
	CComboBox* pPloter = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pPloter != NULL)
	{
		int index = pPloter->GetCurSel();
		pPloter->GetLBText(index, device);
	}
	GPlotSettings::GetInstance()->SetDevice(device);
	CArray<BOOL> checkArray;
	for(int i=1;i<mGrid.GetRowCount();i++)
	{
		CGridCellBase* pCell=mGrid.GetCell(i,0);
		BOOL bCheck=FALSE;
		CGridCellCheck* pCheckCell=static_cast<CGridCellCheck*>(pCell);
		if(pCheckCell!=NULL)
		{
			if(pCheckCell->GetCheck())
			{
				bCheck=TRUE;
			}
		}
		checkArray.Add(bCheck);
	}

	ClearGrid();
	UpdateGrid();
	for(int i=1;i<mGrid.GetRowCount();i++)
	{
		CGridCellBase* pCell=mGrid.GetCell(i,0);
		BOOL bCheck=FALSE;
		CGridCellCheck* pCheckCell=static_cast<CGridCellCheck*>(pCell);
		if(pCheckCell!=NULL)
		{
			if(i<=checkArray.GetCount())
			{
				pCheckCell->SetCheck(checkArray.GetAt(i-1));
			}
		}
	}
	CheckPlotFile();
	SetDlgItemText(IDC_EDIT_EXT,_T(""));
	ReadExt();
}

void GBatchPlotDialog::OnBnClickedButtonBrowserfloder()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	TCHAR path[MAX_PATH];
	bi.pszDisplayName = path;
	bi.lpszTitle = _T("请选择批量打印目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, path);
		SetDlgItemText(IDC_EDIT_FLODER,path);
		if(UpdateFrames(true))
		{
			ClearGrid();
			UpdateGrid();
		}
	}
}

void GBatchPlotDialog::OnBnClickedCheckUnisize()
{
	// TODO: 在此添加控件通知处理程序代码
	int len = mGrid.GetRowCount();
	if (len < 2)
	{
		return;
	}
	CString size = mGrid.GetItemText(1, 2);
	for (int i = 2; i < len; i++)
	{
		mGrid.SetItemText(i, 2, size);
	}
	mGrid.Invalidate();
}

void GBatchPlotDialog::OnBnClickedCheckUnistyle()
{
	// TODO: 在此添加控件通知处理程序代码
	int len = mGrid.GetRowCount();
	if (len < 2)
	{
		return;
	}
	CString style = mGrid.GetItemText(1, 3);
	for (int i = 2; i < len; i++)
	{
		mGrid.SetItemText(i, 3, style);
	}
	mGrid.Invalidate();
}

void GBatchPlotDialog::OnBnClickedCheckUninum()
{
	// TODO: 在此添加控件通知处理程序代码
	int len = mGrid.GetRowCount();
	if (len < 2)
	{
		return;
	}
	CString num = mGrid.GetItemText(1, 4);
	for (int i = 2; i < len; i++)
	{
		mGrid.SetItemText(i, 4, num);
	}
	mGrid.Invalidate();
}
void GBatchPlotDialog::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	if (pItem == NULL)
	{
		return;
	}
	if (pItem->iRow < 1)
	{
		return;
	}

	if (pItem->iColumn == 2)  //图幅
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UNISIZE);
		if (pBtn != NULL && pBtn->GetCheck() == 1)  //统一图幅
		{
			CString size = mGrid.GetItemText(pItem->iRow, 2);
			int len = mGrid.GetRowCount();
			for (int i = 1; i < len; i++)
			{
				mGrid.SetItemText(i, 2, size);
			}
		}
	}
	else if (pItem->iColumn == 3)
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UNISTYLE);
		if (pBtn != NULL &&pBtn->GetCheck() == 1)  //统一打印样式表
		{
			CString style = mGrid.GetItemText(pItem->iRow, 3);
			int len = mGrid.GetRowCount();
			for (int i = 1; i < len; i++)
			{
				mGrid.SetItemText(i, 3, style);
			}
		}
	}
	else if (pItem->iColumn == 4)
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UNINUM);
		if (pBtn != NULL && pBtn->GetCheck() == 1)  //统一打印份数
		{
			CString num = mGrid.GetItemText(pItem->iRow, 4);
			int len = mGrid.GetRowCount();
			for (int i = 1; i < len; i++)
			{
				mGrid.SetItemText(i, 4, num);
			}
		}
	}
	mGrid.Invalidate();
	mGrid.UpdateData();
	mGrid.UpdateWindow();

}
void GBatchPlotDialog::CheckPlotFile()
{
	CString device(_T(""));
	CComboBox* pDevice = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pDevice != NULL)
	{
		int sel = pDevice->GetCurSel();
		pDevice->GetLBText(sel, device);
	}
	if (device.IsEmpty())
	{
		GetDlgItem(IDC_CHECK_PLOTFILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
		return;
	}
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_PLOTFILE);
	if (pBtn != NULL)
	{
		CString strType=device.GetLength()>=4?device.Right(4):_T("");
		if(strType.CompareNoCase(_T(".pc3"))==0)
		{
			pBtn->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(TRUE);
			pBtn->SetCheck(1);
		}
		else
		{
			pBtn->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
			pBtn->SetCheck(0);
			CComboBox* pDescPlot = (CComboBox*)GetDlgItem(IDC_COMBO_DESCPLOT);
			pDescPlot->EnableWindow(TRUE);
		}
	}
}

void GBatchPlotDialog::OnBnClickedCheckPlotfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_PLOTFILE);
	if (pBtn != NULL && pBtn->GetCheck() == 1)
	{
		if(pBtn->GetCheck() == 1)
		{
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
		}
	}
}

void GBatchPlotDialog::OnBnClickedButtonPath()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择目录
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	TCHAR path[MAX_PATH];
	bi.pszDisplayName = path;
	bi.lpszTitle = _T("请选择目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, path);
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(path);
	}
}

void GBatchPlotDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plotItemExs.RemoveAll();
	CString path;
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(path);
	bool bPlotToFile=false;
	if(GetItemCheck(IDC_CHECK_PLOTFILE))
	{
		bPlotToFile=true;
	}
	m_bPlotToFile=bPlotToFile;
	m_strPath=path;
	if(bPlotToFile)
	{
		if(path.IsEmpty())
		{
			AfxMessageBox(_T("请设置文件输出目录"));
			return;
		}
		GetDlgItemText(IDC_EDIT_EXT,m_strExt);
		if(m_strExt.IsEmpty())
		{
			AfxMessageBox(_T("请输入打印文件的后缀"));
			return;
		}
		SaveExt();
	}
	CComboBox* pPloter = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pPloter != NULL)
	{
		int index = pPloter->GetCurSel();
		pPloter->GetLBText(index, m_strDevice);
	}
	int len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		if(i>m_plotItems.GetCount())
		{
			continue;
		}
		CGridCellBase* pCell=mGrid.GetCell(i,0);
		CGridCellCheck* pCheckCell=static_cast<CGridCellCheck*>(pCell);
		if(pCheckCell==NULL)
		{
			continue;
		}
		if(!pCheckCell->GetCheck())
		{
			continue;
		}
		GPlotItem item=m_plotItems.GetAt(i-1);
		GPlotItemEx item1;
		item1.Copy(item);
		item1.m_size = mGrid.GetItemText(i, 2);  //打印尺寸
		item1.m_style = mGrid.GetItemText(i, 3); //打印样式表
		CGridCellNumeric* pCopies = (CGridCellNumeric*)mGrid.GetCell(i, 4);  //打印份数
		item1.m_copies = (int)pCopies->GetNumber();
		m_plotItemExs.Add(item1);
	}
	m_bJoinPDF=GetItemCheck(IDC_CHECK_JOINPDF);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GBatchPlotDialog"),_T("JOINPDF"),m_bJoinPDF);
	m_bDelMidPdf=GetItemCheck(IDC_CHECK_DELMIDPDF);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GBatchPlotDialog"),_T("DELMIDPDF"),m_bDelMidPdf);
	m_bAllow90=GetItemCheck(IDC_CHECK_ALLOW90);
	GUserDataRecord::getInstance()->WriteProfileBool(_T("GBatchPlotDialog"),_T("ALLOW90"),m_bAllow90);
	OnOK();
}
CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName)
{
	MSXML2::IXMLDOMNodePtr lpAttrNode = lpNode->attributes->getNamedItem(strName.GetBuffer());
	strName.ReleaseBuffer();
	if(lpAttrNode==NULL)
		return _T("");
	return (char*)lpAttrNode->text;
}
bool ReadExtArray(CStringArray& strNameArr,CStringArray& strExtArr,LPCTSTR lpFile)
{
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return false;
	}
	//加载XML文件
	if(!lpDocument->load(lpFile))
	{
		return false;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return false;
	}
	MSXML2::IXMLDOMNodeListPtr lpCmdNodeList=lpDocElement->getElementsByTagName(_T("Ploter"));
	MSXML2::IXMLDOMNodePtr lpCmdNode = NULL;

	for ( ; (lpCmdNode = lpCmdNodeList->nextNode()) != NULL ; )
	{
		CString strName=GetNodeAttrValue(lpCmdNode,_T("Name"));
		CString strExtTmp=GetNodeAttrValue(lpCmdNode,_T("Ext"));
		strNameArr.Add(strName);
		strExtArr.Add(strExtTmp);
	}
	return true;
}
bool WriteExtArray(CStringArray& strNameArr,CStringArray& strExtArr,LPCTSTR lpFile)
{
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);

	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='GB2312'"));
	lpDocument->appendChild(lpInstruction);

	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("Root")));

	lpDocument->appendChild(lpRoot);

	// 参数列表
	for (int l=0; l<strNameArr.GetCount(); l++)
	{
		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Ploter"));
		lpParam->setAttribute(_T("Name"), _variant_t(strNameArr.GetAt(l)));
		lpParam->setAttribute(_T("Ext"), _variant_t(strExtArr.GetAt(l)));
		lpRoot->appendChild(lpParam);
	}
	hr=lpDocument->save(lpFile);
	return SUCCEEDED(hr);
}
bool ReadExt(CString& strExt,CString device,LPCTSTR lpFile)
{
	CStringArray strNameArr;
	CStringArray strExtArr;
	ReadExtArray(strNameArr,strExtArr,lpFile);
	for(int i=0;i<strNameArr.GetCount();i++)
	{
		if(device.CompareNoCase(strNameArr.GetAt(i))==0)
		{
			strExt=strExtArr.GetAt(i);
			return true;
		}
	}
	return false;
}
bool GBatchPlotDialog::SaveExt()
{
	CString device(_T(""));
	CComboBox* pDevice = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pDevice != NULL)
	{
		int sel = pDevice->GetCurSel();
		pDevice->GetLBText(sel, device);
	}
	GetDlgItemText(IDC_EDIT_EXT,m_strExt);

	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("Config\\PloterExt.xml"));

	TCHAR lpPath2[MAX_PATH+1]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("UserConfig\\PloterExt.xml"));

	int nMode=0;//0:删除,1:修改
	CString strExt;
	if(::ReadExt(strExt,device,lpPath1))
	{
		if(strExt.CompareNoCase(m_strExt)==0)
		{
			nMode=0;
		}
		else
		{
			nMode=1;
		}
	}
	else
	{
		nMode=1;
	}

	CStringArray strNameArr;
	CStringArray strExtArr;
	ReadExtArray(strNameArr,strExtArr,lpPath2);
	if(nMode==0)
	{
		bool bFind=false;
		for(int i=0;i<strNameArr.GetCount();i++)
		{
			if(device.CompareNoCase(strNameArr.GetAt(i))==0)
			{
				bFind=true;
				strNameArr.RemoveAt(i);
				strExtArr.RemoveAt(i);
				break;
			}
		}
		if(bFind)
		{
			WriteExtArray(strNameArr,strExtArr,lpPath2);
		}

	}
	else
	{
		bool bFind=false;
		bool bModify=false;
		for(int i=0;i<strNameArr.GetCount();i++)
		{
			if(device.CompareNoCase(strNameArr.GetAt(i))==0)
			{
				bFind=true;
				if(m_strExt.CompareNoCase(strExtArr.GetAt(i))==0)
				{
					bModify=false;
				}
				else
				{
					bModify=true;
					strExtArr.SetAt(i,m_strExt);
				}
				break;
			}
		}
		if(!bFind)
		{
			strNameArr.Add(device);
			strExtArr.Add(m_strExt);
		}
		if((!bFind)||bModify)
		{
			WriteExtArray(strNameArr,strExtArr,lpPath2);
		}
	}
	return false;
}

bool GBatchPlotDialog::ReadExt()
{
	CString device(_T(""));
	CComboBox* pDevice = (CComboBox*)GetDlgItem(IDC_COMBO_PLOTER);
	if (pDevice != NULL)
	{
		int sel = pDevice->GetCurSel();
		pDevice->GetLBText(sel, device);
	}

	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("Config\\PloterExt.xml"));

	TCHAR lpPath2[MAX_PATH+1]={0};
	lstrcpy(lpPath2,lpPath);
	PathAppend(lpPath2,_T("UserConfig\\PloterExt.xml"));
	CString strExt;
	if(::ReadExt(strExt,device,lpPath2))
	{
		SetDlgItemText(IDC_EDIT_EXT,strExt);
		return true;
	}
	if(::ReadExt(strExt,device,lpPath1))
	{
		SetDlgItemText(IDC_EDIT_EXT,strExt);
		return true;
	}
	return false;
}
void GBatchPlotDialog::ChangeType()
{
	BOOL bBREF=GetItemCheck(IDC_RADIO_BLOCKREF);
	SetItemEnable(IDC_CHECK_FRAMENAME,bBREF);
	SetItemEnable(IDC_EDIT_FRAMENAME,bBREF);
	SetItemEnable(IDC_CHECK_FRAMELAYER,bBREF);
	SetItemEnable(IDC_EDIT_FRMAELAYER,bBREF);

	SetItemEnable(IDC_CHECK_PLINE,!bBREF);
	SetItemEnable(IDC_CHECK_LINE,!bBREF);
	SetItemEnable(IDC_CHECK_BHXLAYER,!bBREF);
	SetItemEnable(IDC_EDIT_BHXLAYER,!bBREF);
}
void GBatchPlotDialog::OnBnClickedRadioBlockref()
{
	SetItemCheck(IDC_RADIO_BLOCKREF,TRUE);
	SetItemCheck(IDC_RADIO_BHX,FALSE);
	ChangeType();
}

void GBatchPlotDialog::OnBnClickedRadioBhx()
{
	SetItemCheck(IDC_RADIO_BLOCKREF,FALSE);
	SetItemCheck(IDC_RADIO_BHX,TRUE);
	ChangeType();
}
struct GPlotItemEx1
{
	GPlotItemEx item;
	bool bCheck;
	int nIndex;
	GPlotItemEx1()
	{
	}
	GPlotItemEx1(const GPlotItemEx1& item1)
	{
		item=item1.item;
		bCheck=item1.bCheck;
		nIndex=item1.nIndex;
	}
	GPlotItemEx1& operator=(const GPlotItemEx1& item1)
	{
		item=item1.item;
		bCheck=item1.bCheck;
		nIndex=item1.nIndex;
		return *this;
	}
};
void GBatchPlotDialog::OnBnClickedRadioLrtb()
{
	SetItemCheck(IDC_RADIO_LRTB,TRUE);
	SetItemCheck(IDC_RADIO_TBLR,FALSE);
	ClearGrid();
	UpdateFrames(false);
	UpdateGrid();
}

void GBatchPlotDialog::OnBnClickedRadioTblr()
{
	SetItemCheck(IDC_RADIO_LRTB,FALSE);
	SetItemCheck(IDC_RADIO_TBLR,TRUE);
	ClearGrid();
	UpdateFrames(false);
	UpdateGrid();
}
void GetEntLayers(CStringArray& strLayers,AcDbObjectIdArray& EntIds)
{
	for(int i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(EntIds.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		CString strLayerName;
		if(GCADUtils::GetLayerName(strLayerName,spEnt->layerId())!=Acad::eOk)
		{
			continue;
		}
		if(GStringHelper::IsStringInArray(strLayers,strLayerName,true))
		{
			continue;
		}
		strLayers.Add(strLayerName);
	}
}
void GBatchPlotDialog::OnBnClickedButtonSelent()
{
	//选择实体确定图层
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择图框:"),_T("\n移除图框:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
	{
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	GCADUtils::SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()==0)
	{
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	CStringArray strLayers;
	GetEntLayers(strLayers,EntIds);
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);

	if(strLayers.GetCount()==0)
	{
		return;
	}
	int nId=IDC_EDIT_FRMAELAYER;
	if(GetItemCheck(IDC_RADIO_BHX))
	{
		nId=IDC_EDIT_BHXLAYER;
	}
	CString strTmp;
	GetDlgItemText(nId,strTmp);
	CStringArray strOldLayers;
	if(!strTmp.IsEmpty())
	{
		GStringHelper::Split(strTmp,_T(";"),strOldLayers,true);
	}
	for(int i=0;i<strLayers.GetCount();i++)
	{
		if(GStringHelper::IsStringInArray(strOldLayers,strLayers.GetAt(i),true))
		{
			continue;
		}
		strOldLayers.Add(strLayers.GetAt(i));
	}
	strTmp=_T("");
	for(int i=0;i<strOldLayers.GetCount();i++)
	{
		if(i!=0)
		{
			strTmp+=_T(";");
		}
		strTmp+=strOldLayers.GetAt(i);
	}
	SetDlgItemText(nId,strTmp);
	

}
