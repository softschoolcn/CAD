#include "StdAfx.h"
#include "LineCrossReportPanel.h"
extern CLineCrossReportPanel* m_pLineCrossReportPanel;
void ShowLineCrossReportPanel(BOOL bShow)
{
	if(m_pLineCrossReportPanel==NULL&&(!bShow))
		return;
	if(m_pLineCrossReportPanel==NULL)
	{
		//第一次调用命令时,转换资源
		CAcModuleResourceOverride _UseThisRes;

		//####修改：由于图形入库检测功能与避让分析功能共用一个错误面板，当他们功能切换的时候面板显示也进行切换
		m_pLineCrossReportPanel = new CLineCrossReportPanel();
		CWnd *pWnd = acedGetAcadFrame();
		m_pLineCrossReportPanel->Create(pWnd, (LPCSTR)_T("曲线重叠检测结果"));
	}
	if(m_pLineCrossReportPanel==NULL)
	{
		return;
	}
	m_pLineCrossReportPanel->ShowDockPane(bShow);

}
void ShowLineCrossResult(CArray<LineCrossResultItem>& result)
{
	if(m_pLineCrossReportPanel==NULL)
	{
		return;
	}
	m_pLineCrossReportPanel->ShowLineCrossResult(result);
}
