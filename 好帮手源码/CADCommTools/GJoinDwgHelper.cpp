#include "StdAfx.h"
#include "GJoinDwgHelper.h"

GJoinDwgHelper::GJoinDwgHelper(void):m_dMaxY(0.0)
{
}

GJoinDwgHelper::~GJoinDwgHelper(void)
{
}
Acad::ErrorStatus GJoinDwgHelper::Join()
{
	struct resbuf *result = acutNewRb(RTSTR);
	//2^5 Inhibits display of the alert box, which warns that a file exists when a new file of the same name is opened
	if (acedGetFileNavDialog(_T("保存合并图纸"),NULL, _T("dwg"),NULL, 1||32, &result)!=RTNORM)
		return Acad::eInvalidInput;
	CString m_shrPath =result->resval.rstring;
	acutRelRb(result);
	m_insertPt.x=0.0;
	m_insertPt.y=0.0;
	m_insertPt.z=0.0;
	AcDbDatabase* pNewDb=new AcDbDatabase();
	if(m_param.m_nPL==2)
	{
		int nCount=0;
		int nGroup=0;
		while(nCount<m_param.m_fileArray.GetCount())
		{
			m_dMaxY=0.0;
			for(int i=0;i<m_param.m_nNum;i++)
			{
				int nIndex=nCount+i;
				if(nIndex>=m_param.m_fileArray.GetCount())
				{
					break;
				}
				Join(pNewDb,m_param.m_fileArray.GetAt(nIndex));
			}
			nCount+=m_param.m_nNum;
			m_insertPt.x=0.0;
			m_insertPt.y+=m_param.m_dDisVer+m_dMaxY;
		}
	}
	else
	{
		for(int i=0;i<m_param.m_fileArray.GetCount();i++)
		{
			Join(pNewDb,m_param.m_fileArray.GetAt(i));
		}
	}
	
	const SecurityParams* pSecParams=NULL;
	Acad::ErrorStatus es=pNewDb->saveAs(m_shrPath,pSecParams);
	delete pNewDb;
	pNewDb=NULL;
	if(es!=eOk)
	{
		acutPrintf(_T("\n保存DWG文件:%s，发生错误:%s\n"),m_shrPath,acadErrorStatusText(es));
	}
	else
	{
		acutPrintf(_T("合并成功"));
	}
	return es;
}
Acad::ErrorStatus GJoinDwgHelper::Join(AcDbDatabase* pTargetDb,const CString strFile)
{
	AcDbDatabase* pDbTmp=new AcDbDatabase(false);
	Acad::ErrorStatus es=Acad::eOk;
	try
	{
		es=pDbTmp->readDwgFile(strFile);
		if(es!=eOk)
		{
			acutPrintf(_T("\n读取DWG文件:%s，发生错误:%s\n"),strFile,acadErrorStatusText(es));
		}
		else
		{
			es=Join(pTargetDb,pDbTmp);
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
	return es;
}
Acad::ErrorStatus GJoinDwgHelper::Join(AcDbDatabase* pTargetDb,AcDbDatabase* pDb)
{
	AcDbObjectIdArray ids;
	EnumAllEntityInModelSpace(ids,pDb);
	AcGePoint3d ptMin;
	AcGePoint3d ptMax;
	if(!GetMaxExtPoint(ids,ptMin,ptMax))
	{
		return Acad::eInvalidExtents;
	}
	AcGeMatrix3d mat;
	mat.setToTranslation(m_insertPt-ptMin);
	Acad::ErrorStatus es=pTargetDb->insert(mat,pDb);
	if(es==Acad::eOk)
	{
		if(m_param.m_nPL==0)
		{
			m_insertPt.x+=ptMax.x-ptMin.x+m_param.m_dDis;
		}
		else if(m_param.m_nPL==2)
		{
			m_insertPt.x+=ptMax.x-ptMin.x+m_param.m_dDisHor;
			m_dMaxY=max(m_dMaxY,ptMax.y-ptMin.y);
		}
		else
		{
			m_insertPt.y+=ptMax.y-ptMin.y+m_param.m_dDis;
		}
		
	}
	return es;
}
