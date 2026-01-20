#include "StdAfx.h"
#include "BlockCommCommand.h"
#include "GModifyBlockNameDialog.h"
#include "..\\CADGlobalTools\\AcDbEntityPointerEx.h"
//功能:	将点从块引用外面经过矩阵转换转换到块里面
//参数:	pRef,块引用
//      pt,点
//返回:	转换结果
Acad::ErrorStatus TransPointIntoBlockRef(AcDbBlockReference* pRef,AcGePoint3d& pt)
{
	if(pRef==NULL)
	{
		return eNullHandle;
	}
	AcDbObjectPointer<AcDbBlockTableRecord> spRec(pRef->blockTableRecord(),AcDb::kForRead);
	if(spRec.openStatus()!=Acad::eOk)
	{
		return spRec.openStatus();
	}
	AcGePoint3d       originPt=spRec->origin();//定义的基点
#if _MSC_VER < 1400
	spRec->close();
#else
	spRec.close();
#endif
	double  scaleX=pRef->scaleFactors().sx;
	double  scaleY=pRef->scaleFactors().sy;
	AcGePoint3d insertPt=pRef->position();
	double dx=(pt.x-insertPt.x)/scaleX+originPt.x;
	double dy=(pt.y-insertPt.y)/scaleY+originPt.y;
	pt.set(dx,dy,pt.z);
	return eOk;
}
CString GetBlockName(AcDbBlockTableRecord* pRec)
{
	if (pRec == NULL)
	{
		return _T("");
	}
	const ACHAR* pName;
	pRec->getName(pName);
	return CString(pName);
}
CString GetBlockName(AcDbBlockReference* pRef)
{
	if (pRef == NULL)
	{
		return _T("");
	}
	AcDbObjectId id = pRef->blockTableRecord();
	AcDbBlockTableRecord* pRec = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)pRec, id, AcDb::kForRead);
	if (es == Acad::eOk)
	{
		pRec->close();
		return GetBlockName(pRec);
	}
	return _T("");
}

CString GetBlockName(AcDbObjectId objId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,objId,AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return _T("");
	}
	CString strName=_T("");
	if(pObj->isKindOf(AcDbBlockTableRecord::desc()))
	{
		strName=GetBlockName(AcDbBlockTableRecord::cast(pObj));
	}
	else if(pObj->isKindOf(AcDbBlockReference::desc()))
	{
		strName=GetBlockName(AcDbBlockReference::cast(pObj));
	}
	pObj->close();
	return strName;
}
//改块基点
void BlockBasePt()
{
	if(!CheckReg())
	{
		return;
	}
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("INSERT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择块:"),_T("\n移除块:")};
	int rc=acedSSGet(_T(":S:$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	for(int ii=0;ii<ids1.logicalLength();ii++)
	{
		AcDbObjectId id=ids1.at(ii);
		AcDbObjectPointer<AcDbBlockReference> spRef(id,AcDb::kForRead);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		// 输入第二点
		CString strPrompt;
		strPrompt.Format(_T("\n 请指定新的基点或者指定基点位置[横向 L:左边(默认)|M:居中|R:右边][竖向 B:底部(默认)|C:居中|T:顶部]"));

		acedInitGet(INP_NNEG | RSG_OTHER, _T("L M R B C T LB LC LT MB MC MT RB RC RT BL BM BR CL CM CR TL TM TR"));
		ads_point p2;
		CString strInput;
		switch (acedGetPoint(NULL, strPrompt, p2))
		{
		case RTNORM:
			break;
		case RTKWORD:
			{
				TCHAR szInput[128] = {0};
				acedGetInput(szInput);
				strInput = szInput;
			}
			break;
		default:
			return;
		}
		AcGePoint3d pt;
		if(strInput.IsEmpty())
		{
			GCADUtils::UCS2WCS(pt);
			pt.x=p2[X];
			pt.y=p2[Y];
			pt.z=p2[Z];
			TransPointIntoBlockRef(spRef.object(),pt);
		}
		else
		{
			AcDbBlockTableRecordPointer spRec(spRef->blockTableRecord(),AcDb::kForRead);
			if(spRec.openStatus()!=Acad::eOk)
			{
				continue;
			}
			AcDbObjectIdArray ids2;
			EnumAllEntityInBTR(ids2,spRec.object());
			bool bValidExt=false;
			AcDbExtents ext;
			AcDbObjectIdArray ids1;
			for(long i=0;i<ids2.logicalLength();i++)
			{
				AcDbEntityPointer spEnt1(ids2.at(i),AcDb::kForRead);
				if(spEnt1.openStatus()!=Acad::eOk)
				{
					continue;
				}
				ids1.append(ids2.at(i));
				AcDbExtents tmp_ext;
				if(spEnt1->getGeomExtents(tmp_ext)==Acad::eOk)
				{
					bValidExt=true;
					ext.addExt(tmp_ext);
				}
			}
			if(!bValidExt)
			{
				ext.set(AcGePoint3d(0,0,0),AcGePoint3d(0,0,0));
			}
			if(strInput.Find(_T('M'))>=0)
			{
				pt.x=0.5*(ext.minPoint().x+ext.maxPoint().x);
			}
			else if(strInput.Find(_T('R'))>=0)
			{
				pt.x=ext.maxPoint().x;
			}
			else
			{
				pt.x=ext.minPoint().x;
			}
			if(strInput.Find(_T('C'))>=0)
			{
				pt.y=0.5*(ext.minPoint().y+ext.maxPoint().y);
			}
			else if(strInput.Find(_T('T'))>=0)
			{
				pt.y=ext.maxPoint().y;
			}
			else
			{
				pt.y=ext.minPoint().y;
			}

		}
		AcDbBlockTableRecordPointer spRec1(spRef->blockTableRecord(),AcDb::kForWrite);
		spRef->close();
		if(spRec1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		AcDbObjectIdArray ids3;
		EnumAllEntityInBTR(ids3,spRec1.object());
		AcGeVector3d vec=pt-spRec1->origin();
		spRec1->setOrigin(pt);
		
		AcDbObjectIdArray refIds;
		spRec1->getBlockReferenceIds(refIds);
		spRec1->close();
		AcGeMatrix3d mat;
		mat.setToTranslation(vec);
		for(int j=0;j<refIds.logicalLength();j++)
		{
			AcDbEntityPointerEx<AcDbBlockReference> spRef2(refIds.at(j),AcDb::kForWrite,false,true);
			if(spRef2.openStatus()!=Acad::eOk)
				continue;
			AcGeMatrix3d mat1;
			AcGeVector3d vec1=vec;
			vec1*=spRef2->scaleFactors().sx;
			mat1.setToTranslation(vec1);
			spRef2->transformBy(mat1);
			spRef2->recordGraphicsModified();
			spRef2->draw();
		}
	}
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}
//获取所有的块名称
Acad::ErrorStatus GetAllBlcokNames(CStringArray& strNameArr,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase())
{
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbObjectPointer<AcDbBlockTable> spBTR(pDb->blockTableId(),AcDb::kForRead);
	Acad::ErrorStatus es=spBTR.openStatus();
	if(es!=eOk)
		return es;
	AcDbBlockTableIterator* pIterator=NULL;
	spBTR->newIterator(pIterator);
	if(pIterator==NULL)
	{
		return Acad::eNullHandle;
	}
	for(pIterator->start();(!pIterator->done());pIterator->step())
	{
		AcDbBlockTableRecord* pBTR=NULL;
		if(pIterator->getRecord(pBTR,AcDb::kForRead)!=Acad::eOk)
			continue;
		if((!pBTR->isLayout())&&//不是布局
			(!pBTR->isAnonymous()))//不是匿名块
		{
			const TCHAR* lpName=NULL;
			pBTR->getName(lpName);
			strNameArr.Add(lpName);
		}
		pBTR->close();
		pBTR=NULL;

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
//改块名称
void ModifyBlcokName()
{
	if(!CheckReg())
	{
		return;
	}
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("INSERT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择块:"),_T("\n移除块:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	for(int ii=0;ii<ids1.logicalLength();ii++)
	{
		AcDbObjectId id=ids1.at(ii);
		AcDbObjectPointer<AcDbBlockReference> spRef(id,AcDb::kForRead);
		if(spRef.openStatus()!=Acad::eOk)
		{
			continue;
		}
		CString strName=GetBlockName(spRef.object());
		CAcModuleResourceOverride res;
		GModifyBlockNameDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
		GetAllBlcokNames(dlg.m_strBlockNameArr);
		dlg.m_strBlcokName=strName;
		if(dlg.DoModal()!=IDOK)
		{
			continue;
		}
		AcDbBlockTableRecordPointer spRec(spRef->blockTableRecord(),AcDb::kForWrite);
		spRec->setName(dlg.m_strBlcokName);
	}
}
//是否有效合理的块名称
BOOL IsValidBlockName(CString strName,CString& strError)
{
	if(strName.IsEmpty())
	{
		strError=_T("块名称不可以为空");
		return FALSE;
	}
	int nIndex=strName.FindOneOf(_T("<>/\\\":;?*|,=`"));
	if(nIndex>=0)
	{
		strError=_T("块名中包含无效字符。\r\n块名中不能使用下列字符:\r\n<>/\\\":;?*|,=`");
		return FALSE;
	}
	return TRUE;
}
//快速定义块
#include "GBlockDefineDialog.h"
void QuickBlockDefine()
{
	if(!CheckReg())
	{
		return;
	}
	AcDbObjectIdArray EntIds;

	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择定义块的实体:"),_T("\n移除定义块的实体:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
		return;
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	TCHAR result[512]={0};
	CAcModuleResourceOverride res;
	GBlockDefineDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	GetAllBlcokNames(dlg.m_strBlockNameArr);
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	lstrcpy(result,dlg.m_strBlcokName);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pTable = NULL;
	Acad::ErrorStatus es = pDb->getBlockTable(pTable, AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return;
	}
	AcDbObjectPointer<AcDbBlockTable> spTable;
	spTable.acquire(pTable);
	AcDbObjectId recordId;
	if(spTable->has(result))
	{
		//删除原有的对象
		AcDbBlockTableRecord* pRec=NULL;
		es=spTable->getAt(result,pRec,AcDb::kForRead);
		if(es!=eOk)
		{
			return;
		}
		AcDbObjectPointer<AcDbBlockTableRecord> spRec;
		spRec.acquire(pRec);
		recordId=spRec->objectId();
		AcDbBlockTableRecordIterator* pIterator=NULL;
		es=spRec->newIterator(pIterator);
		if(es!=eOk)
		{
			return;
		}
		std::auto_ptr<AcDbBlockTableRecordIterator> spIterator(pIterator);
		for(spIterator->start();!spIterator->done();spIterator->step())
		{
			AcDbEntity* pEntity=NULL;
			es=spIterator->getEntity(pEntity,AcDb::kForWrite);
			if(es!=eOk)
			{
				continue;
			}
			AcDbEntityPointer spEnt;
			spEnt.acquire(pEntity);
			spEnt->erase();
		}
	}
	else
	{
		AcDbBlockTableRecord* pRec = new AcDbBlockTableRecord();
		es=pRec->setName(result);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return;
		}
		es=spTable->add(recordId,pRec);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return;
		}
		pRec->close();
	}
	bool bValidExt=false;
	AcDbExtents ext;
	AcDbObjectIdArray ids1;
	for(long i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointer spEnt1(EntIds.at(i),AcDb::kForRead);
		if(spEnt1.openStatus()!=Acad::eOk)
		{
			continue;
		}
		ids1.append(EntIds.at(i));
		AcDbExtents tmp_ext;
		if(spEnt1->getGeomExtents(tmp_ext)==Acad::eOk)
		{
			bValidExt=true;
			ext.addExt(tmp_ext);
		}
	}
	if(!bValidExt)
	{
		ext.set(AcGePoint3d(0,0,0),AcGePoint3d(0,0,0));
	}
	AcGePoint3d ptCenter(0.0,0.0,0.0);
	if(bValidExt)
	{
		if(dlg.m_nHor==1)
		{
			ptCenter.x=0.5*(ext.minPoint().x+ext.maxPoint().x);
		}
		else if(dlg.m_nHor==2)
		{
			ptCenter.x=ext.maxPoint().x;
		}
		else
		{
			ptCenter.x=ext.minPoint().x;
		}
		if(dlg.m_nVer==1)
		{
			ptCenter.y=0.5*(ext.minPoint().y+ext.maxPoint().y);
		}
		else if(dlg.m_nVer==2)
		{
			ptCenter.y=ext.maxPoint().y;
		}
		else
		{
			ptCenter.y=ext.minPoint().y;
		}
		AcDbObjectPointer<AcDbBlockTableRecord> spRec(recordId,AcDb::kForWrite);
		if(spRec.openStatus()==Acad::eOk)
		{
			spRec->setOrigin(ptCenter);
		}
	}
	AcDbIdMapping idMap;
	es=pDb->deepCloneObjects(ids1,recordId,idMap);
	if(dlg.m_bOrigin)
	{
		AcGeMatrix3d xform;
		xform.setToTranslation(AcGePoint3d(0,0,0)-ptCenter);
		AcDbObjectPointer<AcDbBlockTableRecord> spRec(recordId,AcDb::kForWrite);
		if(spRec.openStatus()==Acad::eOk)
		{
			AcDbBlockTableRecordIterator* pIterator=NULL;
			if(Acad::eOk!=spRec->newIterator(pIterator))
			{
				return;
			}
			for(;!pIterator->done();pIterator->step())
			{
				AcDbObjectId id;
				if(Acad::eOk!=pIterator->getEntityId(id))
				{
					continue;
				}
				AcDbEntityPointerEx<AcDbEntity> spEnt(id,AcDb::kForWrite,false,true);
				if(Acad::eOk!=spEnt.openStatus())
				{
					continue;
				}
				spEnt->transformBy(xform);

			}
			delete pIterator;
			pIterator=NULL;
			spRec->setOrigin(AcGePoint3d(0,0,0));

		}
	}
	//插入一个块参照
	AcDbBlockReference *pBlkRef=new AcDbBlockReference;
	pBlkRef->setBlockTableRecord(recordId);//指定所引用的图块表记录的对象ID
	resbuf to,from;
	from.restype=RTSHORT;//插入图块要进行用户坐标与世界坐标的转换
	from.resval.rint=1;//UCS
	to.restype=RTSHORT;
	to.resval.rint=0;//WCS
	AcGeVector3d normal(0.0,0.0,1.0);
	acedTrans(&(normal.x),&from,&to,Adesk::kTrue,&(normal.x));//转换函数
	pBlkRef->setPosition(ptCenter); //指定基点
	pBlkRef->setRotation(0.0);//旋转角度
	pBlkRef->setNormal(normal);
	pBlkRef->setDatabaseDefaults(pDb);

	AcGeScale3d dGeScale(1.0,1.0,1.0);
	pBlkRef->setScaleFactors(dGeScale);//放大比例

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk == acdbOpenObject(pBlockTableRecord, pDb->currentSpaceId(), AcDb::kForWrite))
	{
		AcDbObjectId resultId;
		pBlockTableRecord->appendAcDbEntity(resultId,pBlkRef);
		pBlkRef->close();
		pBlockTableRecord->close();
	}
	else
	{
		delete pBlkRef;
		pBlkRef=NULL;
	}
	//删除旧的实体对象
	for(long i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointer spEnt2(EntIds.at(i),AcDb::kForWrite);
		if(spEnt2.openStatus()!=Acad::eOk)
		{
			continue;
		}
		spEnt2->erase();
	}

}
#include "GTextBlockDialog.h"
#include "GSameNameBlockDialog.h"
Acad::ErrorStatus _TextBlock(AcDbObjectId TextId,AcDbObjectId recordId,GTextBlockDialog* pDlg)
{
	AcDbObjectIdArray ids1;
	ids1.append(TextId);
	AcDbIdMapping idMap;
	Acad::ErrorStatus es=TextId.database()->deepCloneObjects(ids1,recordId,idMap);
	return es;
}
Acad::ErrorStatus _TextBlock(AcDbObjectId TextId,GTextBlockDialog* pDlg,int& SameNameMode)
{
	AcDbEntityPointer spEnt(TextId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return spEnt.openStatus();
	}
	CString strText;
	AcGePoint3d pt;
	if(spEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText* pText=AcDbText::cast(spEnt.object());
		TCHAR* lpStr=pText->textString();
		strText=lpStr;
		acutDelString(lpStr);
		pt=pText->position();
	}
	else
	{
		AcDbMText* pMText=AcDbMText::cast(spEnt.object());
		strText=GCADUtils::GetMText(pMText);
		pt=pMText->location();
	}
	spEnt->close();
	if(strText.IsEmpty())
	{
		return Acad::eInvalidInput;
	}
	TCHAR result[512]={0};
	lstrcpy(result,strText);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pTable = NULL;
	Acad::ErrorStatus es = pDb->getBlockTable(pTable, AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	AcDbObjectPointer<AcDbBlockTable> spTable;
	spTable.acquire(pTable);
	AcDbObjectId recordId;
	AcGePoint3d pt1=pt;
	pt1.x+=pDlg->m_dX;
	pt1.y+=pDlg->m_dY;
	AcGePoint3d pt2=pt;
	pt2.x+=pDlg->m_dX;
	pt2.y+=pDlg->m_dY;
	if(spTable->has(result))
	{
		int _nSameNameMode=0;
		if(SameNameMode>0)
		{
			_nSameNameMode=SameNameMode;
		}
		else
		{
			CAcModuleResourceOverride res;
			GSameNameBlockDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
			if(dlg.DoModal()!=IDOK)
			{
				return Acad::eInvalidInput;
			}
			if(dlg.m_bSameHandle)
			{
				SameNameMode=dlg.m_nMode;
			}
			_nSameNameMode=dlg.m_nMode;
		}
		//1:不生成块 2:生成相同的块 3:生成不同名称的块
		if(_nSameNameMode==2)
		{
			spTable->getAt(result,recordId);
		}
		else if(_nSameNameMode==3)
		{
			for(long i=1;i<500;i++)
			{
				//不可能有500页的目录，因为那得画500*25=12500张图
				CString name;
				name.Format(_T("%s%d"),strText, i);
				if(!spTable->has(name))
				{
					strText=name;
					break;
				}
			}
			lstrcpy(result,strText);
			AcDbBlockTableRecord* pRec = new AcDbBlockTableRecord();
			es=pRec->setName(result);
			if(es!=eOk)
			{
				delete pRec;
				pRec=NULL;
				return es;
			}
			pRec->setOrigin(pt1);
			es=spTable->add(recordId,pRec);
			if(es!=eOk)
			{
				delete pRec;
				pRec=NULL;
				return es;
			}
			pRec->close();
			spTable->close();
			_TextBlock(TextId,recordId,pDlg);
		}
		else
		{
			//不生成块
			return Acad::eOk;
		}
		
	}
	else
	{
		AcDbBlockTableRecord* pRec = new AcDbBlockTableRecord();
		es=pRec->setName(result);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		pRec->setOrigin(pt1);
		es=spTable->add(recordId,pRec);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		pRec->close();
		spTable->close();
		_TextBlock(TextId,recordId,pDlg);
	}
	//插入一个块参照
	AcDbBlockReference *pBlkRef=new AcDbBlockReference;
	pBlkRef->setBlockTableRecord(recordId);//指定所引用的图块表记录的对象ID
	resbuf to,from;
	from.restype=RTSHORT;//插入图块要进行用户坐标与世界坐标的转换
	from.resval.rint=1;//UCS
	to.restype=RTSHORT;
	to.resval.rint=0;//WCS
	AcGeVector3d normal(0.0,0.0,1.0);
	acedTrans(&(normal.x),&from,&to,Adesk::kTrue,&(normal.x));//转换函数
	pBlkRef->setPosition(pt2); //指定基点
	pBlkRef->setRotation(0.0);//旋转角度
	pBlkRef->setNormal(normal);
	pBlkRef->setDatabaseDefaults(pDb);

	AcGeScale3d dGeScale(1.0,1.0,1.0);
	pBlkRef->setScaleFactors(dGeScale);//放大比例

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk == acdbOpenObject(pBlockTableRecord, pDb->currentSpaceId(), AcDb::kForWrite))
	{
		AcDbObjectId resultId;
		pBlockTableRecord->appendAcDbEntity(resultId,pBlkRef);
		pBlkRef->close();
		pBlockTableRecord->close();
		AcDbEntityPointer spEnt2(TextId,AcDb::kForWrite);
		if(spEnt2.openStatus()==Acad::eOk)
		{
			spEnt2->erase();
		}
		
	}
	else
	{
		delete pBlkRef;
		pBlkRef=NULL;
	}
	return Acad::eOk;
}
//122:文字块
void TextBlock()
{
	CAcModuleResourceOverride res;
	GTextBlockDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	CString strType;
	if(dlg.m_nTextMode==1)
	{
		strType=_T("TEXT");
	}
	else if(dlg.m_nTextMode==2)
	{
		strType=_T("MTEXT");
	}
	else
	{
		strType=_T("TEXT,MTEXT");
	}
	AcDbObjectIdArray EntIds;
	resbuf* rb = acutBuildList(RTDXF0, strType.GetBuffer(), 0);
	strType.ReleaseBuffer();
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择定义块的文字/多行文字:"),_T("\n移除定义块的文字/多行文字:")};
	if(acedSSGet(_T(":$"),promptPtrs,NULL,rb,ssName)!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	SSToIds(EntIds,ssName);
	acedSSFree(ssName);
	int SameNameMode=0;
	for(int i=0;i<EntIds.logicalLength();i++)
	{
		_TextBlock(EntIds.at(i),&dlg,SameNameMode);
	}
}
