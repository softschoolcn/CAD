#pragma once
#include "CmpConfig.h"
#include "CmpDefine.h"
class CmpExcute
{
public:
	CmpExcute(CmpConfig* pConfig);
public:
	~CmpExcute(void);
	Acad::ErrorStatus Excute(long& diffCount);
	void ChangeResultColor();//根据结果改变实体的颜色
	void ChangeResultColor(CArray<CmpDiffResult>&   diffResults,bool bBlock);
	CmpResult m_Result;
private:
	CmpConfig* m_pConfig;
	std::auto_ptr<AcDbDatabase>	m_cmpdb[2];
};
