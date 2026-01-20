// DlgLayerSort.cpp : 实现文件
//

#include "stdafx.h"
#ifndef ACAD_2004
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "sorttab.h"
#include "DlgLayerSort.h"

const int kNameLength = 260;


bool getAllEntitiesOnLayer(TCHAR* layerName, AcDbObjectIdArray &EntObjIdArray)
{
	//设置过虑器
	struct resbuf filter; 
	TCHAR strLyNameBuf[260]={0}; 
	filter.restype = 8;
	lstrcpy(strLyNameBuf, layerName); 
	filter.resval.rstring = strLyNameBuf; 
	filter.rbnext = NULL;

	//查询指定图层所有实体
	ads_name ssName;
	if (RTNORM != acedSSGet(_T("X"),NULL,NULL,&filter,ssName)) 
		return false;
#if _MSC_VER>=1900
	Adesk::Int32 nNum = 0;
#else
	long nNum = 0;
#endif
	if(RTNORM != acedSSLength(ssName,&nNum))
	{
		acedSSFree(ssName); return false;
	}
	//acutPrintf(_T("\n%s=%d"),layerName,nNum);
	EntObjIdArray.setPhysicalLength(0);
	for(int i = 0; i < nNum; i++ ) 
	{
		ads_name eName;
		if(RTNORM != acedSSName(ssName, i, eName)) continue;
		AcDbObjectId tempId;
		if(Acad::eOk == acdbGetObjectId(tempId, eName))
			EntObjIdArray.append(tempId);
	}
	acedSSFree(ssName);

	return true;
}



bool getAllLayers(AcDbObjectIdArray &LyObjIdArray)
{	
	LyObjIdArray.setPhysicalLength(0);
	AcDbLayerTable *pLayerTable = NULL ;    
	Acad::ErrorStatus es ;   

	if (Acad::eOk !=  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead))
	{       
		acutPrintf (_T("\nCouldn''t get the layer table")); return false;    
	}   

	AcDbLayerTableIterator *pLayerTableIterator = NULL ;   
	if (Acad::eOk != pLayerTable->newIterator (pLayerTableIterator))
	{       
		acutPrintf (_T("\nCouldn''t get a new layer table iterator")) ;        
		pLayerTable->close () ;       
		return false ;    
	}    
	pLayerTable->close () ;  



	for (pLayerTableIterator->start();!pLayerTableIterator->done ();pLayerTableIterator->step()) 
	{     
		AcDbObjectId layerId ; 
		if (Acad::eOk == pLayerTableIterator->getRecordId(layerId))
		{
			LyObjIdArray.append(layerId);
		}
	}
	delete pLayerTableIterator;  pLayerTableIterator = NULL;

	return true;
}




// CDlgLayerSort 对话框

IMPLEMENT_DYNAMIC(CDlgLayerSort, CDialog)
CDlgLayerSort::CDlgLayerSort(CWnd* pParent /*=NULL*/)
: CDialog(CDlgLayerSort::IDD, pParent)
{
}

CDlgLayerSort::~CDlgLayerSort()
{
}

void CDlgLayerSort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LY, m_listly);
	DDX_Control(pDX, IDC_BTN_APPLY, m_ApplyBtn);
}


BEGIN_MESSAGE_MAP(CDlgLayerSort, CDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_OK, OnBnClickedBtnOk)
	ON_LBN_SELCHANGE(IDC_LIST_LY, OnLbnSelchangeListLy)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgLayerSort 消息处理程序

BOOL CDlgLayerSort::OnInitDialog( )
{
	m_listly.SubclassDlgItem(IDC_LIST_LY, this);
	//AcDbObjectIdArray arrAllLayers;
	//if(getAllLayers(arrAllLayers)) 
	//{
	//if(DisplayLayerOrder(arrAllLayers, NULL))
	//return TRUE;
	//}
	DisplayEntorder();

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());

	return FALSE;
}


BOOL CDlgLayerSort::DisplayLayerOrder(AcDbObjectIdArray &LyIdArray, CUIntArray* pIRepeatArray)
{
	m_listly.ResetContent();

	if (pIRepeatArray == NULL)  //读当前图层显示次序
	{	
		m_listly.AddString(_T(""));//最后添加一个空行
		for (int i = 0; i < LyIdArray.length(); i++)
		{
			//打开图层记录,读取其名称
			AcDbObject* pLayer=NULL;
			if (Acad::eOk == acdbOpenObject(pLayer, LyIdArray.at(i), AcDb::kForRead))
			{
				const TCHAR* name=NULL;
				AcDbLayerTableRecord::cast(pLayer)->getName(name);
				TCHAR newName[260]={0};
				//				strcpy(newName, _T(" "));
				lstrcat(newName, name);
				//				strcat(newName, _T(" "));
				//m_listly.AddString(newName);	
				m_listly.InsertString(0,newName);
				pLayer->close();
			}
		}	
		return TRUE;
	}
	else 
	{
		m_listly.AddString(_T(""));
		for (int i = 0; i < LyIdArray.length(); i++)
		{
			AcDbObject* pLayer = NULL;
			if (Acad::eOk == acdbOpenObject(pLayer, LyIdArray.at(i), AcDb::kForRead))
			{

				TCHAR* name = NULL;
				AcDbLayerTableRecord::cast(pLayer)->getName(name);
				/*				if (pIRepeatArray->GetAt(i) > 0)//有多个显示次序时,用*号来表示
				{
				char newName[260]={0};
				strcpy(newName, _T("*"));
				strcat(newName, name);
				strcat(newName, _T("*"));
				//m_listly.AddString(newName);
				m_listly.InsertString(0,newName);
				}else 
				*/				{
					TCHAR newName[260]={0};
					lstrcpy(newName, _T(" "));
					lstrcat(newName, name);
					lstrcat(newName, _T(" "));
					//m_listly.AddString(newName);
					m_listly.InsertString(0,newName);
				}
				pLayer->close();
			}
		}

		return TRUE;
	}
}



bool CDlgLayerSort::RefreshList(void)
{
	CStringArray arrItems;
	for(int i=0;i<m_listly.GetCount();i++)
	{
		CString sItem;
		m_listly.GetText(i,sItem);
		if(sItem[0]=='*')
			sItem.Replace('*',' ');
		arrItems.Add(sItem);
	}
	m_listly.ResetContent();

	for(int i=0;i<arrItems.GetCount();i++)
	{
		m_listly.AddString(arrItems.GetAt(i));
	}

	return true;
}


void CDlgLayerSort::ApplyLayerOrder(void)
{
	Acad::ErrorStatus es ;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL) return;

	AcDbObjectIdArray EntObjIdArray;
	EntObjIdArray.setPhysicalLength(0);

	for (int i =m_listly.GetCount()-1;i>=0; i--)
	{
		AcDbObjectIdArray EntOnLayerObjIdArray;
		EntOnLayerObjIdArray.setPhysicalLength(0);
		CString strLyName;
		m_listly.GetText(i, strLyName);
		strLyName.Replace('*',' ');
		strLyName.Trim();

		//	if(strLyName.CompareNoCase(_T("0"))==0) continue;
		if(strLyName.CompareNoCase(_T(""))==0) continue;
		if(getAllEntitiesOnLayer(strLyName.GetBuffer(0), EntOnLayerObjIdArray))
			EntObjIdArray.append(EntOnLayerObjIdArray);
		else
			acutPrintf(_T("\n Read Ly[%s] failed!"),strLyName);
		strLyName.ReleaseBuffer();
	}

	GLockDocument _Lock;//必须先锁定文档
	AcDbSortentsTable *pSortTab = NULL;
	GSortentsTableTools sortentsTable;
	if(sortentsTable.getOrCreateSortEntsDictionary(pSortTab)==Acad::eOk)
	{
		es = pSortTab->setRelativeDrawOrder(EntObjIdArray);
		pSortTab->close();
		pSortTab = NULL;
	}	

	//Regen to update the screen.
	ads_regen();

}


void CDlgLayerSort::OnBnClickedBtnApply()
{
	ApplyLayerOrder();

	//	RefreshList();
	return;
}

void CDlgLayerSort::OnBnClickedBtnOk()
{
	ApplyLayerOrder();
	RefreshList();

	OnOK();
}


void CDlgLayerSort::DisplayEntorder()
{
	Acad::ErrorStatus es ; 
	CUIntArray IRepeatArray;  //记录每一图层的实体个数(如果对应图层中没有实体,对应的值就为零)
	AcDbObjectIdArray EntObjIdArray;
	AcDbObjectIdArray lyObjIdArray;
	AcDbSortentsTable *pSortTab = NULL;
	GLockDocument _Lock;//必须先锁定文档
	GSortentsTableTools sortentsTable;
	//获取次序排列表指针
	if(sortentsTable.getOrCreateSortEntsDictionary(pSortTab)==Acad::eOk)  
	{
		es = pSortTab->getFullDrawOrder(EntObjIdArray);
		pSortTab->close(); pSortTab = NULL;
		if(es == Acad::eOk)
		{
			if(!EntObjIdArray.isEmpty())
			{		
				for(int i = 0; i < EntObjIdArray.length(); i++) 
				{
					AcDbEntity* pEnt=NULL;
					if(Acad::eOk != acdbOpenObject(pEnt,EntObjIdArray.at(i),AcDb::kForRead)) continue;
					if(lyObjIdArray.isEmpty())//第一次,直接插入
					{
						lyObjIdArray.append(pEnt->layerId());
						IRepeatArray.Add(0);
					}
					else 
					{
						//同一图层实体
						if( pEnt->layerId() == lyObjIdArray.last()) //连续同层实体,次序一致( 值 0 )
						{
							pEnt->close(); continue;
						}

						int index = 0;
						if(lyObjIdArray.find( pEnt->layerId(), index))
						{
							IRepeatArray.SetAt(index, 1);	//同一图层中的实体有不同的显示次序( 值 1 )
						}else 
						{
							lyObjIdArray.append(pEnt->layerId()); IRepeatArray.Add(0);
						}
					}
					pEnt->close();	
				}
			}
		}
		DisplayLayerOrder(lyObjIdArray, &IRepeatArray);
	}
}

void CDlgLayerSort::OnLbnSelchangeListLy()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_listly.GetCurSel()==m_listly.GetCount()-1)
	{
		m_listly.SetCurSel(m_listly.GetCount()-2);
	}
}

void CDlgLayerSort::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
#endif
