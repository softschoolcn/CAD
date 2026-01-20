// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- GTableAlignDialog.cpp : Implementation of GTableAlignDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#ifndef ACAD_2004

#if _MSC_VER<=1300
   #ifndef ACAD_2004
      #include "dbtable.h"
   #endif
#endif
#include "resource.h"
#include "GTableAlignDialog.h"
Acad::ErrorStatus GTableCellAlignRecord::Record()
{
	m_recordItemArray.RemoveAll();
	AcDbObjectPointer<AcDbTable> spTable(m_tableId,AcDb::kForRead);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	CStringArray insertRangeArray;//将已经处理的单元格记录下来，不重复处理
	for(int i=0;i<spTable->numRows();i++)
	{
		for(int j=0;j<spTable->numColumns();j++)
		{
			CString strId;
			strId.Format(_T("%d,%d"),i,j);
			if(IsStringInArray(insertRangeArray,strId))
				continue;//已经处理过了的单元格不再处理

			int minRow=0;
			int maxRow=0;
			int minCol=0;
			int maxCol=0;
			AcDb::CellAlignment kAlign=spTable->alignment(i,j);
			GTableCellAlignRecordItem item;
			item.nRow=i;
			item.nCol=j;
			item.kAlign=kAlign;
			m_recordItemArray.Add(item);
			if(spTable->isMergedCell(i,j,&minRow,&maxRow,&minCol,&maxCol))
			{
				for (int k=minRow;k<=maxRow;k++)
				{
					for(int l=minCol;l<=maxCol;l++)
					{
						if(k==i&&l==j)
						{
							continue;
						}
						strId.Format(_T("%d,%d"),k,l);
						if(IsStringInArray(insertRangeArray,strId))
							continue;
						insertRangeArray.Add(strId);
					}
				}
			}
		}
	}
	return Acad::eOk;
}
GTableCellAlignRecord::GTableCellAlignRecord(const GTableCellAlignRecord& src)
{
	m_tableId=src.m_tableId;
	m_recordItemArray.RemoveAll();
	m_recordItemArray.Append(src.m_recordItemArray);
}
GTableCellAlignRecord& GTableCellAlignRecord::operator=(const GTableCellAlignRecord& src)
{
	if(this==&src)
	{
		return *this;
	}
	m_tableId=src.m_tableId;
	m_recordItemArray.RemoveAll();
	m_recordItemArray.Append(src.m_recordItemArray);
	return *this;
}
Acad::ErrorStatus GTableCellAlignRecord::MoveBack()
{
	AcDbObjectPointer<AcDbTable> spTable(m_tableId,AcDb::kForWrite);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	for(int i=0;i<m_recordItemArray.GetCount();i++)
	{
		GTableCellAlignRecordItem item=m_recordItemArray.GetAt(i);
		spTable->setAlignment(item.nRow,item.nCol,item.kAlign);
	}
	return Acad::eOk;
}
Acad::ErrorStatus GTableCellAlignRecord::SetXAlign(int nMode,int nAlign)
{
	AcDbObjectPointer<AcDbTable> spTable(m_tableId,AcDb::kForWrite);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	for(int i=0;i<m_recordItemArray.GetCount();i++)
	{
		GTableCellAlignRecordItem item=m_recordItemArray.GetAt(i);
		AcDb::RowType kRowType=spTable->rowType(item.nRow);
		if(kRowType==AcDb::kHeaderRow)
		{
			if(!IsXor(nMode,0x1))
			{
				continue;
			}
		}
		if(kRowType==AcDb::kTitleRow)
		{
			if(!IsXor(nMode,0x2))
			{
				continue;
			}
		}
		if(kRowType==AcDb::kDataRow)
		{
			if(!IsXor(nMode,0x4))
			{
				continue;
			}
		}
		AcDb::CellAlignment kAlign=spTable->alignment(item.nRow,item.nCol);
		int nXAlign;
		int nYAlign;
		ToXYAlign(kAlign,nXAlign,nYAlign);
		nXAlign=nAlign;
		ToTableAlign(kAlign,nXAlign,nYAlign);
		spTable->setAlignment(item.nRow,item.nCol,kAlign);
	}
	return Acad::eOk;
}
Acad::ErrorStatus GTableCellAlignRecord::SetYAlign(int nMode,int nAlign)
{
	AcDbObjectPointer<AcDbTable> spTable(m_tableId,AcDb::kForWrite);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	for(int i=0;i<m_recordItemArray.GetCount();i++)
	{
		GTableCellAlignRecordItem item=m_recordItemArray.GetAt(i);
		AcDb::RowType kRowType=spTable->rowType(item.nRow);
		if(kRowType==AcDb::kHeaderRow)
		{
			if(!IsXor(nMode,0x1))
			{
				continue;
			}
		}
		if(kRowType==AcDb::kTitleRow)
		{
			if(!IsXor(nMode,0x2))
			{
				continue;
			}
		}
		if(kRowType==AcDb::kDataRow)
		{
			if(!IsXor(nMode,0x4))
			{
				continue;
			}
		}
		AcDb::CellAlignment kAlign=spTable->alignment(item.nRow,item.nCol);
		int nXAlign;
		int nYAlign;
		ToXYAlign(kAlign,nXAlign,nYAlign);
		nYAlign=nAlign;
		ToTableAlign(kAlign,nXAlign,nYAlign);
		spTable->setAlignment(item.nRow,item.nCol,kAlign);
	}
	return Acad::eOk;
}
void GTableCellAlignRecord::ToXYAlign(const AcDb::CellAlignment kAlign,int& nXAlign,int& nYAlign)
{
	if(kAlign==kTopLeft)
	{
		nYAlign=0;
		nXAlign=0;
	}
	else if(kAlign==kTopCenter)
	{
		nYAlign=0;
		nXAlign=1;
	}
	else if(kAlign==kTopRight)
	{
		nYAlign=0;
		nXAlign=2;
	}
	else if(kAlign==kMiddleLeft)
	{
		nYAlign=1;
		nXAlign=0;
	}
	else if(kAlign==kMiddleCenter)
	{
		nYAlign=1;
		nXAlign=1;
	}
	else if(kAlign==kMiddleRight)
	{
		nYAlign=1;
		nXAlign=2;
	}
	else if(kAlign==kBottomLeft)
	{
		nYAlign=2;
		nXAlign=0;
	}
	else if(kAlign==kBottomCenter)
	{
		nYAlign=2;
		nXAlign=1;
	}
	else if(kAlign==kBottomRight)
	{
		nYAlign=2;
		nXAlign=2;
	}
}
void GTableCellAlignRecord::ToTableAlign(AcDb::CellAlignment& kAlign,const int nXAlign,const int nYAlign)
{
	if(nYAlign==0&&nXAlign==0)
	{
		kAlign=kTopLeft;
	}
	else if(nYAlign==0&&nXAlign==1)
	{
		kAlign=kTopCenter;
	}
	else if(nYAlign==0&&nXAlign==2)
	{
		kAlign=kTopRight;
	}
	else if(nYAlign==1&&nXAlign==0)
	{
		kAlign=kMiddleLeft;
	}
	else if(nYAlign==1&&nXAlign==1)
	{
		kAlign=kMiddleCenter;
	}
	else if(nYAlign==1&&nXAlign==2)
	{
		kAlign=kMiddleRight;
	}
	else if(nYAlign==2&&nXAlign==0)
	{
		kAlign=kBottomLeft;
	}
	else if(nYAlign==2&&nXAlign==1)
	{
		kAlign=kBottomCenter;
	}
	else if(nYAlign==2&&nXAlign==2)
	{
		kAlign=kBottomRight;
	}
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (GTableAlignDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(GTableAlignDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_CHECK2, &GTableAlignDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &GTableAlignDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &GTableAlignDialog::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &GTableAlignDialog::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &GTableAlignDialog::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &GTableAlignDialog::OnBnClickedCheck7)
	ON_BN_CLICKED(IDOK, &GTableAlignDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GTableAlignDialog::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &GTableAlignDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
GTableAlignDialog::GTableAlignDialog (AcDbObjectIdArray& m_ids,CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : 
CAcUiDialog (GTableAlignDialog::IDD, pParent, hInstance) 
{
	for(int i=0;i<m_ids.logicalLength();i++)
	{
		ids.append(m_ids.at(i));
		GTableCellAlignRecord rec;
		rec.m_tableId=m_ids.at(i);
		if(rec.Record()!=Acad::eOk)
		{
			continue;
		}
		m_recordArray.Add(rec);
	}
}

//-----------------------------------------------------------------------------
void GTableAlignDialog::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
}
BOOL GTableAlignDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	SetItemCheck(IDC_CHECK8,TRUE);
	SetItemCheck(IDC_CHECK9,TRUE);
	SetItemCheck(IDC_CHECK10,TRUE);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT GTableAlignDialog::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
void GTableAlignDialog::MoveBack()
{
	GLockDocument _lock;
	for(int i=0;i<m_recordArray.GetCount();i++)
	{
		GTableCellAlignRecord& rec=m_recordArray.GetAt(i);
		rec.MoveBack();
	}
}
void GTableAlignDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	//XLeft
	if(GetItemCheck(IDC_CHECK2))
	{
		SetItemCheck(IDC_CHECK3,FALSE);
		SetItemCheck(IDC_CHECK4,FALSE);
	}

	Apply();
}

void GTableAlignDialog::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	//XCenter
	if(GetItemCheck(IDC_CHECK3))
	{
		SetItemCheck(IDC_CHECK2,FALSE);
		SetItemCheck(IDC_CHECK4,FALSE);
	}

	Apply();
}

void GTableAlignDialog::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	//XRight
	if(GetItemCheck(IDC_CHECK4))
	{
		SetItemCheck(IDC_CHECK2,FALSE);
		SetItemCheck(IDC_CHECK3,FALSE);
	}

	Apply();
}

void GTableAlignDialog::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	//YTop
	if(GetItemCheck(IDC_CHECK5))
	{
		SetItemCheck(IDC_CHECK6,FALSE);
		SetItemCheck(IDC_CHECK7,FALSE);
	}
	Apply();
}

void GTableAlignDialog::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	//YCenter
	if(GetItemCheck(IDC_CHECK6))
	{
		SetItemCheck(IDC_CHECK5,FALSE);
		SetItemCheck(IDC_CHECK7,FALSE);
	}
	Apply();

}

void GTableAlignDialog::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	//YBottom
	if(GetItemCheck(IDC_CHECK7))
	{
		SetItemCheck(IDC_CHECK5,FALSE);
		SetItemCheck(IDC_CHECK6,FALSE);
	}

	Apply();
}
//应用
Acad::ErrorStatus GTableAlignDialog::Apply()
{
	MoveBack();
	GLockDocument _lock;
	int nMode=0;
	if(GetItemCheck(IDC_CHECK8))
	{
		nMode|=0x1;
	}
	if(GetItemCheck(IDC_CHECK9))
	{
		nMode|=0x2;
	}
	if(GetItemCheck(IDC_CHECK10))
	{
		nMode|=0x4;
	}
	int nXAlign=-1;
	if(GetItemCheck(IDC_CHECK2))
	{
		nXAlign=0;
	}
	else if(GetItemCheck(IDC_CHECK3))
	{
		nXAlign=1;
	}
	else if(GetItemCheck(IDC_CHECK4))
	{
		nXAlign=2;
	}
	if(nXAlign>=0)
	{
		for(int i=0;i<m_recordArray.GetCount();i++)
		{
			GTableCellAlignRecord& rec=m_recordArray.GetAt(i);
			rec.SetXAlign(nMode,nXAlign);
		}
	}
	int nYAlign=-1;
	if(GetItemCheck(IDC_CHECK5))
	{
		nYAlign=0;
	}
	else if(GetItemCheck(IDC_CHECK6))
	{
		nYAlign=1;
	}
	else if(GetItemCheck(IDC_CHECK7))
	{
		nYAlign=2;
	}
	if(nYAlign>=0)
	{
		for(int i=0;i<m_recordArray.GetCount();i++)
		{
			GTableCellAlignRecord& rec=m_recordArray.GetAt(i);
			rec.SetYAlign(nMode,nYAlign);
		}
	}
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	return Acad::eOk;
}

void GTableAlignDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_recordArray.RemoveAll();
	OnOK();
}

void GTableAlignDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	MoveBack();
	m_recordArray.RemoveAll();
	OnCancel();
}

void GTableAlignDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MoveBack();
	m_recordArray.RemoveAll();
	CAcUiDialog::OnClose();
}

void GTableAlignDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_recordArray.RemoveAll();
	for(int i=0;i<ids.logicalLength();i++)
	{
		GTableCellAlignRecord rec;
		rec.m_tableId=ids.at(i);
		if(rec.Record()!=Acad::eOk)
		{
			continue;
		}
		m_recordArray.Add(rec);
	}
}

#endif
