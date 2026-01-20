#pragma once
class GColorRecord;
struct CmpEntityDat
{
	AcDbExtents m_ext;
	AcRxClass*	m_type;
	AcDbObjectId m_id;
	CString m_strHandle;
	CString m_strName;
	BOOL    m_bNotEnt;
	long nIndex;
	BOOL bDiff;
	bool bTmp;//临时变量
	CArray<CmpEntityDat*> subEntityDats;//暂时只支持一级
	CmpEntityDat();
	~CmpEntityDat();
	CmpEntityDat(const CmpEntityDat& dat);
	CmpEntityDat& operator=(const CmpEntityDat& dat);
	void Free();

};
struct CmpDiffItem
{
	CString strName;
	int nDxfCode;
	BOOL m_bOnlyOne;//是否唯一
	BOOL m_bRevise;//是否已经修复
	CString strOldValue;
	CString strNewValue;
	CStringArray strOldValueArray;
	CStringArray strNewValueArray;
	CmpDiffItem();
	CmpDiffItem(const CmpDiffItem& item);
	CmpDiffItem& operator=(const CmpDiffItem& item);
};
struct CmpDiffResult
{
	int nType;//0,完全相同,1,修改,2,新增,3,删除
	BOOL m_bRevise;//是否已经修复
	CString strHandle1;//句柄1
	CString strHandle2;//句柄2
	GColorRecord colRecord1;//颜色记录1
	GColorRecord colRecord2;//颜色记录2
	CString strName;//名称


	CArray<CmpDiffItem> m_diffItems;
	CmpDiffResult();
	CmpDiffResult(const CmpDiffResult& result);
	CmpDiffResult& operator=(const CmpDiffResult& result);
};
struct CmpBlockDiffResult
{
	int nType;//0,完全相同,1,修改,2,新增,3,删除
	CString strHandle1;//句柄
	CString strHandle2;//句柄
	CString strName;//名称(块名称或者布局名称)
	CArray<CmpDiffResult> m_diffItems;
	CmpBlockDiffResult();
	CmpBlockDiffResult(const CmpBlockDiffResult& result);
	CmpBlockDiffResult& operator=(const CmpBlockDiffResult& result);

};
//比较结果
struct CmpResult
{
	int nType;//0,完全相同,1,修改,2,新增,3,删除,4:单独图纸比较
	CString m_strFile1;
	CString m_strFile2;
	AcApDocument* m_pDoc1;
	AcApDocument* m_pDoc2;
	CArray<CmpDiffResult> m_diffsInModelSpace;//模型空间的差异比较结果
	CArray<CmpDiffResult> m_diffsInLayerTable;//图层的差异比较结果
	CArray<CmpDiffResult> m_diffsInTextStyleTable;//文字样式的差异比较结果
	CArray<CmpDiffResult> m_diffsInLinetypeTable;//线形的差异比较结果
	CArray<CmpBlockDiffResult> m_diffsInBlockDefines;
	CmpResult();
	CmpResult(const CmpResult& result);
	CmpResult& operator=(const CmpResult& result);
};