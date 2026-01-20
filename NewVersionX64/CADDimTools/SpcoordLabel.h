#pragma once
#define USER_TextStyle "属性标注字体" 


void CreatLabelPlineAllPt();
void CreatLabelPlineAllPtTZ();

// 创建红线标注
void CreateHXBZ(CString strLineLayer, CString strTextLayer);


//////////////////////////////////////////////////////////////////////////
// 坐标标注
class CSpcoordLabel
{
public:
	CSpcoordLabel(void);
	virtual ~CSpcoordLabel(void);

	static double FontHeight;
	bool   m_bExchangeXY;//交换XY,默认为false
	double m_dScale;//标注值和实际值的比例,默认为1.0

private:
	struct resbuf *m_OldRbf;

public:

	// 创建带引线的文字标注
	BOOL CreateDimText(CString strText, ads_point startPt = NULL);
	BOOL CreateCoordLabel(ads_point startPt = NULL);

	BOOL CreateWidthLabel(CString strLayer);
	BOOL CreateAlignedLabel(CString strLayer);

	AcDbObjectId GetTextStyleObjID(CString TextStyleName);

protected:
	void CoverAngelP2(ads_real &dAngle,ads_real &dLabelAngle);
	void CoverAngelP3(ads_real &dAngle,ads_real &dLabelAngle);
	double GetTextWidth(LPCTSTR sText, double dHigh);
	double GetTextWidth(CString sText, double dHigh,AcDbObjectId& TextID);
	double GetTextMaxWidth(CStringArray& strTextArray,double dHigh,AcDbObjectId& TextID);
	BOOL AddBigFontToCurDwg(CString BigFontFileName, CString TextStyleName);
};
