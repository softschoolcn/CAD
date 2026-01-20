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
	//创建
	BOOL Create(CString strHint , long nMaxPos , long nMinPos = 0);
	//设置当前位置
	BOOL setCutPos(CString strHint , long nPos);
	BOOL setCutPos(long nPos);
	//移动
	BOOL stepIt(long nStep=1){return setCutPos(m_nCurPosAbs+nStep);}
	//重置
	void Restore();
	//获取当前位置
	long getCurPos(){return m_nCurPos;}
	BOOL IsCreate(){return m_bCreate;}

private:
	BOOL	m_bCreate;
	long		m_nMinPos;
	long		m_nMaxPos;
	long		m_nCurPos;//当前位置，处于0-1000之间的值
	long        m_nCurPosAbs;//当前位置绝对值
};