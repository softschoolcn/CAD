#include "StdAfx.h"
#include "GSelectHelper.h"

GSelectHelper::GSelectHelper(void)
{
}

GSelectHelper::~GSelectHelper(void)
{
}
//设置实体的选中状态
//参数：ObjId,实体的ObjectId
//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
void GSelectHelper::SetEntitySelected(AcDbObjectId ObjId,bool Clear)
{
	AcDbEntityPointer spEnt(ObjId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId realId=ObjId;
	if(spEnt->isKindOf(AcDbAttribute::desc()))
	{
		realId=spEnt->ownerId();
	}
	ads_name tempselect;
	ads_name tempss;
	int es1=0;
	Acad::ErrorStatus es2=acdbGetAdsName(tempss,realId);
	if(es2!=Acad::eOk)
	{
		acutPrintf(_T("实体转换成选择集失败\n"));
		acedSSFree(tempselect);
		acedSSFree(tempss);
		return;
	}
	es1=acedSSAdd(tempss,NULL,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("将实体加入选择集失败\n"));
		acedSSFree(tempselect);
		acedSSFree(tempss);
		return;
	}
	acedSSFree(tempss);//加入成功后，tempss必须释放
	es1=acedSSSetFirst(tempselect,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("设置当前选择集失败\n"));
		acedSSFree(tempselect);
		return;
	}
	acedUpdateDisplay();
	acedSSFree(tempselect);
}
//设置实体为选中状态
void GSelectHelper::SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear)
{
	ads_name tempselect;
	ads_name tempss;
	int es1=0;
	Acad::ErrorStatus es2=Acad::eOk;
	for(int i=0;i<ObjIds.logicalLength();i++)
	{
		AcDbObjectId ObjId=ObjIds.at(i);
		es2=acdbGetAdsName(tempss,ObjId);
		if(es2!=Acad::eOk)
		{
			acutPrintf(_T("实体转换成选择集失败\n"));
			es1=acedSSFree(tempselect);
			es1=acedSSFree(tempss);
			return;
		}
		es1=acedSSAdd(tempss,i==0?NULL:tempselect,tempselect);
		if(es1!=RTNORM)
		{
			acutPrintf(_T("将实体加入选择集失败\n"));
			es1=acedSSFree(tempselect);
			es1=acedSSFree(tempss);
			return;
		}
		es1=acedSSFree(tempss);
	}
	es1=acedSSSetFirst(tempselect,tempselect);
	if(es1!=RTNORM)
	{
		acutPrintf(_T("设置当前选择集失败\n"));
		es1=acedSSFree(tempselect);
		return;
	}
	acedUpdateDisplay();
	es1=acedSSFree(tempselect);
}
