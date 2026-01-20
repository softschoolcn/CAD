#pragma once

class GMultJigEnt:public AcDbEntity
{
public:
	GMultJigEnt(void);
public:
	~GMultJigEnt(void);
public:
	void Release();
	/*!
	@brief 添加到数据库
	*/
	void AddToDb(AcDbObjectIdArray& ids);
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
protected:
	AcGeVoidPointerArray       mEntArr;
private:
	Adesk::Boolean _draw(AcGiWorldDraw*        mode);
};
