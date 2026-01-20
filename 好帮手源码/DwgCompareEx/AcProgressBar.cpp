#include "StdAfx.h"
#include "AcProgressBar.h"

AcProgressBar::AcProgressBar()
{
	m_bCreate	= FALSE;
	m_nMaxPos	= m_nMinPos	= m_nCurPos=m_nCurPosAbs = 0;
}

AcProgressBar::~AcProgressBar()
{
	this->Restore();
}

BOOL AcProgressBar::Create(CString strHint , long nMaxPos , long nMinPos /* = 0 */)
{
	if ((nMaxPos <= 0) || (nMaxPos <= nMinPos))
		return FALSE;

	if(acedSetStatusBarProgressMeter(strHint , 0 , 1000) != 0)
		return FALSE;

	m_bCreate	= TRUE;
	m_nMaxPos	= nMaxPos;
	m_nMinPos	= nMinPos;
	m_nCurPos =m_nCurPosAbs=nMinPos;

	return TRUE;
}

void AcProgressBar::Restore()
{
	if(!m_bCreate)
		return;

	acedRestoreStatusBar();
	m_bCreate	= FALSE;
	m_nMaxPos	= m_nMinPos	= m_nCurPos =m_nCurPosAbs= 0;
}

BOOL AcProgressBar::setCutPos(long nPos)
{
	if(!m_bCreate)
		return FALSE;
	m_nCurPosAbs=nPos;

	if(nPos > m_nMaxPos)
		nPos	= m_nMaxPos;

	// 换算成0~100之间的值
	nPos = double(nPos)/(m_nMaxPos-m_nMinPos)*1000;

	if (m_nCurPos == nPos)
		return TRUE;
	else
		m_nCurPos = nPos;

	if(acedSetStatusBarProgressMeterPos(nPos) != 0)
		return FALSE;

	return TRUE;
}

BOOL AcProgressBar::setCutPos(CString strHint , long nPos)
{
	if(!m_bCreate)
		return FALSE;

	if(m_bCreate)
	{
		m_bCreate = FALSE;
		acedRestoreStatusBar();
	}

	if(acedSetStatusBarProgressMeter(strHint , m_nMinPos , m_nMaxPos) != 0)
		return FALSE;

	m_bCreate	= TRUE;

	if(nPos > m_nMaxPos)
		nPos	= m_nMaxPos;
	m_nCurPosAbs=nPos;

	// 换算成0~100之间的值
	nPos = double(nPos)/(m_nMaxPos-m_nMinPos)*100;

	if (m_nCurPos == nPos)
		return TRUE;
	else
		m_nCurPos = nPos;

	if(acedSetStatusBarProgressMeterPos(nPos) != 0)
		return FALSE;

	return TRUE;
}