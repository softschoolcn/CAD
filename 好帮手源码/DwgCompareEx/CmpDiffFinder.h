#pragma once
#include "CmpDefine.h"
#include "CmpConfig.h"
#include "AcProgressBar.h"
class CmpDiffFinder
{
public:
	CmpDiffFinder(CmpConfig* pConfig);
public:
	~CmpDiffFinder(void);
	virtual Acad::ErrorStatus Find(CArray<CmpDiffResult>& diffResults,long& diffCount,bool bRelease=true,AcProgressBar* pBar=NULL);
	virtual void Free();
	CString strName;
protected:
	BOOL IsOneObj(CmpEntityDat* dat1,CmpEntityDat* dat2);
	BOOL NeedSwap(CmpEntityDat* dat1,CmpEntityDat* dat2);
	BOOL IsDiffType(AcRxClass* pType1,AcRxClass* pType2);
	BOOL _IsDiff(struct resbuf* rb1,struct resbuf* rb2,BOOL ingoreNoCase=FALSE);
	BOOL IsDiff(CmpItem* pItem,CArray<int>& ingoreDxfCodes,struct resbuf* rb1,struct resbuf* rb2,CmpDiffResult& result);
	BOOL IsDiff(CmpEntityDat* dat1,CmpEntityDat* dat2,CmpDiffResult& result);
	void AddSubEntityToResult(CArray<CmpEntityDat*>& dats,int nType,CArray<CmpDiffResult>& diffResults,long& diffCount);
	void DiffSubEntity(CArray<CmpEntityDat*>& dats1,CArray<CmpEntityDat*>& dats2,CArray<CmpDiffResult>& diffResults,long& diffCount);
	CmpType* GetCmpType(AcRxClass* pType);
	CMap<LPCTSTR,LPCTSTR,CmpType*,CmpType*> m_TypeDict;
	CmpConfig* m_pConfig;
public:
	CArray<CmpEntityDat*>        m_cmpDat[2];
	
};
