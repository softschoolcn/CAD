#pragma once
//系统全局变量
class GLOBALTOOLS_EXT GlobalSystemValue
{
private:
	GlobalSystemValue(void);
public:
	~GlobalSystemValue(void);
public:
	//获取软件名称
	CString getSoftName(){Init();return m_strSoftName;}
	CString getWebSite(){Init();return m_strWebSite;}
	CString getRegWebSite(){Init();return m_strRegWebSite;}
	//是否是好帮手
	bool    IsHaoBangshou(){Init();return m_strSysName.CompareNoCase(_T("HaoBangShou"))==0;}
	//是否是小助手
	bool    IsXiaoZhuShou(){Init();return m_strSysName.CompareNoCase(_T("XiaoZhuShou"))==0;}
	//是否是中线CAD
	bool    IsZhongXianCAD(){Init();return m_strSysName.CompareNoCase(_T("ZhongXianCAD"))==0;}
	void    ResetWindowText(HWND hWnd);
private:
	CString m_strSoftName;//软件名称
	CString m_strWebSite;//软件官方网站
	CString m_strRegWebSite;//软件注册网站
	CString m_strSysName;//系统名称
	void Init();//初始化
	bool m_bInit;//是否已经初始化了
public:
	static GlobalSystemValue instance;
};
