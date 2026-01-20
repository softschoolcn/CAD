// GEntAlignDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GEntAlignDialog.h"
#include "GLockDocument.h"
// GEntAlignDialog 对话框

IMPLEMENT_DYNAMIC(GEntAlignDialog, CAcUiDialog)

GEntAlignDialog::GEntAlignDialog(AcDbObjectIdArray& m_ids,CWnd* pParent /*=NULL*/,HINSTANCE hInstance)
	: CAcUiDialog(GEntAlignDialog::IDD, pParent,hInstance)
{
	for(int i=0;i<m_ids.logicalLength();i++)
	{
		ids.append(m_ids.at(i));
		pts.append(AcGePoint3d::kOrigin);
	}
}

GEntAlignDialog::~GEntAlignDialog()
{
}

void GEntAlignDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON1,m_btnPt);
	DDX_Control(pDX,IDC_BUTTON4,m_btnEnt);
}


BEGIN_MESSAGE_MAP(GEntAlignDialog, CDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_CHECK2, &GEntAlignDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &GEntAlignDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &GEntAlignDialog::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &GEntAlignDialog::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &GEntAlignDialog::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &GEntAlignDialog::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_BUTTON1, &GEntAlignDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &GEntAlignDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO2, &GEntAlignDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &GEntAlignDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &GEntAlignDialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDCANCEL, &GEntAlignDialog::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &GEntAlignDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GEntAlignDialog 消息处理程序
BOOL GEntAlignDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetItemCheck(IDC_RADIO3,TRUE);
	m_btnPt.AutoLoad();
	m_btnEnt.AutoLoad();

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GEntAlignDialog::OnBnClickedCheck2()
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

void GEntAlignDialog::OnBnClickedCheck3()
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

void GEntAlignDialog::OnBnClickedCheck4()
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

void GEntAlignDialog::OnBnClickedCheck5()
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

void GEntAlignDialog::OnBnClickedCheck6()
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

void GEntAlignDialog::OnBnClickedCheck7()
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
Acad::ErrorStatus GEntAlignDialog::Apply()
{
	GLockDocument _lock;
	AcDbExtents alginExt;
	int nAlignIndex=-1;
	if(GetItemCheck(IDC_RADIO3))
	{
		for(int i=ids.logicalLength()-1;i>=0;i--)
		{
			AcDbObjectId id=ids.at(i);
			AcDbEntityPointer spEnt(id,AcDb::kForRead);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			spEnt->getGeomExtents(alginExt);
			nAlignIndex=i;
			break;
		}
		if(nAlignIndex<0)
		{
			return Acad::eInvalidExtents;
		}
	}
	else if(GetItemCheck(IDC_RADIO1)||GetItemCheck(IDC_RADIO2))
	{
		alginExt=m_selExt;
	}
	
	for(int i=ids.logicalLength()-1;i>=0;i--)
	{
		if(i>=nAlignIndex&&nAlignIndex>=0)
		{
			continue;
		}
		AcDbExtents ext;
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->getGeomExtents(ext);
		double dx=0;
		bool bx=false;
		if(GetItemCheck(IDC_CHECK2))
		{
			//XLeft
			dx=alginExt.minPoint().x-ext.minPoint().x;
			bx=true;
		}
		else if(GetItemCheck(IDC_CHECK3))
		{
			//XCenter
			AcGePoint3d pt1=GCADUtils::MidPoint(alginExt.minPoint(),alginExt.maxPoint());
			AcGePoint3d pt2=GCADUtils::MidPoint(ext.minPoint(),ext.maxPoint());
			dx=pt1.x-pt2.x;
			bx=true;
		}
		else if(GetItemCheck(IDC_CHECK4))
		{
			//XRight
			dx=alginExt.maxPoint().x-ext.maxPoint().x;
			bx=true;
		}
		double dy=0.0;
		bool by=false;
		if(GetItemCheck(IDC_CHECK5))
		{
			//YTop
			dy=alginExt.maxPoint().y-ext.maxPoint().y;
			by=true;
		}
		else if(GetItemCheck(IDC_CHECK6))
		{
			//YCenter
			AcGePoint3d pt1=GCADUtils::MidPoint(alginExt.minPoint(),alginExt.maxPoint());
			AcGePoint3d pt2=GCADUtils::MidPoint(ext.minPoint(),ext.maxPoint());
			dy=pt1.y-pt2.y;
			by=true;
		}
		else if(GetItemCheck(IDC_CHECK7))
		{
			//YBottom
			dy=alginExt.minPoint().y-ext.minPoint().y;
			by=true;
		}
		AcGePoint3d lastPt=pts.at(i);
		AcGePoint3d lastPt1=lastPt;
		lastPt.x+=dx;
		lastPt.y+=dy;
		if(fabs(dx)>=1E-8||fabs(dy)>=1E-8)
		{
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,dy,0.0));
			spEnt->transformBy(mat);
		}
		if(!bx)
		{
			//X方向还原
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(-lastPt.x,0.0,0.0));
			spEnt->transformBy(mat);
			lastPt.x=0.0;
		}
		if(!by)
		{
			//X方向还原
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(0.0,-lastPt.y,0.0));
			spEnt->transformBy(mat);
			lastPt.y=0.0;
		}
		pts.setAt(i,AcGePoint3d(lastPt.x,lastPt.y,0.0));

	}
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();



	return Acad::eOk;
}

void GEntAlignDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//指定到对齐点
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO2,TRUE);
	SetItemCheck(IDC_RADIO4,FALSE);
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	ads_point p1;
	if (acedGetPoint(NULL, _T("\n请指定对齐点: "), p1) != RTNORM)
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	m_selExt.set(AcGePoint3d(p1[0],p1[1],0.0),AcGePoint3d(p1[0],p1[1],0.0));
	Apply();
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);
}

void GEntAlignDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//指定到对齐对象
	GLockDocument _lock;
	SetItemCheck(IDC_RADIO1,TRUE);
	SetItemCheck(IDC_RADIO2,FALSE);
	SetItemCheck(IDC_RADIO4,FALSE);
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择对齐对象:"),_T("\n移除对齐对象:")};
	if(acedSSGet(_T(":$:S"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	if(EntIds.logicalLength()!=1)
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	AcDbEntityPointer spEnt(EntIds.at(0),AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	spEnt->getGeomExtents(m_selExt);
	Apply();
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);
}

void GEntAlignDialog::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton1();
}

void GEntAlignDialog::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton4();
}
LRESULT GEntAlignDialog::OnAcadKeepFocus (WPARAM, LPARAM) 
{
	return (TRUE) ;
}

void GEntAlignDialog::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	Apply();
}

void GEntAlignDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	MoveBack();
	OnCancel();
}
void GEntAlignDialog::MoveBack()
{
	GLockDocument _lock;
	for(int i=ids.logicalLength()-1;i>=0;i--)
	{
		AcDbExtents ext;
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		AcGePoint3d lastPt=pts.at(i);
		if(fabs(lastPt.x)>1E-8||fabs(lastPt.y)>1E-8)
		{
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(-lastPt.x,-lastPt.y,0.0));
			spEnt->transformBy(mat);
			lastPt.x=0.0;
			pts.setAt(i,AcGePoint3d(lastPt.x,lastPt.y,0.0));
		}
		

	}
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

void GEntAlignDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MoveBack();
	CAcUiDialog::OnClose();
}

void GEntAlignDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ids.setLogicalLength(0);
	pts.setLogicalLength(0);
	OnOK();
}
