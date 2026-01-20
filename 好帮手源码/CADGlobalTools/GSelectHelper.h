#pragma once
//选择辅助类
class GLOBALTOOLS_EXT GSelectHelper
{
public:
	GSelectHelper(void);
public:
	~GSelectHelper(void);
public:
	//设置实体的选中状态
	//参数：ObjId,实体的ObjectId
	//参数:Clear,是否清空当前选择集（默认清空）,不清空暂时无法实现
	static void SetEntitySelected(AcDbObjectId ObjId,bool Clear);
	//设置实体为选中状态
	static void SetEntitySelected(AcDbObjectIdArray& ObjIds,bool Clear);
};
