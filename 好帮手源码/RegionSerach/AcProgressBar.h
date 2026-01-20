/************************************************************************
*说明：定义和实现一个与AutoCAD状态栏上显示进度栏的类
*
*
*
*

************************************************************************/

#pragma once

class AcProgressBar
{
public:
	AcProgressBar();
	~AcProgressBar();

	BOOL Create(CString strHint , int nMaxPos , int nMinPos = 0);

	BOOL StepIt(CString strHint , int nPos);
	BOOL StepIt(int nPos);

	void Restore();

private:
	BOOL	m_bCreate;
	int		m_nMinPos;
	int		m_nMaxPos;
	int		m_nCurPos;
};