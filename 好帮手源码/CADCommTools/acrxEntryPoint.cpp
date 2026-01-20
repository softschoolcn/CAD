// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CommCommand.h"
#include "CurveCommCommand.h"
#include "XlsCommCommand.h"
#include "GMenu.h"
#include "GPurgeCommCommand.h"
#include "GMousePreviewDocReactor.h"
#include "GComputerId.h"

#include "md5.h"
#include "DES.h"
#include "ByteString.h"
#include "AcEdSSGetFilterEx.h"
#include "GCommandReactor.h"
#if _MSC_VER>=1700
#include "acedCmdNF.h"
#endif
GCommandReactor *m_pCommandReactor=NULL;
extern GMousePreviewDocReactor* Glb_MousePreviewDocReactor;
CString Glb_SoftPath;
BOOL    Glb_bReg;
CString Glb_strRegDate;
#define  MENU_TIMER 265
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR nTimerId, DWORD dwTime)
{
	KillTimer(hWnd, nMsg);
	if (!GMenu::AddMenu())
	{
		SetTimer(acedGetAcadFrame()->GetSafeHwnd(), MENU_TIMER, 1000, TimerProc);
	}
}
BOOL loadmenu()
{
	bool bCADMenu=GUserDataRecord::getInstance()->GetProfileBool(_T("CADMenu"),_T("Show"),true);
	if(!bCADMenu)
	{
		return FALSE;
	}
	SetTimer(NULL, MENU_TIMER, 1000, TimerProc);
	return TRUE;
}

BOOL unloadmenu(LPCTSTR lpName)
{
	return TRUE;
}
#if _MSC_VER<=1300
void GSZH()
{
	acutPrintf(_T("\nCAD格式转换功能只支持AutoCAD2007及以上版本，如果您需要使用CAD格式转换，请升级您的CAD版本。"));
};
#endif
//108:图层特性匹配
void LayerMatchprop()
{
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择源对象:"),_T("\n移除源对象:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,NULL,ssName);
	if(rc!=RTNORM)
	{
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);

	if(ids.logicalLength()!=1)
	{
		acedSSFree(ssName);
		return;
	}
	AcDbObjectId matchId=ids.at(0);
	acedSSFree(ssName);
	SetEntitySelected(ids,true);
	AcDbEntityPointer spEnt(matchId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId layerId=spEnt->layerId();
	spEnt->close();
	m_pCommandReactor->m_pFilter->Reset();
	m_pCommandReactor->m_pFilter->m_allowLayerArr.append(layerId);
	addSSgetFilterInputContextReactor(curDoc(), m_pCommandReactor->m_pFilter);
	m_pCommandReactor->Attach();
#if _MSC_VER<1700
	int rc1=acedCommand(RTSTR,_T("matchprop"),RTNONE);
#else
	int rc1 = acedCommandS(RTSTR, _T("matchprop"), RTNONE);
#endif
}
//109:类型特性匹配
void TypeMatchprop()
{
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择源对象:"),_T("\n移除源对象:")};
	int rc=acedSSGet(_T(":$:S"), promptPtrs,NULL,NULL,ssName);
	if(rc!=RTNORM)
	{
		return;
	}
	AcDbObjectIdArray ids;
	SSToIds(ids,ssName);

	if(ids.logicalLength()!=1)
	{
		acedSSFree(ssName);
		return;
	}
	AcDbObjectId matchId=ids.at(0);
	acedSSFree(ssName);
	SetEntitySelected(ids,true);
	AcDbEntityPointer spEnt(matchId,AcDb::kForRead);
	if(spEnt.openStatus()!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId layerId=spEnt->layerId();
	spEnt->close();
	m_pCommandReactor->m_pFilter->Reset();
	m_pCommandReactor->m_pFilter->m_allowLayerArr.append(layerId);
	addSSgetFilterInputContextReactor(curDoc(), m_pCommandReactor->m_pFilter);
	m_pCommandReactor->Attach();
#if _MSC_VER<1700
	int rc1=acedCommand(RTSTR,_T("matchprop"),RTNONE);
#else
	int rc1 = acedCommandS(RTSTR, _T("matchprop"), RTNONE);
#endif
}
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
//重新注册命令
void ReRegCmds()
{
	//1:样条曲线转换成多段线
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("SPToPL") , 
		GetCommandAlias(_T("SPToPL")) ,
		0 ,
		SPlineToPLine);
	//4:面积统计
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("CalcArea"),
		GetCommandAlias(_T("CalcArea")),
		0,
		calcArea);
	//5:恢复原来颜色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SameColorRollback"),
		GetCommandAlias(_T("SameColorRollback")),
		0,
		SameColorRollback);
	//6:统一为黑白色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("AllBlack"),
		GetCommandAlias(_T("AllBlack")),
		0,
		AllBlack);
	//7:选择对象变为黑白色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelectBlack"),
		GetCommandAlias(_T("SelectBlack")),
		0,
		SelectBlack);
	//8:统一为选择的颜色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("AllSelColor"),
		GetCommandAlias(_T("AllSelColor")),
		0,
		AllSelColor);
	//9:选择对象转换为选择的颜色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelSelColor"),
		GetCommandAlias(_T("SelSelColor")),
		0,
		SelSelColor);
	//14:长度统计
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("calcLength"),
		GetCommandAlias(_T("calcLength")),
		0,
		calcLength);
	//15:自相交检测
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelfIntersect"),
		GetCommandAlias(_T("SelfIntersect")),
		0,
		SelfIntersect);
	//16:删除同类对象
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("DelSameEntity") , 
		GetCommandAlias(_T("DelSameEntity")) ,
		0 ,
		DelSameEntity);
	//17:选中同类对象
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("SelSameEntity") , 
		GetCommandAlias(_T("SelSameEntity")) ,
		0 ,
		SelSameEntity);
	//24:块统计
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("CountBlock") , 
		GetCommandAlias(_T("CountBlock")) ,
		0 ,
		CountBlock);
	//25:删除超短线
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("DelShortLine") , 
		GetCommandAlias(_T("DelShortLine")) ,
		0 ,
		DelShortLine);
	//26:多段线抽稀
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("ThinLine") , 
		GetCommandAlias(_T("ThinLine")) ,
		0 ,
		ThinLine);
	//27:多段线删点
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("RemovePLinePoint") , 
		GetCommandAlias(_T("RemovePLinePoint")) ,
		0 ,
		RemovePLinePoint);
	//28:Z轴规零
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("ZToZero") , 
		GetCommandAlias(_T("ZToZero")) ,
		0 ,
		ZToZero);
	/*
	#if _MSC_VER > 1300
	//29:文字转线
	acedRegCmds->addCommand(_T("CADCommTools") ,
	_T("TextToPline") , 
	GetCommandAlias(sys_cmds,_T("TextToPline")) ,
	ACRX_CMD_NOBEDIT,
	TextToPline);
	#else
	acedRegCmds->addCommand(_T("CADCommTools") ,
	_T("TextToPline") , 
	GetCommandAlias(sys_cmds,_T("TextToPline")) ,
	0,
	TextToPline);
	#endif
	*/


	//30:命令别名
	//移动至系统配置中，不再使用
	//acedRegCmds->addCommand(_T("CADCommTools") ,
	//	_T("AssistantCommandAlias") , 
	//	_T("AssistantCommandAlias") ,
	//	0,
	//	AssistantCommandAlias);
	//31:将直线/圆弧/多段线连接起来
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("JoinLines") , 
		GetCommandAlias(_T("JoinLines")) ,
		0,
		JoinLines);
	//32:删除重复点
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("RemoveSamePts") , 
		GetCommandAlias(_T("RemoveSamePts")) ,
		0,
		RemoveSamePts);
	//33:曲线重叠检测
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("LineCross") , 
		GetCommandAlias(_T("LineCross")) ,
		0,
		LineCross);
	//34:插入Excel表格
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssInsertXls") , 
		GetCommandAlias(_T("AssInsertXls")) ,
		0,
		InsertXls);
	//35:导出Excel表格
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssToExcel") , 
		GetCommandAlias(_T("AssToExcel")) ,
		0,
		ToExcel);
	//36:一键清理
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssPurge") , 
		GetCommandAlias(_T("AssPurge")) ,
		0,
		PurgeEx);

	
	//39:简单选择，根据图层和实体类型选择对象
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("SimpleSel") , 
		GetCommandAlias(_T("SimpleSel")) ,
		0,
		SimpleSel);
	//40:批量文件清理
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("BatchPurge") , 
		GetCommandAlias(_T("BatchPurge")) ,
		0,
		BatchPurge);
	//41:鼠标预览
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("MousePreview") , 
		GetCommandAlias(_T("MousePreview")) ,
		0,
		MousePreview);
	//42:统一文件版本
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("SameDwgVer") , 
		GetCommandAlias(_T("SameDwgVer")) ,
		0,
		SameDwgVer);
	//43:软件注册
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("AssReg"),
		_T("AssReg"),
		0,
		AssReg);
	//44:图纸防修改
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("ProtectEnts"),
		GetCommandAlias(_T("ProtectEnts")),
		0,
		ProtectEnts);
	//45:插入二维码
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("QrCode"),
		GetCommandAlias(_T("QrCode")),
		0,
		QrCode);
	//46:插入条形码
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("BarCode"),
		GetCommandAlias(_T("BarCode")),
		0,
		BarCode);
	
	//50:实体对齐
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("EntityAlign"),
		GetCommandAlias(_T("EntityAlign")),
		0,
		EntityAlign);
	//51:对象均布
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("EntAverPos"),
		GetCommandAlias(_T("EntAverPos")),
		0,
		EntAverPos);
	
	//70:按颜色选择
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelectByColor"),
		GetCommandAlias(_T("SelectByColor")),
		0,
		SelectByColor);
	//71:按颜色选择-多色
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelectByMultColor"),
		GetCommandAlias(_T("SelectByMultColor")),
		0,
		SelectByMultColor);
	//72:表格对齐
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("TableAlign"),
		GetCommandAlias(_T("TableAlign")),
		0,
		TableAlign);
	//77:定位当前文件
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("SelCurFile"),
		GetCommandAlias(_T("SelCurFile")),
		0,
		SelCurFile);
	//78:临时隐藏
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("TempHide"),
		GetCommandAlias(_T("TempHide")),
		0,
		TempHide);
	//79:恢复显示
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("RoolbackShow"),
		GetCommandAlias(_T("RoolbackShow")),
		0,
		RoolbackShow);
	//80:删除完全重叠的线+完全被压盖的线
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("DeleteCrossLine"),
		GetCommandAlias(_T("DeleteCrossLine")),
		0,
		DeleteCrossLine);



	//82:导出坐标点
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_ExportPoints"),
		GetCommandAlias(_T("cadgj_ExportPoints")),
		0,
		ExportPoints);

	//83:导入坐标点
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_ImportPoints"),
		GetCommandAlias(_T("cadgj_ImportPoints")),
		0,
		ImportPoints);

	//84:导出标注
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_ExportDims"),
		GetCommandAlias(_T("cadgj_ExportDims")),
		0,
		ExportDims);

	//85:两点成线
	//两个开口的线，你开发两个功能，一个是两点成线，一个是四个点成两条线
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_LDCX"),
		GetCommandAlias(_T("cadgj_LDCX")),
		0,
		LDCX);
	//86:两点成四边形
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_LDCSBX"),
		GetCommandAlias(_T("cadgj_LDCSBX")),
		0,
		LDCSBX);
	//87:面积统计-剪贴板
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("CalcArea1"),
		GetCommandAlias(_T("CalcArea1")),
		0,
		calcArea1);

	//88:长度统计-剪贴板
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("calcLength1"),
		GetCommandAlias(_T("calcLength1")),
		0,
		calcLength1);

	

	//91:直线交点打断
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_ZXJDDL"),
		GetCommandAlias(_T("cadgj_ZXJDDL")),
		0,
		ZXJDDL);
	//103:自由矩形
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_zyjx"),
		GetCommandAlias(_T("cadgj_zyjx")),
		0,
		zyjx);
	//104:临时隐藏其它
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("TempHideOther"),
		GetCommandAlias(_T("TempHideOther")),
		0,
		TempHideOther);

	//108:图层特性匹配
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("layermatchprop"),
		GetCommandAlias(_T("layermatchprop")),
		0,
		LayerMatchprop);
	//109:类型特性匹配
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("typematchprop"),
		GetCommandAlias(_T("typematchprop")),
		0,
		TypeMatchprop);

	//110:图纸合并
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("cadgj_tzhb"),
		GetCommandAlias(_T("cadgj_tzhb")),
		0,
		tzhb);

	//121:填充统计
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("HatchStatistics"),
		GetCommandAlias(_T("HatchStatistics")),
		ACRX_CMD_MODAL|ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW,
		HatchStatistics);
	//
	/*帮助类*/
	//官网CAD工具之家
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssistantWebSite") , 
		_T("AssistantWebSite") ,
		0 ,
		AssistantWebSite);
	//论坛求助
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("BBSSeekhelp") , 
		_T("BBSSeekhelp") ,
		0 ,
		BBSSeekhelp);
	//报告错误
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssistantError") , 
		_T("AssistantError") ,
		0 ,
		AssistantError);
	//功能建议
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssistantSuggest") , 
		_T("AssistantSuggest") ,
		0 ,
		AssistantSuggest);
	//使用说明
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssistantDoc") , 
		GetCommandAlias(_T("AssistantDoc")) ,
		0 ,
		AssistantDoc);
	//使用说明
	acedRegCmds->addCommand(_T("CADCommTools") ,
		_T("AssistantUpdate") , 
		_T("AssistantUpdate") ,
		0 ,
		AssistantUpdate);
#if _MSC_VER<=1300
	//格式转换(说明AutoCAD2006及以下版本不支持)
	acedRegCmds->addCommand(_T("CADCommTools"),
		_T("DwgTrans"),
		_T("GSZH"),
		ACRX_CMD_MODAL,
		GSZH);
#endif

}
BOOL IsValidRegKey(CString& strRegDate,CString strKey);
//功能:	指定的arx是否已经加载
//参数:	strArxFileName,arx文件名，绝对路径
//返回:	arx是否已经加载
BOOL IsArxLoaded(const CString strArxFileName)
{
	TCHAR lpName[MAX_PATH];
	lstrcpy(lpName,PathFindFileName(strArxFileName));
	BOOL bLoad=FALSE;
	struct resbuf * LoadArxs=acedArxLoaded();//已经加载的arx文件
	if(LoadArxs!=NULL)
	{
		struct resbuf * LoadArxNext=LoadArxs->rbnext;//遍历LoadArxs
		while(LoadArxNext!=NULL)
		{
			if(lstrcmpi(LoadArxNext->resval.rstring,lpName)==0)
			{
				bLoad=TRUE;
				break;
			}
			LoadArxNext=LoadArxNext->rbnext;
		}
	}
	return bLoad;
}
extern void InitSpecialRegItemArray();
void InitApplication()
{
	//#ifdef _DEBUG
	//	AfxMessageBox(_T("Test"));
	//#endif
	if(GlobalSystemValue::instance.IsHaoBangshou())
	{
		acutPrintf(_T("\n欢迎使用CAD好帮手，作者boitboy,官方网站:www.cadgj.com"));
		GLog::WriteLine(_T("欢迎使用CAD好帮手，作者boitboy,官方网站:www.cadgj.com"));
		GLog::WriteLine(_T("您使用的CAD版本是:")+GetCadProductName());
	}
	else if(GlobalSystemValue::instance.IsZhongXianCAD())
	{
		acutPrintf(_T("\n欢迎使用中线CAD，官方网站:www.xianshu.cn，热线电话400-800-2007"));
	}
	else if(GlobalSystemValue::instance.IsXiaoZhuShou())
	{
		acutPrintf(_T("\n欢迎使用CAD小助手，官方网站:cadgj12345.ys168.com"));
	}
	Glb_bReg=FALSE;
	Glb_strRegDate=_T("");
	TCHAR m_lpFile[MAX_PATH+1]={0};
	HMODULE hModule=::GetModuleHandle(_T("CADCommTools.arx"));
	if(hModule==NULL)
	{
		hModule=_hdllInstance;
	}
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("Licence.ini"));
	WritePrivateProfileString(_T("System"),_T("RegDate"),_T("20141127"),m_lpFile);
	Glb_strRegDate=_T("20141127");
	Glb_bReg=TRUE;
	InitSpecialRegItemArray();
	loadmenu();
	ReRegCmds();
	Glb_MousePreviewDocReactor=new GMousePreviewDocReactor;
	InitXDataDynView();
	/*帮助类结束*/
	CallUpdate(true);
	/*
	if(!IsArxLoaded(_T("RegionSerach.arx")))
	{
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("RegionSerach.arx"));
	acedArxLoad(m_lpFile);
	}
	*/


}
void UnloadApplication()
{
	acedRegCmds->removeGroup(_T("CADCommTools"));
	//07下关闭崩溃，暂时屏蔽
	//unloadmenu(_T("CAD_GOOD_ASSISTANT"));
	delete Glb_MousePreviewDocReactor;
	Glb_MousePreviewDocReactor=NULL;
	destroyDlgEntAlign();
	destroyDlgEntAverPos();
#ifndef ACAD_2004
	destroyDlgTableAlign();
#endif
}
//获取命令的别名
CString Sys_GetCommandAlias(CString strCmd)
{
	return GetCommandAlias(strCmd);
}

//获取广告的md5
inline BOOL GetAdMD5(AcArray<byte>& byteArray)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	CFile file;
	if(!file.Open(lpPath,CFile::modeRead|CFile::typeBinary))
	{
		return FALSE;
	}
	DWORD dLen=file.GetLength();
	if(dLen==0)
	{
		file.Close();
		return FALSE;
	}
	byte* bytes=new byte[dLen];
	file.Read(bytes,dLen);
	file.Close();
	MD5 md5(bytes,dLen);
	std::string str=md5.toString();
	for(int i=0;i<str.length();i++)
	{
		byteArray.append(str.at(i));
	}
	delete[] bytes;
	bytes=NULL;
	return TRUE;
}
inline BOOL GetVerifyString(CString& strVfy)
{
	AcArray<byte> byteArray;
	if(!GetAdMD5(byteArray))
	{
		return FALSE;
	}
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	GDES::TransformData(byteArray,TRUE);
	CByteString bStr;
	strVfy=bStr.toString(byteArray);
	return TRUE;
}

inline BOOL VerifyAd()
{
	CString strVfy;
	if(!GetVerifyString(strVfy))
	{
		return FALSE;
	}
	TCHAR lpPath[MAX_PATH+1]={0};
	HMODULE hModule=_hdllInstance;
	GetModuleFileName(hModule,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("CADCommTools.ini"));
	TCHAR lpResult[128+1]={0};
	if(!GetPrivateProfileString(_T("System"),_T("VfyStr"),_T(""),lpResult,128,lpPath))
	{
		return FALSE;
	}
	if(strVfy.Compare(lpResult)==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADCommToolsApp : public AcRxArxApp {

public:
	CCADCommToolsApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		// TODO: Add your initialization code here
		TCHAR m_path[MAX_PATH+1]={0};
		::GetModuleFileName(m_hdllInstance, m_path, MAX_PATH);
		PathRemoveFileSpec(m_path);
		Glb_SoftPath=m_path;
#ifndef _ARXDEBUG
		if(GlobalSystemValue::instance.IsHaoBangshou())
		{
			InitApplication();
		}
		else
		{
			InitApplication();
		}
#else
		InitApplication();
#endif
		m_pCommandReactor=new GCommandReactor(false);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		UnloadApplication();
		m_pCommandReactor->Detach();
		removeSSgetFilterInputContextReactor(curDoc(), m_pCommandReactor->m_pFilter);
		delete m_pCommandReactor;
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADCommToolsApp)

