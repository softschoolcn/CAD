#pragma once
class GZYJXEnt:public AcDbEntity
{
public:
	AcGePoint3d m_pt1;
	AcGePoint3d m_pt2;
	AcGePoint3d m_pt3;
	AcGePoint3d m_pt5;
	AcGePoint3d m_pt6;
	AcDbObjectId Append();
	void Update(AcGePoint3d newPt);
	#if _MSC_VER<1500
	virtual Adesk::Boolean      worldDraw(AcGiWorldDraw*        mode);
#else
	virtual Adesk::Boolean      subWorldDraw(AcGiWorldDraw*        mode);
#endif
	#if _MSC_VER<1500
	virtual void      viewportDraw(AcGiViewportDraw* mode);
#else
	
	virtual void      subViewportDraw(AcGiViewportDraw* mode);
#endif
};
class GZYJXJig:public AcEdJig
{
public:
	GZYJXJig(void);
public:
	~GZYJXJig(void);
	virtual int Do();
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const{return m_pEnt;}
public:
	GZYJXEnt* m_pEnt;
	AcGePoint3d m_pointTopLast;//插入点对应的上面的点(上一次用户输入)
	BOOL        m_bStop;
};
