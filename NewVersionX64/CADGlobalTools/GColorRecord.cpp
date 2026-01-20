#include "StdAfx.h"
#include "GColorRecord.h"

GColorRecord::GColorRecord(void)
{
	GradientOneColorMode=false;
	strText=_T("");
}

GColorRecord::~GColorRecord(void)
{
}
GColorRecord::GColorRecord(const GColorRecord& src)
{
	Color=src.Color;
	objId=src.objId;
	Dimclrd=src.Dimclrd;
	Dimclre=src.Dimclre;
	Dimclrt=src.Dimclrt;
	GradientOneColorMode=src.GradientOneColorMode;
	hatchObjectType=src.hatchObjectType;
	GradientColors.setLogicalLength(0);
	GradientColors.append(src.GradientColors);
	GradientValues.setLogicalLength(0);
	GradientValues.append(src.GradientValues);
	strText=src.strText;
}
GColorRecord& GColorRecord::operator=(const GColorRecord& src)
{
	if(&src==this)
	{
		return *this;
	}
	Color=src.Color;
	objId=src.objId;
	Dimclrd=src.Dimclrd;
	Dimclre=src.Dimclre;
	Dimclrt=src.Dimclrt;
	GradientOneColorMode=src.GradientOneColorMode;
	hatchObjectType=src.hatchObjectType;
	GradientColors.setLogicalLength(0);
	GradientColors.append(src.GradientColors);
	GradientValues.setLogicalLength(0);
	GradientValues.append(src.GradientValues);
	strText=src.strText;
	return *this;
}
Acad::ErrorStatus GColorRecord::Read(AcDbEntity* pEnt)
{
	objId=pEnt->objectId();
	Color=pEnt->color();
	if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		//标注类对象
		AcDbDimension* pDim=AcDbDimension::cast(pEnt);
		Dimclrd=pDim->dimclrd();
		Dimclre=pDim->dimclre();
		Dimclrt=pDim->dimclrt();
	}
	else if(pEnt->isKindOf(AcDbHatch::desc()))
	{
		AcDbHatch* pHatch=AcDbHatch::cast(pEnt);
		GradientOneColorMode=pHatch->getGradientOneColorMode();
		hatchObjectType=pHatch->hatchObjectType();
		unsigned int count=0; 
		AcCmColor* colors=NULL;
		float* values=NULL;
		GradientColors.setLogicalLength(0);
		GradientValues.setLogicalLength(0);
		Acad::ErrorStatus es=pHatch->getGradientColors(count,colors,values);
		if(es==eOk&&count>0)
		{
			for(int i=0;i<count;i++)
			{
				GradientColors.append(colors[i].colorIndex());
				GradientValues.append(values[i]);
			}
		}
	}
	else if(pEnt->isKindOf(AcDbLeader::desc()))
	{
		AcDbLeader* pLeader=AcDbLeader::cast(pEnt);
		Dimclrd=pLeader->dimclrd();
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText=AcDbMText::cast(pEnt);
		TCHAR* pStr=pMText->contents();
		strText=pStr;
		acutDelString(pStr);
	}
	return Acad::eOk;
}
Acad::ErrorStatus GColorRecord::Write(AcDbEntity* pEnt)
{
	pEnt->setColor(Color);
	if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		//标注类对象
		AcDbDimension* pDim=AcDbDimension::cast(pEnt);
		pDim->setDimclrd(Dimclrd);
		pDim->setDimclre(Dimclre);
		pDim->setDimclrt(Dimclrt);
	}
	else if(pEnt->isKindOf(AcDbHatch::desc()))
	{
		AcDbHatch* pHatch=AcDbHatch::cast(pEnt);
		pHatch->setGradientOneColorMode(GradientOneColorMode);
		pHatch->setHatchObjectType(hatchObjectType);
		hatchObjectType=pHatch->hatchObjectType();
		if(!GradientOneColorMode)
		{
			unsigned int count=GradientColors.logicalLength();
			AcCmColor* new_colors=new AcCmColor[count];
			float* new_values=new float[count];
			for(unsigned int i=0;i<count;i++)
			{
				new_colors[i]=GradientColors.at(i);
				new_values[i]=GradientValues.at(i);
			}
			pHatch->setGradientColors(count,new_colors,new_values);
#ifdef _WIN64
			delete[] new_colors;
			delete[] new_values;
#else
			delete[] new_colors;
			delete[] new_values;
#endif
		}
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pMText=AcDbMText::cast(pEnt);
		pMText->setContents(strText);
	}
	return Acad::eOk;
}
//功能：锁定\解锁图层
//参数：objId：图层id
//		bFlag：锁定方式。0:强制打开；1:强制锁定；其他:根据当前状态取反。
Acad::ErrorStatus GColorRecord::LockLayer(AcDbObjectId objId,int bFlag)
{
	
	AcDbObject *pObj=NULL;
	Acad::ErrorStatus es=acdbOpenObject(pObj,objId,AcDb::kForWrite);
	if(es!=Acad::eOk)
		return es;

	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		pObj=NULL;
		return Acad::eNotThatKindOfClass;
	}

	AcDbLayerTableRecord *pRcd=AcDbLayerTableRecord::cast(pObj);
	if(pRcd==NULL)
	{
		pObj->close();
		pObj=NULL;
		return Acad::eNullHandle;
	}

	if(bFlag==0)//解锁
		pRcd->setIsLocked(false);
	else if(bFlag==1)//锁定
		pRcd->setIsLocked(true);
	else//根据当前状态取反
		pRcd->setIsLocked(pRcd->isLocked()?false:true);

	pRcd->close();
	pRcd=NULL;
	pObj->close();
	pObj=NULL;
	return Acad::eOk;
}
//回滚
Acad::ErrorStatus GColorRecord::Rollback()
{
	AcDbEntityPointer spEnt(objId,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=Acad::eOk)
	{
		if(es!=Acad::eOnLockedLayer)
		{
			return es;
		}
		AcDbEntity* pEnt=NULL;
		es=acdbOpenObject(pEnt,objId,AcDb::kForRead);
		if(es!=Acad::eOk)
		return es;
		es=LockLayer(pEnt->layerId(),1);
		if(es!=Acad::eOk)
		{
			pEnt->close();
			return es;
		}
		es=Write(pEnt);
		LockLayer(pEnt->layerId(),0);
		pEnt->close();
		return es;
	}
	es=Write(spEnt.object());
	return es;

}
//回滚
Acad::ErrorStatus GColorRecord::Rollback(CArray<GColorRecord>& colorRecordArr)
{
	for(long i=0;i<colorRecordArr.GetCount();i++)
	{
		GColorRecord& rc=colorRecordArr.GetAt(i);
		rc.Rollback();
	}
	return Acad::eOk;
}
