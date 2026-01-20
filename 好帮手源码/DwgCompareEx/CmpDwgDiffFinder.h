#pragma once
#include "CmpBTRDiffFinder.h"
class CmpDwgDiffFinder
{
public:
	CmpDwgDiffFinder(CmpConfig* pConfig,AcDbDatabase* pDb1,AcDbDatabase* pDb2);
	~CmpDwgDiffFinder();
public:
	Acad::ErrorStatus Find(CmpResult& result,long& diffCount);
	Acad::ErrorStatus FindInSymbolTable(CmpResult& result,long& diffCount);
	Acad::ErrorStatus FindInBT(CmpResult& result,long& diffCount);
	Acad::ErrorStatus FindInBT(CmpResult& result,long& diffCount,AcDbObjectIdArray& BlockIds1,CStringArray& BlockNames1,AcDbObjectIdArray& BlockIds2,CStringArray& BlockNames2);
private:
	AcDbDatabase* m_pDb1;
	AcDbDatabase* m_pDb2;
	AcDbObjectIdArray m_ingoreLayerIdArray[2];
	CmpConfig* m_pConfig;
	Acad::ErrorStatus SplitBTRS(AcDbDatabase* pDb,AcDbObjectId& ModelSpaceId,AcDbObjectIdArray& PaperSpaceIds,AcDbObjectIdArray& BlockDefineIds,CStringArray& BlockDefineNames);
	Acad::ErrorStatus IngoreLayers(AcDbObjectIdArray& ids,AcDbLayerTable* pTable);
	bool IsIngoreLayer(AcDbLayerTableRecord* pRecord);
};