#include "StdAfx.h"
#include "GSameDxfHelper.h"
#include "CommCommand.h"
GSameDxfHelper::GSameDxfHelper(void)
{
}

GSameDxfHelper::~GSameDxfHelper(void)
{
}
void GSameDxfHelper::SameResbuf(struct resbuf* rbTarget,struct resbuf* rbSource)
{
	rbTarget->restype=rbSource->restype;
	int dataType = dxfCodeToDataType(rbSource->restype);

	switch (dataType) {
		case RTSHORT:
			rbTarget->resval.rint=rbSource->resval.rint;
			break;
		case RTLONG:
			rbTarget->resval.rlong=rbSource->resval.rlong;
			break;
		case RTREAL:
			rbTarget->resval.rreal=rbSource->resval.rreal;
			break;
		case RTSTR:
			if(rbTarget->resval.rstring!=NULL)
			{
				acutDelString(rbTarget->resval.rstring);
			}
			rbTarget->resval.rstring=NULL;
			if(rbSource->resval.rstring!=NULL)
			{
				acutNewString(rbSource->resval.rstring,rbTarget->resval.rstring);
			}
			break;
		case RT3DPOINT:
			rbTarget->resval.rpoint[X]=rbSource->resval.rpoint[X];
			rbTarget->resval.rpoint[Y]=rbSource->resval.rpoint[Y];
			rbTarget->resval.rpoint[Z]=rbSource->resval.rpoint[Z];
			break;
		case RTPOINT:
			rbTarget->resval.rpoint[X]=rbSource->resval.rpoint[X];
			rbTarget->resval.rpoint[Y]=rbSource->resval.rpoint[Y];
			rbTarget->resval.rpoint[Z]=rbSource->resval.rpoint[Z];
			break;
		case 1004:
			break;
		case -6:
			break;
		case -5:
			break;
		case -4:
			break;
		case -3:
			break;
		case -2:
			break;
		case -1:
		case RTENAME:
			break;
		case RTPICKS:
			break;
		case RTLB:
			break;
		case RTLE:
			break;
		case RTNIL:
			break;
		case RTT:
			break;
		default:
			break;
	}
}
void GSameDxfHelper::RemoveItem(struct resbuf* rbTarget,struct resbuf*& rb)
{
	if(rb==rbTarget)
	{
		//首节点无法移除
		return;
	}
	struct resbuf* rbBefore=NULL;
	struct resbuf* pTmp=rbTarget;
	while(pTmp!=NULL)
	{
		if(pTmp->rbnext==rb)
		{
			rbBefore=pTmp;
			break;
		}
		pTmp=pTmp->rbnext;
	}
	if(rbBefore==NULL)
	{
		//无法找到前面一个节点
		return;
	}
	rbBefore->rbnext=rb->rbnext;
	rb->rbnext=NULL;
	acutRelRb(rb);
}
//移除DXF节点
void GSameDxfHelper::RemoveDxf(struct resbuf* rbTarget,int nDxfCode)
{
	struct resbuf* pTmp=rbTarget;
	while(pTmp!=NULL)
	{
		if(pTmp->restype==nDxfCode)
		{
			RemoveItem(rbTarget,pTmp);
			break;
		}
		pTmp=pTmp->rbnext;
	}
}
//寻找DXF节点
bool GSameDxfHelper::FindDxfItem(struct resbuf* rbTarget,struct resbuf*& rb,int nDxfCode)
{
	struct resbuf* pTmp=rbTarget;
	while(pTmp!=NULL)
	{
		if(pTmp->restype==nDxfCode)
		{
			rb=pTmp;
			return true;
		}
		pTmp=pTmp->rbnext;
	}
	return false;
}
bool GSameDxfHelper::FindBeforeItem(struct resbuf*& rbBefore,struct resbuf* rbTarget,struct resbuf* rb)
{
	struct resbuf* pTmp=rbTarget;
	while(pTmp!=NULL)
	{
		if(pTmp->rbnext==rb)
		{
			rbBefore=pTmp;
			return true;
		}
		pTmp=pTmp->rbnext;
	}
	return false;
}
void GSameDxfHelper::SameDxf(struct resbuf* rbTarget,struct resbuf* rbSource,int nDxfCode)
{
	struct resbuf* rbSourceDxf=NULL;
	if(!FindDxfItem(rbSource,rbSourceDxf,nDxfCode))
	{
		//没找到
		RemoveDxf(rbTarget,nDxfCode);
		return;
	}
	struct resbuf* rbTargetDxf=NULL;
	if(FindDxfItem(rbTarget,rbTargetDxf,nDxfCode))
	{
		//找到
		SameResbuf(rbTargetDxf,rbSourceDxf);
		return;
	}
	//没有找到，说明是默认值
	struct resbuf* rbBefore=NULL;
	struct resbuf* pTmp=rbSourceDxf;
	while(FindBeforeItem(rbBefore,rbSource,pTmp))
	{
		//逐个往前寻找
		struct resbuf* rb;
		if(FindDxfItem(rbTarget,rb,rbBefore->restype))
		{
			struct resbuf* rbNew=acutNewRb(nDxfCode);
			SameResbuf(rbNew,rbSourceDxf);
			rbNew->rbnext=rb->rbnext;
			rb->rbnext=rbNew;
			break;
		}
		pTmp=rbBefore;
	}
}
