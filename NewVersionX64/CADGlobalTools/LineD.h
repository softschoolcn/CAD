#pragma once
#include "CoordinateD.h"
#include "BoxD.h"
class GLOBALTOOLS_EXT LineD
{
	friend class ArcD;
public:
	LineD(void);
	LineD(CoordinateD coord);
	LineD(CoordinateD fromCoord,CoordinateD toCoord);
	LineD(const LineD &m_line);
public:
	//操作符重载
	bool operator ==(LineD & m_line);
	bool operator !=(LineD & m_line);
	LineD& operator = (const LineD& m_line);
public:
	//功能:计算点到直线的距离
	//参数:otherCoord:坐标点
	//返回:点到直线的距离
	double CalcDistance(CoordinateD otherCoord) const;
	//功能:计算直线的两个点和另外一个点构成的三角形的面积
	//参数:thirdCoord:三角形的第三个坐标点
	//返回:三角形的面积
	double CalcArea(CoordinateD thirdCoord) const;
	//功能:获取由起始点连接结束点构成的角度
	//参数:无
	//返回:倾斜角
	double get_angle() const;
	//功能:计算直线和直线之间的交点
	//参数:intersectPoint:交点
	//     otherLine:求交点的直线
	//     beExtern:是否计算延长线
	//返回:0,两个直线之间没有交点
	//     1,两个直线之间有有一个交点
	//     2,两个直线之间有无数个焦点
	long intersectWith(LineD otherLine,CoordinateD &intersectPoint,bool beExtern=false) const;
	//功能:计算直线的标准方程
	//     Ax+By+C=0;
	//参数:
	//返回:无
	void standEquation(double &A,double &B,double &C) const;
	//功能:计算直线的中点
	//参数:无
	//返回:直线的中点
	CoordinateD get_centerPoint() const;
	//功能:计算直线的长度
	//参数:无
	//返回:直线的长度
	double get_length() const;
	//功能:计算关于本直线对称的点
	//参数:coord:参数点
	//返回:对称点
	CoordinateD symmetryPoint(CoordinateD coord);
	//功能:计算垂足
	//参数:coord:参数点
	//     ptOut:返回垂足
	//返回:计算垂足是否成功
	bool plumbPoint(CoordinateD &ptOut,CoordinateD coord);
	//功能:已知某点到起始点和终止点的距离，求出该点
	//     可能有两个，一个是点本身，还有一个是点关于直线的对称点
	//参数:intn:点的数目
	//     p1,第一个交点(当intn>=1时有意义)
	//     p2,第二个交点(当intn==2时有意义)
	//     disToStart:到起始点的距离
	//     disToEnd:到结束点的距离
	//返回:无
	void distancePoint(int &intn,
		CoordinateD &pt1,
		CoordinateD &pt2,
		const double disToStart,
		const double disToEnd);
	//功能:计算垂线
	//参数:pt1:垂线上的点1
	//     pt2:垂线上的点2
	//     pt:已知点
	//     distance:距离一直点的距离
	//返回:无
	void verticalLine(
		CoordinateD &pt1,
	    CoordinateD &pt2,
		CoordinateD pt,
		const double distance);
	//根据X求Y
	bool CalcY(double& y,const double x);
	//根据Y求X
	bool CalcX(double& x,const double y);
public:
	//重载虚基类IGeometryD函数
	//功能:获取空间实体的范围
	//参数:无
	//返回:空间实体范围
	BoxD GetExtern();
	//功能:获取曲线的起始点坐标
	//参数:无
	//返回:曲线的起始点坐标
	virtual CoordinateD get_fromPoint() const;
	//功能:获取曲线的结束点坐标
	//参数:无
	//返回:曲线的结束点坐标
	virtual CoordinateD get_toPoint() const;
	//功能:设置曲线的起始点坐标
	//参数:fromCoord:曲线的起始点坐标
	//返回:无
	virtual void set_fromPoint(CoordinateD fromCoord);
	//功能:设置曲线的结束点坐标
	//参数:toCoord:曲线的结束点坐标
	//返回:无
	virtual void set_toPoint(CoordinateD toCoord);
public:
	~LineD(void);
	private:
		CoordinateD m_fromPoint;
		CoordinateD m_toPoint;
};
