#include "StdAfx.h"
#include "AdskDMgr.h"
// The one and only document manager object
// You can use the DocVars object to retrieve
// document specific data throughout your application
AsdkDataManager<CDocData> DocVars;


//
// Implementation of the document data class.
//
CDocData::CDocData() : m_pIPM(NULL)
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_DATA_INIT(CDocData)
	//}}AFX_ARX_DATA_INIT

	// TODO: add your own initialization.

}

CDocData::CDocData(const CDocData &data)
{
	// TODO: add your own initialization.
}

CDocData::~CDocData()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_DATA_DEL(CDocData)
	//}}AFX_ARX_DATA_DEL

	// TODO: clean up.
	if(m_pIPM != NULL) 
		delete m_pIPM;
}
