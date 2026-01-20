#pragma once
#include "CmpDiffFinder.h"
#include "AcProgressBar.h"
class CmpBTRDiffFinder:public CmpDiffFinder
{
public:
	CmpBTRDiffFinder(CmpConfig* pConfig,AcDbBlockTableRecord* pBTR1,AcDbBlockTableRecord* pBTR2);
	~CmpBTRDiffFinder(void);
	virtual Acad::ErrorStatus Find(CArray<CmpDiffResult>& diffResults,long& diffCount);
	AcDbObjectIdArray m_ingoreLayerIdArray[2];
protected:
	Acad::ErrorStatus ReadEntityDat(CmpEntityDat* dat,AcDbObjectId& id,int nIndex);
	virtual Acad::ErrorStatus Find(CArray<CmpDiffResult>& diffResults,long& diffCount,CmpDiffFinder& finder,int nIndex);
	Acad::ErrorStatus SplitByLen(CArray<CmpDiffResult>& diffResults,long& diffCount,CmpDiffFinder& finder1);
	AcDbBlockTableRecord* m_pBTR1;
	AcDbBlockTableRecord* m_pBTR2;
	AcProgressBar         m_progressBar;
};
