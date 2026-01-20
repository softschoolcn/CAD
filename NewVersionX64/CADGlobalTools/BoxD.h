#pragma once
#include "CoordinateD.h"
class GLOBALTOOLS_EXT BoxD
{
public:
	BoxD(void);
	BoxD(const double xmin,const double ymin,
		const double xmax,const double ymax);
	BoxD(CoordinateD min,CoordinateD max);
	BoxD(const BoxD& box);
public:
	~BoxD(void);
public:
	void AddPoint(CoordinateD coord);
	void AddBox(const BoxD box);
	bool Contains(CoordinateD coord) const;
	bool Contains(BoxD box) const;
	CoordinateD get_Max() const;
	CoordinateD get_Min() const;
	void set_Max(CoordinateD maxCoord);
	void set_Min(CoordinateD minCoord);
	virtual BoxD GetExtern();
public:
	bool operator ==(BoxD & box);
	bool operator !=(BoxD & box);
	BoxD& operator = (const BoxD& box);
private:
	//交换两个值
	void set(const double xmin,const double ymin,
		const double xmax,const double ymax);
	void swap(double &dValue1,double &dValue2);
	//交换值保证第一个值为最小值，第二个值为最大值
	void swapminmax(double &dMin,double &dMax);
	CoordinateD m_max;//最大坐标点
	CoordinateD m_min;//最小坐标点
	bool Init;//是否已经初始化,如果初始化时一个点都没有，最大点和最小默认为(0.0)(0.0)
	//新加入点可能需要和最大值点最小值点进行比较，和没有初始化的点比较是不对的.
};
