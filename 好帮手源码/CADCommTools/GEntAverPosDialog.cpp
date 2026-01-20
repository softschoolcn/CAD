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
//----- GEntAverPosDialog.cpp : Implementation of GEntAverPosDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GEntAverPosDialog.h"
#include "GLockDocument.h"
#include "GxAfxFunction.h"
#include "SortArray.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (GEntAverPosDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(GEntAverPosDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_CHECK2, &GEntAverPosDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &GEntAverPosDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &GEntAverPosDialog::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &GEntAverPosDialog::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &GEntAverPosDialog::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &GEntAverPosDialog::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &GEntAverPosDialog::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &GEntAverPosDialog::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_RADIO4, &GEntAverPosDialog::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON1, &GEntAverPosDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &GEntAverPosDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO1, &GEntAverPosDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &GEntAverPosDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &GEntAverPosDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GEntAverPosDialog::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
GEntAverPosDialog::GEntAverPosDialog (AcDbObjectIdArray& m_ids,CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (GEntAverPosDialog::IDD, pParent, hInstance) 
{
	for(int i=0;i<m_ids.logicalLength();i++)
	{
		ids.append(m_ids.at(i));
		pts.append(AcGePoint3d::kOrigin);
	}
}

//-----------------------------------------------------------------------------
void GEntAverPosDialog::DoDataExchange (CDataExchange *pDX) 
{
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_BUTTON1,m_btnExt);
	DDX_Control(pDX,IDC_BUTTON4,m_btnPos);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT GEntAverPosDialog::OnAcadKeepFocus (WPARAM, LPARAM) 
{
	return (TRUE) ;
}
BOOL GEntAverPosDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_btnExt.AutoLoad();
	m_btnPos.AutoLoad();
	SetItemCheck(IDC_RADIO4,TRUE);

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GEntAverPosDialog::MoveBack()
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
			lastPt.y=0.0;
			pts.setAt(i,AcGePoint3d(lastPt.x,lastPt.y,0.0));
		}


	}
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}
void GEntAverPosDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK2))
	{
		SetItemCheck(IDC_CHECK3,FALSE);
		SetItemCheck(IDC_CHECK4,FALSE);
		SetItemCheck(IDC_CHECK5,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK3))
	{
		SetItemCheck(IDC_CHECK2,FALSE);
		SetItemCheck(IDC_CHECK4,FALSE);
		SetItemCheck(IDC_CHECK5,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK4))
	{
		SetItemCheck(IDC_CHECK2,FALSE);
		SetItemCheck(IDC_CHECK3,FALSE);
		SetItemCheck(IDC_CHECK5,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK5))
	{
		SetItemCheck(IDC_CHECK2,FALSE);
		SetItemCheck(IDC_CHECK3,FALSE);
		SetItemCheck(IDC_CHECK4,FALSE);
	}
	Apply();
}
int MyFindMax(CArray<double>& valueArr,int nMin=0)
{
	double dMax=0.0;
	int nMaxIndex=-1;
	for(int i=nMin;i<valueArr.GetCount();i++)
	{
		if(i==nMin||valueArr.GetAt(i)>dMax)
		{
			nMaxIndex=i;
			dMax=valueArr.GetAt(i);
		}
	}
	return nMaxIndex;
}
int MyFindMin(CArray<double>& valueArr,int nMin=0)
{
	double dMin=0.0;
	int nMinIndex=-1;
	for(int i=nMin;i<valueArr.GetCount();i++)
	{
		if(i==nMin||valueArr.GetAt(i)<dMin)
		{
			nMinIndex=i;
			dMin=valueArr.GetAt(i);
		}
	}
	return nMinIndex;
}
void MySwap(CArray<double>& minArr,CArray<double>& maxArr,CArray<AcDbObjectId>& xIdArr,int i,int j)
{
	double dTmp=0.0;
	AcDbObjectId idTmp=AcDbObjectId::kNull;
	dTmp=minArr.GetAt(i);
	minArr.SetAt(i,minArr.GetAt(j));
	minArr.SetAt(j,dTmp);

	dTmp=maxArr.GetAt(i);
	maxArr.SetAt(i,maxArr.GetAt(j));
	maxArr.SetAt(j,dTmp);

	idTmp=xIdArr.GetAt(i);
	xIdArr.SetAt(i,xIdArr.GetAt(j));
	xIdArr.SetAt(j,idTmp);
}
//特殊的排序
void MySortArray(CArray<double>& minArr,CArray<double>& maxArr,CArray<AcDbObjectId>& xIdArr)
{
	//先找到最大值最小的
	int nIndex=MyFindMin(maxArr);
	if(nIndex!=0)
	{
		//确保0最大值最小
		MySwap(minArr,maxArr,xIdArr,0,nIndex);
	}
	for(int j=1;j<minArr.GetCount()-1;j++)
	{
		nIndex=MyFindMin(minArr,j);
		if(nIndex!=j)
		{
			MySwap(minArr,maxArr,xIdArr,j,nIndex);
		}
	}

}
int FindObjectId(CArray<AcDbObjectId>& xIdArr,AcDbObjectId id)
{
	for(int i=0;i<xIdArr.GetCount();i++)
	{
		if(id==xIdArr.GetAt(i))
		{
			return i;
		}
	}
	return -1;
}
void GEntAverPosDialog::ApplyXSimple(CArray<double>& xMinArr,CArray<double>& xMaxArr,CArray<AcDbObjectId>& xIdArr)
{
	//相对比较简单
	CArray<AcDbObjectId> xIdArr1;
	CArray<double>       xArr;
	if(GetItemCheck(IDC_CHECK2))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add(xMinArr.GetAt(i));
		}
	}
	else if(GetItemCheck(IDC_CHECK3))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add((xMinArr.GetAt(i)+xMaxArr.GetAt(i))*0.5);
		}
	}
	else if(GetItemCheck(IDC_CHECK5))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add(xMaxArr.GetAt(i));
		}
	}
	SortArray(xIdArr1,xArr,false);
	//低于3个无法排序
	if(GetItemCheck(IDC_RADIO4))
	{
		double dis=(xArr.GetAt(xArr.GetCount()-1)-xArr.GetAt(0))/(xArr.GetCount()-1);
		for(int i=1;i<xArr.GetCount()-1;i++)
		{
			double dx=xArr.GetAt(0)+i*dis;
			dx-=xArr.GetAt(i);
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			spEnt->transformBy(mat);
		}
	}
	else if(GetItemCheck(IDC_RADIO1))
	{
		//指定行列间距
		for(int i=1;i<xArr.GetCount();i++)
		{
			double dx=xArr.GetAt(i-1)+ptMax.x-ptMin.x;
			dx-=xArr.GetAt(i);
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			spEnt->transformBy(mat);
			xArr.SetAt(i,xArr.GetAt(i-1)+ptMax.x-ptMin.x);
		}

	}
	else if(GetItemCheck(IDC_RADIO2))
	{
		//指定范围
		double dMin= xMinArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(0)));
		double dMax=xMaxArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(xIdArr1.GetCount()-1)));
		double dEnd=xArr.GetAt(xArr.GetCount()-1)+ptMax.x-dMax;
		double dStart=xArr.GetAt(0)+ptMin.x-dMin;
		double dis=(dEnd-dStart)/(xArr.GetCount()-1);
		for(int i=0;i<xArr.GetCount();i++)
		{
			double dx=0.0;
			if(i>=1&&i<xArr.GetCount()-1)
			{
				dx=xArr.GetAt(0)+i*dis;
				dx-=xArr.GetAt(i);
			}
			else if(i==0)
			{
				dx=ptMin.x-dMin;
				xArr.SetAt(i,xArr.GetAt(i)+dx);
			}
			else
			{
				dx=ptMax.x-dMax;
				xArr.SetAt(i,xArr.GetAt(i)+dx);
			}
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			spEnt->transformBy(mat);
		}

	}

}
void GEntAverPosDialog::ApplyXNotSimple(CArray<double>& xMinArr,CArray<double>& xMaxArr,CArray<AcDbObjectId>& xIdArr)
{
	CArray<AcDbObjectId> xIdArr1;
	CArray<double>       xMinArr1;
	CArray<double>       xMaxArr1;
	for(int i=0;i<xIdArr.GetCount();i++)
	{
		xIdArr1.Add(xIdArr.GetAt(i));
		xMinArr1.Add(xMinArr.GetAt(i));
		xMaxArr1.Add(xMaxArr.GetAt(i));
	}
	MySortArray(xMinArr1,xMaxArr1,xIdArr1);
	if(GetItemCheck(IDC_RADIO4))
	{
		double dis=0.0;
		for(int i=0;i<xIdArr.GetCount()-1;i++)
		{
			dis+=xMinArr1.GetAt(i+1)-xMaxArr1.GetAt(i);
		}
		dis=dis/(xMinArr1.GetCount()-1);
		for(int i=1;i<xMinArr1.GetCount()-1;i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}
	}
	else if(GetItemCheck(IDC_RADIO1))
	{
		double dis=ptMax.x-ptMin.x;
		for(int i=1;i<xMinArr1.GetCount();i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}
	}
	else if(GetItemCheck(IDC_RADIO2))
	{
		double dMin= xMinArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(0)));
		double dMax=xMaxArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(xIdArr1.GetCount()-1)));
		for(int i=0;i<xMinArr1.GetCount();i++)
		{
			double dx=0.0;
			if(i>=1&&i<xMinArr1.GetCount()-1)
			{

			}
			else if(i==0)
			{
				dx=ptMin.x-dMin;
				xMinArr1.SetAt(i,xMinArr1.GetAt(i)+dx);
				xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);
			}
			else
			{
				dx=ptMax.x-dMax;
				xMinArr1.SetAt(i,xMinArr1.GetAt(i)+dx);
				xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);
			}
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			spEnt->transformBy(mat);
		}
		double dis=0.0;
		for(int i=0;i<xIdArr.GetCount()-1;i++)
		{
			dis+=xMinArr1.GetAt(i+1)-xMaxArr1.GetAt(i);
		}
		dis=dis/(xMinArr1.GetCount()-1);
		for(int i=1;i<xMinArr1.GetCount()-1;i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
				continue;
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(dx,0.0,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),dx,0.0);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}

	}
}
void GEntAverPosDialog::ApplyX()
{
	//先计算范围
	CArray<double> xMinArr;
	CArray<double> xMaxArr;
	CArray<double> yMinArr;
	CArray<double> yMaxArr;
	CArray<AcDbObjectId> xIdArr;
	for(int i=ids.logicalLength()-1;i>=0;i--)
	{
		AcDbExtents ext;
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->getGeomExtents(ext);
		xIdArr.Add(id);
		xMinArr.Add(ext.minPoint().x);
		xMaxArr.Add(ext.maxPoint().x);
		yMinArr.Add(ext.minPoint().y);
		yMaxArr.Add(ext.maxPoint().y);
	}
	if(xIdArr.GetCount()<3)
	{
		return;
	}
	//移动X方向
	if(GetItemCheck(IDC_CHECK2)||
		GetItemCheck(IDC_CHECK3)||
		GetItemCheck(IDC_CHECK5))
	{
		ApplyXSimple(xMinArr,xMaxArr,xIdArr);

	}
	else if(GetItemCheck(IDC_CHECK4))
	{
		ApplyXNotSimple(xMinArr,xMaxArr,xIdArr);

	}
}
void GEntAverPosDialog::ApplyYSimple(CArray<double>& yMinArr,
									 CArray<double>& yMaxArr,
									 CArray<AcDbObjectId>& xIdArr)
{
	//相对比较简单
	CArray<AcDbObjectId> xIdArr1;
	CArray<double>       xArr;
	if(GetItemCheck(IDC_CHECK6))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add(yMinArr.GetAt(i));
		}
	}
	else if(GetItemCheck(IDC_CHECK7))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add((yMinArr.GetAt(i)+yMaxArr.GetAt(i))*0.5);
		}
	}
	else if(GetItemCheck(IDC_CHECK9))
	{
		for(int i=0;i<xIdArr.GetCount();i++)
		{
			xIdArr1.Add(xIdArr.GetAt(i));
			xArr.Add(yMaxArr.GetAt(i));
		}
	}
	SortArray(xIdArr1,xArr,false);
	//低于3个无法排序
		if(GetItemCheck(IDC_RADIO4))
		{
			double dis=(xArr.GetAt(xArr.GetCount()-1)-xArr.GetAt(0))/(xArr.GetCount()-1);
			for(int i=1;i<xArr.GetCount()-1;i++)
			{
				double dx=xArr.GetAt(0)+i*dis;
				dx-=xArr.GetAt(i);
				if(fabs(dx)<1E-8)
					continue;
				AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
				if(spEnt.openStatus()!=Acad::eOk)
				{
					continue;
				}
				AcGeMatrix3d mat;
				mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
				AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
				spEnt->transformBy(mat);
			}
		}
		else if(GetItemCheck(IDC_RADIO1))
		{
			//指定行列间距
			for(int i=1;i<xArr.GetCount();i++)
			{
				double dx=xArr.GetAt(i-1)+ptMax.y-ptMin.y;
				dx-=xArr.GetAt(i);
				if(fabs(dx)<1E-8)
					continue;
				AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
				if(spEnt.openStatus()!=Acad::eOk)
				{
					continue;
				}
				AcGeMatrix3d mat;
				mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
				AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
				spEnt->transformBy(mat);
				xArr.SetAt(i,xArr.GetAt(i-1)+ptMax.y-ptMin.y);
			}

		}
		else if(GetItemCheck(IDC_RADIO2))
		{
			//指定范围
			double dMin= yMinArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(0)));
			double dMax=yMaxArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(xIdArr1.GetCount()-1)));
			double dEnd=xArr.GetAt(xArr.GetCount()-1)+ptMax.y-dMax;
			double dStart=xArr.GetAt(0)+ptMin.y-dMin;
			double dis=(dEnd-dStart)/(xArr.GetCount()-1);
			for(int i=0;i<xArr.GetCount();i++)
			{
				double dx=0.0;
				if(i>=1&&i<xArr.GetCount()-1)
				{
					dx=xArr.GetAt(0)+i*dis;
					dx-=xArr.GetAt(i);
				}
				else if(i==0)
				{
					dx=ptMin.y-dMin;
					xArr.SetAt(i,xArr.GetAt(i)+dx);
				}
				else
				{
					dx=ptMax.y-dMax;
					xArr.SetAt(i,xArr.GetAt(i)+dx);
				}
				if(fabs(dx)<1E-8)
					continue;
				AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
				if(spEnt.openStatus()!=Acad::eOk)
				{
					continue;
				}
				AcGeMatrix3d mat;
				mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
				AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
				spEnt->transformBy(mat);
			}

		}
}
void GEntAverPosDialog::ApplyYNotSimple(CArray<double>& yMinArr,CArray<double>& yMaxArr,CArray<AcDbObjectId>& xIdArr)
{
	CArray<AcDbObjectId> xIdArr1;
	CArray<double>       xMinArr1;
	CArray<double>       xMaxArr1;
	for(int i=0;i<xIdArr.GetCount();i++)
	{
		xIdArr1.Add(xIdArr.GetAt(i));
		xMinArr1.Add(yMinArr.GetAt(i));
		xMaxArr1.Add(yMaxArr.GetAt(i));
	}
	MySortArray(xMinArr1,xMaxArr1,xIdArr1);
	if(GetItemCheck(IDC_RADIO4))
	{
		double dis=0.0;
		for(int i=0;i<xIdArr.GetCount()-1;i++)
		{
			dis+=xMinArr1.GetAt(i+1)-xMaxArr1.GetAt(i);
		}
		dis=dis/(xMinArr1.GetCount()-1);
		for(int i=1;i<xMinArr1.GetCount()-1;i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
				continue;
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}
	}
	else if(GetItemCheck(IDC_RADIO1))
	{
		double dis=ptMax.y-ptMin.y;
		for(int i=1;i<xMinArr1.GetCount();i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}
	}
	else if(GetItemCheck(IDC_RADIO2))
	{
		double dMin= yMinArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(0)));
		double dMax=yMaxArr.GetAt(FindObjectId(xIdArr,xIdArr1.GetAt(xIdArr1.GetCount()-1)));
		for(int i=0;i<xMinArr1.GetCount();i++)
		{
			double dx=0.0;
			if(i>=1&&i<xMinArr1.GetCount()-1)
			{

			}
			else if(i==0)
			{
				dx=ptMin.y-dMin;
				xMinArr1.SetAt(i,xMinArr1.GetAt(i)+dx);
				xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);
			}
			else
			{
				dx=ptMax.y-dMax;
				xMinArr1.SetAt(i,xMinArr1.GetAt(i)+dx);
				xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);
			}
			if(fabs(dx)<1E-8)
			{
				continue;
			}
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
			AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
			spEnt->transformBy(mat);
		}
		double dis=0.0;
		for(int i=0;i<xIdArr.GetCount()-1;i++)
		{
			dis+=xMinArr1.GetAt(i+1)-xMaxArr1.GetAt(i);
		}
		dis=dis/(xMinArr1.GetCount()-1);
		for(int i=1;i<xMinArr1.GetCount()-1;i++)
		{
			double dx=xMaxArr1.GetAt(i-1)+dis;
			dx-=xMinArr1.GetAt(i);
			if(fabs(dx)<1E-8)
				continue;
			AcDbEntityPointer spEnt(xIdArr1.GetAt(i),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcGeMatrix3d mat;
			mat.setToTranslation(AcGeVector3d(0.0,dx,0.0));
			spEnt->transformBy(mat);
			AddMoveRec(xIdArr1.GetAt(i),0.0,dx);
			xMaxArr1.SetAt(i,xMaxArr1.GetAt(i)+dx);

		}

	}
}
void GEntAverPosDialog::ApplyY()
{
	if((!GetItemCheck(IDC_CHECK6))&&
		(!GetItemCheck(IDC_CHECK7))&&
		(!GetItemCheck(IDC_CHECK8))&&
		(!GetItemCheck(IDC_CHECK9)))
	{
		return;
	}
	CArray<double> xMinArr;
	CArray<double> xMaxArr;
	CArray<double> yMinArr;
	CArray<double> yMaxArr;
	CArray<AcDbObjectId> xIdArr;
	for(int i=ids.logicalLength()-1;i>=0;i--)
	{
		AcDbExtents ext;
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt->getGeomExtents(ext);
		xIdArr.Add(id);
		xMinArr.Add(ext.minPoint().x);
		xMaxArr.Add(ext.maxPoint().x);
		yMinArr.Add(ext.minPoint().y);
		yMaxArr.Add(ext.maxPoint().y);
	}
	if(GetItemCheck(IDC_CHECK6)||
		GetItemCheck(IDC_CHECK7)||
		GetItemCheck(IDC_CHECK9))
	{
		ApplyYSimple(yMinArr,yMaxArr,xIdArr);
	}
	else if(GetItemCheck(IDC_CHECK8))
	{
		ApplyYNotSimple(yMinArr,yMaxArr,xIdArr);
	}
}
void GEntAverPosDialog::Apply()
{
	MoveBack();
	//先计算范围
	GLockDocument _lock;
	ApplyX();
	ApplyY();
	//刷新
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

void GEntAverPosDialog::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK6))
	{
		SetItemCheck(IDC_CHECK7,FALSE);
		SetItemCheck(IDC_CHECK8,FALSE);
		SetItemCheck(IDC_CHECK9,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK7))
	{
		SetItemCheck(IDC_CHECK6,FALSE);
		SetItemCheck(IDC_CHECK8,FALSE);
		SetItemCheck(IDC_CHECK9,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK8))
	{
		SetItemCheck(IDC_CHECK6,FALSE);
		SetItemCheck(IDC_CHECK7,FALSE);
		SetItemCheck(IDC_CHECK9,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetItemCheck(IDC_CHECK9))
	{
		SetItemCheck(IDC_CHECK6,FALSE);
		SetItemCheck(IDC_CHECK7,FALSE);
		SetItemCheck(IDC_CHECK8,FALSE);
	}
	Apply();
}

void GEntAverPosDialog::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	Apply();
}

void GEntAverPosDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//指定分布区域
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO2,TRUE);
	SetItemCheck(IDC_RADIO4,FALSE);
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	if(!WindowSelect(ptMin,ptMax))
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	Apply();
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);
}

void GEntAverPosDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//指定间距
	SetItemCheck(IDC_RADIO1,TRUE);
	SetItemCheck(IDC_RADIO2,FALSE);
	SetItemCheck(IDC_RADIO4,FALSE);
	ShowWindow(SW_HIDE);
	BeginEditorCommand();
	if(!WindowSelect(ptMin,ptMax))
	{
		SetItemCheck(IDC_RADIO1,FALSE);
		SetItemCheck(IDC_RADIO2,FALSE);
		SetItemCheck(IDC_RADIO4,TRUE);
		CompleteEditorCommand();
		ShowWindow(SW_NORMAL);
		return;
	}
	Apply();
	CompleteEditorCommand();
	ShowWindow(SW_NORMAL);
}

void GEntAverPosDialog::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton4();
}

void GEntAverPosDialog::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton1();
}
//记录偏移
void GEntAverPosDialog::AddMoveRec(AcDbObjectId id,double dx,double dy)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(id==ids.at(i))
		{
			AcGePoint3d pt=pts.at(i);
			pt.x+=dx;
			pt.y+=dy;
			pts.setAt(i,pt);
		}
	}
}

void GEntAverPosDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ids.setLogicalLength(0);
	pts.setLogicalLength(0);
	OnOK();
}

void GEntAverPosDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	MoveBack();
	OnCancel();
}

void GEntAverPosDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MoveBack();
	CAcUiDialog::OnClose();
	OnCancel();
}
