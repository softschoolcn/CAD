#pragma once
struct GViewTableRecordData
{
	AcGePoint2d m_CenterPt;
	double m_dWidth;
	double m_dHeight;
	bool IsSame(GViewTableRecordData& src)
	{
		if(fabs(m_dWidth-src.m_dWidth)>0.01*fabs(max(m_dWidth,src.m_dWidth)))
		{
			return false;
		}
		if(fabs(m_dHeight-src.m_dHeight)>0.01*fabs(max(m_dHeight,src.m_dHeight)))
		{
			return false;
		}
		double dMax=max(max(m_dWidth,src.m_dWidth),max(m_dHeight,src.m_dHeight));
		if(fabs(m_CenterPt.x-src.m_CenterPt.x)>0.01*fabs(dMax))
		{
			return false;
		}
		if(fabs(m_CenterPt.y-src.m_CenterPt.y)>0.01*fabs(dMax))
		{
			return false;
		}
		return true;
	}
};
struct GDocViewTableRecordData:public GViewTableRecordData
{
	GDocViewTableRecordData()
	{
		m_pDoc=NULL;
	}
	~GDocViewTableRecordData()
	{
	}
	AcApDocument* m_pDoc;
};