#pragma once
//曲线重叠检测
void LineCross();
//删除完全重叠的线
void DeleteCrossLine();
struct LineCrossResultItem
{
	AcDbObjectId id1;
	AcDbObjectId id2;
	long rc;
};
//85:两点成线
//两个开口的线，你开发两个功能，一个是两点成线，一个是四个点成两条线
void LDCX();
//86:两点成四边形
void LDCSBX();
//91:直线交点打断
void ZXJDDL();
//103:自由矩形
void zyjx();
