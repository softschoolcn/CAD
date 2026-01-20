#pragma once
//描述坐标点
class GLOBALTOOLS_EXT CoordinateD
{
public:
	//构造函数
	CoordinateD(void);
	CoordinateD(double x, double y,double z=0.0,double m=0.0);
	CoordinateD(const CoordinateD& coord);
public:
	//析构函数
	~CoordinateD(void);
private:
	//成员变量
	double m_x;
	double m_y;
	double m_z;
	double m_m;
public:
	//方法接口
	double get_X() const;
	double get_Y() const;
	double get_Z() const;
	double get_M() const;
	void set_X(double x);
	void set_Y(double y);
	void set_Z(double z);
	void set_M(double m);
	//计算两点之间的距离
	double CalcDistance(CoordinateD otherCoord) const;
	CString ToString();
	void fromCadPoint(AcGePoint3d pt);
	AcGePoint3d toCadPoint();
public:
	//操作符重载
	bool operator ==(CoordinateD & coord);
	bool operator !=(CoordinateD & coord);
	CoordinateD& operator = (const CoordinateD& coord);
public:
	//静态方法
	static bool IsSame(double value1,double value2);
	//计算两点构成的直线和水平线的夹角
	//计算结果在0到2*PI之间
	static double CalcAngle(CoordinateD fromPoint,CoordinateD toPoint);
};