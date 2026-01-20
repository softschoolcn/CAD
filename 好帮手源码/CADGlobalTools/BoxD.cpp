#include "StdAfx.h"
#include "BoxD.h"

BoxD::BoxD(void)
{
	Init=false;//没有初始化
}

BoxD::~BoxD(void)
{
}
BoxD::BoxD(const double xmin, const double ymin,
		   const double xmax, const double ymax)
{
	set(xmin,ymin,xmax,ymax);
}
BoxD::BoxD(const BoxD& box)
{
	m_max=box.m_max;
	m_min=box.m_min;
	Init=box.Init;
}
BoxD::BoxD(CoordinateD min,CoordinateD max)
{
	set(min.get_X(),min.get_Y(),
		max.get_X(),max.get_Y());
}
void BoxD::swap(double &dValue1, double &dValue2)
{
	double tmp=dValue1;
	dValue1=dValue2;
	dValue2=tmp;
}
void BoxD::swapminmax(double &dMin, double &dMax)
{
	//如果小的值比大的值还要大的话
	//需要相互交换一下
	if(dMin>dMax)
	{
		swap(dMin,dMax);
	}
}
void BoxD::AddPoint(CoordinateD coord)
{
	if(Init)
	{
		set(coord.get_X(),coord.get_Y(),coord.get_X(),coord.get_Y());
	}
	else
	{
		if(coord.get_X()<m_min.get_X())
		{
			m_min.set_X(coord.get_X());
		}
		if(coord.get_Y()<m_min.get_Y())
		{
			m_min.set_Y(coord.get_Y());
		}
		if(coord.get_X()>m_max.get_X())
		{
			m_max.set_X(coord.get_X());
		}
		if(coord.get_Y()>m_max.get_Y())
		{
			m_max.set_Y(coord.get_Y());
		}
	}
}
void BoxD::AddBox(const BoxD box)
{
	AddPoint(box.m_min);
	AddPoint(box.m_max);
}
void BoxD::set(const double xmin, const double ymin, 
			   const double xmax, const double ymax)
{
	double tmpxmin=xmin;
	double tmpymin=ymin;
	double tmpxmax=xmax;
	double tmpymax=ymax;
	//保证tmpxmin<tmpxmax
	swapminmax(tmpxmin,tmpxmax);
	//保证tmpymin<tmpymax
	swapminmax(tmpymin,tmpymax);
	m_min.set_X(tmpxmin);
	m_min.set_Y(tmpymin);
	m_max.set_X(tmpxmax);
	m_max.set_Y(tmpymax);
	Init=true;
}
bool BoxD::Contains(CoordinateD coord) const
{
	if(coord.get_X()<m_min.get_X())
		return false;
	if(coord.get_Y()<m_min.get_Y())
		return false;
	if(coord.get_X()>m_max.get_X())
		return false;
	if(coord.get_Y()>m_max.get_Y())
		return false;
	return true;
}
CoordinateD BoxD::get_Max() const
{
	return m_max;
}
CoordinateD BoxD::get_Min() const
{
	return m_min;
}
void BoxD::set_Max(CoordinateD maxCoord)
{
	if(Init)
	{
		if(maxCoord.get_X()>m_min.get_X()&&
			maxCoord.get_Y()>m_min.get_Y())
		{
			m_max=maxCoord;
		}
	}
	else
	{
		set(maxCoord.get_X(),maxCoord.get_Y(),
			maxCoord.get_X(),maxCoord.get_Y());
	}
}
void BoxD::set_Min(CoordinateD minCoord)
{
	if(Init)
	{
		if(minCoord.get_X()<m_max.get_X()&&
			minCoord.get_Y()<m_max.get_Y())
		{
			m_min=minCoord;
		}
	}
	else
	{
		set(minCoord.get_X(),minCoord.get_Y(),
			minCoord.get_X(),minCoord.get_Y());
	}
}
bool BoxD::Contains(BoxD box) const
{
	if(!Contains(box.get_Min()))
		return false;
	if(!Contains(box.get_Max()))
		return false;
	return true;
}
BoxD BoxD::GetExtern()
{
	return *this;
}
bool BoxD::operator ==(BoxD & box)
{
	if(box.Init!=Init)
		return false;
	if(!Init)
		return true;//两个都没有初始化的Box认为相等
	if(box.m_max!=m_max)
		return false;
	if(box.m_min!=m_min)
		return false;
	return true;
}
bool BoxD::operator!=(BoxD & box)
{
	return !(box==*this);
}
BoxD& BoxD::operator = (const BoxD& box)
{
	m_max=box.m_max;
	m_min=box.m_min;
	Init=box.Init;
	return *this;
}