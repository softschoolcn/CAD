#pragma once
#include "CmpExcute.h"
#include "afxmt.h"
#include "GViewTableRecord.h"
class CmpReportHelper
{
public:
	CmpReportHelper();
	~CmpReportHelper();
	static CmpReportHelper instance;
	void OpenCmpDoc(CmpResult* pRc);//打开对比文档
	void OnTimer(HWND hWnd,UINT_PTR nIDEvent);
	void ShowCmpResult(int nMode,bool bReg);
	void Stop();
	bool m_bInChangeDwg;//是否在切换比较图纸
protected:
	GViewTableRecordData m_selfSetView1;
	GViewTableRecordData m_selfSetView2;
	int        m_nMode;
	CMutex m_mutex1;
	
	bool m_bInChangeEnt;//是否正在切换实体
	CArray<GDocViewTableRecordData> m_viewDatas;
	GDocViewTableRecordData m_lastSameData;//最后一次同步视图的数据
	bool       m_bReg;
	void ShowCmpResult();
	void OnTimer_StartCmpFile(HWND hWnd,UINT_PTR nIDEvent);
};
#ifdef _WIN64
void CALLBACK TimerProc_StartPlot(HWND hWnd, UINT nMsg, UINT_PTR nTimerId, DWORD dwTime);
#else
void CALLBACK TimerProc_StartPlot(HWND hWnd, UINT nMsg, UINT nTimerId, DWORD dwTime);
#endif