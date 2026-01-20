#pragma once

class GSameDxfHelper
{
public:
	GSameDxfHelper(void);
public:
	~GSameDxfHelper(void);
	void SameDxf(struct resbuf* rbTarget,struct resbuf* rbSource,int nDxfCode);
private:
	//同步resbuf
	void SameResbuf(struct resbuf* rbTarget,struct resbuf* rbSource);
	//移除DXF节点
	void RemoveDxf(struct resbuf* rbTarget,int nDxfCode);
	void RemoveItem(struct resbuf* rbTarget,struct resbuf*& rb);
	//寻找DXF节点
	bool FindDxfItem(struct resbuf* rbTarget,struct resbuf*& rb,int nDxfCode);
	//寻找之前的节点
	bool FindBeforeItem(struct resbuf*& rbBefore,struct resbuf* rbTarget,struct resbuf* rb);
};
