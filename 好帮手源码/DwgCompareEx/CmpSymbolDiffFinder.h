#pragma once
#include "CmpDiffFinder.h"
class CmpSymbolDiffFinder:public CmpDiffFinder
{
public:
	CmpSymbolDiffFinder(CmpConfig* pConfig,AcDbSymbolTable* pTable1,AcDbSymbolTable* pTable2);
	~CmpSymbolDiffFinder(void);
	virtual Acad::ErrorStatus Find(CArray<CmpDiffResult>& diffResults,long& diffCount);
private:
	Acad::ErrorStatus ReadSymbolDat(CmpEntityDat* dat,AcDbObjectId& id);
	AcDbSymbolTable* m_pTable1;
	AcDbSymbolTable* m_pTable2;
};
