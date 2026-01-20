#include "StdAfx.h"
#include "GConfigCommand.h"
#include "GSystemConfigDialog.h"
//115:œµÕ≥≈‰÷√
void SystemConfig()
{
	CAcModuleResourceOverride res;
	GSystemConfigDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();
}
