#include "StdAfx.h"
#include "GMultiJigEntity.h"

GMultiJigEntity::GMultiJigEntity(void)
{
}

GMultiJigEntity::~GMultiJigEntity(void)
{
}
void GMultiJigEntity::Release()
{
	vector<AcDbEntity*>::iterator iter = mEntityArr.begin();
	for (; iter != mEntityArr.end(); iter++)
	{
		delete (*iter);
	}

	mEntityArr.clear();
	mEntityTransArr.clear();
}
#if _MSC_VER<1500
Adesk::Boolean GMultiJigEntity::worldDraw(AcGiWorldDraw* mode)
#else
Adesk::Boolean GMultiJigEntity::subWorldDraw(AcGiWorldDraw* mode)
#endif
{
	vector<AcDbEntity*>::iterator iter = mEntityArr.begin();
	for (; iter != mEntityArr.end(); iter++)
	{
		if((*iter)->worldDraw(mode)!=Adesk::kTrue)
		{
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
#if _MSC_VER<1500
	void      GMultiJigEntity::viewportDraw(AcGiViewportDraw* mode)
#else
	
	void      GMultiJigEntity::subViewportDraw(AcGiViewportDraw* mode)
#endif
	{
		vector<AcDbEntity*>::iterator iter = mEntityArr.begin();
	for (; iter != mEntityArr.end(); iter++)
	{
		mode->geometry().draw(*iter);
		(*iter)->viewportDraw(mode);
	}
	}
void GMultiJigEntity::Update(AcGePoint3d& pnt)
{
	unsigned int len = mEntityArr.size();
	for (unsigned int i = 0; i < len; i++)
	{
		mEntityArr[i]->transformBy(mEntityTransArr[i].inverse());
		AcGeVector3d vec(pnt.x,pnt.y,pnt.z);
		AcGeMatrix3d mat;
		mat.setToTranslation(vec);
		mEntityTransArr[i]=mat;
		mEntityArr[i]->transformBy(mat);
	}
}
/*!
@brief 添加用于jig时显示的实体
*/
void GMultiJigEntity::Add(AcDbEntity* pEnt)
{
	mEntityArr.push_back(pEnt);
	mEntityTransArr.push_back(AcGeMatrix3d::kIdentity);
}
/*!
@brief 添加到数据库
*/
void GMultiJigEntity::AddToDb(AcDbObjectIdArray& ids)
{
	//获取活动空间
    AcDbLayoutManager* pLayoutMng = acdbHostApplicationServices()->layoutManager();
    AcDbObjectId spaceId = pLayoutMng->getActiveLayoutBTRId();
    AcDbBlockTableRecord* pSpace = NULL;
    Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)pSpace, spaceId, AcDb::kForWrite);
    if (es != Acad::eOk)
    {
        //GLOG_ERROR(_T("获取活动空间失败:") << acadErrorStatusText(es));
        return;
    }

    //添加到活动空间
    vector<AcDbEntity*>::iterator iter = mEntityArr.begin();
	for (; iter != mEntityArr.end(); iter++)
    {
		AcDbObjectId id;
        pSpace->appendAcDbEntity(id,*iter);
		ids.append(id);
        (*iter)->close();
    }

    pSpace->close();
}
