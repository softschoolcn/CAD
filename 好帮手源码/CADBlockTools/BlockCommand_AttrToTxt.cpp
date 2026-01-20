#include "StdAfx.h"
#include "BlockCommCommand.h"
AcDbText* BrefAttrToText(AcDbAttribute* pAttr)
{
	AcDbText* pText=new AcDbText;
	GCADUtils::SameEntityAttr(pText,pAttr);
	pText->setHeight(pAttr->height());
	pText->setHorizontalMode(pAttr->horizontalMode());
	pText->setVerticalMode(pAttr->verticalMode());
	pText->setNormal(pAttr->normal());
	pText->setOblique(pAttr->oblique());
	pText->setPosition(pAttr->position());
	pText->setAlignmentPoint(pAttr->alignmentPoint());
	pText->setRotation(pAttr->rotation());
	TCHAR* lpStr=pAttr->textString();
	pText->setTextString(lpStr);
	acutDelString(lpStr);
	pText->setTextStyle(pAttr->textStyle());
	pText->setThickness(pAttr->thickness());
	pText->setWidthFactor(pAttr->widthFactor());
	return pText;
}
Acad::ErrorStatus BrefAttrToText(AcDbBlockReference* pRef)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbObjectIterator* pIter=pRef->attributeIterator();
	if(pIter==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbObjectIdArray ids;
	for(pIter->start();!pIter->done();pIter->step())
	{
		AcDbObjectId id=pIter->objectId();
		ids.append(id);
	}
	delete pIter;
	pIter=NULL;
	AcDbBlockTableRecordPointer spRec(pRef->blockId(),AcDb::kForWrite);
	if(spRec.openStatus()!=Acad::eOk)
	{
		return spRec.openStatus();
	}
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId id=ids.at(i);
		AcDbAttribute* pAttr=NULL;
		es=pRef->openAttribute(pAttr,id,AcDb::kForWrite);
		if(es!=Acad::eOk)
		{
			continue;
		}
		if(pAttr->visibility()!=AcDb::kVisible)
		{
			//不可见的属性不转换
			pAttr->erase();
		pAttr->close();
		continue;
		}
		AcDbText* pText=BrefAttrToText(pAttr);
		double dHeight=pAttr->height();
		pAttr->erase();
		pAttr->close();
		pText->setLayer(pRef->layerId());
		pText->setHeight(dHeight);
		AcDbObjectId textId;
		textId.setNull();
		spRec->appendAcDbEntity(textId,pText);
		pText->close();

	}
	return Acad::eOk;
}
//属性转文字
void AttrToTxt()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("INSERT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择属性块:"),_T("\n移除属性块:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId id=ids1.at(i);
		AcDbObjectPointer<AcDbBlockReference> spRef(id,AcDb::kForRead);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		BrefAttrToText(spRef.object());
	}
}