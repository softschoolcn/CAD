#include "StdAfx.h"
#include "TextCrossReportFunction.h"
#include "TextCrossReportPanel.h"
extern CTextCrossReportPanel* m_pTextCrossReportPanel;
void ShowTextCrossReportPanel(BOOL bShow)
{
	if(m_pTextCrossReportPanel==NULL&&(!bShow))
		return;
	if(m_pTextCrossReportPanel==NULL)
	{
		//第一次调用命令时,转换资源
		CAcModuleResourceOverride _UseThisRes;

		//####修改：由于图形入库检测功能与避让分析功能共用一个错误面板，当他们功能切换的时候面板显示也进行切换
		m_pTextCrossReportPanel = new CTextCrossReportPanel();
		CWnd *pWnd = acedGetAcadFrame();
		m_pTextCrossReportPanel->Create(pWnd, (LPCSTR)_T("文字重叠检测结果"));
	}
	if(m_pTextCrossReportPanel==NULL)
	{
		return;
	}
	m_pTextCrossReportPanel->ShowDockPane(bShow);

}
void ShowTextCrossResult(CArray<TextCrossRecord>& result)
{
	if(m_pTextCrossReportPanel==NULL)
	{
		return;
	}
	m_pTextCrossReportPanel->ShowTextCrossResult(result);
}
