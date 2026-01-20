#pragma once
//1:选择所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus GLOBALTOOLS_EXT SelectAllEntity(AcDbObjectIdArray& ids);
//2:选择某一个图层上的所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus GLOBALTOOLS_EXT SelectAllEntityInLayer(AcDbObjectIdArray& ids,const CString strLayerName);
//3:选择某一个图层上的所有的实体(类似于使用Ctrl+A选择)
Acad::ErrorStatus GLOBALTOOLS_EXT SelectAllEntityInLayer(AcDbObjectIdArray& ids,AcDbObjectId layerId);
////////////////////////////////////////////////////////在块定义中遍历///////////////////////////////////////////////////////////////////////////
//1:枚举块定义上的的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbBlockTableRecord* pBTR);
//2:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbBlockTableRecord* pBTR);
//3:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,const CString strLayerName,AcDbBlockTableRecord* pBTR);
//4:枚举块定义上的的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId btrId);
//5:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,const CString strLayerName,AcDbObjectId btrId);
//6:枚举块定义某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInBTR(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbObjectId btrId);
////////////////////////////////////////////////////////在数据库中枚举///////////////////////////////////////////////////////////////////////////
//1:枚举所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntity(AcDbObjectIdArray& ids,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//2:枚举某一个图层上的所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInLayer(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//3:枚举某一个图层上的所有实体(包括冻结图层,锁定图层的,模型空间的,布局空间的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInLayer(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//4:枚举模型空间上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//5:枚举模型空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//6:枚举模型空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInModelSpace(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//7:枚举当前空间上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//8:枚举当前空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,const CString strLayerName,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
//9:枚举当前空间上某一个图层上的所有实体(包括冻结图层,锁定图层的)
Acad::ErrorStatus GLOBALTOOLS_EXT EnumAllEntityInCurrentSpace(AcDbObjectIdArray& ids,AcDbObjectId layerId,AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase());
