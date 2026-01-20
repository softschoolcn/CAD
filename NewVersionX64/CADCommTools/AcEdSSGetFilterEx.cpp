#include "StdAfx.h"
#include "AcEdSSGetFilterEx.h"
void
    AcEdSSGetFilterEx::ssgetAddFilter(
	    int ssgetFlags,
	    AcEdSelectionSetService &service,
	    const AcDbObjectIdArray& selectionSet,
	    const AcDbObjectIdArray& subSelectionSet)
{
	for(int i=subSelectionSet.logicalLength()-1;i>=0;i--)
	{
		if(CanBeSelect(subSelectionSet.at(i)))
		{
			continue;
		}
		service.remove(i);
	}
}
void AcEdSSGetFilterEx::endSSGet(
								 Acad::PromptStatus returnStatus,
								 int ssgetFlags,
								 AcEdSelectionSetService &service,
								 const AcDbObjectIdArray& selectionSet)
{
	for(int i=selectionSet.logicalLength()-1;i>=0;i--)
	{
		if(CanBeSelect(selectionSet.at(i)))
		{
			continue;
		}
		service.remove(i);
	}
}
void AcEdSSGetFilterEx::endEntsel(
								  Acad::PromptStatus       returnStatus,
								  const AcDbObjectId&      pickedEntity,
								  const AcGePoint3d&       pickedPoint,
								  AcEdSelectionSetService& service)
{
	if(CanBeSelect(pickedEntity))
	{
		return;
	}
	service.remove(0);
}
//是否可以选中
bool AcEdSSGetFilterEx::CanBeSelect(AcDbObjectId id)
{
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return false;
	}
	bool rc=CanBeSelect(pEnt);
	pEnt->close();
	return rc;
}
//是否可以选中
bool AcEdSSGetFilterEx::CanBeSelect(AcDbEntity* pEnt)
{
	return true;
}

void GLayerSSGetFilter::Reset()
{
	m_allowLayerArr.setLogicalLength(0);
	m_notAllowLayerArr.setLogicalLength(0);
	m_pClass=NULL;
}
bool Find(AcDbObjectIdArray& ids,AcDbObjectId id)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		if(ids.at(i)==id)
		{
			return true;
		}
	}
	return false;
}
//是否可以选中
bool GLayerSSGetFilter::CanBeSelect(AcDbEntity* pEnt)
{
	if(m_pClass!=NULL)
	{
		return pEnt->isKindOf(m_pClass);
	}
	AcDbObjectId layerId=pEnt->layerId();
	if(m_allowLayerArr.logicalLength()>0)
	{
		return Find(m_allowLayerArr,layerId);
	}
	if(m_notAllowLayerArr.logicalLength()>0)
	{
		return Find(m_notAllowLayerArr,layerId);
	}
	return true;
}
