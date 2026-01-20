#include "StdAfx.h"
#include "CoordinateD.h"
#include <math.h>
#define CoordinateDPrecision 0.000000001
CoordinateD::CoordinateD(void)
{
	m_x=0.0;
	m_y=0.0;
	m_z=0.0;
	m_m=0.0;
}
CoordinateD::CoordinateD(double x, double y,double z,double m)
{
	m_x=x;
	m_y=y;
	m_z=z;
	m_m=m;
}
CoordinateD::CoordinateD(const CoordinateD& coord)
{
	m_x=coord.m_x;
	m_y=coord.m_y;
	m_z=coord.m_z;
	m_m=coord.m_m;
}
CoordinateD::~CoordinateD(void)
{

}
double CoordinateD::get_X() const
{
	return m_x;
}
double CoordinateD::get_Y() const
{
	return m_y;
}
double CoordinateD::get_Z() const
{
	return m_z;
}
double CoordinateD::get_M() const
{
	return m_m;
}
void CoordinateD::set_X(double x)
{
	m_x=x;
}
void CoordinateD::set_Y(double y)
{
	m_y=y;
}
void CoordinateD::set_Z(double z)
{
	m_z=z;
}
void CoordinateD::set_M(double m)
{
	m_m=m;
}
CString CoordinateD::ToString()
{
	CString str;
	str.Format(_T("%f,%f,%f,%f"),m_x,m_y,m_z,m_m);
	return str;
}
bool CoordinateD::operator ==(CoordinateD & coord)
{
	if(!IsSame(coord.get_X(),get_X()))
		return false;
	if(!IsSame(coord.get_Y(),get_Y()))
		return false;
	if(!IsSame(coord.get_Z(),get_Z()))
		return false;
	if(!IsSame(coord.get_M(),get_M()))
		return false;
	return true;
}
bool CoordinateD::operator !=(CoordinateD & coord)
{
	return !(coord==*this);
}
CoordinateD& CoordinateD::operator = (const CoordinateD& coord)
{
	m_x=coord.m_x;
	m_y=coord.m_y;
	m_z=coord.m_z;
	m_m=coord.m_m;
	return *this;
}
bool CoordinateD::IsSame(double value1,double value2)
{
	if(fabs(value1-value2)>CoordinateDPrecision)
		return false;
	else
		return true;
}
//计算两点之间的距离
double CoordinateD::CalcDistance(CoordinateD otherCoord) const
{
	double x1=get_X();
	double y1=get_Y();
	double x2=otherCoord.get_X();
	double y2=otherCoord.get_Y();
	double tmp=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	return sqrt(tmp);//求平方根，不记得对不对了
}
//计算两点构成的直线和水平线的夹角
double CoordinateD::CalcAngle(CoordinateD fromPoint, CoordinateD toPoint)
{
	double x=toPoint.get_X()-fromPoint.get_X();
	double y=toPoint.get_Y()-fromPoint.get_Y();
	if(x==0.00)
		return PI/2.0;
	double tmp=y/x;
	double angle=atan(tmp);
	if(tmp<0)
		return angle+PI;
	else
		return angle;
}
void CoordinateD::fromCadPoint(AcGePoint3d pt)
{
	m_x=pt.x;
	m_y=pt.y;
	m_z=pt.z;
}
AcGePoint3d CoordinateD::toCadPoint()
{
	return AcGePoint3d(m_x,m_y,m_z);
}

