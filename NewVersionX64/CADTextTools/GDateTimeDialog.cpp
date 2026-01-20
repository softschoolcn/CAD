// GDateTimeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GDateTimeDialog.h"


// GDateTimeDialog 对话框

IMPLEMENT_DYNAMIC(GDateTimeDialog, CDialog)

GDateTimeDialog::GDateTimeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GDateTimeDialog::IDD, pParent)
{

}

GDateTimeDialog::~GDateTimeDialog()
{
}

void GDateTimeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GDateTimeDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK3, &GDateTimeDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK7, &GDateTimeDialog::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_RADIO1, &GDateTimeDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO5, &GDateTimeDialog::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO7, &GDateTimeDialog::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &GDateTimeDialog::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO3, &GDateTimeDialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &GDateTimeDialog::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO9, &GDateTimeDialog::OnBnClickedRadio9)
	ON_BN_CLICKED(IDOK, &GDateTimeDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// GDateTimeDialog 消息处理程序
void GDateTimeDialog::UpdateDateTime(BOOL updateTime)
{
	CTime tm=updateTime?CTime::GetCurrentTime():m_time;
	bool bYear=GetItemCheck(IDC_CHECK3);//是否显示为年月日
	bool bZero=false;
	if(bYear)
	{
		SetItemEnable(IDC_CHECK7,FALSE);//显示年月日的时候不应该缺位补零
	}
	else
	{
		SetItemEnable(IDC_CHECK7,TRUE);
		bZero=GetItemCheck(IDC_CHECK7);//是否缺位补零
	}
	CString strYear,strMonth,strDay,strHour,strMinute,strSecond;
	strYear.Format(_T("%4d"),tm.GetYear());
	strMonth.Format(bZero?_T("%.2d"):_T("%d"),tm.GetMonth());
	strDay.Format(bZero?_T("%.2d"):_T("%d"),tm.GetDay());
	strHour.Format(bZero?_T("%.2d"):_T("%d"),tm.GetHour());
	strMinute.Format(bZero?_T("%.2d"):_T("%d"),tm.GetMinute());
	strSecond.Format(bZero?_T("%.2d"):_T("%d"),tm.GetSecond());
	if(bYear)
	{
		//2013.9.26
		//IDC_RADIO5
		SetDlgItemText(IDC_RADIO5,strYear+_T("年")+strMonth+_T("月")+strDay+_T("日"));
		//13.9.26
		//IDC_RADIO7
		SetDlgItemText(IDC_RADIO7,strYear.Right(2)+_T("年")+strMonth+_T("月")+strDay+_T("日"));
		//9.26
		//IDC_RADIO8
		SetDlgItemText(IDC_RADIO8,strMonth+_T("月")+strDay+_T("日"));
		//9:37:21
		//IDC_RADIO6
		SetDlgItemText(IDC_RADIO6,strHour+_T("时")+strMinute+_T("分")+strSecond+_T("秒"));
		//9:37
		//IDC_RADIO9
		SetDlgItemText(IDC_RADIO9,strHour+_T("时")+strMinute+_T("分"));
	}
	else
	{
		//2013.9.26
		//IDC_RADIO5
		SetDlgItemText(IDC_RADIO5,strYear+_T(".")+strMonth+_T(".")+strDay);
		//13.9.26
		//IDC_RADIO7
		SetDlgItemText(IDC_RADIO7,strYear.Right(2)+_T(".")+strMonth+_T(".")+strDay);
		//9.26
		//IDC_RADIO8
		SetDlgItemText(IDC_RADIO8,strMonth+_T(".")+strDay);
		//9:37:21
		//IDC_RADIO6
		SetDlgItemText(IDC_RADIO6,strHour+_T(":")+strMinute+_T(":")+strSecond);
		//9:37
		//IDC_RADIO9
		SetDlgItemText(IDC_RADIO9,strHour+_T(":")+strMinute);
	}
	CString strDate=_T("");
	if(GetItemCheck(IDC_RADIO5))
	{
		GetDlgItemText(IDC_RADIO5,strDate);
	}
	else if(GetItemCheck(IDC_RADIO7))
	{
		GetDlgItemText(IDC_RADIO7,strDate);
	}
	else if(GetItemCheck(IDC_RADIO8))
	{
		GetDlgItemText(IDC_RADIO8,strDate);
	}
	CString strTime=_T("");
	if(GetItemCheck(IDC_RADIO6))
	{
		GetDlgItemText(IDC_RADIO6,strTime);
	}
	else if(GetItemCheck(IDC_RADIO9))
	{
		GetDlgItemText(IDC_RADIO9,strTime);
	}
	if(strDate.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT1,strTime);
	}
	else
	{
		if(strTime.IsEmpty())
		{
			SetDlgItemText(IDC_EDIT1,strDate);
		}
		else
		{
			if(bYear)
			{
				SetDlgItemText(IDC_EDIT1,strDate+strTime);
			}
			else
			{
				SetDlgItemText(IDC_EDIT1,strDate+_T(" ")+strTime);
			}
			
		}
	}

}
BOOL GDateTimeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_time=CTime::GetCurrentTime();
	bool bYear=false;
	CString strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GDateTimeDialog"),_T("Year"),12,_T("False"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		bYear=true;
	}
	if(bYear)
	{
		SetItemEnable(IDC_CHECK7,FALSE);//显示年月日的时候不应该缺位补零
		SetItemCheck(IDC_CHECK3,TRUE);
	}
	strTmp=GUserDataRecord::getInstance()->GetProfileString(_T("GDateTimeDialog"),_T("Zero"),12,_T("False"));
	if(strTmp.CompareNoCase(_T("True"))==0)
	{
		SetItemCheck(IDC_CHECK7);
	}
	int nTmp=GUserDataRecord::getInstance()->GetProfileInt(_T("GDateTimeDialog"),_T("DayFormat"),2);
	if(nTmp<1||nTmp>5)
	{
		nTmp=2;
	}
	if(1==nTmp)
	{
		SetItemCheck(IDC_RADIO1);
	}
	else if(2==nTmp)
	{
		SetItemCheck(IDC_RADIO5);
	}
	else if(3==nTmp)
	{
		SetItemCheck(IDC_RADIO7);
	}
	else if(4==nTmp)
	{
		SetItemCheck(IDC_RADIO8);
	}

	nTmp=GUserDataRecord::getInstance()->GetProfileInt(_T("GDateTimeDialog"),_T("TimeFormat"),1);
	if(nTmp<1||nTmp>4)
	{
		nTmp=1;
	}
	if(1==nTmp)
	{
		SetItemCheck(IDC_RADIO3);
	}
	else if(2==nTmp)
	{
		SetItemCheck(IDC_RADIO6);
	}
	else if(3==nTmp)
	{
		SetItemCheck(IDC_RADIO9);
	}

	UpdateDateTime();

	GlobalSystemValue::instance.ResetWindowText(GetSafeHwnd());
	return TRUE;
}
void GDateTimeDialog::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,TRUE);
	SetItemCheck(IDC_RADIO5,FALSE);
	SetItemCheck(IDC_RADIO7,FALSE);
	SetItemCheck(IDC_RADIO8,FALSE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO5,TRUE);
	SetItemCheck(IDC_RADIO7,FALSE);
	SetItemCheck(IDC_RADIO8,FALSE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO5,FALSE);
	SetItemCheck(IDC_RADIO7,TRUE);
	SetItemCheck(IDC_RADIO8,FALSE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO1,FALSE);
	SetItemCheck(IDC_RADIO5,FALSE);
	SetItemCheck(IDC_RADIO7,FALSE);
	SetItemCheck(IDC_RADIO8,TRUE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO3,TRUE);
	SetItemCheck(IDC_RADIO6,FALSE);
	SetItemCheck(IDC_RADIO9,FALSE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO3,FALSE);
	SetItemCheck(IDC_RADIO6,TRUE);
	SetItemCheck(IDC_RADIO9,FALSE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedRadio9()
{
	// TODO: 在此添加控件通知处理程序代码
	SetItemCheck(IDC_RADIO3,FALSE);
	SetItemCheck(IDC_RADIO6,FALSE);
	SetItemCheck(IDC_RADIO9,TRUE);
	UpdateDateTime();
}

void GDateTimeDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1,m_str);
	if(m_str.IsEmpty())
	{
		AfxMessageBox(_T("至少需要显示日期或时间中的一个"));
		return;
	}

	GUserDataRecord::getInstance()->WriteProfileString(_T("GDateTimeDialog"),_T("Year"),GetItemCheck(IDC_CHECK3)?_T("True"):_T("False"));
	GUserDataRecord::getInstance()->WriteProfileString(_T("GDateTimeDialog"),_T("Zero"),GetItemCheck(IDC_CHECK7)?_T("True"):_T("False"));
	int nTmp=0;
	if(GetItemCheck(IDC_RADIO1))
	{
		nTmp=1;
	}
	else if(GetItemCheck(IDC_RADIO5))
	{
		nTmp=2;
	}
	else if(SetItemCheck(IDC_RADIO7))
	{
		nTmp=3;
	}
	else if(GetItemCheck(IDC_RADIO8))
	{
		nTmp=4;
	}
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GDateTimeDialog"),_T("DayFormat"),nTmp);

	if(GetItemCheck(IDC_RADIO3))
	{
		nTmp=1;
	}
	else if(GetItemCheck(IDC_RADIO6))
	{
		nTmp=2;
	}
	else if(GetItemCheck(IDC_RADIO9))
	{
		nTmp=3;
	}
	GUserDataRecord::getInstance()->WriteProfileInt(_T("GDateTimeDialog"),_T("TimeFormat"),nTmp);

	OnOK();
}
