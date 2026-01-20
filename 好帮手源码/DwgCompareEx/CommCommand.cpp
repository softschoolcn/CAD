#include "StdAfx.h"
#include "CommCommand.h"
#include "CmpReportDockPanel.h"
void ShowReportPanel(BOOL bShow)
{
	if(m_pReportDockPanel==NULL&&(!bShow))
		return;
	if(m_pReportDockPanel==NULL)
	{
		//第一次调用命令时,转换资源
		CAcModuleResourceOverride _UseThisRes;

		//####修改：由于图形入库检测功能与避让分析功能共用一个错误面板，当他们功能切换的时候面板显示也进行切换
		m_pReportDockPanel = new CmpReportDockPanel();
		CWnd *pWnd = acedGetAcadFrame();
		m_pReportDockPanel->Create(pWnd, _T("图纸比较结果"));
	}
	if(m_pReportDockPanel==NULL)
	{
		return;
	}
	m_pReportDockPanel->ShowDockPane(bShow);

}
void ShowHidePanel()
{
	if(m_pReportDockPanel==NULL)
	{
		//第一次调用命令时,转换资源
		CAcModuleResourceOverride _UseThisRes;

		//####修改：由于图形入库检测功能与避让分析功能共用一个错误面板，当他们功能切换的时候面板显示也进行切换
		m_pReportDockPanel = new CmpReportDockPanel();
		CWnd *pWnd = acedGetAcadFrame();
		m_pReportDockPanel->Create(pWnd, _T("图纸比较结果"));
	}
	if(m_pReportDockPanel==NULL)
	{
		return;
	}
	m_pReportDockPanel->ShowDockPane(FALSE);
}
//nMode=0,正常模式
//nMode=0,较小模式，用于对比查看单个文件
void SetReportPanelSizeMode(int nMode)
{
	if(m_pReportDockPanel==NULL)
	{
		return;
	}
	if(nMode==0)
	{
		m_pReportDockPanel->DockControlBar(CBRS_ALIGN_BOTTOM,CRect(0, 0, 800, 100));
	}
	else
	{
		m_pReportDockPanel->DockControlBar(CBRS_ALIGN_BOTTOM,CRect(0, 0, 800, 50));
		m_pReportDockPanel->RestoreControlBar(AFX_IDW_DOCKBAR_BOTTOM);
	}
	
}
void ShowCmpResult(int nMode,bool bReg)
{
	if(m_pReportDockPanel==NULL)
	{
		return;
	}
	m_pReportDockPanel->ShowCmpResult(nMode,bReg);
}
void ShowCmpResultEx(int nMode,bool bReg)
{
	if(m_pReportDockPanel==NULL)
	{
		return;
	}
	m_pReportDockPanel->ShowCmpResultEx(nMode,bReg);
}
void SetEntityColor(AcDbEntity* pEnt,int nColorIndex,GColorRecord& cr)
{
	pEnt->setColorIndex(nColorIndex);
	AcCmColor cor;
	cor.setColorIndex(nColorIndex);
	if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		//标注类对象
		AcDbDimension* pDim=AcDbDimension::cast(pEnt);
		pDim->setDimclrd(cor);//尺寸线颜色
		pDim->setDimclre(cor);//尺寸界线颜色
		pDim->setDimclrt(cor);//文字颜色
	}
	else if(pEnt->isKindOf(AcDbHatch::desc()))
	{
		//填充类对象
		AcDbHatch* pHatch=AcDbHatch::cast(pEnt);
		pHatch->setGradientOneColorMode(true);
		pHatch->setHatchObjectType(AcDbHatch::kHatchObject);
	}
	else if(pEnt->isKindOf(AcDbLeader::desc()))
	{
		AcDbLeader* pLeader=AcDbLeader::cast(pEnt);
		pLeader->setDimclrd(cor);
	}
}
void SetEntityColor(const AcDbObjectId id,int nColorIndex,GColorRecord& cr)
{
	AcDbObjectPointer<AcDbEntity> spEnt(id,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
		return;
	SetEntityColor(spEnt.object(),nColorIndex,cr);

}
void SetEntityColor(AcDbEntity* pEnt,GColorRecord& cr)
{
	cr.Write(pEnt);
}
void SetEntityColor(const AcDbObjectId id,GColorRecord& cr)
{
	AcDbObjectPointer<AcDbEntity> spEnt(id,AcDb::kForWrite);
	Acad::ErrorStatus es=spEnt.openStatus();
	if(es!=eOk)
		return;
	SetEntityColor(spEnt.object(),cr);

}
int dxfCodeToDataType(int resType)
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
	else if ((resType >= 110) && (resType <= 112))
		return RTSTR;
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
LPCTSTR
bytesToHexStr(char* buffer, int len, CString& hexStr)
{
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
void
dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr)
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
				valueStr.Format(_T("%s"), rb->resval.rstring);
			break;
		case RT3DPOINT:
			valueStr.Format(_T("%g,%g,%g"), rb->resval.rpoint[X],
				rb->resval.rpoint[Y], rb->resval.rpoint[Z]);
			break;
		case RTPOINT:
			valueStr.Format(_T("%g,%g"), rb->resval.rpoint[X], rb->resval.rpoint[Y]);
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
CString dxfToStr(const resbuf* rb)
{
	CString dxfCodeStr;
	CString valueStr;
	if(rb==NULL)
	{
		return _T("NULL");
	}
	dxfToStr(rb,dxfCodeStr,valueStr);
	return valueStr;
}
CString dxfToStr(CArray<struct resbuf*>& rbs,CStringArray& strValueArray)
{
	CString strValue=_T("");
	for(long i=0;i<rbs.GetCount();i++)
	{
		CString strTmp=dxfToStr(rbs.GetAt(i));
		strValueArray.Add(strTmp);
		strTmp.Format(_T("(%s)"),strTmp);
		if(i!=0)
		{
			strValue+=_T(",");
		}
		strValue+=strTmp;
	}
	return strValue;
}
void SSToIds(ads_name ssName,AcDbObjectIdArray& ObjIds)
{
	AcDbObjectId ObjId;
	ads_name EntName;
#if _MSC_VER>=1900
	Adesk::Int32 nLength = 0;
#else
	long nLength = 0;
#endif
	acedSSLength(ssName,&nLength);

	for(int nLen=0; nLen<nLength; nLen++)
	{
		acedSSName(ssName,nLen,EntName);
		acdbGetObjectId(ObjId,EntName);
		ObjIds.append(ObjId);
	}
}
//设置实体的选中状态
//参数：ObjId,实体的ObjectId
//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
void SetEntitySelected(AcDbObjectId ObjId,bool Clear)
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
void SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear)
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
//功能:	设置视图（相当于Zoom Window命令）
//参数:	Pt1		视图左上角点
//			Pt2		视图右下角点
//			ex_ratio	扩展比率,一般为1.0
Acad::ErrorStatus view_set(AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio/*=1.0*/)
{
	Acad::ErrorStatus es=Acad::eOk;

	AcGePoint2d CenterPt;
	if ((fabs(Pt1.x-Pt2.x)<1e-6)||(fabs(Pt1.y-Pt2.y)<1e-6))
		return Acad::eInvalidExtents;//若X坐标或Y坐标重合，判为意外，不进行操作

	if (Pt1.x>Pt2.x) 
	{//确保两个坐标点分别为左上角和右下角
		double tmp;
		tmp=Pt1.x;
		Pt1.x=Pt2.x;
		Pt2.x=tmp;
	}

	if (Pt2.y>Pt1.y) 
	{
		double tmp;
		tmp=Pt1.y;
		Pt1.y=Pt2.y;
		Pt2.y=tmp;
	}

	//获取当前DwgView的尺寸
	CRect CADrect;
	acedGetAcadDwgView()->GetClientRect(&CADrect);
	double width,height,ratio;
	ratio=(double)(CADrect.right-CADrect.left)/(double)(CADrect.bottom-CADrect.top);
	if (fabs(ratio)<1e-6)
		return Acad::eInvalidExtents;

	if ((Pt2.x-Pt1.x)/(Pt1.y-Pt2.y)>ratio) 
	{
		width=Pt2.x-Pt1.x;
		height=width/ratio;
	}
	else
	{
		height=Pt1.y-Pt2.y;
		width=height * ratio;
	}

	//设置当前视图中心点
	CenterPt.x=(Pt1.x+Pt2.x)/2;
	CenterPt.y=(Pt1.y+Pt2.y)/2;

	//改变当前视图
	AcDbViewTableRecord pVwRec;
	pVwRec.setCenterPoint(CenterPt);
	pVwRec.setWidth(width*ex_ratio);
	pVwRec.setHeight(height*ex_ratio);

	es=acedSetCurrentView(&pVwRec,NULL);
	return es;
}
bool GetMaxExtPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax)
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
			
			AcGePoint3d tempMax;
			AcGePoint3d tempMin;
			if (!bHasEntity)
			{
				tempMax = ex.maxPoint();
				ptMax.x = tempMax.x;
				ptMax.y = tempMax.y;
				ptMax.z = tempMax.z;

				tempMin = ex.minPoint();
				ptMin.x = tempMin.x;
				ptMin.y = tempMin.y;
				ptMin.z = tempMin.z;

			}
			else
			{
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
			bHasEntity=true;
		}
	}
	return bHasEntity;
}
bool DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio)
{
	AcGePoint3d ptMin,ptMax;
	
	bool rc=GetMaxExtPoint(ObjIdArray,ptMin,ptMax);
	if(ptMax.x-ptMin.x<1E-2)
	{
		double max_tmp=max(ptMax.y-ptMin.y,20);
		ptMax.x=ptMax.x+max_tmp*0.2;
		ptMin.x=ptMin.x-max_tmp*0.2;
	}
	else if(ptMax.y-ptMin.y<1E-2)
	{
		double max_tmp=max(ptMax.x-ptMin.x,20);
		ptMax.y=ptMax.y+max_tmp*0.2;
		ptMin.y=ptMin.y-max_tmp*0.2;
	}
	if(!rc)
		return rc;
	return view_set(ptMin,ptMax,ex_ratio)==Acad::eOk;

}
bool DyZoom(const AcDbObjectId ObjId,double ex_ratio)
{
	AcDbObjectIdArray ObjIdArray;
	ObjIdArray.append(ObjId);
	return DyZoom(ObjIdArray,ex_ratio);
}
Acad::ErrorStatus objIdToHandle(CString& strHandle,AcDbObjectId objId)
{
	AcDbObject* pObj=NULL;
	Acad::ErrorStatus es=acdbOpenAcDbObject(pObj,objId,AcDb::kForRead);
	if(es!=eOk)
		return es;
	AcDbHandle handle;
	pObj->getAcDbHandle(handle);
	TCHAR tmpStr[256]={0};
	handle.getIntoAsciiBuffer(tmpStr);
	strHandle=tmpStr;
	pObj->close();
	return eOk;

}
Acad::ErrorStatus UnLockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
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
	Acad::ErrorStatus LockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen)
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
	Acad::ErrorStatus ForceEraseEntity(const AcDbObjectId objId)
	{
		AcDbEntityPointer spEnt(objId,AcDb::kForRead);
		Acad::ErrorStatus es=spEnt.openStatus();
		if(es!=Acad::eOk)
		{
			if(es==Acad::eNotThatKindOfClass)
			{
				AcDbObjectPointer<AcDbObject> spObj(objId,AcDb::kForWrite);
				es=spObj.openStatus();
				if(es!=Acad::eOk)
				{
					return es;
				}
				return spObj->erase();
			}
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
	void GetCurrentSelectObjIds(AcDbObjectIdArray& objIds)
	{
		objIds.setLogicalLength(0);
		ads_name curSSname;
		if(acedSSGet(_T("I"), NULL, NULL, NULL, curSSname)==RTNORM)
		{
			SSToIds(curSSname,objIds);
			acedSSFree(curSSname);
		}
	}