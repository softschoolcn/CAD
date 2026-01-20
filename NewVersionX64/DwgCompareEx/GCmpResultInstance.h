#pragma once
#include "CmpDefine.h"
//记录比较结果的单例
class GCmpResultInstance
{
public:
	~GCmpResultInstance(void);
	//获取结果数量
	int GetResultCount();
	//获取特定结果
	CmpResult* GetResult(int nIndex);
	//删除所有结果
	void DeleteAllResult();
	//增加结果
	void AppendResult(CmpResult* pRC);
	//获取当前结果
	CmpResult* GetCurrentResult();
	//设置当前结果
	void SetCurrentResult(CmpResult* pRC);
	//设置当前结果索引
	void SetCurrentResultIndex(int nIndex);
	//唯一单例
	static GCmpResultInstance instance;
	//检查文档
	BOOL CheckDocument();
	BOOL CheckDocument(CmpResult* pRC);
protected:
	GCmpResultInstance(void);
	
private:
	CArray<CmpResult*> mp_ResultArray;
	CmpResult* mp_CurResult;
};
