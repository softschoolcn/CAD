#include "StdAfx.h"
#include "GCADFonts.h"
INT CALLBACK NEnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
{
 CStringArray* pArr = (CStringArray*) lParam;
 pArr->Add(plf->lfFaceName);
 return TRUE;
}
GCADFonts GCADFonts::instance;
GCADFonts::GCADFonts(void)
{
}

GCADFonts::~GCADFonts(void)
{
}
GCADFonts* GCADFonts::GetInstance()
{
	return &instance;
}
void GCADFonts::Enum()
{
	if(!IsEmpty())
	{
		return;
	}
	ReadFromXmlFile();
	if(!IsEmpty())
	{
		return;
	}
	TCHAR lpPath[MAX_PATH+1];
	GetModuleFileName(GetModuleHandle(_T("acad.exe")),lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("Fonts"));
	EnumCadFonts(lpPath);
	EnumSystemFonts();
	WriteToXmlFile();
}
void GCADFonts::Update()
{
	m_sysFontsArray.RemoveAll();
	m_strBigFontsArray.RemoveAll();
	m_strCommShxFontsArray.RemoveAll();
	TCHAR lpPath[MAX_PATH+1];
	GetModuleFileName(GetModuleHandle(_T("acad.exe")),lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("Fonts"));
	EnumCadFonts(lpPath);
	EnumSystemFonts();
	WriteToXmlFile();
}
//是否是空的
bool GCADFonts::IsEmpty()
{
	if(!m_sysFontsArray.IsEmpty())
	{
		return false;
	}
	if(!m_strBigFontsArray.IsEmpty())
	{
		return false;
	}
	if(!m_strCommShxFontsArray.IsEmpty())
	{
		return false;
	}
	return true;
}
void GCADFonts::EnumSystemFonts()
{
	m_sysFontsArray.RemoveAll();
	HDC hDC = GetWindowDC(acedGetAcadFrame()->GetSafeHwnd()); // DC for CAD Windows
	EnumFontFamilies(hDC, (LPTSTR) NULL, (FONTENUMPROC)NEnumFontNameProc, (LPARAM)&(m_sysFontsArray));
	ReleaseDC(NULL,hDC);
}
void GCADFonts::EnumCadFonts_File(CString strFile)
{
	TCHAR lpPath[MAX_PATH+1]={0};
	lstrcpy(lpPath,strFile);
	TCHAR lpExt[MAX_PATH+1]={0};
	lstrcpy(lpExt,PathFindExtension(lpPath));
	if(lstrcmpi(lpExt,_T(".SHX"))==0)
	{
		try
		{
			CFile file;
			if(!file.Open(lpPath, CFile::modeRead|CFile::typeBinary))
			{
				GLog::WriteLine(_T("打开文件:%s失败, 错误号：%d"),strFile, GetLastError());
				if(lstrcmpi(_T("gbcbig.shx"),PathFindFileName(lpPath))==0)
				{
					m_strBigFontsArray.Add(PathFindFileName(lpPath));
				}
				return;
			}
			CString rString;
			char buff[51]={0};
			file.Read(buff,50);
			file.Close();
			rString=buff;
			rString.MakeLower();
			if(rString.Find(_T("bigfont"))>=1)
			{
				m_strBigFontsArray.Add(PathFindFileName(lpPath));
			}
			else
			{
				m_strCommShxFontsArray.Add(PathFindFileName(lpPath));
			}
		}
		catch (CFileException* pe) 
		{
			CString strErr;
			strErr.Format(_T("读取文件:%s发生异常, 错误号：%d"),strFile, pe->m_cause);
			GLog::WriteLine(strErr);
			pe->Delete();
		}
	}

}
void GCADFonts::EnumCadFonts(CString strPath)
{
	CStringArray arrSubDirNames;
	CStringArray arrSubFileNames;
	GFileTools::EnumDirectoryContent(strPath,arrSubDirNames,arrSubFileNames);
	for(int i=0;i<arrSubFileNames.GetCount();i++)
	{
		TCHAR lpPath[MAX_PATH+1]={0};
		lstrcpy(lpPath,strPath);
		PathAppend(lpPath,arrSubFileNames.GetAt(i));
		EnumCadFonts_File(lpPath);
	}
}
void GCADFonts::WriteToXmlFile()
{
	TCHAR lpPath[MAX_PATH+1];
	GetModuleFileName(GetModuleHandle(_T("acad.exe")),lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("Fonts\\cadgj_com_Fonts.xml"));
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
		_com_raise_error(hr);

	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='GB2312'"));
	lpDocument->appendChild(lpInstruction);

	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("Root")));

	lpDocument->appendChild(lpRoot);

	// 参数列表
	for (int l=0; l<m_sysFontsArray.GetCount(); l++)
	{

		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Fonts"));
		lpParam->setAttribute(_T("Name"), _variant_t(m_sysFontsArray.GetAt(l)));
		lpParam->setAttribute(_T("Type"), _variant_t(_T("SysFont")));

		lpRoot->appendChild(lpParam);
	}
	for (int l=0; l<m_strBigFontsArray.GetCount(); l++)
	{

		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Fonts"));
		lpParam->setAttribute(_T("Name"), _variant_t(m_strBigFontsArray.GetAt(l)));
		lpParam->setAttribute(_T("Type"), _variant_t(_T("BigFont")));

		lpRoot->appendChild(lpParam);
	}

	for (int l=0; l<m_strCommShxFontsArray.GetCount(); l++)
	{

		MSXML2::IXMLDOMElementPtr lpParam = lpDocument->createElement(_T("Fonts"));
		lpParam->setAttribute(_T("Name"), _variant_t(m_strCommShxFontsArray.GetAt(l)));
		lpParam->setAttribute(_T("Type"), _variant_t(_T("CommShxFont")));

		lpRoot->appendChild(lpParam);
	}

	hr=lpDocument->save(lpPath);
}
void GCADFonts::ReadFromXmlFile()
{
	TCHAR lpPath[MAX_PATH+1];
	GetModuleFileName(GetModuleHandle(_T("acad.exe")),lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath,_T("Fonts\\cadgj_com_Fonts.xml"));
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return;
	}
	//加载XML文件
	if(!lpDocument->load(lpPath))
	{
		return;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		_com_raise_error(hr);
		return;
	}
	m_sysFontsArray.RemoveAll();
	m_strBigFontsArray.RemoveAll();
	m_strCommShxFontsArray.RemoveAll();
	MSXML2::IXMLDOMNodeListPtr lpCmdNodeList=lpDocElement->getElementsByTagName(_T("Fonts"));
	MSXML2::IXMLDOMNodePtr lpCmdNode = NULL;

	for ( ; (lpCmdNode = lpCmdNodeList->nextNode()) != NULL ; )
	{
		CString strName=GetNodeAttrValue(lpCmdNode,_T("Name"));
		CString strType=GetNodeAttrValue(lpCmdNode,_T("Type"));
		if(strName.IsEmpty()||strType.IsEmpty())
		{
			continue;
		}
		if(strType.CompareNoCase(_T("SysFont"))==0)
		{
			m_sysFontsArray.Add(strName);
		}
		else if(strType.CompareNoCase(_T("BigFont"))==0)
		{
			m_strBigFontsArray.Add(strName);
		}
		else if(strType.CompareNoCase(_T("CommShxFont"))==0)
		{
			m_strCommShxFontsArray.Add(strName);
		}
	}
}