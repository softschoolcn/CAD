#include "StdAfx.h"
#include "LineD.h"

LineD::LineD(void)
{
}
LineD::LineD(CoordinateD coord)
{
	m_toPoint=coord;
}
LineD::LineD(CoordinateD fromCoord, CoordinateD toCoord)
{
	m_fromPoint=fromCoord;
	m_toPoint=toCoord;
}
LineD::LineD(const LineD &m_line)
{
	m_fromPoint=m_line.m_fromPoint;
	m_toPoint=m_line.m_toPoint;
}


LineD::~LineD(void)
{
}
bool LineD::operator ==(LineD & m_line)
{
	if(get_fromPoint()!=m_line.get_fromPoint())
		return false;
	if(get_toPoint()!=m_line.get_toPoint())
		return false;
	return true;
}
bool LineD::operator !=(LineD & m_line)
{
	return !(m_line==*this);
}
LineD& LineD::operator = (const LineD& m_line)
{
	m_fromPoint=m_line.m_fromPoint;
	m_toPoint=m_line.m_toPoint;
	return *this;
}
//功能:计算点到直线的距离
//参数:otherCoord:坐标点
//返回:点到直线的距离
double LineD::CalcDistance(CoordinateD otherCoord) const
{
	double dArea=CalcArea(otherCoord);
	return 2*dArea/get_length();
}
//功能:计算直线的两个点和另外一个点构成的三角形的面积
//参数:thirdCoord:三角形的第三个坐标点
//返回:三角形的面积
double LineD::CalcArea(CoordinateD thirdCoord) const
{
	double c=get_length();
	double b=thirdCoord.CalcDistance(get_fromPoint());
	double a=thirdCoord.CalcDistance(get_toPoint());
	double p=(a+b+c)/2.0;
	double tmp=p*(p-a)*(p-b)*(p-c);
	return sqrt(tmp);//海伦公式
}
//功能:获取由起始点连接结束点构成的角度
//参数:无
//返回:倾斜角
double LineD::get_angle() const
{
	return CoordinateD::CalcAngle(get_fromPoint(),get_toPoint());
}
//功能:计算直线和直线之间的交点
//参数:intersectPoint:交点
//     otherLine:求交点的直线
//     beExtern:是否计算延长线
//返回:0,两个直线之间没有交点
//     1,两个直线之间有有一个交点
//     2,两个直线之间有无数个焦点
long LineD::intersectWith(LineD otherLine,CoordinateD &intersectPoint,bool beExtern) const
{
	double a[2], b[2], c[2], x, y, D;
	standEquation(a[0],b[0],c[0]);
	otherLine.standEquation(a[1],b[1],c[1]);
	D = a[0] * b[1] - a[1] * b[0];
	//GxTrace::Output(_T("LineD::intersectWith::D=%f"),D);
	if(fabs(D)>DoublePrecision)
	{
		x = (b[0] * c[1] - b[1] * c[0]) / D;
		y = (a[1] * c[0] - a[0] * c[1]) / D;
		intersectPoint.set_X(x);
		intersectPoint.set_Y(y);
		if(beExtern)
			return 1;
		else
		{
			if((get_fromPoint().get_X()-x)*(get_toPoint().get_X()-x)>0)
			{
				if(fabs(get_fromPoint().get_X()-x)>DoublePrecision&&
					fabs(get_toPoint().get_X()-x)>DoublePrecision)
				return 0;
			}
			if((get_fromPoint().get_Y()-y)*(get_toPoint().get_Y()-y)>0)
			{
				if(fabs(get_fromPoint().get_Y()-y)>DoublePrecision&&
					fabs(get_toPoint().get_Y()-y)>DoublePrecision)
				return 0;
			}
			if((otherLine.get_fromPoint().get_X()-x)*(otherLine.get_toPoint().get_X()-x)>0)
			{
				if(fabs(otherLine.get_fromPoint().get_X()-x)>DoublePrecision&&
					fabs(otherLine.get_toPoint().get_X()-x)>DoublePrecision)
				return 0;
			}
			if((otherLine.get_fromPoint().get_Y()-y)*(otherLine.get_toPoint().get_Y()-y)>0)
			{
				if(fabs(otherLine.get_fromPoint().get_Y()-y)>DoublePrecision&&
					fabs(otherLine.get_toPoint().get_Y()-y)>DoublePrecision)
				return 0;
			}
			return 1;
			/*
			long res1=CalcRelationship(intersectPoint);
			long res2=otherLine.CalcRelationship(intersectPoint);
			if((res1&distPointOnSegment)==distPointOnSegment&&
				(res2&distPointOnSegment)==distPointOnSegment)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			*/
		}
	}
	else
	{
		double dis=CalcDistance(otherLine.get_fromPoint());
		if(fabs(dis)>DistancePrecision)
		{
			//平行线
			return 0;
		}
		if(beExtern)
			return 2;
		else
		{
			double d1,d2;
			double d3,d4;
			bool bX=true;
			if(fabs(get_fromPoint().get_X()-get_toPoint().get_X())<DoublePrecision)
			{
				d1=get_fromPoint().get_Y();
				d2=get_toPoint().get_Y();

				d3=otherLine.get_fromPoint().get_Y();
				d4=otherLine.get_toPoint().get_Y();
				bX=false;
			}
			else
			{
				d1=get_fromPoint().get_X();
				d2=get_toPoint().get_X();

				d3=otherLine.get_fromPoint().get_X();
				d4=otherLine.get_toPoint().get_X();
			}
			double dMin1,dMax1;
			dMin1=min(d1,d2);
			dMax1=max(d1,d2);
			double dMin2,dMax2;
			dMin2=min(d3,d4);
			dMax2=max(d3,d4);

			if(dMax2<dMin1-DoublePrecision)
			{
				return 0;
			}
			else if(dMax2>=dMin1-DoublePrecision&&dMax2<=dMin1+DoublePrecision)
			{
				return 1;
			}
			else if(dMax2>dMin1+DoublePrecision&&dMax2<=dMax1+DoublePrecision)
			{
				return 2;
			}
			else
			{
				//dMax2>dMax1+DoublePrecision
				if(dMin2<dMax1-DoublePrecision)
				{
					return 2;
				}
				else if(dMin2>=dMax1-DoublePrecision&&
					dMin2<=dMax1+DoublePrecision)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			/*
			//两条直线重叠
			//两条线段关系需要继续分析
			CoordinateD A0,B0;//调整前直线的起始点坐标，终止点坐标
			CoordinateD C0,D0;//调整前待求直线的起始点坐标，终止点坐标
			A0=get_fromPoint();
			B0=get_toPoint();
			C0=otherLine.get_fromPoint();
			D0=otherLine.get_toPoint();
			double AB=A0.CalcDistance(B0);
			//GxTrace::Output(_T("LineD::intersectWith::AB=%f"),AB);
			double AC=A0.CalcDistance(C0);
			//GxTrace::Output(_T("LineD::intersectWith::AC=%f"),AC);
			double AD=A0.CalcDistance(D0);
			//GxTrace::Output(_T("LineD::intersectWith::AD=%f"),AD);
			double BC=B0.CalcDistance(C0);
			//GxTrace::Output(_T("LineD::intersectWith::BC=%f"),BC);
			double BD=B0.CalcDistance(D0);
			//GxTrace::Output(_T("LineD::intersectWith::BD=%f"),BD);
			double CD=C0.CalcDistance(D0);
			//GxTrace::Output(_T("LineD::intersectWith::CD=%f"),CD);
			double A=AB+AC+AD;//A点和其它三个点的距离之和
			//GxTrace::Output(_T("LineD::intersectWith::A=%f"),A);
			double B=AB+BC+BD;//B点和其它三个点的距离之和
			//GxTrace::Output(_T("LineD::intersectWith::B=%f"),B);
			double C=AC+BC+CD;//C点和其它三个点的距离之和
			//GxTrace::Output(_T("LineD::intersectWith::C=%f"),C);
			double D=AD+BD+CD;//D点和其它三个点的距离之和
			//GxTrace::Output(_T("LineD::intersectWith::D=%f"),D);

			//不管是何种情况ABCD中有两个偏大，有两个偏小
			//偏大的属于两边的点，偏小的属于中间点
			double max=A;
			double min=A;
			if(B>max)
			{
				max=B;
			}
			if(B<min)
			{
				min=B;
			}
			if(C>max)
			{
				max=C;
			}
			if(C<min)
			{
				min=C;
			}
			if(D>max)
			{
				max=D;
			}
			if(D<min)
			{
				min=D;
			}
			if(max==min)
			{
				return 2;
			}
			else
			{
				if(A==max)
				{
					if(B==max)
					{
						//AB在外侧，则CD段之间均为交点，所以有无限个
						return 2;
					}
					else
					{
						if(C==max)
						{
							if(AD>AB)
							{
								return 0;
							}
							else if(AD==AB)
							{
								intersectPoint=B0;
								return 1;
							}
							else
							{
								return 2;
							}
						}
						else
						{
							//D==max
							if(AC>AB)
							{
								return 0;
							}
							else if(AC==AB)
							{
								intersectPoint=B0;
								return 1;
							}
							else
							{
								return 2;
							}
						}
					}
				}
				else if(B==max)
				{
					//A!=max
					if(C==max)
					{
						if(BD>AB)
						{
							return 0;
						}
						else if(BD==AB)
						{
							intersectPoint=A0;
							return 1;
						}
						else
						{
							return 2;
						}
					}
					else
					{
						//D==max
						if(BC>AB)
						{
							return 0;
						}
						else if(BC==AB)
						{
							intersectPoint=A0;
							return 1;
						}
						else
						{
							return 2;
						}
					}
				}
				else
				{
					//如果AB均不为大值，那么C,D将同时为大值
					//CD在外侧，同样有无限个交点
					return 2;
				}
			}*/

		}
		
	}
}
//功能:计算直线的标准方程
//     Ax+By+C=0;
//参数:
//返回:无
void LineD::standEquation(double &A,double &B,double &C) const
{
	A = get_fromPoint().get_Y() - get_toPoint().get_Y();
	B = get_toPoint().get_X() - get_fromPoint().get_X();
	C = get_fromPoint().get_X() * get_toPoint().get_Y() - 
		get_toPoint().get_X() * get_fromPoint().get_Y();
}
//功能:计算直线的中点
//参数:无
//返回:直线的中点
CoordinateD LineD::get_centerPoint() const
{
	CoordinateD start=get_fromPoint();
	CoordinateD end=get_toPoint();
	double x=(start.get_X()+end.get_X())/2.0;
	double y=(start.get_Y()+end.get_Y())/2.0;
	double z=(start.get_Z()+end.get_Z())/2.0;
	return CoordinateD(x,y,z);
}
//功能:计算直线的长度
//参数:无
//返回:直线的长度
double LineD::get_length() const
{
	return get_fromPoint().CalcDistance(get_toPoint());
}
//功能:计算关于本直线对称的点
//参数:coord:参数点
//返回:对称点
CoordinateD LineD::symmetryPoint(CoordinateD coord)
{
	CoordinateD ptPlumb;//垂足
	if(plumbPoint(ptPlumb,coord))
	{
		double x=2*ptPlumb.get_X()-coord.get_X();
		double y=2*ptPlumb.get_Y()-coord.get_Y();
		return CoordinateD(x,y);
	}
	else
	{
		return coord;
	}
}
//功能:计算垂足
//参数:coord:参数点
//     ptOut:返回垂足
//返回:计算垂足是否成功
bool LineD::plumbPoint(CoordinateD &ptOut,CoordinateD coord)
{
	double dba,dbb;
	double x1=get_fromPoint().get_X();
	double y1=get_fromPoint().get_Y();
	double x2=get_toPoint().get_X();
	double y2=get_toPoint().get_Y();
	double x3=coord.get_X();
	double y3=coord.get_Y();
	dba = sqrt(((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) ));
	dbb = ((x2 - x1) * (x3 -x1) + (y2 - y1) * (y3 - y1));
	if(fabs(dba)<DoublePrecision)
	{
		return false;
	}
	else
	{
		dba = dbb / (dba * dba);   
		double x4 = x1 +(x2 - x1)*dba;   
		double y4 = y1 +(y2 - y1)*dba;
		ptOut.set_X(x4);
		ptOut.set_Y(y4);
		return true;
	}

}
//功能:已知某点到起始点和终止点的距离，求出该点
//     可能有两个，一个是点本身，还有一个是点关于直线的对称点
//参数:intn:点的数目
//     p1,第一个交点(当intn>=1时有意义)
//     p2,第二个交点(当intn==2时有意义)
//     disToStart:到起始点的距离
//     disToEnd:到结束点的距离
//返回:无
void LineD::distancePoint(int &intn,
						  CoordinateD &pt1,
						  CoordinateD &pt2,
						  const double disToStart,
						  const double disToEnd)
{
	ASSERT(disToStart>=0);
	ASSERT(disToEnd>=0);
	double length=get_length();
	double x1=get_fromPoint().get_X();
	double y1=get_fromPoint().get_Y();
	double x2=get_toPoint().get_X();
	double y2=get_toPoint().get_Y();
	if(disToStart+disToEnd<length+DistancePrecision||
		disToStart+length<disToEnd+DistancePrecision||
		disToEnd+length<disToStart+DistancePrecision)
	{
		intn=0;
	}
	else if(fabs(fabs(disToEnd-disToStart)-length)<DistancePrecision||
		fabs(fabs(disToEnd+disToStart)-length)<DistancePrecision)
	{
		//求出的点在直线上或者在直线的延长线上
		intn=1;
		double x,y;
		if(fabs(fabs(disToEnd-disToStart)-length)<DistancePrecision)
		{
			//点在直线的延长线上
			if(disToStart<disToEnd)
			{
				//靠近起始点
				x=disToStart*(x1-x2)/length+x1;
				y=disToStart*(y1-y2)/length+y1;
			}
			else
			{
				//靠近终止点
				x=disToStart*(x2-x1)/length+x1;
				y=disToStart*(y2-y1)/length+y1;
			}
		}
		else
		{
			//点在直线上
			x=disToStart*(x2-x1)/length+x1;
			y=disToStart*(y2-y1)/length+y1;
		}
	}
	else
	{
		//点在直线外
		intn=2;
		double p=(length+disToStart+disToEnd)/2.0;
		double tmp=p*(p-length)*(p-disToStart)*(p-disToEnd);
		double h=2*tmp/length;//点到直线的距离
		double tmp1=sqrt(disToStart*disToStart-h*h);
		double tmp2=sqrt(disToEnd*disToEnd-h*h);
		CoordinateD pt_tmp;//点到直线的垂足
		CoordinateD _pt_tmp;//函数需要，不会被赋值
		int tmp_int;
		distancePoint(tmp_int,pt_tmp,_pt_tmp,tmp1,tmp2);
		verticalLine(pt1,pt2,pt_tmp,h);
	}
}
//功能:计算垂线
//参数:pt1:垂线上的点1
//     pt2:垂线上的点2
//     pt:已知点
//     distance:距离一直点的距离
//返回:无
void LineD::verticalLine(CoordinateD &pt1,
CoordinateD &pt2,
CoordinateD pt,
const double distance)
{
	double A,B,C;
	standEquation(A,B,C);
	double x=pt.get_X();
	double y=pt.get_Y();
	if(fabs(B)<=DoublePrecision)
	{
		//直线为竖直线
		//垂线为水平线
		pt1.set_X(x-distance);
		pt1.set_Y(y);
		pt2.set_X(x+distance);
		pt2.set_Y(y);
		
	}
	else
	{
		double k=-A/B;
		k=-1/k;
		double tmp=sqrt(1+k*k);
		pt1.set_X(x+distance/tmp);
		pt1.set_X(y+k*distance/tmp);
		pt2.set_X(x-distance/tmp);
		pt2.set_X(y-k*distance/tmp);
	}
}
//根据X求Y
bool LineD::CalcY(double& y,const double x)
{
	double maxx=max(get_fromPoint().get_X(),get_toPoint().get_X());
	double minx=min(get_fromPoint().get_X(),get_toPoint().get_X());
	if(x>maxx+DoublePrecision||x<minx-DoublePrecision)
		return false;
	//功能:计算直线的标准方程
	//     Ax+By+C=0;
	double A=0.0;
	double B=0.0;
	double C=0.0;
	standEquation(A,B,C);
	if(B>DoublePrecision||B<-DoublePrecision)
	{
		y=-(A*x+C)/B;
		return true;
	}
	else
	{
		if(x-(-C/A)>DoublePrecision||x-(-C/A)<-DoublePrecision)
		{
			return false;
		}
		else
		{
			y=get_fromPoint().get_Y();
			return true;
		}
	}

}
//根据X求Y
bool LineD::CalcX(double& x,const double y)
{
	double maxy=max(get_fromPoint().get_Y(),get_toPoint().get_Y());
	double miny=min(get_fromPoint().get_Y(),get_toPoint().get_Y());
	if(y>maxy+DoublePrecision||y<miny-DoublePrecision)
		return false;
	//功能:计算直线的标准方程
	//     Ax+By+C=0;
	double A=0.0;
	double B=0.0;
	double C=0.0;
	standEquation(A,B,C);
	if(A>DoublePrecision||A<-DoublePrecision)
	{
		x=-(B*y+C)/A;
		return true;
	}
	else
	{
		if(y-(-C/B)>DoublePrecision||y-(-C/B)<-DoublePrecision)
		{
			return false;
		}
		else
		{
			x=get_fromPoint().get_X();
			return true;
		}
	}
}
//功能:获取曲线的起始点坐标
//参数:无
//返回:曲线的起始点坐标
CoordinateD LineD::get_fromPoint() const
{
	return m_fromPoint;
}
//功能:获取曲线的结束点坐标
//参数:无
//返回:曲线的结束点坐标
CoordinateD LineD::get_toPoint() const
{
	return m_toPoint;
}
//功能:设置曲线的起始点坐标
//参数:曲线的起始点坐标
//返回:无
void LineD::set_fromPoint(CoordinateD fromCoord)
{
	m_fromPoint=fromCoord;
}
//功能:设置曲线的结束点坐标
//参数:曲线的结束点坐标
//返回:无
void LineD::set_toPoint(CoordinateD toCoord)
{
	m_toPoint=toCoord;
}
//重载虚基类IGeometryD函数
//功能:获取空间实体的范围
//参数:无
//返回:空间实体范围
BoxD LineD::GetExtern()
{
	return BoxD(m_fromPoint,m_toPoint);
}
