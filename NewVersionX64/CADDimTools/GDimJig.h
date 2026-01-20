#pragma once

class GDimJig:public AcEdJig
{
public:
	GDimJig(void);
public:
	~GDimJig(void);
	virtual int Do();
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const{return m_pDim;}
	virtual void SetDim(AcDbDimension* pDim)
	{
		m_pDim=pDim;
		if(m_bJigDimLinePoint)
		{
			if(m_pDim->isKindOf(AcDbAlignedDimension::desc()))
			{
				AcDbAlignedDimension* pDim=AcDbAlignedDimension::cast(m_pDim);
				m_DimPt=pDim->dimLinePoint();
			}
		}
	}
	void SetEntity(AcDbEntity* pEnt);
	AcDbEntity* m_pEnt;
	AcGePoint3d m_point;             //插入点
	AcGePoint3d m_pointTopLast;//插入点对应的上面的点(上一次用户输入)
	BOOL        m_bStop;
	bool m_bJigDimLinePoint;//是否JIG对齐点
	AcGePoint3d m_DimPt;
	AcGeVector3d m_vec;
	AcGePoint3d m_pt1;//第一个点
	AcGePoint3d m_pt2;//第二个点
	bool        m_bInLine;//要求一定要在第一个点和第二个点的连线上
private:
	AcDbDimension* m_pDim;
};
