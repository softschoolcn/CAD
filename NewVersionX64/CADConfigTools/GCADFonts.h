#pragma once

class GCADFonts
{
private:
	GCADFonts(void);
public:
	~GCADFonts(void);
public:
	static GCADFonts* GetInstance();
	CStringArray m_sysFontsArray;
	CStringArray m_strCommShxFontsArray;
	CStringArray m_strBigFontsArray;
	void Enum();
	void Update();
private:
	static GCADFonts instance;
	bool IsEmpty();//ÊÇ·ñÊÇ¿ÕµÄ
	void EnumSystemFonts();
	void EnumCadFonts_File(CString strFile);
	void EnumCadFonts(CString strPath);
	void WriteToXmlFile();
	void ReadFromXmlFile();
};
