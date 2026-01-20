#pragma once
#include "CmpType.h"
class CmpConfig
{
public:
	CmpConfig(void);
public:
	~CmpConfig(void);
public:
	BOOL Read();
	BOOL Write();
	CmpType* m_cmpTypes[1024];
	int     m_nCmpTypeCount;
	int m_nMode;//比较模式,0:位置模式,1:句柄模式
	int m_nColorSame;//相同对象的颜色
	int m_nColorChange;//改变对象的颜色
	int m_nColorNew;//增加对象的颜色
	int m_nColorDelete;//删除对象的颜色
	bool       m_bEx;//文件夹对比
	bool       m_bSourceFile;//原图对比模式
	bool       m_bIngoreCloseLayer;//忽略关闭的图层
	bool       m_bIngoreLockLayer;//忽略锁定的图层
	CString    m_strIngoreLayer;//始终忽略的图层
	CStringArray m_strIngoreLayerArray;//始终忽略的图层数组
	CString m_strFile1;//文件1
	CString m_strFile2;//文件2
	AcApDocument* m_pDoc1;//文档1
	AcApDocument* m_pDoc2;//文档2
	CString m_strFloder1;//目录1
	CString m_strFloder2;//目录2
	CString m_strTmpFile1;
	CString m_strTmpFile2;
	void UpdateLockLayers();
private:
	BOOL ReadType(CmpType* type,MSXML2::IXMLDOMNodePtr lpCmpTypeNode);
	BOOL WriteType(CmpType* type,MSXML2::IXMLDOMElementPtr lpCmpTypeNode);
	BOOL ReadItem(CmpItem& item,MSXML2::IXMLDOMNodePtr lpCmpItemNode);
	BOOL WriteItem(CmpItem& item,MSXML2::IXMLDOMElementPtr lpCmpItemNode);
	void FindParentTypes();
};
