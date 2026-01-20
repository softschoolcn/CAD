// GFindDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GFindDialog.h"
#include "GFindTypeDialog.h"
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
// GFindDialog 对话框
IMPLEMENT_DYNAMIC(GFindDialog, CAcUiDialog)
GFindDialog::GFindDialog(CWnd* pParent /*=NULL*/,HINSTANCE hInstance/* =NULL*/)
: CAcUiDialog(GFindDialog::IDD, pParent,hInstance)
{
}

GFindDialog::~GFindDialog()
{
}

void GFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CUSTOM1,mGrid);
	DDX_Control(pDX,IDC_BUTTON_SELECTENT,m_btnEnt);
	DDX_Control(pDX,IDC_COMBO_EXT,m_comboExt);
}


BEGIN_MESSAGE_MAP(GFindDialog, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_FINDTYPE, &GFindDialog::OnBnClickedButtonFindtype)
	ON_BN_CLICKED(IDC_BUTTON_SELECTENT, &GFindDialog::OnBnClickedButtonSelectent)
	ON_BN_CLICKED(IDC_BUTTON_FLODER, &GFindDialog::OnBnClickedButtonFloder)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &GFindDialog::OnBnClickedButtonFile)
	ON_CBN_SELCHANGE(IDC_COMBO_EXT, &GFindDialog::OnCbnSelchangeComboExt)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &GFindDialog::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_CHECK_FULLMATCH, &GFindDialog::OnBnClickedCheckFullmatch)
	ON_BN_CLICKED(IDC_CHECK_TPF, &GFindDialog::OnBnClickedCheckTpf)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &GFindDialog::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// GFindDialog 消息处理程序

void GFindDialog::OnBnClickedButtonFindtype()
{
	CAcModuleResourceOverride res;
	GFindTypeDialog dlg(this);
	dlg.DoModal();
}
BOOL GFindDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	ReadTypeAlias();
	m_btnEnt.AutoLoad();
	m_comboExt.SetCurSel(1);
	m_ext.m_nType=1;
	mGrid.InsertColumn(_T("内容"));
	mGrid.InsertColumn(_T("图纸名称"));
	mGrid.InsertColumn(_T("查找类型"));
	mGrid.InsertColumn(_T("实体类型"));
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnWidthPercent(0, 40);
	mGrid.SetColumnWidthPercent(1, 25);
	mGrid.SetColumnWidthPercent(2, 15);
	mGrid.SetColumnWidthPercent(3, 15);

	m_findParam.Read();
    SetDlgItemText(IDC_EDIT_FINDSTR,m_findParam.strFind);
	SetItemCheck(IDC_CHECK_CASE,m_findParam.m_bCase);
	SetItemCheck(IDC_CHECK_FULLMATCH,m_findParam.m_bFullMatch);
	SetItemCheck(IDC_CHECK_TPF,m_findParam.m_bTPF);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}

void GFindDialog::OnBnClickedButtonSelectent()
{
	GLockDocument _lock;
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	if(acedSSGet(_T(":S"),NULL,NULL,NULL,ssName)!=RTNORM)
	{
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);
	m_comboExt.SetCurSel(0);
	m_ext.m_nType=0;
	m_ext.m_ids.setLogicalLength(0);
	m_ext.m_ids.append(EntIds);
}

void GFindDialog::OnBnClickedButtonFloder()
{
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
		m_comboExt.SetCurSel(2);
		m_ext.m_nType=2;
		m_ext.m_strFloder=path;
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(path);
	}

}

void GFindDialog::OnBnClickedButtonFile()
{
	const TCHAR* filea = NULL;
	const TCHAR* dlgname = _T("选择文件"); 
	struct resbuf* result = acutNewRb(RTSTR);
	int rc=acedGetFileNavDialog(filea, NULL, _T("dwg"), dlgname, 1|32|4096, &result);
	if(rc!=RTNORM)
	{
		acutRelRb(result);
		return;
	}
	m_comboExt.SetCurSel(3);
	m_ext.m_nType=3;
	m_ext.m_strFileNames.RemoveAll();

	struct resbuf* pTmp=result;
	int nIngore=0;
	int nCount=0;
	while(pTmp)
	{
		CString lpDwgFile=CString(pTmp->resval.rstring);
		m_ext.m_strFileNames.Add(lpDwgFile);
		pTmp=pTmp->rbnext;
	}
	acutRelRb(result);
	CString strText;
	if(m_ext.m_strFileNames.GetCount()==0)
	{
		strText=_T("未选择任何文件");
	}
	else
	{
		strText.Format(_T("\"%s\"等%d个文件"),m_ext.m_strFileNames.GetAt(0),m_ext.m_strFileNames.GetCount());
	}
	SetDlgItemText(IDC_EDIT_PATH,strText);
}

void GFindDialog::OnCbnSelchangeComboExt()
{
	int nSel=m_comboExt.GetCurSel();
	m_ext.m_nType=nSel;
	if(nSel==2)
	{
		OnBnClickedButtonFloder();
	}
	else if(nSel==3)
	{
		OnBnClickedButtonFile();
	}
	else if(nSel==0)
	{
		OnBnClickedButtonSelectent();
	}
}
void GFindDialog::OnBnClickedButtonFind()
{
	//查找
	GetDlgItemText(IDC_EDIT_FINDSTR,m_findParam.strFind);
	if(m_findParam.strFind.IsEmpty())
	{
		AfxMessageBox(_T("请输入查找字符串"));
		return;
	}
	m_findParam.m_bCase=GetItemCheck(IDC_CHECK_CASE);
	m_findParam.m_bFullMatch=GetItemCheck(IDC_CHECK_FULLMATCH);
	m_findParam.m_bTPF=GetItemCheck(IDC_CHECK_TPF);
	if(m_findParam.m_bTPF)
	{
		if(m_findParam.strFind.Find(_T("?"))<0&&m_findParam.strFind.Find(_T("*"))<0)
		{
			AfxMessageBox(_T("通配符中必须包含至少一个*号或者?号"));
			return;
		}
	}
	m_resultItems.RemoveAll();
	m_blockIds.setLogicalLength(0);
	m_types.Read();
	m_findParam.Write();
	GLockDocument _Lock;
	if(m_ext.m_nType==0)
	{
		//当前选择
		if(m_ext.m_ids.logicalLength()==0)
		{
			AfxMessageBox(_T("请先选择搜索对象"));
			return;
		}
		Find(_T(""),m_ext.m_ids);
	}
	else if(m_ext.m_nType==1)
	{
		Find(_T(""),acdbHostApplicationServices()->workingDatabase());
	}
	else if(m_ext.m_nType==2)
	{
		if(m_ext.m_strFloder.IsEmpty())
		{
			AfxMessageBox(_T("请先设置搜索文件夹"));
			return;
		}
		FindInFloder(m_ext.m_strFloder);
	}
	else if(m_ext.m_nType==3)
	{
		if(m_ext.m_strFileNames.GetCount()==0)
		{
			AfxMessageBox(_T("请先设置搜索文件"));
			return;
		}
		for(DWORD i=0;i<m_ext.m_strFileNames.GetCount();i++)
		{
			Find(m_ext.m_strFileNames.GetAt(i));
		}
	}
	UpdateGrid();


}
void GFindDialog::FindInFloder(CString strPath)
{
	CStringArray arrSubDirNames;
	CStringArray arrSubFileNames;
	EnumDirectoryContent(strPath,arrSubDirNames,arrSubFileNames);
	for(int i=0;i<arrSubDirNames.GetCount();i++)
	{
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,strPath);
		PathAppend(lpPath,arrSubDirNames.GetAt(i));
		FindInFloder(lpPath);
	}
	for(int i=0;i<arrSubFileNames.GetCount();i++)
	{
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,strPath);

		PathAppend(lpPath,arrSubFileNames.GetAt(i));
		if(lstrcmpi(PathFindExtension(lpPath),_T(".dwg"))==0)
		{
			Find(lpPath);
		}
	}
}
void GFindDialog::Find(CString strFile)
{
	AcApDocumentIterator* pIter = acDocManager->newAcApDocumentIterator();
	for (; !pIter->done(); pIter->step())
	{
		if (strFile.CompareNoCase(pIter->document()->fileName()) == 0)
		{
			//已经打开的文件
			Find(strFile,pIter->document()->database());
			delete pIter;
			return;
		}
	}
	delete pIter;
	AcDbDatabase* pDbTmp=new AcDbDatabase(false);
	try
	{
		Acad::ErrorStatus es=pDbTmp->readDwgFile(strFile);
		if(es!=eOk)
		{
			acutPrintf(_T("\n读取DWG文件:%s，发生错误:%s\n"),strFile,acadErrorStatusText(es));
		}
		else
		{
			Find(strFile,pDbTmp);
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
void GFindDialog::Find(CString strFile,AcDbDatabase* pDb)
{
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	Acad::ErrorStatus es=spBTR.openStatus();
	if(es!=eOk)
		return;
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		return;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if(pBTR->isLayout())
		{
			Find(strFile,pBTR);
			pBTR->close();
			pBTR=NULL;

		}
		else
		{
			pBTR->close();
			pBTR=NULL;
		}

	}
	delete pIterator;
	pIterator=NULL;
	return;
}
void GFindDialog::Find(CString strFile,AcDbBlockTableRecord* pBTR)
{
	AcDbBlockTableRecordIterator* pIter = NULL;
	pBTR->newIterator(pIter);

	std::auto_ptr<AcDbBlockTableRecordIterator> spIter(pIter);
	AcDbObjectId id;
	AcDbObjectIdArray ids;
	for(; !pIter->done(); pIter->step() )
	{
		if(Acad::eOk!=pIter->getEntityId(id))
		{
			continue;
		}
		ids.append(id);
	}
	Find(strFile,ids);
}
void GFindDialog::Find(CString strFile,AcDbObjectIdArray& ids)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		Find(strFile,ids.at(i));
	}
}
//获取标注中的文字
Acad::ErrorStatus GetDimText(CString& strText,CString& strTextCAD,AcDbDimension* pDim)
{
	AcDbObjectIdArray ids;
	Acad::ErrorStatus es=EnumAllEntityInBTR(ids,pDim->dimBlockId());
	if(es!=Acad::eOk)
	{
		return es;
	}
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForRead);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());
			TCHAR* pTextStr=pText->textString();
			strText=pTextStr;
			strTextCAD=strText;
			acutDelString(pTextStr);
			return Acad::eOk;
		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			TCHAR* pTextStr=pText->contents();
			strText=pTextStr;
			strTextCAD=strText;
			acutDelString(pTextStr);
			MTextItemArray itemArr;
			itemArr.item.str=strText;
			itemArr.item.nType=-1;
			itemArr.updateChildren();
			strText=itemArr.getText();
			return Acad::eOk;
		}
	}
	return Acad::eNotApplicable;

}
Acad::ErrorStatus _getEntXData(const resbuf* rb, int nItem, CStringArray& strValueArray)
{
	CString dxfCodeStr, valueStr;
	const resbuf* tmp = rb;

	CString msg;
	int i = 0;
	while (tmp)
	{
		if (nItem != tmp->restype)
		{
			tmp = tmp->rbnext;
			continue;
		}
		GCADUtils::dxfToStr(tmp, dxfCodeStr, valueStr);
		valueStr.Format(_T("%s"), tmp->resval.rstring) ;
			strValueArray.Add(valueStr);
		tmp = tmp->rbnext;
	}

	return Acad::eOk;
}
void GFindDialog::Find(CString strFile,AcDbObjectId id)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbEntityPointer spEnt(id,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	if(spEnt->isKindOf(AcDbBlockReference::desc()))//1
	{
		AcDbBlockReference* pRef=AcDbBlockReference::cast(spEnt.object());
		//块参照
		if(m_types.m_bAttr)//2
		{
			//遍历属性
			AcDbObjectIterator* pIter=pRef->attributeIterator();
			if(pIter!=NULL)
			{
				AcDbObjectIdArray ids;
				for(pIter->start();!pIter->done();pIter->step())
				{
					AcDbObjectId id=pIter->objectId();
					ids.append(id);
				}
				delete pIter;
				pIter=NULL;
				for(int i=0;i<ids.logicalLength();i++)
				{
					AcDbObjectId id=ids.at(i);
					AcDbAttribute* pAttr=NULL;
					es=pRef->openAttribute(pAttr,id,AcDb::kForWrite);
					if(es!=Acad::eOk)
					{
						continue;
					}
					if(pAttr->visibility()!=AcDb::kVisible)
					{
						//不可见的属性不转换
						pAttr->erase();
						pAttr->close();
						continue;
					}
					TCHAR* lpAttr=pAttr->textString();
					CString strAlias=GetTypeAlias(pAttr->isA()->name());
					CString strAttr=lpAttr;
					acutDelString(lpAttr);
					pAttr->erase();
					pAttr->close();
					if(m_findParam.IsMatch(strAttr))
					{
						GFindResultItem item;
						item.m_nType=0;
						item.m_handle=id.handle();
						item.m_strFile=strFile;
						item.m_strText=strAttr;
						item.m_strEntType=strAlias;
						m_resultItems.Add(item);
					}
				}
			}
		}//2
		if(m_types.m_bBlock)//2
		{
			//块中的定义
			bool bFind=false;
			for(int i=0;i<m_blockIds.logicalLength();i++)
			{
				if(m_blockIds.at(i)==pRef->blockTableRecord())
				{
					bFind=true;
					break;
				}
			}
			if(!bFind)
			{
				m_blockIds.append(pRef->blockTableRecord());
				AcDbBlockTableRecordPointer spRec(pRef->blockTableRecord(),AcDb::kForRead);
				if(spRec.openStatus()==Acad::eOk)
				{
					Find(strFile,spRec.object());
				}
			}
		}//2

	}//1
	else if(spEnt->isKindOf(AcDbText::desc()))//1
	{
		if(m_types.m_bText)//2
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());
			CString strAlias=GetTypeAlias(pText->isA()->name());
			TCHAR* lpAttr=pText->textString();
			CString strAttr=lpAttr;
			acutDelString(lpAttr);
			if(m_findParam.IsMatch(strAttr))
			{
				GFindResultItem item;
				item.m_nType=2;
				item.m_handle=id.handle();
				item.m_strFile=strFile;
				item.m_strText=strAttr;
				item.m_strEntType=strAlias;
				m_resultItems.Add(item);
			}
		}//2
	}//1
	else if(spEnt->isKindOf(AcDbMText::desc()))//1
	{
		if(m_types.m_bText)//2
		{
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			CString strAlias=GetTypeAlias(pText->isA()->name());
			TCHAR* lpAttr=pText->contents();
			CString strAttr=lpAttr;
			acutDelString(lpAttr);
			if(m_findParam.IsMatch(strAttr))
			{
				GFindResultItem item;
				item.m_nType=2;
				item.m_handle=id.handle();
				item.m_strFile=strFile;
				item.m_strText=strAttr;
				item.m_strEntType=strAlias;
				m_resultItems.Add(item);
			}
		}//2
	}//1
	else if(spEnt->isKindOf(AcDbDimension::desc()))
	{
		AcDbDimension* pDim=AcDbDimension::cast(spEnt.object());
		if(m_types.m_bDim)//2
		{
			CString strText;
			CString strTextCAD;
			if(Acad::eOk==GetDimText(strText,strTextCAD,pDim))//3
			{
				if(m_findParam.IsMatch(strText))//4
				{
					GFindResultItem item;
					item.m_nType=1;
					item.m_handle=id.handle();
					item.m_strFile=strFile;
					item.m_strText=strTextCAD;
					item.m_strEntType=GetTypeAlias(pDim->isA()->name());
					m_resultItems.Add(item);
				}//4
			}//3
		}//2
	}
	#ifndef ACAD_2004
	else if(spEnt->isKindOf(AcDbTable::desc()))
	{
		if(m_types.m_bTable)
		{
			Find(strFile,AcDbTable::cast(spEnt.object()));
		}
	}
#endif
	if(m_types.m_bLink||m_types.m_bLinkDesc)
	{
		struct resbuf* rb=spEnt->xData(_T("PE_URL"));
		if(rb!=NULL)
		{
			CStringArray strValueArray;
			_getEntXData(rb,1000,strValueArray);
		acutRelRb(rb);
		if(strValueArray.GetCount()>=1)
		{
			CString strLink=strValueArray.GetAt(0);
			CString strLinkDesc=strValueArray.GetCount()>=2?strValueArray.GetAt(1):strLink;
			if(m_types.m_bLink&&m_findParam.IsMatch(strLink))
			{
				GFindResultItem item;
					item.m_nType=5;
					item.m_handle=id.handle();
					item.m_strFile=strFile;
					item.m_strText=strLink;
					item.m_strEntType=GetTypeAlias(spEnt->isA()->name());
					m_resultItems.Add(item);
			}
			if(m_types.m_bLinkDesc&&m_findParam.IsMatch(strLinkDesc))
			{
				GFindResultItem item;
					item.m_nType=4;
					item.m_handle=id.handle();
					item.m_strFile=strFile;
					item.m_strText=strLinkDesc;
					item.m_strEntType=GetTypeAlias(spEnt->isA()->name());
					m_resultItems.Add(item);
			}
		}
		}
	}

}
#ifndef ACAD_2004
void GFindDialog::Find(CString strFile,AcDbTable* pTable)
{
	//遍历行
	CStringArray insertRangeArray;//将已经处理的单元格记录下来，不重复处理
	for(int i=0;i<pTable->numRows();i++)
	{
		for(int j=0;j<pTable->numColumns();j++)
		{
			CString strId;
			strId.Format(_T("%d,%d"),i,j);
			if(GStringHelper::IsStringInArray(insertRangeArray,strId))
				continue;//已经处理过了的单元格不再处理

			int minRow=0;
			int maxRow=0;
			int minCol=0;
			int maxCol=0;
			CString strText=pTable->textStringConst(i,j);
			if(m_findParam.IsMatch(strText))//4
				{
					GFindResultItem item;
					item.m_nType=3;
					pTable->getAcDbHandle(item.m_handle);
					item.m_strFile=strFile;
					item.m_strText=strText;
					item.m_strEntType=GetTypeAlias(pTable->isA()->name());
					m_resultItems.Add(item);
				}//4
			if(pTable->isMergedCell(i,j,&minRow,&maxRow,&minCol,&maxCol))
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
						if(GStringHelper::IsStringInArray(insertRangeArray,strId))
							continue;
						insertRangeArray.Add(strId);
					}
				}
			}
		}
	}
}
#endif

void GFindDialog::UpdateGrid()
{
	ClearGrid();
	for(int i=0;i<m_resultItems.GetCount();i++)
	{
		GFindResultItem item=m_resultItems.GetAt(i);
		int row = mGrid.InsertRow(item.m_strText);
		CString strName=_T("当前图纸");
		if(!item.m_strFile.IsEmpty())
		{
		TCHAR lpFile[MAX_PATH+1]={0};
		lstrcpy(lpFile,item.m_strFile);
		strName=PathFindFileName(lpFile);
		}
		mGrid.SetItemText(row,1,strName);
		mGrid.SetItemText(row,2,GFindTypes::GetTypeDesc(item.m_nType));
		mGrid.SetItemText(row,3,item.m_strEntType);
	}
}
/*!
     @brief 清空结果表格
     */
	void GFindDialog::ClearGrid()
	{
		for (int nRow = mGrid.GetRowCount(); nRow >= 1; nRow--)
		mGrid.DeleteRow(nRow);
	}

	void GFindDialog::OnBnClickedCheckFullmatch()
	{
		if(GetItemCheck(IDC_CHECK_FULLMATCH))
		{
			SetItemCheck(IDC_CHECK_TPF,FALSE);
		}
	}

	void GFindDialog::OnBnClickedCheckTpf()
	{
		if(GetItemCheck(IDC_CHECK_TPF))
		{
			SetItemCheck(IDC_CHECK_FULLMATCH,FALSE);
		}
	}
	BOOL GFindDialog::ReadTypeAlias()
{
	TCHAR m_lpFile[MAX_PATH+1]={0};
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);
	HMODULE hModule=_hdllInstance;
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
CString GFindDialog::GetTypeAlias(const CString strName) const
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
CString GFindDialog::GetTypeName(const CString strAlias) const
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

void GFindDialog::OnBnClickedButtonHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(_T("通配符*表示匹配任何内容，但是不能匹配空的内容\r\n通配符?表示匹配任何内容，包含空的内容\r\n例如\"*线\"可以匹配\"进线\"或者\"出线\"但是不能匹配\"线\"\r\n\"?线\"可以匹配\"进线\"或者\"出线\"也同时能匹配\"线\""));
}
