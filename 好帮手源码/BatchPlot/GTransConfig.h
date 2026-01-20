#pragma once
//选择输出对象模式
enum GTransEntitysMode
{
	kTransEntitysScreen=0,//整个屏幕
	kTransEntitysAll=1,//所有对象
	kTransEntitysExtents=2,//选择范围
	kTransEntitysEnts=3//选择实体对象
};
enum GTransPaperMode
{
	kISOAOPaper=0,
	kISOA1Paper=1,
	kISOA2Paper=2,
	kISOA3Paper=3,
	kISOA4Paper=4
};
//格式转换配置
class GTransConfig
{
public:
	GTransConfig(void);
public:
	~GTransConfig(void);
public:
	CStringArray m_TypeArray;//转换个格式列表
	bool m_bBlackWhite;//转换成黑白的
	GTransEntitysMode m_EntitysMode;//选择输出对象模式
	AcDbObjectIdArray m_ids;//输出对象(选择实体对象)
	AcDbExtents m_ext;//输出对象(选择范围)
	CString m_strFloder;//输出文件目录
	CString m_strFileName;//输出文件名称
	bool m_bGetExt;
	GTransPaperMode m_PaperMode;
public:
	BOOL Read();
	BOOL Write();
private:
	CString m_strConfigFile;
	BOOL WriteString(CString strKey,CString strValue);
	BOOL WriteInt(CString strKey,int nValue);
	BOOL WriteDouble(CString strKey,double dValue);
	BOOL ReadString(CString strKey,CString& strValue);
	BOOL ReadInt(CString strKey,int& nValue);
	BOOL ReadDouble(CString strKey,double& dValue);
};
