#include "StdAfx.h"
#include "GCommand.h"
#include "GPurgeDialog.h"
#include "GRemoveCoreFunction.h"
#if _MSC_VER>=1700
void RemoveEduStamp()
{
	acutPrintf(_T("\r\n2015以上版本不需要去除教育版功能"));
}
#else
void RemoveEduStamp()
{
	if (!CheckReg())
	{
		return;
	}
	CAcModuleResourceOverride res;
	GPurgeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	int rc = dlg.DoModal();
	if (rc != IDOK)
	{
		return;
	}
	for (int i = 0; i<dlg.m_strFileArray.GetCount(); i++)
	{
		RemoveEdu(dlg.m_strFileArray.GetAt(i));
	}

}
#endif
