#pragma once
#ifdef CADCONFIGTOOLS_MODULE 
#define CONFIGTOOLS_EXT __declspec(dllexport)
#pragma message("导出CADConfigTools.lib……")
#else 
#define CONFIGTOOLS_EXT __declspec(dllimport)
#pragma message("导入CADConfigTools.lib……")
#endif

//根据配置修改标注样式,返回标注样式的ID
//bCreateNew=false,没有则不创建，不更新
//bCreateNew=true,没有则创建，更新
Acad::ErrorStatus CONFIGTOOLS_EXT UpdateDimStyleByConfig(AcDbObjectId&   rcId,bool bCreateNew=false,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
//根据配置修改图层,返回图层的ID
//bCreateNew=false,没有则不创建，不更新
//bCreateNew=true,没有则创建，更新
Acad::ErrorStatus CONFIGTOOLS_EXT UpdateLayerByConfig(AcDbObjectId&   rcId,CString strName,bool bCreateNew=false,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase());
//获取配置的命令别名
CString CONFIGTOOLS_EXT GetCommandAlias(CString strCmd);