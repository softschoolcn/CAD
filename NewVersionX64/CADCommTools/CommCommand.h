#pragma once
void SPlineToPLine();
//面积统计
void calcArea();
//长度统计
void calcLength();
//恢复原来颜色
void SameColorRollback();
//统一为黑白色
void AllBlack();
//选择对象变为黑白色
void SelectBlack();
//统一为选择的颜色
void AllSelColor();
//选择对象转换为选择的颜色
void SelSelColor();
//自相交检测
void SelfIntersect();
//选中同类对象
void SelSameEntity();
//删除同类对象
void DelSameEntity();
//官网CAD工具之家
void AssistantWebSite();
//论坛求助
void BBSSeekhelp();
//报告错误
void AssistantError();
//功能建议
void AssistantSuggest();
//使用说明
void AssistantDoc();
//最新版本
void AssistantUpdate();
//块统计
void CountBlock();
//删除超短线
void DelShortLine();
//多段线抽稀
void ThinLine();
//多段线删点
void RemovePLinePoint();
//Z轴规零
void ZToZero();
//30:命令别名
void AssistantCommandAlias();
//将直线/圆弧/多段线连接起来
void JoinLines();
//删除重复点
void RemoveSamePts();
//曲线重叠检测
void LineCross();
//简单选择，根据图层和实体类型选择对象
void SimpleSel();
//鼠标预览
void MousePreview();
//初始化鼠标预览
void InitXDataDynView();
//软件注册
void AssReg();
//图纸防修改
void ProtectEnts();
//插入条形码
void BarCode();
//实体对齐
void EntityAlign();
//对象均布
void EntAverPos();
//表格对齐
void TableAlign();
void destroyDlgEntAlign();
void destroyDlgEntAverPos();
#ifndef ACAD_2004
void destroyDlgTableAlign();
#endif
//按颜色选择
void SelectByColor();
//按颜色选择-多色
void SelectByMultColor();
//选中当前文件
void SelCurFile();
//临时隐藏
void TempHide();
//79:恢复显示
void RoolbackShow();
//82:导出坐标点
void ExportPoints();
//83:导入坐标点
void ImportPoints();
//84:导出标注
void ExportDims();
//87:面积统计-剪贴板
void calcArea1();
//88:长度统计-剪贴板
void calcLength1();
//临时隐藏其它
void TempHideOther();
#include "GQrCodeCommand.h"
//110:图纸合并
void tzhb();

//121:填充统计
void HatchStatistics();