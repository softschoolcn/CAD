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
//----- GMousePreviewDocReactor.cpp : Implementation of GMousePreviewDocReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GMousePreviewDocReactor.h"
extern CStringArray Glb_MousePreviewKeyArr;
extern CStringArray Glb_MousePreviewValueArr;
extern CArray<bool> Glb_MousePreviewOpenArr;
extern bool m_bXDataDynViewOpen;
GMousePreviewDocReactor* Glb_MousePreviewDocReactor = NULL;
//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(GMousePreviewDocReactor, AcApDocManagerReactor, 1)

//-----------------------------------------------------------------------------
GMousePreviewDocReactor::GMousePreviewDocReactor (const bool autoInitAndRelease) : AcApDocManagerReactor(), mbAutoInitAndRelease(autoInitAndRelease) 
{
	#if _MSC_VER>=1500
	pTempSysInt=new GTempIntVarHelper(_T("ROLLOVERTIPS"));
#endif
	if ( autoInitAndRelease ) 
	{
		if ( acDocManager )
			acDocManager->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
GMousePreviewDocReactor::~GMousePreviewDocReactor () 
{
#if _MSC_VER>=1500
	if(pTempSysInt!=NULL)
	{
		delete pTempSysInt;
		pTempSysInt=NULL;
	}
#endif
	Detach () ;
}

//-----------------------------------------------------------------------------
void GMousePreviewDocReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void GMousePreviewDocReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcApDocManager *GMousePreviewDocReactor::Subject () const {
	return (acDocManager) ;
}

bool GMousePreviewDocReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}
void    GMousePreviewDocReactor::documentBecameCurrent(AcApDocument* doc)
{
	SetXDataDynViewState(doc,m_bXDataDynViewOpen?1:0);
}
void	GMousePreviewDocReactor::documentToBeDestroyed(AcApDocument* doc)
{
	for(int i=pDocArr.GetCount()-1;i>=0;i--)
	{
		if(pDocArr.GetAt(i)==doc)
		{
			AsdkDynViewInputPoint* pRec=pAsdkDynViewInputPointArr.GetAt(i);
			if(pRec!=NULL)
			{
				delete pRec;
				pRec=NULL;
			}
			pDocArr.RemoveAt(i);
			pAsdkDynViewInputPointArr.RemoveAt(i);
			m_bXDataDynViewOpenArr.RemoveAt(i);
		}
	}
}
//code=0,不显示
//code=1,显示
//code=2,反向
void GMousePreviewDocReactor::SetXDataDynViewState(AcApDocument* doc,int code)
{
	if(doc==NULL)
		return;
	int nIndex=-1;
	for(int i=0;i<pDocArr.GetCount();i++)
	{
		if(pDocArr.GetAt(i)==doc)
		{
			nIndex=i;
		}
	}
	bool m_bXDataDynViewOpen=false;
	if(nIndex>=0)
	{
		m_bXDataDynViewOpen=m_bXDataDynViewOpenArr.GetAt(nIndex);
	}
	AsdkDynViewInputPoint* pAsdkDynViewInputPoint=NULL;
	if(nIndex>=0)
	{
		pAsdkDynViewInputPoint=pAsdkDynViewInputPointArr.GetAt(nIndex);
	}
	if(code==1)
	{
#if _MSC_VER>=1500
		if(pTempSysInt==NULL)
		{
			pTempSysInt=new GTempIntVarHelper(_T("ROLLOVERTIPS"));
		}
		pTempSysInt->SetValue(0);
#endif

		if(m_bXDataDynViewOpen)
			return;
		//开始动态显示
		acutPrintf(_T("\n属性动态显示"));
		m_bXDataDynViewOpen=true;
		if(nIndex>=0)
		{
			m_bXDataDynViewOpenArr.SetAt(nIndex,m_bXDataDynViewOpen);
		}
		else
		{
			m_bXDataDynViewOpenArr.Add(m_bXDataDynViewOpen);
			pDocArr.Add(doc);
		}
		if (NULL==pAsdkDynViewInputPoint)
		{
			pAsdkDynViewInputPoint =new AsdkDynViewInputPoint;
			if(doc->inputPointManager()!=NULL)
			{
				doc->inputPointManager()->addPointMonitor(pAsdkDynViewInputPoint);
			}
			if(nIndex>=0)
			{
				pAsdkDynViewInputPointArr.SetAt(nIndex,pAsdkDynViewInputPoint);
			}
			else
			{
				pAsdkDynViewInputPointArr.Add(pAsdkDynViewInputPoint);
			}
		}
	}
	else if(code==0)
	{
#if _MSC_VER>=1500
		if(pTempSysInt!=NULL)
		{
			delete pTempSysInt;
			pTempSysInt=NULL;
		}
#endif
		if(!m_bXDataDynViewOpen)
			return;
		//关闭动态显示
		acutPrintf(_T("\n属性动态显示关闭"));
		m_bXDataDynViewOpen=false;
		if(nIndex>=0)
		{
			m_bXDataDynViewOpenArr.SetAt(nIndex,m_bXDataDynViewOpen);
		}
		else
		{
			m_bXDataDynViewOpenArr.Add(m_bXDataDynViewOpen);
			pDocArr.Add(doc);
		}
		if (NULL!=pAsdkDynViewInputPoint)
		{
			AcApDocument* pDoc = curDoc();
			if(pDoc!=NULL)
			{
				if(pDoc->inputPointManager()!=NULL)
				{

					pDoc->inputPointManager()->removePointMonitor(pAsdkDynViewInputPoint);
				}
			}
			delete pAsdkDynViewInputPoint;
			pAsdkDynViewInputPoint=NULL;

		}
		if(nIndex>=0)
		{
			pAsdkDynViewInputPointArr.SetAt(nIndex,NULL);
		}
		else
		{
			pAsdkDynViewInputPointArr.Add(NULL);
		}
	}
	else
	{
		if(m_bXDataDynViewOpen)
		{
			SetXDataDynViewState(doc,0);
		}
		else
		{
			SetXDataDynViewState(doc,1);
		}
	}
}
