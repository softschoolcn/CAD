#pragma once

//打印项
struct GPlotItem
{
	AcGePoint3d m_ptMin;//打印范围(最小点)
	AcGePoint3d m_ptMax;//打印范围(最大点)
	CString     m_strFile;//打印文件
	AcDbHandle  m_ownerHandle;//所在空间句柄
	CString     m_strOwnerName;//模型空间/布局空间
	CString     m_strBlockName;//块名称(可能为空)
	void Copy(const GPlotItem& src);
};
struct GPlotItemEx:public GPlotItem
{
	CString m_size;      //打印尺寸名称
    CString m_style;     //打印样式表
	int m_copies;        //打印份数
};
struct GFrameDefine
{
	int m_nOrder;//排序,0:从左往右打印，至上而下打印,1:从上往下打印，至左而右打印
	BOOL m_bBREF;
	BOOL m_bFrameNameCheck;
	CString m_strFrameName;
	BOOL m_bFrameLayerCheck;
	CString m_strFrameLayer;
	CString m_strBHXLayer;
	BOOL    m_bBHXLayer;
	BOOL    m_bPLine;
	BOOL    m_bLine;
	bool IsFrame(AcDbBlockReference* pRef);
	//是闭合线的时候过滤对象
	bool IsFrameX(AcDbEntity* pEnt);
};
