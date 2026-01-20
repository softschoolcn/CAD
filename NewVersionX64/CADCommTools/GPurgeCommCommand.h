//清理类命令
#pragma once
#include "GlobalDefine.h"
//一键清理
void PurgeEx();
//批量文件清理
void BatchPurge();
void PurgeMsgOutput(CString strMsg,void*);
//清理数据库
//pDb,需要清理的数据库
//bPrompt,提示
Acad::ErrorStatus PurgeDatabase(AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase(),MSGOUTPUTPROC pMsgProc=PurgeMsgOutput,void* pArg=NULL);
//统一文件版本
void SameDwgVer();
