#pragma once
#include <string.h>
using namespace std;
//获取样条曲线的拟合点
bool getSplineSamplePoints(AcDbSpline *&spline, AcGePoint3dArray &pnts);
AcGePoint2d convert3dPointTo2d(AcGePoint3d pt);
//获取三个点构成的夹角
double GetIntersectionAngle(AcGePoint2d pt1,AcGePoint2d pt2,AcGePoint2d pt3);
//获取三个点构成的夹角
double GetIntersectionAngle(AcGePoint3d pt1,AcGePoint3d pt2,AcGePoint3d pt3);
//删除构成直线的三个点中间的点
//dMinAngle,构成的夹角最小值,默认为0.5度
void RemovePointBetweenLine(AcGePoint3dArray &pnts,double dMinAngle=PI-0.5*PI/180.0);
// 功能：添加实体到块表记录中
// 参数：	  pEnt：待添加的实体指针
//			  btrID,AcDbBlockTableRecord的ID		
// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
AcDbObjectId AppendEntity(AcDbEntity* pEnt,const AcDbObjectId btrID);
// 功能：添加实体到数据库中
// 参数：	  pEnt：待添加的实体指针
//			  pDB：接收实体的数据库		
// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
AcDbObjectId AddToCurrentSpace(AcDbEntity* pEnt,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
//功能:	激活当前视图
Acad::ErrorStatus activeCurrentview();
//获取当前图层
Acad::ErrorStatus getCurrentLayer(CString &strLayerName);
//获取图层的ID
Acad::ErrorStatus GetLayerId(AcDbObjectId& layerId,const CString strLayerName,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//获取图层名称
Acad::ErrorStatus GetLayerName(CString& strLayerName,AcDbObjectId layerId);
//获取实体图层ID
Acad::ErrorStatus GetEntityLayerId(AcDbObjectId& layerId,AcDbObjectId entId);
//获取多行文字中的文字内容
CString GetMText(AcDbMText* pText);
//设置为相同的属性
//将pTargetEnt设置为和pSourceEnt设置为一样的属性
void SameEntityAttr(AcDbEntity* pTargetEnt,AcDbEntity* pSourceEnt);
bool RgnIntersect(double& dScale,AcGePoint3dArray& pts1,AcGePoint3dArray& pts2);
//设置实体的选中状态
//参数：ObjId,实体的ObjectId
//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
void SetEntitySelected(AcDbObjectId ObjId,bool Clear);
//设置实体为选中状态
void SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear);
bool GetMaxExtPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax);
bool DyZoom(AcDbObjectIdArray& ObjIdArray,double ex_ratio);
void GetEntVertPntsAndBlgs(AcDbObjectId CurId,AcGePoint3dArray &VertPnts,AcGeDoubleArray &VertBlgs);
BOOL JudgeCureIsSelInterSect(const AcGePoint3dArray &VertPnts,const AcGeDoubleArray &ptBlgs,AcGePoint2dArray &ptBacks);
Acad::ErrorStatus DyZoom(AcGePoint2d& CenterPt,double dHeight,double dWidth);
//枚举图层
Acad::ErrorStatus EnumAllLayer(AcDbObjectIdArray& layeIds,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//枚举图层
Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//枚举图层
Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbObjectIdArray& layeIds,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//调用自动更新
//bSys=true,系统调用,初次打开的时候调用,如果是最新版本不提示
//bSys=false,用户调用
void CallUpdate(bool bSys);
//点是否在范围内
bool IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext);
AcGePoint3d MinPoint(AcDbExtents& ext);
//功能:	将点从块引用里面经过矩阵转换转换到块外面(不考虑旋转)
//参数:	pRef,块引用
//      pt,点
//返回:	转换结果
Acad::ErrorStatus TransPointOutBlockRef(const AcDbBlockReference* pRef,AcGePoint3d& pt);
Acad::ErrorStatus EraseEntity(const AcDbObjectId objId);
Acad::ErrorStatus EraseBlockTableRecord(AcDbBlockTable* pTable,const CString strName);
Acad::ErrorStatus EraseBlockTableRecord(AcDbObjectId btrId);
Acad::ErrorStatus EraseBlockTableRecord(AcDbBlockTable* pTable,const CString strName);
//功能： 设置int型环境变量
Acad::ErrorStatus SetCADIntVar(CString sMode,int nRet);
//功能： 获取int型环境变量
Acad::ErrorStatus GetCADIntVar(CString sMode,int &nRet);
//计算三角形的面积
double CalcTriangleArea(AcGePoint3d ptA,AcGePoint3d ptB,AcGePoint3d ptC);
//点是否在面域中(暂时只建议计算四个点构成的面域)
bool IsPointInRegion(AcGePoint3d pt,AcGePoint3dArray& pts);
LPCTSTR  objToHandleStr(const AcDbObject* obj, CString& str);
//功能:判断字符串是否在字符串数组中
//参数:	array:字符串数组
//			string:字符串
//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
//返回:	true:在数组中；false:不在
bool IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase=true);
/*!
@zhangshl 生成GUID
*/
CString NewGuid();
string BSTRToString(const BSTR bstr);
CString IntToString(const int nValue);
CString DoubleToString(const double dValue);
//ANSI字符串转换为宽字符串
LPWSTR ConvertToLPWSTR(LPCSTR szString);
//获取CAD产品名称
CString GetCadProductName();
//获取块名称
CString GetBlockName(AcDbBlockTableRecord* pRec);
CString GetBlockName(AcDbObjectId objId);
CString GetBlockName(AcDbBlockReference* pRef);
//功能:将字符串数组连成一个字符串
CString CombineArrayToString(const CStringArray& array,CString linkSymbol);
//功能，保存CAD数据库
//pDb,数据库
//lpFileName,文件名称，如果为NULL，表示按原有的文件名称保存
//ver,保存的DWG的版本号,AcDb::kDHL_Unknown,表示当前DWG文件是什么版本就保存成什么版本
//pSecParams,加密参数
Acad::ErrorStatus SaveDatabase(AcDbDatabase* pDb,const TCHAR* lpFileName=NULL,AcDb::AcDbDwgVersion ver=AcDb::kDHL_Unknown,const SecurityParams* pSecParams=NULL);
//强制删除实体
Acad::ErrorStatus ForceEraseEntity(const AcDbObjectId objId);
//选择范围
bool WindowSelect(AcGePoint3d& ptMin,AcGePoint3d& ptMax);
//获取绝对的颜色号
int GetAbsColorIndex(AcDbEntity* pEnt);