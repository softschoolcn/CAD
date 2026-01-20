#pragma once
#include <vector>
using namespace std;
//多个实体jig辅助实体
class GMultiJigEntity:public AcDbEntity
{
public:
	GMultiJigEntity(void);
public:
	~GMultiJigEntity(void);
	#if _MSC_VER<1500
    virtual Adesk::Boolean worldDraw(AcGiWorldDraw *mode);
#else
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
#endif
	#if _MSC_VER<1500
	virtual void      viewportDraw(AcGiViewportDraw* mode);
#else
	
	virtual void      subViewportDraw(AcGiViewportDraw* mode);
#endif
	/*!
     @brief 添加用于jig时显示的实体
     */
    void Add(AcDbEntity* pEnt);
	/*!
     @brief 释放new处理的实体
     */
    void Release();
	/*!
     @brief 更新实体
     */
    void Update(AcGePoint3d& pnt);
	/*!
     @brief 添加到数据库
     */
    void AddToDb(AcDbObjectIdArray& ids);
private:
	vector<AcDbEntity*> mEntityArr;  //用于附加显示的实体
    vector<AcGeMatrix3d> mEntityTransArr;  //用于附加显示的块参照的位置
};
