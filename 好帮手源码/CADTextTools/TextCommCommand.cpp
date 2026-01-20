#include "StdAfx.h"
#include "TextCommCommand.h"
#include "TextCrossReportFunction.h"
#include "..\\CADGlobalTools\\SortArray.h"
#include "GLockDocument.h"
#include "GBatchCopyDialog.h"
#include "GItemSelectDialog.h"
#include "GlobalFunction.h"
#if _MSC_VER>=1700
#include "acedCmdNF.h"
#endif
AcGePoint3d ptMTextPosition;
int EnumMTextPosition(AcDbMTextFragment * pFragment, void *pData)
{
	ptMTextPosition=pFragment->location;
	return 0;
}
AcDbMText::AttachmentPoint SameAlign(AcDb::TextHorzMode hMode,AcDb::TextVertMode vMode)
{
	AcDbMText::AttachmentPoint attachmentPoint=AcDbMText::kMiddleCenter;
	if(vMode==AcDb::kTextBase)
	{
		if(hMode==AcDb::kTextLeft||
			hMode==AcDb::kTextAlign||
			hMode==AcDb::kTextFit)
		{
			//hMode==AcDb::kTextLeft,左
			//hMode==AcDb::kTextAlign,对齐
			//hMode==AcDb::kTextFit,调整
			attachmentPoint=AcDbMText::kBottomLeft;
		}
		else if(hMode==AcDb::kTextCenter)
		{
			//中
			attachmentPoint=AcDbMText::kBottomCenter;
		}
		else if(hMode==AcDb::kTextRight)
		{
			//右
			attachmentPoint=AcDbMText::kBottomRight;
		}
		else if(hMode==AcDb::kTextMid)
		{
			attachmentPoint=AcDbMText::kBottomCenter;
		}
	}
	else if(vMode==AcDb::kTextBottom)
	{
		if(hMode==AcDb::kTextLeft||
			hMode==AcDb::kTextAlign||
			hMode==AcDb::kTextFit)
		{
			//左下
			attachmentPoint=AcDbMText::kBottomLeft;
		}
		else if(hMode==AcDb::kTextCenter)
		{
			//中下
			attachmentPoint=AcDbMText::kMiddleCenter;
		}
		else if(hMode==AcDb::kTextRight)
		{
			//右下
			attachmentPoint=AcDbMText::kBottomRight;
		}
		else if(hMode==AcDb::kTextMid)
		{
			attachmentPoint=AcDbMText::kBottomCenter;
		}
	}
	else if(vMode==AcDb::kTextVertMid)
	{
		if(hMode==AcDb::kTextLeft||
			hMode==AcDb::kTextAlign||
			hMode==AcDb::kTextFit)
		{
			//hMode==AcDb::kTextLeft,左中
			attachmentPoint=AcDbMText::kMiddleLeft;
		}
		else if(hMode==AcDb::kTextCenter)
		{
			//正中
			attachmentPoint=AcDbMText::kMiddleCenter;
		}
		else if(hMode==AcDb::kTextRight)
		{
			//右中
			attachmentPoint=AcDbMText::kMiddleRight;
		}
		else if(hMode==AcDb::kTextMid)
		{
			attachmentPoint=AcDbMText::kMiddleCenter;
		}
	}
	else if(vMode==AcDb::kTextTop)
	{
		if(hMode==AcDb::kTextLeft||
			hMode==AcDb::kTextAlign||
			hMode==AcDb::kTextFit)
		{
			//hMode==AcDb::kTextLeft,左上
			attachmentPoint=AcDbMText::kTopLeft;
		}
		else if(hMode==AcDb::kTextCenter)
		{
			//中上
			attachmentPoint=AcDbMText::kTopCenter;
		}
		else if(hMode==AcDb::kTextRight)
		{
			//右上
			attachmentPoint=AcDbMText::kTopRight;
		}
		else if(hMode==AcDb::kTextMid)
		{
			attachmentPoint=AcDbMText::kTopCenter;
		}
	}
	return attachmentPoint;
}
void SameAlign(AcDb::TextHorzMode& hMode,AcDb::TextVertMode& vMode,AcDbMText::AttachmentPoint attachmentPoint)
{
	if(AcDbMText::kBottomLeft==attachmentPoint)
	{
		hMode=AcDb::kTextLeft;
		vMode=AcDb::kTextBottom;
	}
	else if(AcDbMText::kBottomCenter==attachmentPoint)
	{
		hMode=AcDb::kTextCenter;
		vMode=AcDb::kTextBottom;
	}
	else if(AcDbMText::kBottomRight==attachmentPoint)
	{
		hMode=AcDb::kTextRight;
		vMode=AcDb::kTextBottom;
	}
	else if(AcDbMText::kMiddleLeft==attachmentPoint)
	{
		hMode=AcDb::kTextLeft;
		vMode=AcDb::kTextVertMid;
	}
	else if(AcDbMText::kMiddleCenter==attachmentPoint)
	{
		hMode=AcDb::kTextCenter;
		vMode=AcDb::kTextVertMid;
	}
	else if(AcDbMText::kMiddleRight==attachmentPoint)
	{
		hMode=AcDb::kTextRight;
		vMode=AcDb::kTextVertMid;
	}
	else if(AcDbMText::kTopLeft==attachmentPoint)
	{
		hMode=AcDb::kTextLeft;
		vMode=AcDb::kTextTop;
	}
	else if(AcDbMText::kTopCenter==attachmentPoint)
	{
		hMode=AcDb::kTextCenter;
		vMode=AcDb::kTextTop;
	}
	else if(AcDbMText::kTopRight==attachmentPoint)
	{
		hMode=AcDb::kTextRight;
		vMode=AcDb::kTextTop;
	}
	else
	{
		hMode=AcDb::kTextLeft;
		vMode=AcDb::kTextBase;
	}
}
AcDbMText* TxtToMText(AcDbText* pText)
{
	AcDbMText* pMText=new AcDbMText();
	GCADUtils::SameEntityAttr(pMText,pText);
	pMText->setLocation(pText->position());
	pMText->setTextStyle(pText->textStyle());
	TCHAR* pTxt=pText->textString();
	pMText->setContents(pTxt);
	acutDelString(pTxt);
	pMText->setTextHeight(pText->height());
	pMText->setRotation(pText->rotation());
	AcDb::TextHorzMode hMode=pText->horizontalMode();
	AcDb::TextVertMode vMode=pText->verticalMode();
	AcDbMText::AttachmentPoint attachmentPoint=SameAlign(hMode,vMode);
	//设置相同的对齐方式
	pMText->setAttachment(attachmentPoint);
	return pMText;
}
Acad::ErrorStatus TxtToMText(AcDbObjectId id)
{
	AcDbObjectPointer<AcDbText> spText(id,AcDb::kForRead);
	if(spText.openStatus()!=Acad::eOk)
	{
		return spText.openStatus();
	}
	AcDbMText* pMText=TxtToMText(spText.object());
	if(pMText==NULL)
	{

		return Acad::eNullHandle;
	}
	AcDbObjectId id1=GCADUtils::AppendEntity(pMText,spText->blockId());
	pMText->close();
	AcDb::TextVertMode vMode=spText->verticalMode();
	double dx=0.0;
	double dy=0.0;
	AcDbObjectPointer<AcDbMText> spMText(id1,AcDb::kForWrite);
	if(spMText.openStatus()==Acad::eOk)
	{
		spMText->explodeFragments(EnumMTextPosition,pMText,NULL);
		AcGeVector3d vec=spText->position()-ptMTextPosition;
		dx=vec.x;
		dy=vec.y;
		AcGePoint3d ptNew=spText->position();
		ptNew.x+=dx;
		ptNew.y+=dy;
		spMText->setLocation(ptNew);

	}
	if(spText->upgradeOpen()==Acad::eOk)
	{
		spText->erase();
	}
	return Acad::eOk;
}
#if _MSC_VER>=1500
Acad::ErrorStatus AttrToMAttr(AcDbBlockTableRecord* pBTR,const TCHAR* lpText)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcDbObjectIdArray ids;
	EnumAllEntityInBTR(ids,pBTR);
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
			continue;
		if(!spEnt->isKindOf(AcDbAttributeDefinition::desc()))
			continue;
		AcDbAttributeDefinition* pAttr=AcDbAttributeDefinition::cast(spEnt.object());
		pAttr->setTextString(lpText);
		pAttr->setLockPositionInBlock(false);
		if(pAttr->isMTextAttributeDefinition())
			continue;
		pAttr->convertIntoMTextAttributeDefinition();
	}
	return es;
}
Acad::ErrorStatus AttrToMAttr(AcDbBlockReference* pRef,bool& needReset)
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
	AcDbBlockTableRecordPointer spRec2(pRef->blockTableRecord(),AcDb::kForRead);
	if(spRec2.openStatus()==Acad::eOk)
	{
		AttrToMAttr(spRec2.object(),_T(""));
	}
	spRec2->close();

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
			pAttr->close();
			continue;
		}
		AcDbExtents ext1;
		es=pAttr->getGeomExtents(ext1);

		if(pAttr->isMTextAttribute())
		{
			pAttr->close();
			continue;
		}
		pAttr->setLockPositionInBlock(false);
		needReset=true;
		es=pAttr->convertIntoMTextAttribute();

		AcDbExtents ext2;
		es=pAttr->getGeomExtents(ext2);
		AcGeMatrix3d xform;
		xform.setToTranslation(ext1.minPoint()-ext2.minPoint());
		es=pAttr->transformBy(xform);
		pAttr->close();

	}
	return Acad::eOk;
}
#endif
//重置块
void ResetBlock(AcDbObjectIdArray& ObjIds)
{
	ads_name tempselect;
	acedSSAdd(NULL,NULL,tempselect);
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
		es1=acedSSAdd(tempss,tempselect,tempselect);
		if(es1!=RTNORM)
		{
			acutPrintf(_T("将实体加入选择集失败:%d\n"),es1);
			es1=acedSSFree(tempselect);
			es1=acedSSFree(tempss);
			return;
		}
		es1=acedSSFree(tempss);
	}
	#if _MSC_VER>=1700
	acedCommandS(RTSTR,_T("resetblock"),RTPICKS,tempselect,RTSTR,_T(""),RTNONE);
#else
	acedCommand(RTSTR,_T("resetblock"),RTPICKS,tempselect,RTSTR,_T(""),RTNONE);
#endif
	acedUpdateDisplay();
	es1=acedSSFree(tempselect);
}
//单行文本转换为多行文本
void TxtToMText()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,INSERT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择单行文字/块:"),_T("\n移除单行文字/块:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	AcDbObjectIdArray btr_ids;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointer spEnt(ids.at(i),AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
			continue;
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			spEnt->close();
			TxtToMText(ids.at(i));
		}
#if _MSC_VER>=1500
		else if(spEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef=AcDbBlockReference::cast(spEnt.object());
			bool needReset=false;
			AttrToMAttr(pRef,needReset);
			if(needReset&&GCADUtils::FindObjectId(btr_ids,pRef->blockTableRecord())<0)
			{
				btr_ids.append(pRef->blockTableRecord());
			}
		}
#endif
	}
	AcDbObjectIdArray ids_all;
	for(int i=0;i<btr_ids.logicalLength();i++)
	{
		AcDbObjectIdArray ids1;
		EnumAllEntityInBTR(ids1,btr_ids.at(i));
		for(int j=0;j<ids1.logicalLength();j++)
		{
			AcDbEntityPointer spEnt(ids1.at(j),AcDb::kForRead);
			if(spEnt.openStatus()!=Acad::eOk)
				continue;
			if(spEnt->isKindOf(AcDbText::desc()))
			{
				spEnt->close();
				TxtToMText(ids1.at(j));
			}
		}
		AcDbBlockTableRecordPointer spBTR(btr_ids.at(i),AcDb::kForRead);
		if(spBTR.openStatus()!=Acad::eOk)
			continue;
		AcDbObjectIdArray ids2;
		spBTR->getBlockReferenceIds(ids2);
		ids_all.append(ids2);
		for(int j=0;j<ids2.logicalLength();j++)
		{
			AcDbEntityPointer spEnt(ids2.at(j),AcDb::kForWrite);
			if(spEnt.openStatus()!=Acad::eOk)
				continue;
			spEnt->recordGraphicsModified();
		}
	}
	if(ids_all.logicalLength()>0)
		ResetBlock(ids_all);
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}
Acad::ErrorStatus ExplodeMText(AcDbMText* pMText,const AcDbObjectId btrID)
{
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=pMText->explode(entitySet);
	if(es!=Acad::eOk)
		return es;
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//不可能的
			continue;
		}
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			GCADUtils::AppendEntity(pEnt,btrID);
			pEnt->close();
		}
		else if(pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt);
			ExplodeMText(pMText,btrID);
		}
		else
		{
			pEnt->close();
		}


	}
	return Acad::eOk;
}
Acad::ErrorStatus ExplodeMText(AcDbObjectIdArray& ids,AcDbMText* pMText,const AcDbObjectId btrID)
{
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=pMText->explode(entitySet);
	if(es!=Acad::eOk)
		return es;
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//不可能的
			continue;
		}
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			ids.append(GCADUtils::AppendEntity(pEnt,btrID));
			pEnt->close();
		}
		else if(pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt);
			ExplodeMText(ids,pMText,btrID);
		}
		else
		{
			pEnt->close();
		}


	}
	return Acad::eOk;
}
Acad::ErrorStatus MTextToTxt(AcDbObjectIdArray& ids,AcDbObjectId id)
{
	AcDbObjectPointer<AcDbMText> spMText(id,AcDb::kForRead);
	if(spMText.openStatus()!=Acad::eOk)
	{
		return spMText.openStatus();
	}
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=spMText->explode(entitySet);
	if(es!=Acad::eOk)
		return es;
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//不可能的
			continue;
		}
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			ids.append(GCADUtils::AppendEntity(pEnt,spMText->blockId()));
			pEnt->close();
		}
		else if(pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt);
			ExplodeMText(ids,pMText,spMText->blockId());
		}
		else
		{
			pEnt->close();
		}


	}
	if(spMText->upgradeOpen()==Acad::eOk)
	{
		spMText->erase();
	}
	return Acad::eOk;
}
Acad::ErrorStatus MTextToTxt(AcDbObjectId id)
{
	AcDbObjectPointer<AcDbMText> spMText(id,AcDb::kForRead);
	if(spMText.openStatus()!=Acad::eOk)
	{
		return spMText.openStatus();
	}
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=spMText->explode(entitySet);
	if(es!=Acad::eOk)
		return es;
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//不可能的
			continue;
		}
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			GCADUtils::AppendEntity(pEnt,spMText->blockId());
			pEnt->close();
		}
		else if(pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt);
			ExplodeMText(pMText,spMText->blockId());
		}
		else
		{
			pEnt->close();
		}


	}
	if(spMText->upgradeOpen()==Acad::eOk)
	{
		spMText->erase();
	}
	return Acad::eOk;
}
//多行文字转换为单行文字
void MTextToTxt()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多行文字:"),_T("\n移除多行文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	for(int i=0;i<ids.logicalLength();i++)
	{
		MTextToTxt(ids.at(i));
	}
}
//获取范围对应的矩形四个点
void GetExtentsPts(AcGePoint3dArray& pts,AcDbExtents extents)
{
	pts.append(extents.minPoint());
	pts.append(AcGePoint3d(extents.maxPoint().x,extents.minPoint().y,0.0));
	pts.append(extents.maxPoint());
	pts.append(AcGePoint3d(extents.minPoint().x,extents.maxPoint().y,0.0));
}
AcDbObjectId AppendPolyline(AcGePoint3dArray& pts,AcDbEntity* pAttrEnt,bool bClose,const AcDbObjectId btrID,int nColorIndex=-1)
{
	AcDbPolyline* pLine=new AcDbPolyline(pts.logicalLength());
	for(int i=0;i<pts.logicalLength();i++)
	{
		pLine->addVertexAt(i,AcGePoint2d(pts.at(i).x,pts.at(i).y));
	}
	GCADUtils::SameEntityAttr(pLine,pAttrEnt);
	if(nColorIndex>=0)
	{
		pLine->setColorIndex(nColorIndex);
	}
	pLine->setClosed(bClose?Adesk::kTrue:Adesk::kFalse);
	AcDbObjectId id=GCADUtils::AppendEntity(pLine,btrID);
	pLine->close();
	return id;
}
Acad::ErrorStatus GetBorderPts(AcGePoint3dArray& pts,AcDbText* pText)
{
	Acad::ErrorStatus es=Acad::eOk;
	double oldRotation=pText->rotation();
	if(oldRotation==0||
		oldRotation==PI/2.0||
		oldRotation==PI||
		oldRotation==3.0*PI/2.0||
		oldRotation==2.0*PI)
	{
		AcDbExtents extents;
		pText->getGeomExtents(extents);
		GetExtentsPts(pts,extents);
	}
	else
	{
		es=pText->upgradeOpen();
		if(es!=Acad::eOk)
			return es;
		pText->setRotation(0.0);
		AcDbExtents extents;
		pText->getGeomExtents(extents);
		GetExtentsPts(pts,extents);
		pText->setRotation(oldRotation);
		AcGeMatrix3d mat;
		mat.setToRotation(oldRotation,AcGeVector3d::kZAxis,pText->position());
		for(int i=0;i<pts.logicalLength();i++)
		{
			AcGePoint3d pt=pts.at(i);
			pt=pt.transformBy(mat);
			pts.setAt(i,pt);
		}
	}
	return Acad::eOk;
}
Acad::ErrorStatus GetMTextGeomExtents(AcDbExtents& extents,bool& extentsInit,AcDbMText* pMText)
{
	AcDbVoidPtrArray entitySet;
	Acad::ErrorStatus es=pMText->explode(entitySet);
	if(es!=Acad::eOk)
		return es;
	for(int i=0;i<entitySet.logicalLength();i++)
	{
		AcDbEntity* pEnt=(AcDbEntity*)entitySet.at(i);
		if(pEnt==NULL)
		{
			//不可能的
			continue;
		}
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(pEnt);
			if(extentsInit)
			{
				AcDbExtents ext;
				pText->getGeomExtents(ext);
				extents.addExt(ext);
			}
			else
			{
				pText->getGeomExtents(extents);
				extentsInit=true;
			}
		}
		else if(pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText=AcDbMText::cast(pEnt);
			GetMTextGeomExtents(extents,extentsInit,pText);
		}
		else
		{
		}
		delete pEnt;
		entitySet.setAt(i,NULL);
	}
	return Acad::eOk;
}
Acad::ErrorStatus GetBorderPts(AcGePoint3dArray& pts,AcDbMText* pText)
{
	Acad::ErrorStatus es=Acad::eOk;
	double oldRotation=pText->rotation();
	if(oldRotation==0||
		oldRotation==PI/2.0||
		oldRotation==PI||
		oldRotation==3.0*PI/2.0||
		oldRotation==2.0*PI)
	{
		AcDbExtents extents;
		bool extentsInit=false;
		es=GetMTextGeomExtents(extents,extentsInit,pText);
		if(es!=Acad::eOk)
			return es;
		if(!extentsInit)
		{
			return Acad::eNotApplicable;
		}
		GetExtentsPts(pts,extents);
	}
	else
	{
		es=pText->upgradeOpen();
		if(es!=Acad::eOk)
			return es;
		pText->setRotation(0.0);
		AcDbExtents extents;
		bool extentsInit=false;
		es=GetMTextGeomExtents(extents,extentsInit,pText);
		if(es!=Acad::eOk)
		{
			pText->setRotation(oldRotation);
			return es;
		}
		if(!extentsInit)
		{
			pText->setRotation(oldRotation);
			return Acad::eNotApplicable;
		}
		GetExtentsPts(pts,extents);
		pText->setRotation(oldRotation);
		AcGeMatrix3d mat;
		mat.setToRotation(oldRotation,AcGeVector3d::kZAxis,pText->location());
		for(int i=0;i<pts.logicalLength();i++)
		{
			AcGePoint3d pt=pts.at(i);
			pt=pt.transformBy(mat);
			pts.setAt(i,pt);
		}
	}
	return Acad::eOk;
}
Acad::ErrorStatus TextBorder(AcDbObjectId& lineId,AcDbObjectId objId,int nColorIndex=-1)
{
	AcDbEntityPointer spEnt(objId,AcDb::kForRead);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=Acad::eOk)
	{
		GLog::WriteLine(_T("TextBorder类型出错,打开实体对象失败:%s"),acadErrorStatusText(es));
		return es;
	}
	if(spEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=AcDbText::cast(spEnt.object());
		AcGePoint3dArray pts;
		es=GetBorderPts(pts,pText);
		if(es!=Acad::eOk)
			return es;
		lineId=AppendPolyline(pts,pText,true,pText->blockId(),nColorIndex);
	}
	else if(spEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText* pText=AcDbMText::cast(spEnt.object());
		AcGePoint3dArray pts;
		es=GetBorderPts(pts,pText);
		if(es!=Acad::eOk)
			return es;
		lineId=AppendPolyline(pts,pText,true,pText->blockId(),nColorIndex);
	}
	else
	{
		GLog::WriteLine(_T("TextBorder类型出错:%s"),spEnt->isA()->name());
	}
	return Acad::eOk;
}
//文字边框
void TextBorder()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId lineId;
		TextBorder(lineId,ids.at(i));
	}
}
//文字重叠
void TextCross()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	CArray<AcGePoint3dArray*> ptsArray;
	CStringArray strTextArr;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcGePoint3dArray* pts=new AcGePoint3dArray();
		AcDbObjectId objId=ids.at(i);
		AcDbEntityPointer spEnt(objId,AcDb::kForRead);
		Acad::ErrorStatus es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			ptsArray.Add(pts);
			strTextArr.Add(_T(""));
			continue;
		}
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());

			es=GetBorderPts(*pts,pText);
			if(es!=Acad::eOk)
			{
				ptsArray.Add(pts);
				strTextArr.Add(_T(""));
				continue;
			}
			ptsArray.Add(pts);
			TCHAR* pTxt=pText->textString();
			strTextArr.Add(pTxt);
			acutDelString(pTxt);
		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText=AcDbMText::cast(spEnt.object());
			es=GetBorderPts(*pts,pText);
			if(es!=Acad::eOk)
			{
				ptsArray.Add(pts);
				strTextArr.Add(_T(""));
				continue;
			}
			ptsArray.Add(pts);
			strTextArr.Add(GCADUtils::GetMText(pText));
		}
		else
		{
			ptsArray.Add(pts);
			strTextArr.Add(_T(""));
		}
	}
	double dScale=0.0;
	CArray<TextCrossRecord> result;
	CArray<double> valueResult;
	for(int i=0;i<ptsArray.GetCount();i++)
	{
		AcDbObjectId id1=ids.at(i);
		AcGePoint3dArray* pts1=ptsArray.GetAt(i);
		if(pts1->logicalLength()<3)
			continue;
		for(int j=i+1;j<ptsArray.GetCount();j++)
		{
			AcGePoint3dArray* pts2=ptsArray.GetAt(j);
			if(pts2->logicalLength()<3)
			{
				continue;
			}
			bool bIntersect=false;
			for(int k=0;k<pts2->logicalLength();k++)
			{
				if(IsPointInRegion(pts2->at(k),*pts1))
				{
					bIntersect=true;
					break;
				}
			}
			if(!bIntersect)
			{
				for(int k=0;k<pts1->logicalLength();k++)
				{
					if(IsPointInRegion(pts1->at(k),*pts2))
					{
						bIntersect=true;
						break;
					}
				}
			}
			if(!bIntersect)
			{
				for(int k=0;k<pts2->logicalLength()-1;k++)
				{
					AcDbLine line1(pts2->at(k),pts2->at(k+1));
					for(int l=0;l<pts1->logicalLength()-1;l++)
					{
						AcDbLine line2(pts1->at(l),pts1->at(l+1));
						AcGePoint3dArray pts;
						es=line1.intersectWith(&line2,AcDb::kOnBothOperands,pts);
						if(es!=Acad::eOk)
						{
							continue;
						}
						if(pts.logicalLength()==0)
						{
							continue;
						}
						bIntersect=true;
						break;
					}
					if(bIntersect)
					{
						break;
					}
				}
			}
			if(!bIntersect)
				continue;

			AcDbObjectId id2=ids.at(j);
			if(RgnIntersect(dScale,*pts1,*pts2))
			{
				TextCrossRecord tcr;
				tcr.dScale=dScale;
				valueResult.Add(dScale);
				tcr.pts1.append(*pts1);
				tcr.pts2.append(*pts2);
				tcr.textId1=id1;
				tcr.textId2=id2;
				tcr.strText1=strTextArr.GetAt(i);
				tcr.strText2=strTextArr.GetAt(j);
				result.Add(tcr);
			}
		}
	}
	for(int i=0;i<ptsArray.GetCount();i++)
	{
		AcGePoint3dArray* pts=ptsArray.GetAt(i);
		delete pts;
		pts=NULL;
	}
	//按照重叠比例的大小依次排序
	SortArray(result,valueResult,true);
	if(result.GetCount()==0)
	{
		ShowTextCrossReportPanel(FALSE);
		acutPrintf(_T("\n未发现任何重叠文字"));
		return;
	}
	ShowTextCrossReportPanel(TRUE);
	ShowTextCrossResult(result);

}
void TextBatchCopy()
{
	if(!CheckReg(_T("20130917")))
	{
		return;
	}
	Acad::ErrorStatus es=Acad::eOk;
	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	if(pDb==NULL)
		return;
	const TCHAR* lpFileName=NULL;
	pDb->getFilename(lpFileName);
label_select:
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择需要递增的数字/字母:"),_T("\n移除需要递增的数字/字母:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	CString strText;
	AcDbEntityPointer spEnt(ids.at(0),AcDb::kForRead);
	if(spEnt->isKindOf(AcDbText::desc()))
	{
		TCHAR* lpText=AcDbText::cast(spEnt.object())->textString();
		strText=lpText;
		acutDelString(lpText);
	}
	else if(spEnt->isKindOf(AcDbMText::desc()))
	{
		TCHAR* lpText=AcDbMText::cast(spEnt.object())->contents();
		strText=lpText;
		acutDelString(lpText);
	}
	spEnt->close();
	CArray<int> startIndexArray;
	CArray<int> endIndexArray;
	NumCanPlus(strText,startIndexArray,endIndexArray);
	if(startIndexArray.GetCount()==0)
	{
		acutPrintf(_T("\n不是合理的可以递增的数字或字母\n"));
		goto label_select;

	}
	int nStartIndex=-1;
	int nEndIndex=-1;
	if(startIndexArray.GetCount()>1)
	{
		CStringArray strArray;
		for(int i=0;i<startIndexArray.GetCount();i++)
		{
			strArray.Add(strText.Mid(startIndexArray.GetAt(i),endIndexArray.GetAt(i)-startIndexArray.GetAt(i)+1));
		}
		CAcModuleResourceOverride res;
		GItemSelectDialog dlg(strArray,0,_T("选择递增项"),CWnd::FromHandle(adsw_acadMainWnd()));
		if(dlg.DoModal()!=IDOK)
		{
			return;
		}
		nStartIndex=startIndexArray.GetAt(dlg.m_nSelectIndex);
		nEndIndex=endIndexArray.GetAt(dlg.m_nSelectIndex);
	}
	else
	{
		nStartIndex=startIndexArray.GetAt(0);
		nEndIndex=endIndexArray.GetAt(0);
	}
	CString strLeft=strText.Left(nStartIndex);
	CString strMid=strText.Mid(nStartIndex,nEndIndex-nStartIndex+1);
	CString strRight=strText.Right(strText.GetLength()-(nEndIndex+1));

	showDlgBatchCopy(ids.at(0),strLeft,strMid,strRight);





}
GBatchCopyDialog * pDlgBatchCopy = NULL;
Adesk::Boolean createBatchCopyDialog(AcDbObjectId id,CString strLeft,CString strMid,CString strRight)
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) {
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}
	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( pDlgBatchCopy == NULL ) 
	{
		if ( (pDlgBatchCopy = new GBatchCopyDialog ( id,strLeft,strMid,strRight,pWnd )) == NULL ) {
			//AfxMessageBox ( _T("Unable to allocate a CMDITestDialog.") );
			return Adesk::kFalse;
		}

		BOOL succeeded = pDlgBatchCopy->Create ( GBatchCopyDialog::IDD,pWnd );
		if ( !succeeded ) {
			//AfxMessageBox ( _T("Unable to create the dialog.") );
			return Adesk::kFalse;
		}
	}
	return Adesk::kTrue;
}
void showDlgBatchCopy(AcDbObjectId id,CString strLeft,CString strMid,CString strRight)
{
	//if( pDlgReadInfo ) {
	//	// dialog was already created, just display it
	//	// (the dialog only gets deallocated when docman.arx is unloaded)
	//	pDlgReadInfo->ShowWindow( SW_SHOWNORMAL );
	//	return;
	//}
	if( pDlgBatchCopy ) 
	{
		pDlgBatchCopy->DestroyWindow();
		delete pDlgBatchCopy;
		pDlgBatchCopy = NULL;
	}
	if( !pDlgBatchCopy && !createBatchCopyDialog(id,strLeft,strMid,strRight) ) 
	{
		//acutPrintf( _T("Failed to create modeless dialog box.\n") );
	}
	else
	{
		pDlgBatchCopy->OnBegin();
	}
}
void destroyDlgBatchCopy()
{
	if( pDlgBatchCopy )
	{
		pDlgBatchCopy->DestroyWindow();
		delete pDlgBatchCopy;
		pDlgBatchCopy = NULL;
	}
}
#include "GTextBrushDialog.h"
//刷文字格式
Acad::ErrorStatus BrushText(AcDbText* pText,GTextBrushState& state,AcDbText* pFormatText)
{
	if(state.bContent)
	{
		TCHAR* lpText=pFormatText->textString();
		pText->setTextString(lpText);
		acutDelString(lpText);
	}
	if(state.bColor)
	{
		pText->setColorIndex(pFormatText->colorIndex());
	}
	if(state.bFontHeight)
	{
		pText->setHeight(pFormatText->height());
	}
	if(state.bAlign)
	{
		pText->setHorizontalMode(pFormatText->horizontalMode());
		pText->setVerticalMode(pFormatText->verticalMode());
	}
	if(state.bLayer)
	{
		pText->setLayer(pFormatText->layerId());
	}
	if(state.bScale)
	{
		//比较复杂，下一步做
	}
	if(state.bStyle)
	{
		pText->setTextStyle(pFormatText->textStyle());
	}
	return Acad::eOk;
}
Acad::ErrorStatus BrushText(AcDbText* pText,GTextBrushState& state,AcDbMText* pFormatText)
{
	if(state.bContent)
	{
		pText->setTextString(GCADUtils::GetMText(pFormatText));
	}
	if(state.bColor)
	{
		pText->setColorIndex(pFormatText->colorIndex());
	}
	if(state.bFontHeight)
	{
		pText->setHeight(pFormatText->textHeight());
	}
	if(state.bAlign)
	{
		AcDb::TextHorzMode hMode;
		AcDb::TextVertMode vMode;
		SameAlign(hMode,vMode,pFormatText->attachment());
		pText->setHorizontalMode(hMode);
		pText->setVerticalMode(vMode);
	}
	if(state.bLayer)
	{
		pText->setLayer(pFormatText->layerId());
	}
	if(state.bScale)
	{
		//比较复杂，下一步做
	}
	if(state.bStyle)
	{
		pText->setTextStyle(pFormatText->textStyle());
	}
	return Acad::eOk;
}
Acad::ErrorStatus BrushText(AcDbText* pText,GTextBrushState& state,AcDbEntity* pFormatText)
{
	if(pFormatText->isKindOf(AcDbText::desc()))
	{
		return BrushText(pText,state,AcDbText::cast(pFormatText));
	}
	else if(pFormatText->isKindOf(AcDbMText::desc()))
	{
		return BrushText(pText,state,AcDbMText::cast(pFormatText));
	}
	else
	{
		return Acad::eNotThatKindOfClass;
	}
}


Acad::ErrorStatus BrushText(AcDbMText* pText,GTextBrushState& state,AcDbText* pFormatText)
{
	if(state.bContent)
	{
		TCHAR* lpText=pFormatText->textString();
		pText->setContents(lpText);
		acutDelString(lpText);
	}
	if(state.bColor)
	{
		pText->setColorIndex(pFormatText->colorIndex());
	}
	if(state.bFontHeight)
	{
		pText->setTextHeight(pFormatText->height());
	}
	if(state.bAlign)
	{
		AcDbMText::AttachmentPoint attachmentPoint=SameAlign(pFormatText->horizontalMode(),pFormatText->verticalMode());
		pText->setAttachment(attachmentPoint);
	}
	if(state.bLayer)
	{
		pText->setLayer(pFormatText->layerId());
	}
	if(state.bScale)
	{
		//比较复杂，下一步做
	}
	if(state.bStyle)
	{
		pText->setTextStyle(pFormatText->textStyle());
	}
	return Acad::eOk;
}
Acad::ErrorStatus BrushText(AcDbMText* pText,GTextBrushState& state,AcDbMText* pFormatText)
{
	if(state.bContent)
	{
		TCHAR* lpText=pFormatText->contents();
		pText->setContents(lpText);
		acutDelString(lpText);
	}
	if(state.bColor)
	{
		pText->setColorIndex(pFormatText->colorIndex());
	}
	if(state.bFontHeight)
	{
		pText->setTextHeight(pFormatText->textHeight());
	}
	if(state.bAlign)
	{
		pText->setAttachment(pFormatText->attachment());
	}
	if(state.bLayer)
	{
		pText->setLayer(pFormatText->layerId());
	}
	if(state.bScale)
	{
		//比较复杂，下一步做
	}
	if(state.bStyle)
	{
		pText->setTextStyle(pFormatText->textStyle());
	}
	return Acad::eOk;
}
Acad::ErrorStatus BrushText(AcDbMText* pText,GTextBrushState& state,AcDbEntity* pFormatText)
{
	if(pFormatText->isKindOf(AcDbText::desc()))
	{
		return BrushText(pText,state,AcDbText::cast(pFormatText));
	}
	else if(pFormatText->isKindOf(AcDbMText::desc()))
	{
		return BrushText(pText,state,AcDbMText::cast(pFormatText));
	}
	else
	{
		return Acad::eNotThatKindOfClass;
	}
}

Acad::ErrorStatus BrushText(AcDbEntity* pText,GTextBrushState& state,AcDbEntity* pFormatText)
{
	if(pText->isKindOf(AcDbText::desc()))
	{
		return BrushText(AcDbText::cast(pText),state,pFormatText);
	}
	else if(pText->isKindOf(AcDbMText::desc()))
	{
		return BrushText(AcDbMText::cast(pText),state,pFormatText);
	}
	else
	{
		return Acad::eNotThatKindOfClass;
	}
}
Acad::ErrorStatus BrushText(AcDbObjectId textId,GTextBrushState& state,AcDbObjectId formatTextId)
{
	AcDbEntityPointer spText(textId,AcDb::kForWrite);
	if(spText.openStatus()!=Acad::eOk)
	{
		return spText.openStatus();
	}
	AcDbEntityPointer spFormatText(formatTextId,AcDb::kForRead);
	if(spFormatText.openStatus()!=Acad::eOk)
	{
		return spFormatText.openStatus();
	}
	return BrushText(spText.object(),state,spFormatText.object());
}
resbuf* ssCallback(const TCHAR* kword)
{
	CAcModuleResourceOverride res;
	GTextBrushDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();
	return NULL;
}
//文字刷
void TextBrush()
{
	GTextBrushState state;
	state.setState(GUserDataRecord::getInstance()->GetProfileInt(_T("GTextBrushDialog"),_T("State"),0x80));
	CString strInfo;
	if(state.bContent)
	{
		strInfo+=_T("内容、");
	}
	if(state.bColor)
	{
		strInfo+=_T("颜色、");
	}
	if(state.bFontHeight)
	{
		strInfo+=_T("文字高度、");
	}
	if(state.bAngle)
	{
		strInfo+=_T("角度、");
	}
	if(state.bAlign)
	{
		strInfo+=_T("对齐、");
	}
	if(state.bLayer)
	{
		strInfo+=_T("图层、");
	}
	if(state.bStyle)
	{
		strInfo+=_T("字体、");
	}
	if(state.bScale)
	{
		strInfo+=_T("宽高比、");
	}
	if(!strInfo.IsEmpty())
	{
		strInfo=strInfo.Left(strInfo.GetLength()-1);
	}
	acutPrintf(_T("\n当前设置:%s\n"),strInfo);
	resbuf* (*oldFunc) (const ACHAR*);
	acedSSGetKwordCallbackPtr(&oldFunc);
	acedSSSetKwordCallbackPtr(ssCallback);
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择样本文字[设置(S)]:"),_T("\n移除样本文字:")};
	acedInitGet(RSG_NONULL,_T("S"));
	ACHAR kwordlist[] = { _T("S _ S") };
	int rc=acedSSGet(_T(":$:S:K"), promptPtrs,kwordlist,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		acedSSSetKwordCallbackPtr(*oldFunc);
		return;
	}
	acutRelRb(rb);
	acedSSSetKwordCallbackPtr(*oldFunc);

	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	state.setState(GUserDataRecord::getInstance()->GetProfileInt(_T("GTextBrushDialog"),_T("State"),0x80));
	while(true)
	{
		resbuf* rb1 = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
		ads_name ssName1;
		acutPrintf(_T("\n请选择匹配文字:\n"));
		TCHAR* promptPtrs1[2]={_T("\n选择匹配文字:"),_T("\n移除匹配文字:")};
		rc=acedSSGet(_T(":$"), promptPtrs1,NULL,rb1,ssName1);
		if(rc!=RTNORM)
		{
			acutRelRb(rb1);
			return;
		}
		acutRelRb(rb1);
		AcDbObjectIdArray ids1;
		SSToIds(ids1,ssName1);
		acedSSFree(ssName1);
		for(long i=0;i<ids1.logicalLength();i++)
		{
			AcDbObjectId textId=ids1.at(i);
			if(textId==ids.at(0))
			{
				continue;
			}
			BrushText(textId,state,ids.at(0));

		}
	}


}
AcDbText* BrefAttrToText(AcDbAttribute* pAttr)
{
	//isConstant，不变的，恒定的，经常的
	//isVerifiable，可检验的
	//isPreset，预置的
	/*
	AcGePoint3d position=pAttr->position();
	AcGePoint3d alignmentPoint=pAttr->alignmentPoint();
	GLog::WriteLine(_T("属性转文字:isConstant=%s,isVerifiable=%s,isPreset=%s,Position=(%5.2f,%5.2f,%5.2f),alignmentPoint=(%5.2f,%5.2f,%5.2f)"),
	pAttr->isConstant()?_T("true"):_T("false"),
	pAttr->isVerifiable()?_T("true"):_T("false"),
	pAttr->isPreset()?_T("true"):_T("false"),
	position.x,position.y,position.z,
	alignmentPoint.x,alignmentPoint.y,alignmentPoint.z);
	*/
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
#include "GTextDialog.h"
#include "GMTextDialog.h"
//连续修改(这个功能延后)
void ContinueModify()
{
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	for(long i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForRead);
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(spEnt.object());
			CAcModuleResourceOverride res;
			GTextDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
			dlg.m_strText=GCADUtils::GetText(pText);
			if(dlg.DoModal()!=IDOK)
				continue;
			if(pText->upgradeOpen()!=Acad::eOk)
			{
				continue;
			}
			pText->setTextString(dlg.m_strText);
			pText->recordGraphicsModified();
			//刷新
			actrTransactionManager->queueForGraphicsFlush();
			actrTransactionManager->flushGraphics();
			acedUpdateDisplay();
		}
		else if(spEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(spEnt.object());
			CAcModuleResourceOverride res;
			GTextDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
			dlg.m_strText=GCADUtils::GetText(pMText);
			if(dlg.DoModal()!=IDOK)
				continue;
			if(pMText->upgradeOpen()!=Acad::eOk)
			{
				continue;
			}
			pMText->recordGraphicsModified();
			pMText->setContents(dlg.m_strText);
			//刷新
			actrTransactionManager->queueForGraphicsFlush();
			actrTransactionManager->flushGraphics();
			acedUpdateDisplay();
		}
	}

}
#include "GDateTimeDialog.h"
//日期时间
void AssDateTime()
{
	AcDbObjectId   textLayerId;
	Acad::ErrorStatus es=UpdateLayerByConfig(textLayerId,_T("文字"),true);
	if(es!=Acad::eOk)
	{
		acutPrintf(_T("\n 设置[文字]图层失败:%s"),acadErrorStatusTextChs(es));
		return;
	}
	int nHTBL=1;
	int nCTBL=1;
	GCADUtils::GetScale(nHTBL,nCTBL);
	CAcModuleResourceOverride res;
	GDateTimeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
		return;
	ads_point p1;
	if (acedGetPoint(NULL, _T("\n请选择插入点: "), p1) != RTNORM)
	{
		return;
	}
	AcDbText* pText=new AcDbText;
	pText->setLayer(textLayerId);
	pText->setHeight(pText->height()*nHTBL);
	pText->setPosition(GCADUtils::UCS2WCS(AcGePoint3d(p1[0],p1[1],p1[2])));
	pText->setTextString(dlg.m_str);
	GCADUtils::AddToCurrentSpace(pText);
	pText->close();
}
#include "..\\CADGlobalTools\\AcDbEntityPointerEx.h"
//清除格式
void ClearFormat()
{
	if(!CheckReg(_T("20131029")))
	{
		return;
	}
	resbuf* rb = acutBuildList(RTDXF0, _T("MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择多行文字:"),_T("\n移除多行文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	for(long i=0;i<ids.logicalLength();i++)
	{
		AcDbEntityPointerEx<AcDbMText> spEnt(ids.at(i),AcDb::kForWrite,false,true);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开多行文字失败:%s"),acadErrorStatusTextChs(spEnt.openStatus()));
			continue;
		}
		MTextItemArray itemArr;
		TCHAR* lpStr=spEnt->contents();
		itemArr.item.str=lpStr;
		itemArr.item.nType=-1;
		itemArr.updateChildren();
		spEnt->setContents(itemArr.getTextCAD());
	}
}
//89:拷贝文字内容
void CopyText()
{
	if(!CheckReg(_T("20140210")))
	{
		return;
	}
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()!=1)
		return;
	AcDbObjectId id=ids.at(0);
	AcDbEntityPointer spEnt(id,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		acutPrintf(_T("\r\n复制失败，打开文字失败，失败原因:%s\r\n"),acadErrorStatusTextChs(spEnt.openStatus()));
		return;
	}
	if(spEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=AcDbText::cast(spEnt.object());
		TCHAR* lpStr=pText->textString();
		CopyTextToClipboard(lpStr);
		acutDelString(lpStr);
		acutPrintf(_T("\r\n拷贝成功\r\n"));
	}
	else
	{
		AcDbMText* pMText=AcDbMText::cast(spEnt.object());
		MTextItemArray itemArr;
		TCHAR* lpStr=pMText->contents();
		itemArr.item.str=lpStr;
		itemArr.item.nType=-1;
		itemArr.updateChildren();
		CopyTextToClipboard(itemArr.getText());
		acutPrintf(_T("\r\n拷贝成功\r\n"));
	}
}
