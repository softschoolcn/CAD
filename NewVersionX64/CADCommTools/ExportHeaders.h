#pragma once
#ifdef CADCOMMTOOLS_MODULE 
#define CADCOMMTOOLS_EXT __declspec(dllexport)
#pragma message("导出CADCommTools.lib……")
#else 
#define CADCOMMTOOLS_EXT __declspec(dllimport)
#pragma message("导入CADCommTools.lib……")
#endif
//检查软件注册
CADCOMMTOOLS_EXT BOOL CheckReg();
//检查专业功能注册
CADCOMMTOOLS_EXT BOOL CheckSpecialReg(CString strKey);
//获取命令的别名
CADCOMMTOOLS_EXT CString Sys_GetCommandAlias(CString strCmd);
CADCOMMTOOLS_EXT void Exp_JoinLines(AcDbObjectIdArray& ids);
Acad::ErrorStatus CADCOMMTOOLS_EXT GetBorderPts(AcGePoint3dArray& pts,AcDbText* pText);
Acad::ErrorStatus CADCOMMTOOLS_EXT GetBorderPts(AcGePoint3dArray& pts,AcDbMText* pText);