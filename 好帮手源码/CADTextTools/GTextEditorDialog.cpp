// GTextEditorDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GTextEditorDialog.h"

// GTextEditorDialog 对话框

IMPLEMENT_DYNAMIC(GTextEditorDialog, CDialog)

GTextEditorDialog::GTextEditorDialog(AcDbObjectIdArray& ids,CWnd* pParent /*=NULL*/)
: CDialog(GTextEditorDialog::IDD, pParent)
{
	mIds.append(ids);
}

GTextEditorDialog::~GTextEditorDialog()
{
}

void GTextEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CUSTOM1,mGrid);
}


BEGIN_MESSAGE_MAP(GTextEditorDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GTextEditorDialog::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM1, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_CUSTOM1, OnGridEndEdit)
	ON_BN_CLICKED(IDC_CHECK1, &GTextEditorDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &GTextEditorDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &GTextEditorDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &GTextEditorDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// GTextEditorDialog 消息处理程序
BOOL GTextEditorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CGridCell* pCell = new CGridCell();
	mGrid.SetDefaultCellType(pCell->GetRuntimeClass());
	int nCol=0;
	nCol=mGrid.InsertColumn(_T("序号"));
	mGrid.SetColumnWidth(nCol,120);
	nCol=mGrid.InsertColumn(_T("内容"));
	mGrid.SetColumnWidth(nCol,550);
	nCol=mGrid.InsertColumn(_T("字体样式"));
	mGrid.SetColumnWidth(nCol,120);
	nCol=mGrid.InsertColumn(_T("字高"));
	mGrid.SetColumnWidth(nCol,120);
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetFixedColumnCount(1);
	mGrid.AutoSizeColumn(0);
	int nCount=1;
	for(int i=0;i<mIds.logicalLength();i++)
	{
		AcDbObjectId id=mIds.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForRead);
		if(Acad::eOk!=spEnt.openStatus())
			continue;
		CString strId;
		strId.Format(_T("%d"),nCount);
		int nIndex=mGrid.InsertRow(strId);
		mGrid.SetItemText(nIndex,1,GCADUtils::GetText(spEnt.object()));
		mGrid.SetCellType(nIndex, 2, RUNTIME_CLASS(CGridCellCombo));
		CGridCellCombo* pTextStyle = (CGridCellCombo*)mGrid.GetCell(nIndex, 2);
		pTextStyle->SetOptions(mTextStyleNames);
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());
			CString strInfo;
			strInfo.Format(_T("%.2f"),pText->height());
			mGrid.SetItemText(nIndex,3,strInfo);
			CString strStyle=mTextStyleNames.GetAt(0);
			for(long j=0;j<mTextStyleIds.logicalLength();j++)
			{
				if(mTextStyleIds.at(j)==pText->textStyle())
				{
					strStyle=mTextStyleNames.GetAt(j);
				}
			}
			mGrid.SetItemText(nIndex,2,strStyle);

		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			CString strInfo;
			strInfo.Format(_T("%.2f"),pText->textHeight());
			mGrid.SetItemText(nIndex,3,strInfo);
			CString strStyle=mTextStyleNames.GetAt(0);
			for(long j=0;j<mTextStyleIds.logicalLength();j++)
			{
				if(mTextStyleIds.at(j)==pText->textStyle())
				{
					strStyle=mTextStyleNames.GetAt(j);
				}
			}
			mGrid.SetItemText(nIndex,2,strStyle);
		}
		mGrid.SetItemData(nIndex,1,(LPARAM)i);
		nCount++;
	}
	SetDlgItemText(IDC_EDIT1,_T("0"));
	SetDlgItemText(IDC_EDIT2,_T("1"));

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GTextEditorDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CArray<double> dTextHeightArray;
	int len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText=mGrid.GetItemText(i,3);
		double dHeight=0.0;
		if(!GStringHelper::IsValidDouble(strText,dHeight)||(dHeight<1E-8||dHeight>1E8))
		{
			mGrid.SetSelectedRange(i,2,i,3);
				mGrid.SetFocusCell(i,3);
			AfxMessageBox(_T("输入的字高不合理"));
			return;
		}
		dTextHeightArray.Add(dHeight);
	}
	len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText=mGrid.GetItemText(i,1);
		int nIndex=(int)mGrid.GetItemData(i,1);
		if(nIndex<0||nIndex>=mIds.logicalLength())
			continue;
		AcDbObjectId id=mIds.at(nIndex);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(Acad::eOk!=spEnt.openStatus())
			continue;
		CString strStyle=mGrid.GetItemText(i,2);
		AcDbObjectId textStyleId=mTextStyleIds.at(0);
		for(long j=0;j<mTextStyleIds.logicalLength();j++)
		{
			if(strStyle.Compare(mTextStyleNames.GetAt(j))==0)
			{
				textStyleId=mTextStyleIds.at(j);
				break;
			}
		}
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText::cast(spEnt.object())->setTextString(strText);
			AcDbText* pText=AcDbText::cast(spEnt.object());
			pText->setTextStyle(textStyleId);
			pText->setHeight(dTextHeightArray.GetAt(i-1));
			
		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText::cast(spEnt.object())->setContents(strText);
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			pText->setTextStyle(textStyleId);
			pText->setTextHeight(dTextHeightArray.GetAt(i-1));
		}
	}
	OnOK();
}
void GTextEditorDialog::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	int row = pItem->iRow;
	int column = pItem->iColumn; //得到当前行、列
	if (row > 0 && column==2)  //不是固定行且是打印尺寸列和打印样式表列
	{
		CGridCellCombo* pCombo = (CGridCellCombo*)mGrid.GetCell(row, column);
		CCellID cellId(row, column);
		pCombo->Click(cellId);
	}
}

void GTextEditorDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	//统一字体样式
	if(!GetItemCheck(IDC_CHECK1))
	{
		return;
	}
	int len = mGrid.GetRowCount();
	if (len < 2)
	{
		return;
	}
	CString num = mGrid.GetItemText(1, 2);
	for (int i = 2; i < len; i++)
	{
		mGrid.SetItemText(i, 2, num);
	}
	mGrid.Invalidate();

}

void GTextEditorDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	//统一字高
	if(!GetItemCheck(IDC_CHECK2))
	{
		return;
	}
	int len = mGrid.GetRowCount();
	if (len < 2)
	{
		return;
	}
	CString num = mGrid.GetItemText(1, 3);
	for (int i = 2; i < len; i++)
	{
		mGrid.SetItemText(i, 3, num);
	}
	mGrid.Invalidate();
}
void GTextEditorDialog::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
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
		if(!GetItemCheck(IDC_CHECK1))
		{
			return;
		}
		CString size = mGrid.GetItemText(pItem->iRow, 2);
		int len = mGrid.GetRowCount();
		for (int i = 1; i < len; i++)
		{
			mGrid.SetItemText(i, 2, size);
		}
	}
	else if (pItem->iColumn == 3)
	{
		if(!GetItemCheck(IDC_CHECK2))
		{
			return;
		}
		CString style = mGrid.GetItemText(pItem->iRow, 3);
		int len = mGrid.GetRowCount();
		for (int i = 1; i < len; i++)
		{
			mGrid.SetItemText(i, 3, style);
		}
	}
	mGrid.Invalidate();
	mGrid.UpdateData();
	mGrid.UpdateWindow();
}

void GTextEditorDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//整体增大或减小
	CString strAdd;
	GetDlgItemText(IDC_EDIT1,strAdd);
	double dAdd=0.0;
	if(!GStringHelper::IsValidDouble(strAdd,dAdd))
	{
		AfxMessageBox(_T("请输入合理的数字"));
		return;
	}
	if(dAdd<-1E8||dAdd>1E8||(dAdd>-1E-8&&dAdd<1E-8))
	{
		AfxMessageBox(_T("请输入有效的数字"));
		return;
	}
	CArray<double> dTextHeightArray;
	int len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText=mGrid.GetItemText(i,3);
		double dHeight=0.0;
		if(!GStringHelper::IsValidDouble(strText,dHeight)||(dHeight<1E-8||dHeight>1E8))
		{
			mGrid.SetSelectedRange(i,2,i,3);
				mGrid.SetFocusCell(i,3);
			AfxMessageBox(_T("输入的字高不合理"));
			return;
		}
		dHeight+=dAdd;
		if((dHeight<1E-8||dHeight>1E8))
		{
			mGrid.SetSelectedRange(i,2,i,3);
				mGrid.SetFocusCell(i,3);
			AfxMessageBox(_T("整体增大或减小后的字高不合理"));
			return;
		}
		dTextHeightArray.Add(dHeight);
	}
	len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText;
		strText.Format(_T("%.2f"),dTextHeightArray.GetAt(i-1));
		mGrid.SetItemText(i,3,strText);
	}
	mGrid.Invalidate();
	mGrid.UpdateData();
	mGrid.UpdateWindow();
}

void GTextEditorDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//整体放大或缩小
	CString strAdd;
	GetDlgItemText(IDC_EDIT2,strAdd);
	double dAdd=0.0;
	if(!GStringHelper::IsValidDouble(strAdd,dAdd))
	{
		AfxMessageBox(_T("请输入合理的数字"));
		return;
	}
	if(dAdd>1E8||dAdd<1E-8)
	{
		AfxMessageBox(_T("请输入有效的数字"));
		return;
	}
	CArray<double> dTextHeightArray;
	int len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText=mGrid.GetItemText(i,3);
		double dHeight=0.0;
		if(!GStringHelper::IsValidDouble(strText,dHeight)||(dHeight<1E-8||dHeight>1E8))
		{
			mGrid.SetSelectedRange(i,2,i,3);
				mGrid.SetFocusCell(i,3);
			AfxMessageBox(_T("输入的字高不合理"));
			return;
		}
		dHeight*=dAdd;
		if((dHeight<1E-8||dHeight>1E8))
		{
			mGrid.SetSelectedRange(i,2,i,3);
				mGrid.SetFocusCell(i,3);
			AfxMessageBox(_T("整体放大或缩小后的字高不合理"));
			return;
		}
		dTextHeightArray.Add(dHeight);
	}
	len = mGrid.GetRowCount();
	for (int i = 1; i < len; i++)
	{
		CString strText;
		strText.Format(_T("%.2f"),dTextHeightArray.GetAt(i-1));
		mGrid.SetItemText(i,3,strText);
	}
	mGrid.Invalidate();
	mGrid.UpdateData();
	mGrid.UpdateWindow();
}
