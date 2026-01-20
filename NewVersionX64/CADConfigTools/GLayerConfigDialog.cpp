// GLayerConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GLayerConfigDialog.h"
#include "GConfigReader.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "ConfigToolsHeaders.h"
//功能:	激活当前视图
Acad::ErrorStatus activeCurrentview()
{
	Acad::ErrorStatus es=Acad::eOk;

	if(acedGetAcadFrame()!=NULL)
	{
		CFrameWnd	*pFrame;
		pFrame	= acedGetAcadFrame()->GetActiveFrame();

		if(pFrame != NULL)
		{
			CView	*pView;
			pView	= pFrame->GetActiveView();
			if(pView != NULL)
				pView->SetFocus();
			else
				es=Acad::eNotApplicable;
		}
		else
			es=Acad::eNotApplicable;
	}
	else
		es=Acad::eNotApplicable;

	return es;
}
 Acad::ErrorStatus UpdateLayerByConfig(AcDbLayerTableRecord* pRec,GLayerDefine& layer,AcDbDatabase* pDB)
 {
	 AcCmColor color;
	 color.setColorIndex(layer.nColorIndex);
	 pRec->setColor(color);
	 AcDbObjectId			lnTypeId;
	 lnTypeId.setNull();
	 if(GCADUtils::GetLineTypeId(lnTypeId,layer.strLineType,pDB)!=Acad::eOk)
	 {
		 GCADUtils::LoadLineType(lnTypeId,layer.strLineType,pDB);
	 }
	 if(lnTypeId.isValid())
	 {
		 pRec->setLinetypeObjectId(lnTypeId);
	 }
	 return Acad::eOk;
 }
 //根据配置修改图层,返回图层的ID
//bCreateNew=false,没有则不创建，不更新
//bCreateNew=true,没有则创建，更新
Acad::ErrorStatus UpdateLayerByConfig(AcDbObjectId&   rcId,GLayerDefine& layer,bool bCreateNew,AcDbDatabase* pDB)
{
	if(pDB==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbLayerTablePointer spTable(pDB->layerTableId(),AcDb::kForRead);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	AcDbObjectId recordId;
	Acad::ErrorStatus es=spTable->getAt(layer.strLayerName,recordId);
	if(es!=Acad::eOk)
	{
		if(!bCreateNew)
		{
			return Acad::eOk;
		}
		AcDbLayerTableRecord   * pRec=new AcDbLayerTableRecord();
		pRec->setName(layer.strLayerName);
		es=UpdateLayerByConfig(pRec,layer,pDB);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		if(spTable->upgradeOpen()!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return spTable->upgradeOpen();
		}
		es=spTable->add(rcId,pRec);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		pRec->close();
		return es;
		
	}
	else
	{
		AcDbLayerTableRecordPointer spRec(recordId,AcDb::kForWrite);
		if(spRec.openStatus()!=Acad::eOk)
		{
			return spRec.openStatus();
		}
		es=UpdateLayerByConfig(spRec.object(),layer,pDB);
		if(es!=Acad::eOk)
		{
			return es;
		}
		rcId=recordId;
		return es;
	}
}
//根据配置修改图层,返回图层的ID
//bCreateNew=false,没有则不创建，不更新
//bCreateNew=true,没有则创建，更新
Acad::ErrorStatus UpdateLayerByConfig(AcDbObjectId&   rcId,
									  CString strName,
									  bool bCreateNew,
									  AcDbDatabase* pDB)
{
	CArray<GLayerDefine> layers;
	ReadDefinedLayer(layers);
	for(int i=0;i<layers.GetCount();i++)
	{
		GLayerDefine& layer=layers.GetAt(i);
		if(layer.strName.CompareNoCase(strName)!=0)
		{
			continue;
		}
		return UpdateLayerByConfig(rcId,layer,bCreateNew,pDB);
	}
	return Acad::eKeyNotFound;
}
//old_linetypeId
 bool acedLinetypeDialog( AcDbObjectId old_linetypeId, // ObjectId for current linetype: acdbCurDwg()->celtype();
	 bool IncludeByBlockByLayer, // Posibility of selecting "bylayer" or "byblock" 
	 TCHAR *&new_linetypeName, // Name of selected linetype 
	 AcDbObjectId &new_linetypeId // ObjectId for selected linetype 
	 ); 
 void ads_regen();
//根据配置修改图层,返回图层的ID
//bCreateNew=false,没有则不创建，不更新
//bCreateNew=true,没有则创建，更新
Acad::ErrorStatus UpdateAllLayerByConfig(AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase())
{
	AcDbObjectId   rcId;
	CArray<GLayerDefine> layers;
	ReadDefinedLayer(layers);
	for(int i=0;i<layers.GetCount();i++)
	{
		GLayerDefine& layer=layers.GetAt(i);
		UpdateLayerByConfig(rcId,layer,false,pDB);
	}
	return Acad::eOk;
}
// GLayerConfigDialog 对话框

IMPLEMENT_DYNAMIC(GLayerConfigDialog, CDialog)

GLayerConfigDialog::GLayerConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GLayerConfigDialog::IDD, pParent)
{

}

GLayerConfigDialog::~GLayerConfigDialog()
{
}

void GLayerConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRID,mGrid);
}


BEGIN_MESSAGE_MAP(GLayerConfigDialog, CDialog)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
END_MESSAGE_MAP()


// GLayerConfigDialog 消息处理程序
BOOL GLayerConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CGridCell* pCell = new CGridCell();
	mGrid.SetDefaultCellType(pCell->GetRuntimeClass());
	int nCol=0;
	nCol=mGrid.InsertColumn(_T("    图层含义    "));
	mGrid.SetColumnWidth(nCol,150);
	nCol=mGrid.InsertColumn(_T("图名"));
	mGrid.SetColumnWidth(nCol,120);
	nCol=mGrid.InsertColumn(_T("颜色"));
	mGrid.SetColumnWidth(nCol,100);
	nCol=mGrid.InsertColumn(_T("线型"));
	mGrid.SetColumnWidth(nCol,120);
	mGrid.SetRowHeight(0, 25);
	mGrid.SetFixedRowCount(1);
	mGrid.SetFixedColumnCount(1);
	mGrid.AutoSizeColumn(0);

	CArray<GLayerDefine> layers;
	ReadDefinedLayer(layers);
	for(int i=0;i<layers.GetCount();i++)
	{
		GLayerDefine& layer=layers.GetAt(i);
		int row = mGrid.InsertRow(layer.strName);  //插入命令名称
		mGrid.SetItemText(row,1,layer.strLayerName);
		mGrid.SetItemText(row,2,layer.GetColorName());
		mGrid.SetItemData(row,2,(LPARAM)layer.nColorIndex);
		mGrid.SetCellType(row,2,RUNTIME_CLASS(CGridCellColor));
		mGrid.SetItemState(row,2,mGrid.GetItemState(row,2)|GVIS_READONLY);
		mGrid.SetItemText(row,3,layer.strLineType);
		mGrid.SetItemState(row,3,mGrid.GetItemState(row,3)|GVIS_READONLY);
	}
	return TRUE;
}
void GLayerConfigDialog::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	int nRow=pItem->iRow;
	CString strLayer=mGrid.GetItemText(nRow,1);
	int nColorIndex=(int)mGrid.GetItemData(nRow,2);
	CString strColor=mGrid.GetItemText(nRow,2);
	CString strLienType=mGrid.GetItemText(nRow,3);
	for(int i=1;i<mGrid.GetRowCount();i++)
	{
		if(i==nRow)
		{
			continue;
		}
		CString strLayer1=mGrid.GetItemText(i,1);
		if(strLayer1.CompareNoCase(strLayer)!=0)
		{
			continue;
		}
		mGrid.SetItemText(i,3,strLienType);
		mGrid.SetItemText(i,2,strColor);
		mGrid.SetItemData(i,2,(LPARAM)nColorIndex);
	}
	mGrid.Invalidate();
	mGrid.UpdateWindow();
}
void GLayerConfigDialog::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	if(pItem->iColumn!=2&&pItem->iColumn!=3)
	{
		return;
	}
	if(pItem->iColumn==3)
	{
		AcDbObjectId new_linetypeId;
        TCHAR * new_linetypename = NULL;
		if(!acedLinetypeDialog(acdbCurDwg()->celtype(),false,new_linetypename,new_linetypeId))
		{
			return;
		}
		mGrid.SetItemText(pItem->iRow,pItem->iColumn,new_linetypename);
		acutDelString(new_linetypename);

	}
	int nRow=pItem->iRow;
	CString strLayer=mGrid.GetItemText(nRow,1);
	int nColorIndex=(int)mGrid.GetItemData(nRow,2);
	CString strColor=mGrid.GetItemText(nRow,2);
	CString strLienType=mGrid.GetItemText(nRow,3);
	for(int i=1;i<mGrid.GetRowCount();i++)
	{
		if(i==nRow)
		{
			continue;
		}
		CString strLayer1=mGrid.GetItemText(i,1);
		if(strLayer1.CompareNoCase(strLayer)!=0)
		{
			continue;
		}
		mGrid.SetItemText(i,3,strLienType);
		mGrid.SetItemText(i,2,strColor);
		mGrid.SetItemData(i,2,(LPARAM)nColorIndex);
	}
	mGrid.Invalidate();
	mGrid.UpdateWindow();
}
//执行设置
BOOL GLayerConfigDialog::ExcSet()
{
	TCHAR lpPath[MAX_PATH+1]={0};
	GetModuleFileName(_hdllInstance,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathRemoveFileSpec(lpPath);
	TCHAR lpPath1[MAX_PATH+1]={0};
	lstrcpy(lpPath1,lpPath);
	PathAppend(lpPath1,_T("UserConfig\\LayerDefine.xml"));
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	long hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);

	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='GB2312'"));
	lpDocument->appendChild(lpInstruction);

	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("Root")));

	lpDocument->appendChild(lpRoot);

	// 参数列表
	for(int i=1;i<mGrid.GetRowCount();i++)
	{
		int nColorIndex=(int)mGrid.GetItemData(i,2);
		CString strColor;
		strColor.Format(_T("%d"),nColorIndex);
		CString strLienType=mGrid.GetItemText(i,3);
		CString strLayerName=mGrid.GetItemText(i,1);
		CString strName=mGrid.GetItemText(i,0);
		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Layer"));
		lpParam->setAttribute(_T("Name"), _variant_t(strName));
		lpParam->setAttribute(_T("LayerName"), _variant_t(strLayerName));
		lpParam->setAttribute(_T("Color"), _variant_t(strColor));
		lpParam->setAttribute(_T("LineType"), _variant_t(strLienType));

		lpRoot->appendChild(lpParam);
	}
	hr=lpDocument->save(lpPath1);
	if(SUCCEEDED(hr))
	{
		GLockDocument _lock;
		UpdateAllLayerByConfig();
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("保存失败"));
		return FALSE;
	}
}