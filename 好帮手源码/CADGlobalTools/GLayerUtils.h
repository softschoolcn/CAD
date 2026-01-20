#pragma once

class GLOBALTOOLS_EXT GLayerUtils
{
public:
	GLayerUtils(void);
public:
	~GLayerUtils(void);
	//Ã¶¾ÙÍ¼²ã
	static Acad::ErrorStatus EnumAllLayer(AcDbObjectIdArray& layeIds,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
	//Ã¶¾ÙÍ¼²ã
	static Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
	//Ã¶¾ÙÍ¼²ã
	static Acad::ErrorStatus EnumAllLayer(CStringArray& layeNames,AcDbObjectIdArray& layeIds,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
};
