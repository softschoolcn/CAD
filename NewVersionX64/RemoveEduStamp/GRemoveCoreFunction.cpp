#include "stdafx.h"
#include "GRemoveCoreFunction.h"
#include "GLockDocument.h"
#if _MSC_VER<1700
Acad::ErrorStatus RemoveEdu(CString strFileName)
{
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
			if(pDbTmp->isEMR())
			{
				RemoveEduPlotStamp(pDbTmp);
				const SecurityParams* pSecParams=NULL;
				es=pDbTmp->saveAs(strFileName,pSecParams);
				if(es!=eOk)
				{
					acutPrintf(_T("\n保存DWG文件:%s，发生错误:%s\n"),strFileName,acadErrorStatusText(es));
				}
			}

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
	return Acad::eOk;
}
#endif