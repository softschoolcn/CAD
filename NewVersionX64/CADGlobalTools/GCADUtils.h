#pragma once
//CAD工具类
class GLOBALTOOLS_EXT GCADUtils
{
public:
	GCADUtils(void);
public:
	~GCADUtils(void);
	//获取中点
	static AcGePoint3d MidPoint(AcGePoint3d pt1,AcGePoint3d pt2);
	static AcGePoint2d MidPoint(AcGePoint2d pt1,AcGePoint2d pt2);
	//获取文字内容
	static CString     GetText(AcDbEntity* pEnt);
	static CString     GetText(AcDbText* pText);
	static CString     GetText(AcDbMText* pMText);
	static CString     GetMText(AcDbMText* pMText);
	// 功能：将选择集转换为实体ID数组
	// 参数：	 ObjIds,实体ID数组
	// ssName,选择集
	// 返回：
	static void SSToIds(AcDbObjectIdArray& ObjIds,ads_name ssName);
	//枚举字体样式表
	static Acad::ErrorStatus EnumTextStyles(AcDbObjectIdArray& ids,CStringArray& names,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
	//获取当前文档的dwg文件名
	static BOOL GetCurDwgFileName(CString& strDwgFileName);
	//给CAD中的对象做一个标记
	static Acad::ErrorStatus MarkObject(AcDbObject* pObj,CString strKey,AcDbDatabase* pDb=NULL,bool bMark=true);
	static Acad::ErrorStatus MarkObject(AcDbObjectId objId,CString strKey,bool bMark=true);
	//是否是被标记的对象
	static Acad::ErrorStatus IsMarkedObject(AcDbObjectId objId,CString strKey,bool& bMark);
	static Acad::ErrorStatus IsMarkedObject(AcDbObject* pObj,CString strKey,bool& bMark);
	//点是否在范围内
	static bool IsPointInExtents(AcGePoint3d& pt,AcDbExtents& ext);
	//范围1是否在范围2内
	static bool IsExtInExtents(AcDbExtents& ext1,AcDbExtents& ext2);
	//范围1和范围2是否相交
	static bool IsExtIntersect(AcDbExtents& ext1,AcDbExtents& ext2);
	//获取图层名称
	static Acad::ErrorStatus GetLayerName(CString& strLayerName,AcDbObjectId layerId);
	static void dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr);
	static int FindObjectId(CArray<AcDbObjectId>& xIdArr,AcDbObjectId id);
	static int FindObjectId(AcDbObjectIdArray& xIdArr,AcDbObjectId id);
	static AcGePoint3d WCS2UCS(AcGePoint3d wcsPnt);
	static AcGePoint2d WCS2UCS(AcGePoint2d wcsPnt);
	static AcGePoint3d UCS2WCS(AcGePoint3d ucsPnt);
	static void UCS2WCS(ads_point ucsPnt);
	static void WCS2UCS(ads_point wcsPnt);
	//功能： 设置int型环境变量
	static Acad::ErrorStatus SetCADIntVar(CString sMode,int nRet);
	//功能： 获取int型环境变量
	static Acad::ErrorStatus GetCADIntVar(CString sMode,int &nRet);
	//删除实体
	static	Acad::ErrorStatus EraseEntity(const AcDbObjectId objId);
	//强制删除实体
	static	Acad::ErrorStatus ForceEraseEntity(const AcDbObjectId objId);
	// 功能：添加实体到数据库中
	// 参数：	  pEnt：待添加的实体指针
	//			  pDB：接收实体的数据库		
	// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
	static	AcDbObjectId AddToCurrentSpace(AcDbEntity* pEnt,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	// 功能：添加实体到模型空间
	// 参数：	  pEnt：待添加的实体指针
	//			  pDB：接收实体的数据库		
	// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
	static	AcDbObjectId AddToModelSpace(AcDbEntity* pEnt,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	//获取最大化视口范围
	static bool GetMaxViewPoint(AcDbObjectIdArray& ObjIdArray,AcGePoint3d& ptMin, AcGePoint3d& ptMax);
	static Acad::ErrorStatus OpenAllLayer(AcDbDatabase* pDb,AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
	static Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens);
	//写入扩展数据
	static	Acad::ErrorStatus WriteXData(AcDbObjectId objId,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray);
	static	Acad::ErrorStatus WriteXData(AcDbObject* pObj,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray);
	//读取扩展数据
	static	Acad::ErrorStatus ReadXData(AcDbObjectId objId,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray);
	static	Acad::ErrorStatus ReadXData(AcDbObject* pObj,CString strName,CStringArray& strKeyArray,CStringArray& strValueArray);
	//获取绘图比例和出图比例
	static	Acad::ErrorStatus GetScale(int& nHTBL,int& nCTBL,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	//设置绘图比例和出图比例
	static	Acad::ErrorStatus SetScale(int& nHTBL,int& nCTBL,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	//获取图层的ID
	static Acad::ErrorStatus GetLayerId(AcDbObjectId& layerId,const CString strLayerName,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
	// 查找CAD实体线型
	static Acad::ErrorStatus GetLineTypeId(AcDbObjectId& resultId,const CString strLnTypeName,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	// 加载CAD实体线型
	static Acad::ErrorStatus LoadLineType(AcDbObjectId& resultId,const CString strLnTypeName,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
	//获取当前选择对象
	static void GetCurrentSelectObjIds(AcDbObjectIdArray& objIds,struct resbuf* rb=NULL);
	//设置为相同的属性
	//将pTargetEnt设置为和pSourceEnt设置为一样的属性
	static void SameEntityAttr(AcDbEntity* pTargetEnt,AcDbEntity* pSourceEnt);
	// 功能：添加实体到块表记录中
	// 参数：	  pEnt：待添加的实体指针
	//			  btrID,AcDbBlockTableRecord的ID		
	// 返回：     实体的ObjectID,如果插入失败返回一个空的ObjectId
	static AcDbObjectId AppendEntity(AcDbEntity* pEnt,const AcDbObjectId btrID);
private:
	static int dxfCodeToDataType(int resType);
	static LPCTSTR bytesToHexStr(char* buffer, int len, CString& hexStr);
	static Acad::ErrorStatus UnLockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen);
	static Acad::ErrorStatus LockLayer(const AcDbObjectId layerId,bool& bLock,bool& bFrozen);
	static Acad::ErrorStatus RegisterApp(AcDbDatabase* pDb, const TCHAR * pAppName);
};
