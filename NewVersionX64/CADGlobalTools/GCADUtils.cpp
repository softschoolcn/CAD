#include "StdAfx.h"
#include "GCADUtils.h"
Acad::ErrorStatus registerApp(AcDbDatabase* pDb, const TCHAR * pAppName)

{

	AcDbRegAppTable* pAppTable;

	Acad::ErrorStatus es;


	// get the RegAppTable

	if((es = pDb->getRegAppTable(pAppTable,AcDb::kForWrite)) != Acad::eOk)

		return es;


	// if RegAppTable has application already - fine

	if(pAppTable->has(pAppName))

	{

		pAppTable->close();

		return Acad::eOk;

	}


	AcDbRegAppTableRecord* pAppTableRecord = new AcDbRegAppTableRecord();

	pAppTableRecord->setName(pAppName);


	if((es = pAppTable->add(pAppTableRecord)) != Acad::eOk)

	{

		delete pAppTableRecord;

		pAppTable->close();

		return es;

	}

	pAppTableRecord->close();

	pAppTable->close();

	return es;

}
GCADUtils::GCADUtils(void)
{
}

GCADUtils::~GCADUtils(void)
{
}
//获取中点
AcGePoint3d GCADUtils::MidPoint(AcGePoint3d pt1,AcGePoint3d pt2)
{
	return AcGePoint3d(0.5*(pt1.x+pt2.x),0.5*(pt1.y+pt2.y),0.5*(pt1.z+pt2.z));
}
AcGePoint2d GCADUtils::MidPoint(AcGePoint2d pt1,AcGePoint2d pt2)
{
	return AcGePoint2d(0.5*(pt1.x+pt2.x),0.5*(pt1.y+pt2.y));
}
//获取文字内容
CString     GCADUtils::GetText(AcDbEntity* pEnt)
{
	if(pEnt->isKindOf(AcDbText::desc()))
	{
		return GetText(AcDbText::cast(pEnt));
	}
	else if(pEnt->isKindOf(AcDbMText::desc()))
	{
		return GetText(AcDbMText::cast(pEnt));
	}
	else
	{
		return _T("");
	}
}
CString     GCADUtils::GetText(AcDbText* pText)
{
	TCHAR* lpText=pText->textString();
	CString str=lpText;
	acutDelString(lpText);
	return str;
}
CString     GCADUtils::GetText(AcDbMText* pMText)
{
	TCHAR* lpText=pMText->contents();
	CString str=lpText;
	acutDelString(lpText);
	return str;
}
CString     GCADUtils::GetMText(AcDbMText* pMText)
{
	    MTextItemArray itemArr;
		TCHAR* lpStr=pMText->contents();
		itemArr.item.str=lpStr;
		itemArr.item.nType=-1;
		itemArr.updateChildren();
		return itemArr.getText();
}
// 功能：将选择集转换为实体ID数组
// 参数：	 ObjIds,实体ID数组
// ssName,选择集
// 返回：
void GCADUtils::SSToIds(AcDbObjectIdArray& ObjIds,ads_name ssName)
{
	AcDbObjectId ObjId;
	ads_name EntName;
#if _MSC_VER>=1900
	Adesk::Int32 nLength = 0;
#else
	long nLength=0;
#endif
	acedSSLength(ssName,&nLength);

	for(int nLen=0; nLen<nLength;nLen++) 
	{
		acedSSName(ssName,nLen,EntName);
		acdbGetObjectId(ObjId,EntName);
		ObjIds.append(ObjId);
	}
}
//枚举字体样式表
Acad::ErrorStatus GCADUtils::EnumTextStyles(AcDbObjectIdArray& ids,CStringArray& names,AcDbDatabase* pDb)
{
	if(pDb==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbObjectId id=pDb->textStyleTableId();
	AcDbTextStyleTablePointer spTable(id,AcDb::kForRead);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	AcDbTextStyleTableIterator* pIterator=NULL;
	Acad::ErrorStatus es=spTable->newIterator(pIterator);
	if(es!=Acad::eOk)
	{
		return es;
	}
	for(pIterator->start();!pIterator->done();pIterator->step())
	{
		AcDbTextStyleTableRecord* pRecord=NULL;
		es=pIterator->getRecord(pRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
		{
			continue;
		}
		ACHAR* pName=NULL;
		pRecord->getName(pName);
		names.Add(pName);
		acutDelString(pName);
		ids.append(pRecord->objectId());
		pRecord->close();

	}
	delete pIterator;
	pIterator=NULL;
	return Acad::eOk;
}
BOOL GCADUtils::GetCurDwgFileName(CString& strDwgFileName)
{
	if(curDoc()==NULL)
		return FALSE;
	if(PathFileExists(curDoc()->fileName()))
	{
		TCHAR lpFile[MAX_PATH];
		lstrcpy(lpFile,curDoc()->fileName());
		if(!PathRemoveFileSpec(lpFile))
		{
			return FALSE;
		}
		if(!PathFileExists(lpFile))
		{
			//寻找所在目录
			return FALSE;
		}
		strDwgFileName=curDoc()->fileName();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
Acad::ErrorStatus GCADUtils::MarkObject(AcDbObject* pObj,CString strKey,AcDbDatabase* pDb,bool bMark)
{
	if(pObj==NULL)
		return eNullHandle;
	struct  resbuf  *pHead=NULL, *pTemp=NULL;


	pHead = pObj->xData(strKey);
	if (NULL == pHead)
	{
		Acad::ErrorStatus es=registerApp(pObj->database()!=NULL?pObj->database():pDb,strKey);
		if(es!=eOk)
		{
			return es;
		}
	}

	pHead = acutNewRb(AcDb::kDxfRegAppName);
	pTemp = pHead;
#ifndef _UNICODE
	pTemp->resval.rstring = (char*) malloc(strlen(strKey) + 1);
	strcpy(pTemp->resval.rstring, strKey);
#else
	pTemp->resval.rstring = (TCHAR*) malloc(sizeof(TCHAR*)*(lstrlen(strKey) + 1));
	lstrcpy(pTemp->resval.rstring, strKey);
#endif


	CString strCode=bMark?_T("True"):_T("False");
	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;
#ifndef _UNICODE
	pTemp->resval.rstring = (char*) malloc(strCode.GetLength() + 1);
	strcpy(pTemp->resval.rstring, strCode);
#else
	pTemp->resval.rstring = (TCHAR*) malloc(sizeof(TCHAR*)*(strCode.GetLength() + 1));
	lstrcpy(pTemp->resval.rstring, strCode);
#endif
	pTemp->rbnext = NULL;

	Acad::ErrorStatus tt = pObj->setXData(pHead);
	if(tt != Acad::eOk)
	{
		acutPrintf(_T("\n写入扩展数据失败0:%s"),acadErrorStatusText(tt));
		tt =pObj->upgradeOpen();//从 Read 方式到 Write 方式
		if(tt != Acad::eOk)
		{
			acutPrintf(_T("\n写入扩展数据失败1:%s"),acadErrorStatusText(tt));
			acutRelRb(pHead);
			return tt;
		}
		tt =pObj->setXData(pHead);
		if(tt != Acad::eOk)
		{
			acutPrintf(_T("\n写入扩展数据失败2:%s"),acadErrorStatusText(tt));
			acutRelRb(pHead);
			return tt;
		}
	}
	acutRelRb(pHead);
	return eOk;
}
//给CAD中的对象做一个标记
Acad::ErrorStatus GCADUtils::MarkObject(AcDbObjectId objId,CString strKey,bool bMark)
{
	AcDbObjectPointer<AcDbObject> spObj(objId,AcDb::kForWrite);
	if(spObj.openStatus()!=Acad::eOk)
	{
		return spObj.openStatus();
	}
	return MarkObject(spObj.object(),strKey,NULL,bMark);
}
//是否是被标记的对象
Acad::ErrorStatus GCADUtils::IsMarkedObject(AcDbObject* pObj,CString strKey,bool& bMark)
{
	bMark=false;
	if(pObj==NULL)
		return eNullHandle;
	struct resbuf *pHead = NULL;
	pHead = pObj->xData(strKey);
	if (pHead != NULL)
	{
		int nCount=0;
		while(pHead != NULL)
		{
			nCount++;
			if(pHead->restype==AcDb::kDxfXdAsciiString)
			{
				if(lstrcmpi(_T("True"),pHead->resval.rstring)==0)
				{
					bMark=true;
				}
				else if(lstrcmpi(_T("False"),pHead->resval.rstring)==0)
				{
					bMark=false;
				}
			}
			pHead = pHead->rbnext;

		}
		acutRelRb(pHead);
		bMark=bMark&&(nCount>=2);
		return eOk;
	}
	else
	{
		bMark=false;
		return eOk;
	}
}
//是否是被标记的对象
Acad::ErrorStatus GCADUtils::IsMarkedObject(AcDbObjectId objId,CString strKey,bool& bMark)
{
	bMark=false;
	AcDbObjectPointer<AcDbObject> spObj(objId,AcDb::kForRead);
	if(spObj.openStatus()!=Acad::eOk)
	{
		return spObj.openStatus();
	}
	return IsMarkedObject(spObj.object(),strKey,bMark);
}
//点是否在范围内
bool GCADUtils::IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext)
{
	if(pt.x>ext.maxPoint().x)
		return false;
	if(pt.y>ext.maxPoint().y)
		return false;
	if(pt.x<ext.minPoint().x)
		return false;
	if(pt.y<ext.minPoint().y)
		return false;
	return true;
}
//范围1是否在范围2内
bool GCADUtils::IsExtInExtents(AcDbExtents& ext1,AcDbExtents& ext2)
{
	return IsPointInExtents(ext1.minPoint(),ext2)&&IsPointInExtents(ext1.maxPoint(),ext2);
}
//范围1和范围2是否相交
bool GCADUtils::IsExtIntersect(AcDbExtents& ext1,AcDbExtents& ext2)
{
	return IsPointInExtents(ext1.minPoint(),ext2)||IsPointInExtents(ext1.maxPoint(),ext2)||
		IsPointInExtents(AcGePoint3d(ext1.minPoint().x,ext1.maxPoint().y,0.0),ext2)||IsPointInExtents(AcGePoint3d(ext1.minPoint().y,ext1.maxPoint().x,0.0),ext2)||
		IsPointInExtents(ext2.minPoint(),ext1)||IsPointInExtents(ext2.maxPoint(),ext1)||
		IsPointInExtents(AcGePoint3d(ext2.minPoint().x,ext2.maxPoint().y,0.0),ext1)||IsPointInExtents(AcGePoint3d(ext2.minPoint().y,ext2.maxPoint().x,0.0),ext1);
}
//获取图层名称
Acad::ErrorStatus GCADUtils::GetLayerName(CString& strLayerName,AcDbObjectId layerId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord* pRec=AcDbLayerTableRecord::cast(pObj);
	if(pRec==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	const TCHAR* pName=NULL;
	es=pRec->getName(pName);
	if(es!=Acad::eOk)
	{
		pObj->close();
		return es;
	}
	pObj->close();
	strLayerName=CString(pName);
	return Acad::eOk;
}
/************************************************************************
**
**  dxfCodeToDataType:
**
**  **jma
**
******************************************/

int
GCADUtils::dxfCodeToDataType(int resType)
{
	// which data type is this value
	if ((resType >= 0) && (resType <= 9))
		return RTSTR;
	else if ((resType >= 10) && (resType <= 17))
		return RT3DPOINT;
	else if ((resType >= 38) && (resType <= 59))
		return RTREAL;
	else if ((resType >= 60) && (resType <= 79))
		return RTSHORT;
	else if ((resType >= 90) && (resType <= 99))
		return RTLONG;
	else if ((resType == 100) || (resType == 101) || (resType == 102) ||(resType == 105))
		return RTSTR;
	else if ((resType >= 110) && (resType <= 119))
		return RT3DPOINT;
	else if ((resType >= 140) && (resType <= 149))
		return RTREAL;
	else if ((resType >= 170) && (resType <= 179))
		return RTSHORT;
	else if ((resType >= 210) && (resType <= 219))
		return RT3DPOINT;
	else if ((resType >= 270) && (resType <= 299))
		return RTSHORT;
	else if ((resType >= 300) && (resType <= 309))
		return RTSTR;
	else if ((resType >= 310) && (resType <= 369))
		return RTENAME;
	else if ((resType >= 370) && (resType <= 379))
		return RTSHORT;
	else if ((resType >= 380) && (resType <= 389))
		return RTSHORT;
	else if ((resType >= 390) && (resType <= 399))
		return RTENAME;
	else if ((resType >= 400) && (resType <= 409))
		return RTSHORT;
	else if ((resType >= 410) && (resType <= 419))
		return RTSTR;
	else if (resType == 1004)
		return resType;        // binary chunk
	else if ((resType >= 999) && (resType <= 1009))
		return RTSTR;
	else if ((resType >= 1010) && (resType <= 1013))
		return RT3DPOINT;
	else if ((resType >= 1038) && (resType <= 1059))
		return RTREAL;
	else if ((resType >= 1060) && (resType <= 1070))
		return RTSHORT;
	else if ((resType == 1071))
		return RTLONG;
	else if ((resType < 0) || (resType > 4999))
		return resType;
	else
		return RTNONE;
}
/**********************************************************************
**
**  bytesToHexStr:
**
**  **jma
**
*****************************************/

// Unicode: Leaving as char type because it is a buffer
LPCTSTR
GCADUtils::bytesToHexStr(char* buffer, int len, CString& hexStr)
{
	// Unicode: Leaving as char type because it is a byte
	unsigned char k;
	int j;
	CString tmpStr;

	hexStr.Empty();    // make sure nothing in it yet since we are concatenating it

	for (j=0;j<len;j++) {
		k = static_cast<unsigned char>(buffer[j]);
		tmpStr.Format(_T("%02hX"), k);
		hexStr += tmpStr;
	}
	return hexStr;
}
/*********************************************************************
**
**  dxfToStr:
**
**  **jma
**
******************************************/

void
GCADUtils::dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr)
{
	int dataType = dxfCodeToDataType(rb->restype);
	dxfCodeStr.Format(_T("%d"), rb->restype);

	CString tmpStr;

	switch (dataType) {
		case RTSHORT:
			valueStr.Format(_T("%d"), rb->resval.rint);
			break;
		case RTLONG:
			valueStr.Format(_T("%ld"), rb->resval.rlong);
			break;
		case RTREAL:
			valueStr.Format(_T("%g"), rb->resval.rreal);
			break;
		case RTSTR:
			if (rb->resval.rstring == NULL)
				valueStr = _T("(NULL)");
			else
				valueStr.Format(_T("\"%s\""), rb->resval.rstring);
			break;
		case RT3DPOINT:
			valueStr.Format(_T("(%g, %g, %g)"), rb->resval.rpoint[X],
				rb->resval.rpoint[Y], rb->resval.rpoint[Z]);
			break;
		case RTPOINT:
			valueStr.Format(_T("(%g, %g)"), rb->resval.rpoint[X], rb->resval.rpoint[Y]);
			break;
		case 1004:
			valueStr.Format(_T("Binary Chunk: \"%s\""),
				bytesToHexStr(rb->resval.rbinary.buf,
				rb->resval.rbinary.clen, tmpStr));
			break;
		case -6:
			valueStr = _T("Extension Dictionary");
			break;
		case -5:
			valueStr = _T("Persistent Reactors");
			break;
		case -4:
			valueStr.Format(_T("Conditional Operator: \"%s\""), rb->resval.rstring);
			break;
		case -3:
			valueStr = _T("Start of Xdata");
			break;
		case -2:
			valueStr.Format(_T("<Entity Name Reference: %8lx>"), rb->resval.rlname[0]);
			break;
		case -1:
		case RTENAME:
			if ((rb->restype >= 330 )&& (rb->restype < 340))
				valueStr.Format(_T("<Soft Pointer: %8lx>"), rb->resval.rlname[0]);
			else if((rb->restype >= 340) && (rb->restype < 350))
				valueStr.Format(_T("<Hard Pointer: %8lx>"), rb->resval.rlname[0]);
			else if((rb->restype >= 350) && (rb->restype < 360))
				valueStr.Format(_T("<Soft Ownership: %8lx>"), rb->resval.rlname[0]);
			else if((rb->restype >= 360) && (rb->restype < 370))
				valueStr.Format(_T("<Hard Ownership: %8lx>"), rb->resval.rlname[0]);
			else if((rb->restype >= 390) && (rb->restype < 399))
				valueStr.Format(_T("<Hard Pointer: %8lx>"), rb->resval.rlname[0]);
			else
				valueStr.Format(_T("<Entity Name: %8lx>"), rb->resval.rlname[0]);
			break;
		case RTPICKS:
			valueStr.Format(_T("<Selection Set: %8lx>"), rb->resval.rlname[0]);
			break;
		case RTLB:
			valueStr = _T("List Begin");
			break;
		case RTLE:
			valueStr = _T("List End");
			break;
		case RTNIL:
			valueStr = _T("NIL");
			break;
		case RTT:
			valueStr = _T("T");
			break;
		default:
			valueStr = _T("*Unknown*");
			break;
	}
}
int GCADUtils::FindObjectId(CArray<AcDbObjectId>& xIdArr,AcDbObjectId id)
{
	for(int i=0;i<xIdArr.GetCount();i++)
	{
		if(id==xIdArr.GetAt(i))
		{
			return i;
		}
	}
	return -1;
}
int GCADUtils::FindObjectId(AcDbObjectIdArray& xIdArr,AcDbObjectId id)
{
	for(int i=0;i<xIdArr.logicalLength();i++)
	{
		if(id==xIdArr.at(i))
		{
			return i;
		}
	}
	return -1;
}
AcGePoint3d GCADUtils::WCS2UCS(AcGePoint3d wcsPnt)
{
	ads_point wcsPt = {wcsPnt.x, wcsPnt.y, wcsPnt.z};
	ads_point ucsPt;
	acdbWcs2Ucs(wcsPt, ucsPt, false);
	return AcGePoint3d(ucsPt[X], ucsPt[Y], ucsPt[Z]);
}
AcGePoint2d GCADUtils::WCS2UCS(AcGePoint2d wcsPnt)
{
	AcGePoint3d pt(wcsPnt.x,wcsPnt.y,0.0);
	AcGePoint3d pt1=WCS2UCS(pt);
	return AcGePoint2d(pt1.x,pt1.y);
}
AcGePoint3d GCADUtils::UCS2WCS(AcGePoint3d ucsPnt)
{
	ads_point ucsPt = {ucsPnt.x, ucsPnt.y, ucsPnt.z};
	ads_point wcsPt;
	acdbUcs2Wcs(ucsPt, wcsPt, false);
	return AcGePoint3d(wcsPt[X], wcsPt[Y], wcsPt[Z]);
}
void GCADUtils::UCS2WCS(ads_point ucsPt)
{
	ads_point wcsPt;
	acdbUcs2Wcs(ucsPt, wcsPt, false);
	ucsPt[X]=wcsPt[X];
	ucsPt[Y]=wcsPt[Y];
	ucsPt[Z]=wcsPt[Z];
}
void GCADUtils::WCS2UCS(ads_point wcsPt)
{
	ads_point ucsPt;
	acdbWcs2Ucs(wcsPt,ucsPt,  false);
	wcsPt[X]=ucsPt[X];
	wcsPt[Y]=ucsPt[Y];
	wcsPt[Z]=ucsPt[Z];
}
//功能： 设置int型环境变量
Acad::ErrorStatus GCADUtils::SetCADIntVar(CString sMode,int nRet)
{
	Acad::ErrorStatus es=Acad::eOk;

	resbuf *pNewRb=acutBuildList(RTSHORT,nRet,0);
	acedSetVar(sMode,pNewRb);
	acutRelRb(pNewRb);

	return es;
}
//功能： 获取int型环境变量
Acad::ErrorStatus GCADUtils::GetCADIntVar(CString sMode,int &nRet)
{
	Acad::ErrorStatus es=Acad::eOk;

	resbuf *pRb=acutBuildList(RTSHORT,23,0);
	acedGetVar(sMode,pRb);
	if(pRb==NULL)
	{
		acutRelRb(pRb);
		return Acad::eNullHandle;
	}
	nRet=pRb->resval.rint;
	acutRelRb(pRb);

	return es;
}
Acad::ErrorStatus GCADUtils::EraseEntity(const AcDbObjectId objId)
{
	AcDbEntity* pEnt=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt,objId,AcDb::kForWrite);
	if(es!=eOk)
		return es;
	es=pEnt->erase();
	pEnt->close();
	pEnt=NULL;
	return es;
}
Acad::ErrorStatus GCADUtils::UnLockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord *pLayerTableRecord=AcDbLayerTableRecord::cast(pObj);
	if(pLayerTableRecord==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	bLock=pLayerTableRecord->isLocked();
	bFrozen=pLayerTableRecord->isFrozen();
	es=pLayerTableRecord->upgradeOpen();
	if(es!=eOk)
	{
		pObj->close();
		return es;
	}
	pLayerTableRecord->setIsLocked(false);
	pLayerTableRecord->setIsFrozen(false);
	pObj->close();
	return Acad::eOk;
}
Acad::ErrorStatus GCADUtils::LockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,layerId,AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if(!pObj->isKindOf(AcDbLayerTableRecord::desc()))
	{
		pObj->close();
		return Acad::eNotThatKindOfClass;
	}
	AcDbLayerTableRecord *pLayerTableRecord=AcDbLayerTableRecord::cast(pObj);
	if(pLayerTableRecord==NULL)
	{
		pObj->close();
		return Acad::eNullHandle;
	}
	es=pLayerTableRecord->upgradeOpen();
	if(es!=eOk)
	{
		pObj->close();
		return es;
	}
	pLayerTableRecord->setIsLocked(bLock);
	pLayerTableRecord->setIsFrozen(bFrozen);
	pObj->close();
	return Acad::eOk;
}
//强制删除实体
Acad::ErrorStatus GCADUtils::ForceEraseEntity(const AcDbObjectId objId)
{
	AcDbEntityPointer spEnt(objId,AcDb::kForRead);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=Acad::eOk)
	{
		return es;
	}
	es=spEnt->upgradeOpen();
	if(es==Acad::eOk)
	{
		es=spEnt->erase();
		return es;
	}
	else if(es==Acad::eOnLockedLayer)
	{
		bool bLock=false;
		bool bFrozen=false;
		es=UnLockLayer(spEnt->layerId(),bLock,bFrozen);
		if(es!=Acad::eOk)
		{
			return es;
		}
		es=spEnt->upgradeOpen();
		if(es!=Acad::eOk)
		{
			LockLayer(spEnt->layerId(),bLock,bFrozen);
			return es;
		}
		es=spEnt->erase();
		LockLayer(spEnt->layerId(),bLock,bFrozen);
		return es;
	}
	else
	{
		return es;
	}
}
// 功能：添加实体到数据库中
	// 参数：	  pEnt：待添加的实体指针
	//			  pDB：接收实体的数据库		
	// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
	AcDbObjectId GCADUtils::AddToCurrentSpace(AcDbEntity* pEnt,AcDbDatabase* pDB)
	{
		AcDbObjectId resultId;
		resultId.setNull();
		if(pDB==NULL)
			return resultId;

		AcDbBlockTableRecord* pBlkRec = NULL;//打开表
		Acad::ErrorStatus es = acdbOpenObject(pBlkRec, pDB->currentSpaceId(), AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\n 打开模型空间失败！%s"),acadErrorStatusText(es));
			return resultId;
		}

		es = pBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
		if (es != Acad::eOk) 
		{
			acutPrintf(_T("\n 添加对象[%s]到模型空间失败！%s"),pEnt->isA()->name(),acadErrorStatusText(es));
			pBlkRec->close();
			return resultId;
		}
		es=pBlkRec->close();

		return resultId;
	}
	// 功能：添加实体到模型空间
	// 参数：	  pEnt：待添加的实体指针
	//			  pDB：接收实体的数据库		
	// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
	AcDbObjectId GCADUtils::AddToModelSpace(AcDbEntity* pEnt,AcDbDatabase* pDB)
	{
		AcDbObjectId resultId;
		resultId.setNull();
		if(pDB==NULL)
			return resultId;
		AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDB, AcDb::kForWrite);
		Acad::ErrorStatus es = spBlkRec.openStatus();
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\n 打开模型空间失败！%s"),acadErrorStatusText(es));
			return resultId;
		}

		es = spBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
		if (es != Acad::eOk) 
		{
			acutPrintf(_T("\n 添加对象[%s]到模型空间失败！%s"),pEnt->isA()->name(),acadErrorStatusText(es));
			spBlkRec->close();
			return resultId;
		}
		es=spBlkRec->close();

		return resultId;
	}
	//获取最大化视口范围
bool GCADUtils::GetMaxViewPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax)
{
	bool bHasEntity=false;
	int nCount = ObjIdArray.length();
	for(int k=0; k<nCount; k++)
	{
		AcDbObjectId EntId;
		EntId = ObjIdArray[k];
		AcDbEntity *pEnt=NULL;
		AcDbExtents ex;
		if(acdbOpenObject(pEnt,EntId,AcDb::kForRead)==Acad::eOk)
		{
			Acad::ErrorStatus es=pEnt->getGeomExtents(ex);
			pEnt->close();
			pEnt=NULL;
			if(es!=Acad::eOk)
			{
				continue;
			}
			if(!bHasEntity)
			{
				bHasEntity=true;
				ptMin=ex.minPoint();
				ptMax=ex.maxPoint();
				continue;
			}
			AcGePoint3d tempMax;
			AcGePoint3d tempMin;
			tempMax = ex.maxPoint();
			if(tempMax.x > ptMax.x)
				ptMax.x = tempMax.x;
			if(tempMax.y > ptMax.y)
				ptMax.y = tempMax.y;
			if(tempMax.z > ptMax.z)
				ptMax.z = tempMax.z;

			tempMin = ex.minPoint();
			if(tempMin.x < ptMin.x)
				ptMin.x = tempMin.x;
			if(tempMin.y < ptMin.y)
				ptMin.y = tempMin.y;
			if(tempMin.z < ptMin.z)
				ptMin.z = tempMin.z;
		}
	}
	return bHasEntity;
}
Acad::ErrorStatus GCADUtils::OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	Acad::ErrorStatus es=Acad::eOk;

	AcDbLayerTable *pLayerTable=NULL;
	if((es=pDb->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk ) 
	{
		//acutPrintf (_T("\n不能创建表迭代器")) ;
		pLayerTable->close () ;
		return es;
	}
	pLayerTable->close () ;

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		bool bLock=pLayerTableRecord->isLocked();
		bool bFrozen=pLayerTableRecord->isFrozen();
		if(bLock||bFrozen)
		{
			es=pLayerTableRecord->upgradeOpen();
			if(es!=eOk)
			{
				pLayerTableRecord->close();
				continue;
			}
			pLayerTableRecord->setIsLocked(false);
			pLayerTableRecord->setIsFrozen(false);
			ids.append(pLayerTableRecord->objectId());
			locks.append(bLock);
			frozens.append(bFrozen);

		}
		pLayerTableRecord->close();
	}
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	return Acad::eOk;
}
Acad::ErrorStatus GCADUtils::ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbLayerTableRecordPointer spLayer(ids.at(i),AcDb::kForWrite);
		if(spLayer.openStatus()==Acad::eOk)
		{
			spLayer->setIsLocked(locks.at(i));
			spLayer->setIsFrozen(frozens.at(i));
		}
	}
	return Acad::eOk;
}
Acad::ErrorStatus GCADUtils::WriteXData(AcDbObjectId objId,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray)
{
	AcDbObjectPointer<AcDbObject> spEnt(objId,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
		return es;
	es=WriteXData(spEnt.object(),strName,strKeyArray,strValueArray);
	return es;
}
Acad::ErrorStatus GCADUtils::RegisterApp(AcDbDatabase* pDb, const TCHAR * pAppName)

	{

		AcDbRegAppTable* pAppTable;

		Acad::ErrorStatus es;


		// get the RegAppTable

		if((es = pDb->getRegAppTable(pAppTable,AcDb::kForWrite)) != Acad::eOk)

			return es;


		// if RegAppTable has application already - fine

		if(pAppTable->has(pAppName))

		{

			pAppTable->close();

			return Acad::eOk;

		}


		AcDbRegAppTableRecord* pAppTableRecord = new AcDbRegAppTableRecord();

		pAppTableRecord->setName(pAppName);


		if((es = pAppTable->add(pAppTableRecord)) != Acad::eOk)

		{

			delete pAppTableRecord;

			pAppTable->close();

			return es;

		}

		pAppTableRecord->close();

		pAppTable->close();

		return es;

	}
Acad::ErrorStatus GCADUtils::WriteXData(AcDbObject* pEnt,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray)
{
	struct  resbuf  *pHead=NULL, *pTemp=NULL;


	pHead = pEnt->xData(strName);
	if (NULL == pHead)
	{
		Acad::ErrorStatus es=RegisterApp(pEnt->database(),strName);
		if(es!=eOk)
		{
			return es;
		}
	}

	pHead = acutNewRb(AcDb::kDxfRegAppName);
	pTemp = pHead;
#ifndef _UNICODE
	pTemp->resval.rstring = (char*) malloc(strlen(strName) + 1);
	strcpy(pTemp->resval.rstring, strName);
#else
	pTemp->resval.rstring = (TCHAR*) malloc(sizeof(TCHAR*)*(lstrlen(strName) + 1));
	lstrcpy(pTemp->resval.rstring, strName);
#endif
	CStringArray strTmpArr;
	for(long i=0;i<strKeyArray.GetCount();i++)
	{
		strTmpArr.Add(strKeyArray.GetAt(i));
		strTmpArr.Add(strValueArray.GetAt(i));
	}
	for(long j=0;j<strTmpArr.GetCount();j++)
	{
		CString strCode=strTmpArr.GetAt(j);
		pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
#ifndef _UNICODE
		pTemp->resval.rstring = (char*) malloc(strCode.GetLength() + 1);
		strcpy(pTemp->resval.rstring, strCode);
#else
		pTemp->resval.rstring = (TCHAR*) malloc(sizeof(TCHAR*)*(strCode.GetLength() + 1));
		lstrcpy(pTemp->resval.rstring, strCode);
#endif
		pTemp->rbnext = NULL;
	}


	Acad::ErrorStatus tt = pEnt->setXData(pHead);
	if(tt != Acad::eOk)
	{
		acutPrintf(_T("\n写入扩展数据失败0:%s"),acadErrorStatusText(tt));
		return tt;
	}
	acutRelRb(pHead);
	return eOk;
}
//读取扩展数据
Acad::ErrorStatus GCADUtils::ReadXData(AcDbObjectId objId,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray)
{
	AcDbObjectPointer<AcDbObject> spEnt(objId,AcDb::kForRead);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
		return es;
	es=ReadXData(spEnt.object(),strName,strKeyArray,strValueArray);
	return es;
}
Acad::ErrorStatus GCADUtils::ReadXData(AcDbObject* pObj,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray)
{
	struct  resbuf  *pHead=NULL, *pTemp=NULL;
	pHead = pObj->xData(strName);
	if (NULL == pHead)
	{
		return eNullHandle;
	}
	pTemp=pHead;
	bool bKey=true;
	while(pTemp!=NULL)
	{
		if(pTemp->restype==AcDb::kDxfXdAsciiString)
		{
			if(bKey)
			{
				strKeyArray.Add(pTemp->resval.rstring);
			}
			else
			{
				strValueArray.Add(pTemp->resval.rstring);
			}
			bKey=!bKey;
		}
		pTemp=pTemp->rbnext;
	}
	acutRelRb(pHead);
	if(strValueArray.GetCount()+1==strKeyArray.GetCount())
	{
		strKeyArray.RemoveAt(strKeyArray.GetCount()-1);
	}
	return eOk;

}
//获取绘图比例和出图比例
Acad::ErrorStatus GCADUtils::GetScale(int& nHTBL,int& nCTBL,AcDbDatabase* pDB)
{
	nHTBL=1;
	nCTBL=1;
	CString strHTBL;
	CString strCTBL;
	if(GlobalSystemValue::instance.IsXiaoZhuShou())
	{
		strHTBL=GUserDataRecord::getInstance()->GetProfileString(_T("GMainConfigDialog"),_T("HTBL"),10,_T("100"));
		strCTBL=GUserDataRecord::getInstance()->GetProfileString(_T("GMainConfigDialog"),_T("CTBL"),10,_T("100"));
	}
	else
	{
		strHTBL=GUserDataRecord::getInstance()->GetProfileString(_T("GMainConfigDialog"),_T("HTBL"),10,_T("1"));
		strCTBL=GUserDataRecord::getInstance()->GetProfileString(_T("GMainConfigDialog"),_T("CTBL"),10,_T("1"));
	}
	GStringHelper::IsValidInt(strHTBL,nHTBL);
	GStringHelper::IsValidInt(strCTBL,nCTBL);
	AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDB, AcDb::kForRead);
	Acad::ErrorStatus es = spBlkRec.openStatus();
	if (es == Acad::eOk)
	{
		CStringArray strKeyArray;
		CStringArray strValueArray;
		GCADUtils::ReadXData(spBlkRec.object(),_T("cadgj_info"),strKeyArray,strValueArray);
		for(int i=0;i<strKeyArray.GetCount();i++)
		{
			CString strKey=strKeyArray.GetAt(i);
			CString strValue=strValueArray.GetAt(i);
			if(strKey.CompareNoCase(_T("HTBL"))==0)
			{
				GStringHelper::IsValidInt(strValue,nHTBL);
			}
			else if(strKey.CompareNoCase(_T("CTBL"))==0)
			{
				GStringHelper::IsValidInt(strValue,nCTBL);
			}
		}
	}
	return Acad::eOk;
}
//设置绘图比例和出图比例
Acad::ErrorStatus GCADUtils::SetScale(int& nHTBL,int& nCTBL,AcDbDatabase* pDB)
{
	AcDbBlockTableRecordPointer spBlkRec(ACDB_MODEL_SPACE, pDB, AcDb::kForWrite);
	Acad::ErrorStatus es = spBlkRec.openStatus();
	if (es != Acad::eOk)
	{
		return es;
	}
	CString strHTBL;
	strHTBL.Format(_T("%d"),nHTBL);
	CString strCTBL;
	strCTBL.Format(_T("%d"),nCTBL);
	CStringArray strKeyArray;
	CStringArray strValueArray;
	GCADUtils::ReadXData(spBlkRec.object(),_T("cadgj_info"),strKeyArray,strValueArray);
	for(int i=0;i<strKeyArray.GetCount();i++)
	{
		CString strKey=strKeyArray.GetAt(i);
		CString strValue=strValueArray.GetAt(i);
		if(strKey.CompareNoCase(_T("HTBL"))==0)
		{
			strValueArray.SetAt(i,strHTBL);
			strHTBL=_T("");
		}
		else if(strKey.CompareNoCase(_T("CTBL"))==0)
		{
			strValueArray.SetAt(i,strCTBL);
			strCTBL=_T("");
		}
	}
	if(!strHTBL.IsEmpty())
	{
		strKeyArray.Add(_T("HTBL"));
		strValueArray.Add(strHTBL);
	}
	if(!strCTBL.IsEmpty())
	{
		strKeyArray.Add(_T("CTBL"));
		strValueArray.Add(strCTBL);
	}
	es=WriteXData(spBlkRec.object(),_T("cadgj_info"),strKeyArray,strValueArray);
	return es;

}
//获取图层的ID
Acad::ErrorStatus GCADUtils::GetLayerId(AcDbObjectId& layerId,const CString strLayerName,AcDbDatabase* pDb)
{
	CString lyName=strLayerName;
	if(pDb==NULL)
		return Acad::eNullHandle;
	AcDbLayerTable*     pTable=NULL;
	Acad::ErrorStatus es=pDb->getSymbolTable(pTable,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;
	bool bHave=pTable->has(lyName.GetBuffer());
	lyName.ReleaseBuffer();
	if(!bHave)
	{
		pTable->close();
		return Acad::eNotInDatabase;
	}
	es=pTable->getAt(lyName.GetBuffer(),layerId);
	lyName.ReleaseBuffer();
	pTable->close();
	return es;
}
// 查找CAD实体线型
Acad::ErrorStatus GCADUtils::GetLineTypeId(AcDbObjectId& resultId,const CString strLnTypeName,AcDbDatabase* pDB)
{
	resultId.setNull();

	AcDbLinetypeTable		*pDbLtT;
	if(pDB==NULL)
	{
		return Acad::eNullHandle;
	}
	Acad::ErrorStatus es=pDB->getSymbolTable(pDbLtT, AcDb::kForRead);
	if(es!=Acad::eOk)
	{
		return es;
	}
	if (!pDbLtT->has(strLnTypeName))
	{
		pDbLtT->close();
		return Acad::eKeyNotFound;
	}
	else
	{
		pDbLtT->getAt(strLnTypeName, resultId);
		pDbLtT->close();
		return Acad::eOk;
	}
}

// 加载CAD实体线型
Acad::ErrorStatus GCADUtils::LoadLineType(AcDbObjectId& resultId,const CString strLnTypeName,AcDbDatabase* pDB)
{

	AcDbLinetypeTable		* pDbLtT;
	Acad::ErrorStatus		es;

	resultId.setNull();

	es = pDB->loadLineTypeFile(strLnTypeName, _T(".\\acadiso.lin"));
	if( es != Acad::eOk)
	{
		return es;
	}
	else
	{
		es = pDB->getLinetypeTable(pDbLtT, AcDb::kForRead);
		if ( es == Acad::eOk)
		{
			pDbLtT->getAt(strLnTypeName, resultId);
			pDbLtT->close();
			return es;
		}
		else
		{
			pDbLtT->close();
			return es;
		}
	}

}
void GCADUtils::GetCurrentSelectObjIds(AcDbObjectIdArray& objIds,struct resbuf* rb)
{
	objIds.setLogicalLength(0);
	ads_name curSSname;
	int rc=acedSSGet(_T("I"), NULL, NULL, rb, curSSname);
	if(rc==RTNORM)
	{
		SSToIds(objIds,curSSname);
		acedSSFree(curSSname);
	}
}
//设置为相同的属性
//将pTargetEnt设置为和pSourceEnt设置为一样的属性
void GCADUtils::SameEntityAttr(AcDbEntity* pTargetEnt,AcDbEntity* pSourceEnt)
{
	pTargetEnt->setColor(pSourceEnt->color());
	pTargetEnt->setLayer(pSourceEnt->layer());
	pTargetEnt->setLinetype(pSourceEnt->linetype());
	pTargetEnt->setLineWeight(pSourceEnt->lineWeight());
#if _MSC_VER>=1400
	pTargetEnt->setMaterial(pSourceEnt->materialId());
#endif
}
// 功能：添加实体到块表记录中
// 参数：	  pEnt：待添加的实体指针
//			  btrID,AcDbBlockTableRecord的ID		
// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
AcDbObjectId GCADUtils::AppendEntity(AcDbEntity* pEnt,const AcDbObjectId btrID)
{
	AcDbObjectId resultId;
	resultId.setNull();

	AcDbBlockTableRecord* pBlkRec = NULL;//打开表
	Acad::ErrorStatus es = acdbOpenObject(pBlkRec, btrID, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开模型空间失败！%s"),acadErrorStatusText(es));
		return resultId;
	}

	es = pBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
	if (es != Acad::eOk) 
	{
		acutPrintf(_T("\n 添加对象到模型空间失败！%s"),acadErrorStatusText(es));
		pBlkRec->close();
		return resultId;
	}
	es=pBlkRec->close();

	return resultId;
}