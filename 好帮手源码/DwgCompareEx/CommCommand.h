#pragma once
#include "CmpExcute.h"
void ShowReportPanel(BOOL bShow = TRUE);
void ShowHidePanel();
//nMode=0,正常模式
//nMode=0,较小模式，用于对比查看单个文件
void SetReportPanelSizeMode(int nMode);
void ShowCmpResult(int nMode,bool bReg);
void ShowCmpResultEx(int nMode,bool bReg);
void SetEntityColor(AcDbEntity* pEnt,int nColorIndex,GColorRecord& cr);
void SetEntityColor(AcDbEntity* pEnt,GColorRecord& cr);
void SetEntityColor(const AcDbObjectId id,int nColorIndex,GColorRecord& cr);
void SetEntityColor(const AcDbObjectId id,GColorRecord& cr);
int dxfCodeToDataType(int resType);
LPCTSTR bytesToHexStr(char* buffer, int len, CString& hexStr);
void dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr);
CString dxfToStr(const resbuf* rb);
CString dxfToStr(CArray<struct resbuf*>& rbs,CStringArray& strValueArray);
void SSToIds(ads_name ssName,AcDbObjectIdArray& ObjIds);
//设置实体的选中状态
//参数：ObjId,实体的ObjectId
//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
void SetEntitySelected(AcDbObjectId ObjId,bool Clear);
void SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear);
//功能:	设置视图（相当于Zoom Window命令）
//参数:	Pt1		视图左上角点
//			Pt2		视图右下角点
//			ex_ratio	扩展比率,一般为1.0
Acad::ErrorStatus view_set(AcGePoint3d Pt1,AcGePoint3d Pt2,double ex_ratio=1.0);
bool DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio=1.0);
bool DyZoom(const AcDbObjectId ObjId,double ex_ratio=1.0);
Acad::ErrorStatus objIdToHandle(CString& strHandle,AcDbObjectId objId);
//强制删除实体
	Acad::ErrorStatus ForceEraseEntity(const AcDbObjectId objId);
	void GetCurrentSelectObjIds(AcDbObjectIdArray& objIds);

