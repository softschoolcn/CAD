#include "StdAfx.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "LayerCommCommand.h"
#include "..\\CADGlobalTools\\AcDbEntityPointerEx.h"
//重新注册图层命令
void ReRegLayerCmds()
{
	//2:选择关闭图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("CloseSelLayer") , 
		GetCommandAlias(_T("CloseSelLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		CloseSelLayer);

	//18:打开所有图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("OpenAllLayer") , 
		GetCommandAlias(_T("OpenAllLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		OpenAllLayer);
	//19:选择关闭其它图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("CloseSelOtherLayer") , 
		GetCommandAlias(_T("CloseSelOtherLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		CloseSelOtherLayer);
	//20:冻结图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("FrozenSelLayer") , 
		GetCommandAlias(_T("FrozenSelLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		FrozenSelLayer);
	//21:锁定图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("LockSelLayer") , 
		GetCommandAlias(_T("LockSelLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		LockSelLayer);
	//22:解锁图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("UnLockSelLayer") , 
		GetCommandAlias(_T("UnLockSelLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		UnLockSelLayer);
	//23:解冻所有图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("UnFrozenAllLayer") , 
		GetCommandAlias(_T("UnFrozenAllLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		UnFrozenAllLayer);

	//37:设为当前图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("AssSetCurLayer") , 
		GetCommandAlias(_T("AssSetCurLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW,
		SetCurLayer);

	//67:图层规整
	acedRegCmds->addCommand(_T("CADLayerToolsApp"),
		_T("StandLayer"),
		GetCommandAlias(_T("StandLayer")),
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW,
		StandLayer);
	//68:图层排序
	acedRegCmds->addCommand(_T("CADLayerToolsApp"),
		_T("SortLayer"),
		GetCommandAlias(_T("SortLayer")),
		0,
		SortLayer);


	//111:选择冻结其它图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("FrozenSelOtherLayer") , 
		GetCommandAlias(_T("FrozenSelOtherLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		FrozenSelOtherLayer);

	//112:选择锁定其它图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("LockSelOtherLayer") , 
		GetCommandAlias(_T("LockSelOtherLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		LockSelOtherLayer);

	//113:解锁所有图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("UnLockAllLayer") , 
		GetCommandAlias(_T("UnLockAllLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		UnLockAllLayer);

	//114:全开所有图层
	acedRegCmds->addCommand(_T("CADLayerToolsApp") ,
		_T("OpenExAllLayer") , 
		GetCommandAlias(_T("OpenExAllLayer")) ,
		ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW ,
		OpenExAllLayer);

}

//获取当前图层
Acad::ErrorStatus getCurrentLayer(CString &strLayerName)
{
	Acad::ErrorStatus es=Acad::eOk;
	resbuf rb;
	acedGetVar(_T("clayer"), &rb);
	strLayerName=rb.resval.rstring;
	return es;
}
//功能:	激活当前视图
Acad::ErrorStatus activeCurrentview()
{
	Acad::ErrorStatus es=Acad::eOk;

	if(acedGetAcadFrame()!=NULL)
	{
		CFrameWnd	*pFrame;
		pFrame	= acedGetAcadFrame()->GetActiveFrame();

		if(pFrame != NULL)
		{
			CView	*pView;
			pView	= pFrame->GetActiveView();
			if(pView != NULL)
				pView->SetFocus();
			else
				es=Acad::eNotApplicable;
		}
		else
			es=Acad::eNotApplicable;
	}
	else
		es=Acad::eNotApplicable;

	return es;
}
//选择关闭图层
void CloseSelLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		TCHAR* promptPtrs[2]={_T("\n选择关闭图层的对象:"),_T("\n移除关闭图层的对象:")};
		if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	int nSuccess=0;
	int nError=0;
	// 关闭图层
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n关闭图层<%s>..."),lpName);
		nSuccess++;
		spLayer->setIsOff(true);

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n选中图层%d个已全部关闭..."),nSuccess);
	}
	else
	{
		acutPrintf(_T("\n选中图层%d个，其中%d个成功关闭，%d个关闭失败..."),nError+nSuccess,nSuccess,nError);
	}
}
//选择关闭其它图层
void CloseSelOtherLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		TCHAR* promptPtrs[2]={_T("\n选择对象:"),_T("\n移除对象:")};
		if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	AcDbObjectIdArray allLayerIdArray;
	GLayerUtils::EnumAllLayer(allLayerIdArray,pDB);
	int nSuccess=0;
	int nError=0;
	for(i=0;i<allLayerIdArray.logicalLength();i++)
	{
		AcDbObjectId layerId=allLayerIdArray.at(i);
		if(layerIdArray.contains(layerId))
			continue;
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n关闭图层<%s>..."),lpName);
		spLayer->setIsOff(true);
		nSuccess++;
	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n其它图层%d个已全部关闭..."),nSuccess);
	}
	else
	{
		acutPrintf(_T("\n其它图层%d个，其中%d个成功关闭，%d个关闭失败..."),nError+nSuccess,nSuccess,nError);
	}
}
//冻结其它
void FrozenSelOtherLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		TCHAR* promptPtrs[2]={_T("\n选择对象:"),_T("\n移除对象:")};
		if(acedSSGet(_T(":$"),promptPtrs,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	AcDbObjectIdArray allLayerIdArray;
	GLayerUtils::EnumAllLayer(allLayerIdArray,pDB);
	int nSuccess=0;
	int nError=0;
	for(i=0;i<allLayerIdArray.logicalLength();i++)
	{
		AcDbObjectId layerId=allLayerIdArray.at(i);
		if(layerIdArray.contains(layerId))
			continue;
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n冻结图层<%s>..."),lpName);
		spLayer->setIsFrozen(true);
		nSuccess++;
	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n其它图层%d个已全部冻结..."),nSuccess);
	}
	else
	{
		acutPrintf(_T("\n其它图层%d个，其中%d个成功冻结，%d个冻结失败..."),nError+nSuccess,nSuccess,nError);
	}
}
//锁定其它
void LockSelOtherLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		if(acedSSGet(NULL,NULL,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	AcDbObjectIdArray allLayerIdArray;
	GLayerUtils::EnumAllLayer(allLayerIdArray,pDB);
	int nSuccess_Lock=0;
	int nSuccess_UnLock=0;
	int nError=0;
	for(i=0;i<allLayerIdArray.logicalLength();i++)
	{
		bool bLock=true;
		AcDbObjectId layerId=allLayerIdArray.at(i);
		if(layerIdArray.contains(layerId))
		{
			bLock=false;
		}
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(bLock?_T("\n锁定图层<%s>..."):_T("\n解锁图层<%s>..."),lpName);
		spLayer->setIsLocked(bLock);
		if(bLock)
		{
			nSuccess_Lock++;
		}
		else
		{
			nSuccess_UnLock++;
		}
	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n其它图层%d个已全部锁定,选中图层%d个已全部解锁..."),nSuccess_Lock,nSuccess_UnLock);
	}
	else
	{
		acutPrintf(_T("\n其它图层%d个已锁定,选中图层%d个已解锁,发生错误%d个..."),nSuccess_Lock,nSuccess_UnLock,nError);
	}
}
//图层冻结
void FrozenSelLayer(bool bFrozen)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;



	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		if(acedSSGet(NULL,NULL,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	int nSuccess=0;
	int nError=0;
	// 冻结图层
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(bFrozen?_T("\n冻结图层失败%s..."):_T("\n解冻图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		if(layerId==curLayerId&&bFrozen)
		{
			acutPrintf(_T("\n忽略当前图层<%s>,不支持冻结当前图层..."),lpName);
			nError++;
			continue;
		}
		//acutPrintf(bFrozen?_T("\n冻结图层<%s>..."):_T("\n解冻图层<%s>..."),lpName);
		spLayer->setIsFrozen(bFrozen);
		nSuccess++;

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n选中图层%d个已全部%s..."),nSuccess,bFrozen?_T("冻结"):_T("解冻"));
	}
	else
	{
		acutPrintf(_T("\n选中图层%d个，其中%d个成功%s，%d个%s失败..."),nError+nSuccess,nSuccess,bFrozen?_T("冻结"):_T("解冻"),nError,bFrozen?_T("冻结"):_T("解冻"));
	}
}
//锁定图层
void LockSelLayer(bool bLock)
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 支持多选
	AcDbObjectIdArray EntIds;

	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()==0)
	{

		ads_name ssName;
		if(acedSSGet(NULL,NULL,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	int i=0;
	for(i=0; i<EntIds.logicalLength();i++)
	{
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,EntIds[i],AcDb::kForRead)==Acad::eOk)
		{
			AcDbObjectId layerId=pEnt->layerId();
			pEnt->close();
			if (!layerIdArray.contains(layerId))
			{
				layerIdArray.append(layerId);
			}
		}
	}
	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	// 冻结图层
	int nSuccess=0;
	int nError=0;
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(bLock?_T("\n锁定图层失败%s..."):_T("\n解锁图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(bLock?_T("\n锁定图层<%s>..."):_T("\n解锁图层<%s>..."),lpName);
		spLayer->setIsLocked(bLock);
		nSuccess++;

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n选中图层%d个已全部%s..."),nSuccess,bLock?_T("锁定"):_T("解锁"));
	}
	else
	{
		acutPrintf(_T("\n选中图层%d个，其中%d个成功%s，%d个%s失败..."),nError+nSuccess,nSuccess,bLock?_T("锁定"):_T("解锁"),nError,bLock?_T("锁定"):_T("解锁"));
	}
}

//冻结图层
void FrozenSelLayer()
{
	FrozenSelLayer(true);
}
//锁定图层
void LockSelLayer()
{
	LockSelLayer(true);
}
//解冻所有图层
void UnFrozenAllLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	GLayerUtils::EnumAllLayer(layerIdArray,pDB);
	int i=0;

	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	// 关闭图层
	int nError=0;
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n解冻图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n解冻图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n解冻图层<%s>..."),lpName);
		spLayer->setIsFrozen(false);

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n图层全部解冻..."));
	}
	else
	{
		acutPrintf(_T("\n图层部分解冻,失败%d个..."),nError);
	}
}
//解锁图层
void UnLockSelLayer()
{
	LockSelLayer(false);
}
void OpenAllLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	GLayerUtils::EnumAllLayer(layerIdArray,pDB);
	int i=0;

	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	int nError=0;
	// 关闭图层
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n打开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n打开图层<%s>..."),lpName);
		spLayer->setIsOff(false);

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n图层全部打开..."));
	}
	else
	{
		acutPrintf(_T("\n图层部分打开,失败%d个..."),nError);
	}
}
//解锁所有图层
void UnLockAllLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	GLayerUtils::EnumAllLayer(layerIdArray,pDB);
	int i=0;

	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	int nError=0;
	// 关闭图层
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n解锁图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n解锁图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n解锁图层<%s>..."),lpName);
		spLayer->setIsLocked(false);

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n图层全部解锁..."));
	}
	else
	{
		acutPrintf(_T("\n图层部分解锁,失败%d个..."),nError);
	}
}
//全开所有图层
void OpenExAllLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 根据选择实体获取图层名列表
	AcDbObjectIdArray layerIdArray;
	GLayerUtils::EnumAllLayer(layerIdArray,pDB);
	int i=0;

	CString strLayerName;
	getCurrentLayer(strLayerName);
	AcDbObjectId curLayerId;
	curLayerId.setNull();
	GCADUtils::GetLayerId(curLayerId,strLayerName);
	int nError=0;
	// 关闭图层
	for (i=0; i<layerIdArray.logicalLength(); i++) 
	{
		AcDbObjectId layerId=layerIdArray.at(i);
		AcDbLayerTableRecordPointer spLayer(layerId,AcDb::kForRead);
		if(spLayer.openStatus()!=Acad::eOk)
		{
			acutPrintf(_T("\n全开图层失败%s..."),acadErrorStatusText(spLayer.openStatus()));
			nError++;
			continue;
		}
		const TCHAR* lpName=NULL;
		es=spLayer->getName(lpName);
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n获取图层名称失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		es=spLayer->upgradeOpen();
		if(es!=Acad::eOk)
		{
			acutPrintf(_T("\n全开图层失败%s..."),acadErrorStatusText(es));
			nError++;
			continue;
		}
		//acutPrintf(_T("\n全开图层<%s>..."),lpName);
		spLayer->setIsLocked(false);
		spLayer->setIsFrozen(false);
#if _MSC_VER>=1400
		spLayer->setIsHidden(false);
#endif
		spLayer->setIsOff(false);

	}
	// 刷新视图
	activeCurrentview();
	if(nError==0)
	{
		acutPrintf(_T("\n图层全部全开..."));
	}
	else
	{
		acutPrintf(_T("\n图层部分全开,失败%d个..."),nError);
	}
}
//设置当前图层
void SetCurLayer()
{
	Acad::ErrorStatus es=Acad::eOk;
	AcApDocument * pDoc = curDoc();

	if (NULL == pDoc)
		return;
	//锁定文档
	GLockDocument lock;

	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	if (NULL == pDB)
		return;

	// 只支持单选
	AcDbObjectIdArray EntIds;

	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()!=1)
	{
		EntIds.setLogicalLength(0);
		ads_name ssName;
		if(acedSSGet(_T(":S"),NULL,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}
	if(EntIds.logicalLength()!=1)
	{
		return;
	}
	AcDbEntityPointer spEnt(EntIds.at(0),AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	CString strLayerName;
	es=GCADUtils::GetLayerName(strLayerName,spEnt->layerId());
	if(es!=Acad::eOk)
	{
		return;
	}
	pDB->setClayer(spEnt->layerId());
	acutPrintf(_T("\n当前图层切换为<%s>\n"),strLayerName);

}
#include "GStandLayerDialog.h"
//图层规整
void StandLayer()
{
	if(!CheckReg(_T("20131010")))
	{
		return;
	}
	// 支持多选
	AcDbObjectIdArray EntIds;
	GCADUtils::GetCurrentSelectObjIds(EntIds);
	if(EntIds.logicalLength()!=1)
	{

		ads_name ssName;
		if(acedSSGet(NULL,NULL,NULL,NULL,ssName)!=RTNORM)
			return;
		SSToIds(EntIds,ssName);
		acedSSFree(ssName);
	}
	CStringArray strLayerArr;
	AcDbObjectIdArray layeIds;
	GLayerUtils::EnumAllLayer(strLayerArr,layeIds);
	CAcModuleResourceOverride res;
	GStandLayerDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.m_strLayerNameArr.Append(strLayerArr);
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	for(int i=0;i<EntIds.logicalLength();i++)
	{
		AcDbEntityPointerEx<AcDbEntity> spEnt(EntIds.at(i),AcDb::kForWrite,false,true);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		int nKind=-1;
		if(spEnt->isKindOf(AcDbText::desc())||
			spEnt->isKindOf(AcDbMText::desc()))
		{
			nKind=0;
		}
		else if(spEnt->isKindOf(AcDbDimension::desc()))
		{
			nKind=1;
		}
		else if(spEnt->isKindOf(AcDbHatch::desc()))
		{
			nKind=2;
		}
		if(nKind<0)
		{
			continue;
		}
		int nLayerIndex=dlg.m_selectLayerIndexArr.GetAt(nKind);
		if(nLayerIndex<0)
		{
			continue;
		}
		spEnt->setLayer(layeIds.at(nLayerIndex));
	}

}
#include "DlgLayerSort.h"
//图层排序
void SortLayer()
{
#ifndef ACAD_2004
	if(!CheckReg(_T("20131010")))
	{
		return;
	}
	CAcModuleResourceOverride res;
	CDlgLayerSort dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
#else
	acutPrintf(_T("\nAutoCAD2004不支持对排序表进行操作，如果您需要使用图层排序功能，请升级至AutoCAD2005或以上版本"));
#endif
}
