#include "StdAfx.h"
#include "GHighlightHelper.h"

GHighlightHelper::GHighlightHelper(void)
{
}

GHighlightHelper::~GHighlightHelper(void)
{
	UnHighlight(highlightIds);
}
Acad::ErrorStatus GHighlightHelper::Highlight(AcDbObjectId objId)
{
	return _Highlight(objId,true);
}
bool GHighlightHelper::IsHighlight(AcDbObjectId objId)
{
	return highlightIds.contains(objId);
}
Acad::ErrorStatus GHighlightHelper::UnHighlight(AcDbObjectId objId)
{
	return _UnHighlight(objId,true);
}
Acad::ErrorStatus GHighlightHelper::Highlight(AcDbObjectIdArray& ids)
{
	if(ids.logicalLength()==0)
		return Acad::eOk;
	for(int i=0;i<ids.logicalLength();i++)
	{
		_Highlight(ids.at(i),false);
	}
	acedUpdateDisplay();
	return Acad::eOk;
}
Acad::ErrorStatus GHighlightHelper::UnHighlight(AcDbObjectIdArray& ids)
{
	if(ids.logicalLength()==0)
		return Acad::eOk;
	for(int i=0;i<ids.logicalLength();i++)
	{
		_UnHighlight(ids.at(i),false);
	}
	acedUpdateDisplay();
	return Acad::eOk;
}
Acad::ErrorStatus GHighlightHelper::_Highlight(AcDbObjectId objId,bool update)
{
	if(IsHighlight(objId))
		return Acad::eInvalidInput;
	AcDbEntity *pEnt=NULL; 
	Acad::ErrorStatus es;
	if((es = acdbOpenAcDbEntity(pEnt,objId,AcDb::kForWrite)) != Acad::eOk )
	{
		return es; 
	}
	pEnt->highlight();
	pEnt->close();
	highlightIds.append(objId);
	if(update)
	{
		acedUpdateDisplay();
	}
	
	return Acad::eOk;
}
Acad::ErrorStatus GHighlightHelper::_UnHighlight(AcDbObjectId objId,bool update)
{
	if(!IsHighlight(objId))
		return Acad::eInvalidInput;
	AcDbEntity *pEnt=NULL; 
	Acad::ErrorStatus es;
	if((es = acdbOpenAcDbEntity(pEnt,objId,AcDb::kForWrite)) != Acad::eOk )
	{
		return es; 
	}
	pEnt->unhighlight();
	pEnt->close();
	highlightIds.remove(objId);
	if(update)
	{
		acedUpdateDisplay();
	}
	return Acad::eOk;
}
