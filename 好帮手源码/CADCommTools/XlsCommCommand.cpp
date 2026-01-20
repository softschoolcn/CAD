#include "StdAfx.h"
#if _MSC_VER<=1300
   #ifndef ACAD_2004
      #include "dbtable.h"
   #endif
#endif
#include "XlsCommCommand.h"
#include "GMultiJig.h"
void XlsToTable(AcDbVoidPtrArray& pTableArr,const CString xlsFileName,AcGePoint3d insertPt);
#ifdef ACAD_2004
void InsertXls()
{
	acutPrintf(_T("\nAutoCAD2004不支持表格功能，如果您需要使用插入表格功能，请升级至AutoCAD2005或以上版本"));
}
//导出Excel表格
void ToExcel()
{
	acutPrintf(_T("\nAutoCAD2004不支持表格功能，如果您需要使用导出Excel表格功能，请升级至AutoCAD2005或以上版本"));
}
#else
void InsertXls()
{
	// 默认以项目编号命名文件名
	CString lpDwgFile;
	CFileDialog fileDlg(true, NULL, NULL, OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
		_T("Excel文件 (*.xls;*.xlsx)|*.xls;*.xlsx||"), acedGetAcadFrame());
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	lpDwgFile=fileDlg.GetPathName();
	AcDbVoidPtrArray pTableArr;
	XlsToTable(pTableArr,lpDwgFile,AcGePoint3d(0.0,0.0,0.0));
	if(pTableArr.logicalLength()==0)
	{
		return;
	}
	GMultiJig jig;
	for(int i=0;i<pTableArr.logicalLength();i++)
	{
		jig.Add((AcDbEntity*)pTableArr.at(i));
	}
	int ret = jig.Do();
	if(ret==RTNORM)
	{
		AcDbObjectIdArray ids;
		jig.GetIds(ids);
		DyZoom(ids,1.1);
	}
}
struct XmlCellData
{
	int column;
	int row;
	int mergecolumn;
	int mergerow;
	double height;
	double width;
	CString strVAlign;//垂直对齐
	CString strHAlign;//水平对齐

};
struct XmlCellValue
{
	int column;
	int row;
	CString text;
};
extern CString GetNodeAttrValue(MSXML2::IXMLDOMNodePtr lpNode,CString strName);
BOOL GetCellData(XmlCellData& data,CArray<XmlCellData>& datas,int row,int column)
{
	for(int i=0;i<datas.GetCount();i++)
	{
		XmlCellData& tmp=datas.GetAt(i);
		if(tmp.column==column&&tmp.row==row)
		{
			data=tmp;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL GetCellText(XmlCellValue& val,CArray<XmlCellValue>& values,int row,int column)
{
	for(int i=0;i<values.GetCount();i++)
	{
		XmlCellValue& tmp=values.GetAt(i);
		if(tmp.column==column&&tmp.row==row)
		{
			val=tmp;
			return TRUE;
		}
	}
	return FALSE;
}
void XmlToTable2(AcDbVoidPtrArray& pTableArr,MSXML2::IXMLDOMNodePtr lpSheetNode,AcGePoint3d& insertPt)
{

	MSXML2::IXMLDOMNodePtr lpCellsNode=lpSheetNode->selectSingleNode("cells");
	if(lpCellsNode==NULL)
	{
		return;
	}
	MSXML2::IXMLDOMNodeListPtr lpCellNodeList=lpCellsNode->selectNodes("cell");
	MSXML2::IXMLDOMNodePtr lpCellNode = NULL;
	CArray<XmlCellData> datas;

	for ( ; (lpCellNode = lpCellNodeList->nextNode()) != NULL ; )
	{
		XmlCellData data;
		data.column=_ttoi(GetNodeAttrValue(lpCellNode,_T("column")));
		data.row=_ttoi(GetNodeAttrValue(lpCellNode,_T("row")));
		data.mergecolumn=_ttoi(GetNodeAttrValue(lpCellNode,_T("mergecolumn")));
		data.mergerow=_ttoi(GetNodeAttrValue(lpCellNode,_T("mergerow")));
		data.width=_tstof(GetNodeAttrValue(lpCellNode,_T("width")));
		data.height=_tstof(GetNodeAttrValue(lpCellNode,_T("height")));
		data.strHAlign=GetNodeAttrValue(lpCellNode,_T("horizontalalignment"));
		data.strVAlign=GetNodeAttrValue(lpCellNode,_T("verticalalignment"));



		datas.Add(data);
	}
	CArray<XmlCellValue> values;
	MSXML2::IXMLDOMNodePtr lpValuesNode=lpSheetNode->selectSingleNode("values");
	if(lpValuesNode==NULL)
	{
		return;
	}
	int nRowCount=0,nColumnCount=0;
	MSXML2::IXMLDOMNodeListPtr lpValueNodeList=lpValuesNode->selectNodes("value");
	MSXML2::IXMLDOMNodePtr lpValueNode = NULL;

	for ( ; (lpValueNode = lpValueNodeList->nextNode()) != NULL ; )
	{
		XmlCellValue tmp;
		tmp.column=_ttoi(GetNodeAttrValue(lpValueNode,_T("column")));
		tmp.row=_ttoi(GetNodeAttrValue(lpValueNode,_T("row")));
		tmp.text=GetNodeAttrValue(lpValueNode,_T("text"));
		values.Add(tmp);
		XmlCellData dat;
		if(GetCellData(dat,datas,tmp.row,tmp.column))
		{
			if(!tmp.text.IsEmpty())
			{
				if(dat.column+dat.mergecolumn-1>nColumnCount)
				{
					nColumnCount=dat.column+dat.mergecolumn-1;
				}
				if(dat.row+dat.mergerow-1>nRowCount)
				{
					nRowCount=dat.row+dat.mergerow-1;
				}
			}

		}
	}
	AcDbTable *pTable=new AcDbTable();
	pTable->setNumColumns(nColumnCount);
	pTable->setNumRows(nRowCount);
	//设置列宽
	for(int i=0;i<nColumnCount;i++)
	{
		double dColumnWidth=-1.0;
		int nMerge=-1;
		for (int j=0;j<nRowCount;j++)
		{

			XmlCellData dat;
			if(GetCellData(dat,datas,j+1,i+1))
			{
				if(nMerge<0||dat.mergecolumn<nMerge)
				{
					nMerge=dat.mergecolumn;
					dColumnWidth=dat.width/dat.mergecolumn;
				}
			}
		}
		if(dColumnWidth>0)
		{
			pTable->setColumnWidth(i,dColumnWidth);
		}


	}
	//设置行高
	for (int j=0;j<nRowCount;j++)
	{
		double dRowHeight=-1.0;
		int nMerge=-1;
		for(int i=0;i<nColumnCount;i++)
		{

			XmlCellData dat;
			if(GetCellData(dat,datas,j+1,i+1))
			{
				if(nMerge<0||dat.mergerow<nMerge)
				{
					nMerge=dat.mergerow;
					dRowHeight=dat.height/dat.mergerow*0.5;
				}
			}

		}
		if(dRowHeight>0)
		{
			pTable->setRowHeight(j,dRowHeight);
		}
	}
	//设置单元格合并
	CStringArray insertRangeArray;//将已经处理的单元格记录下来，不重复处理
	for (int j=0;j<nRowCount;j++)
	{
		for(int i=0;i<nColumnCount;i++)
		{
			CString strId;
			strId.Format(_T("%d,%d"),j,i);
			if(IsStringInArray(insertRangeArray,strId))
				continue;//已经处理过了的单元格不再处理
			XmlCellData dat;
			if(GetCellData(dat,datas,j+1,i+1))
			{
				if(dat.mergecolumn>1||dat.mergerow>1)
				{
					pTable->mergeCells(j,min(j+dat.mergerow-1,nRowCount-1),i,min(i+dat.mergecolumn-1,nColumnCount-1));
					for (int k=1;k<=dat.mergerow;k++)
					{
						for(int l=1;l<=dat.mergecolumn;l++)
						{
							if(k==1&&l==1)
							{
								continue;
							}
							strId.Format(_T("%d,%d"),j+k-1,i+l-1);
							if(IsStringInArray(insertRangeArray,strId))
								continue;
							insertRangeArray.Add(strId);
						}
					}
				}
				AcDb::CellAlignment align=AcDb::kMiddleCenter;
				if(dat.strHAlign.CompareNoCase(_T("left"))==0)
				{
					//左对齐
					if(dat.strVAlign.CompareNoCase(_T("top"))==0)
					{
						//上对齐
						align=AcDb::kTopLeft;
					}
					else if(dat.strVAlign.CompareNoCase(_T("bottom"))==0)
					{
						//下对齐
						align=AcDb::kBottomLeft;
					}
					else
					{
						//居中对齐
						align=AcDb::kMiddleLeft;
					}
				}
				else if(dat.strHAlign.CompareNoCase(_T("right"))==0)
				{
					//右对齐
					if(dat.strVAlign.CompareNoCase(_T("top"))==0)
					{
						//上对齐
						align=AcDb::kTopRight;
					}
					else if(dat.strVAlign.CompareNoCase(_T("bottom"))==0)
					{
						//下对齐
						align=AcDb::kBottomRight;
					}
					else
					{
						//居中对齐
						align=AcDb::kMiddleRight;
					}

				}
				else
				{
					if(dat.strVAlign.CompareNoCase(_T("top"))==0)
					{
						//上对齐
						align=AcDb::kTopCenter;
					}
					else if(dat.strVAlign.CompareNoCase(_T("bottom"))==0)
					{
						//下对齐
						align=AcDb::kBottomCenter;
					}
					else
					{
						//居中对齐
						align=AcDb::kMiddleCenter;
					}
				}
				pTable->setAlignment(align);
			}
		}
	}
	//设置文本内容
	for (int j=0;j<nRowCount;j++)
	{
		for(int i=0;i<nColumnCount;i++)
		{
			XmlCellValue val;
			if(GetCellText(val,values,j+1,i+1))
			{
				pTable->setTextString(j,i,val.text);
			}
		}
	}
	//插入位置
	pTable->setPosition(insertPt);

	pTable->generateLayout(); // Very very important, else expect crashes later on
	AcDbExtents extents;
	pTable->getGeomExtents(extents);
	pTableArr.append(pTable);
	double dWidth=extents.maxPoint().x-extents.minPoint().x;
	insertPt.x+=dWidth*1.1;


}
void XmlToTable1(AcDbVoidPtrArray& pTableArr,const CString xmlFileName,AcGePoint3d insertPt)
{
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		acutPrintf(_T("\n创建XMLDocument对象失败"));
		return;
	}
	TCHAR lpFile[MAX_PATH+1]={0};
	lstrcpy(lpFile,xmlFileName);
	//加载XML文件
	if(!lpDocument->load((LPCTSTR)lpFile))
	{
		acutPrintf(_T("\n读取XML文件失败"));
		return;
	}
	MSXML2::IXMLDOMElementPtr lpDocElement = NULL;
	hr=lpDocument->get_documentElement(&lpDocElement);
	if ( FAILED(hr) ) 
	{
		acutPrintf(_T("\n获取根节点失败"));
		return;
	}
	_variant_t vt=lpDocElement->getAttribute("success");
	string tmp=BSTRToString(vt.bstrVal);
	CString strSuccess(tmp.c_str());
	vt=lpDocElement->getAttribute("message");
	tmp=BSTRToString(vt.bstrVal);
	if(strSuccess.CompareNoCase(_T("True"))!=0)
	{
		CString strMsg(tmp.c_str());
		acutPrintf(_T("\n读取EXCEL文件的内容出错:%s"),strMsg);
		return;
	}
	MSXML2::IXMLDOMNodeListPtr lpSheetNodeList=lpDocElement->getElementsByTagName(_T("sheet"));
	MSXML2::IXMLDOMNodePtr lpSheetNode = NULL;
	AcGePoint3d pt=insertPt;

	for ( ; (lpSheetNode = lpSheetNodeList->nextNode()) != NULL ; )
	{
		XmlToTable2(pTableArr,lpSheetNode,pt);
	}





}
void XlsToTable(AcDbVoidPtrArray& pTableArr,const CString xlsFileName,AcGePoint3d insertPt)
{
#if _MSC_VER <= 1300
	//CAD2007以上均支持表格
	if(acdbHostAppMinorVersion()<1)
	{
		acutPrintf(_T("\n当前CAD版本为AUTOCAD2004,不支持Table功能，如需实现标注点坐标，请更换至AUTOCAD2005或者AUTOCAD2006"));
		return;
	}
#endif
	CString strXlsFileName=xlsFileName;
	TCHAR lpFile1[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpFile1);
	CString strTmp=NewGuid()+".xml";
	PathAppend(lpFile1,strTmp);

	TCHAR lpFile[MAX_PATH+1];
	GetModuleFileName(_hdllInstance, lpFile, MAX_PATH);
	PathRemoveFileSpec(lpFile);
	PathRemoveFileSpec(lpFile);
	PathAppend(lpFile,_T("bin\\XlsToXml.exe"));
	if(!PathFileExists(lpFile))
	{
		acedAlert(_T("未找到XlsToXml.exe，请联系软件开发人员"));
		return;
	}
	CString strArg;
	strArg.Format(_T("\"%s\" \"0\" \"%s\" \"%s\""),lpFile,strXlsFileName,lpFile1);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(NULL,   // No module name (use command line). 
		strArg.GetBuffer(), // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 

	{
		//todo log
		//
		acedAlert(_T("调用XlsToXml.exe出错，请联系软件开发人员"));
		strArg.ReleaseBuffer();
		return;
	}
	strArg.ReleaseBuffer();
	acutPrintf(_T("\n正在打开EXCEL文件，请您稍后，如果文件较大可能需要一些时间，请耐心等待..."));
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if(!PathFileExists(lpFile1))
	{
		CString strInfo;
		strInfo.Format(_T("读取EXCEL文件内容失败，可能是由于下面三个原因照成的\r\n1:您的机器未安装.Net FrameWork\r\n2:您的机器未安装Microsoft Office\r\n3:EXCEL文件内容或格式不正确"));
		acedAlert(strInfo);
		return;
	}
	XmlToTable1(pTableArr,lpFile1,insertPt);
	DeleteFile(lpFile1);

}
void ReadValues(MSXML2::IXMLDOMElementPtr lpValues,AcDbTable* pTable)
{
	//遍历行
	CStringArray insertRangeArray;//将已经处理的单元格记录下来，不重复处理
	for(int i=0;i<pTable->numRows();i++)
	{
		for(int j=0;j<pTable->numColumns();j++)
		{
			CString strId;
			strId.Format(_T("%d,%d"),i,j);
			if(IsStringInArray(insertRangeArray,strId))
				continue;//已经处理过了的单元格不再处理

			int minRow=0;
			int maxRow=0;
			int minCol=0;
			int maxCol=0;
			MSXML2::IXMLDOMElementPtr lpValue =lpValues->ownerDocument->createElement(LPCTSTR(_T("value")));
			lpValue->setAttribute(_T("text"),pTable->textStringConst(i,j));
			lpValue->setAttribute(_T("column"),_variant_t(IntToString(j+1)));
			lpValue->setAttribute(_T("row"),_variant_t(IntToString(i+1)));
			lpValues->appendChild(lpValue);
			if(pTable->isMergedCell(i,j,&minRow,&maxRow,&minCol,&maxCol))
			{
				for (int k=minRow;k<=maxRow;k++)
				{
					for(int l=minCol;l<=maxCol;l++)
					{
						if(k==i&&l==j)
						{
							continue;
						}
						strId.Format(_T("%d,%d"),k,l);
						if(IsStringInArray(insertRangeArray,strId))
							continue;
						insertRangeArray.Add(strId);
					}
				}
			}
		}
	}
}
void ReadCells(MSXML2::IXMLDOMElementPtr lpCells, AcDbTable* pTable)
{
	//遍历行
	CStringArray insertRangeArray;//将已经处理的单元格记录下来，不重复处理
	for(int i=0;i<pTable->numRows();i++)
	{
		for(int j=0;j<pTable->numColumns();j++)
		{
			CString strId;
			strId.Format(_T("%d,%d"),i,j);
			if(IsStringInArray(insertRangeArray,strId))
				continue;//已经处理过了的单元格不再处理

			int minRow=0;
			int maxRow=0;
			int minCol=0;
			int maxCol=0;
			MSXML2::IXMLDOMElementPtr lpCell =lpCells->ownerDocument->createElement(LPCTSTR(_T("cell")));
			lpCell->setAttribute(_T("column"),_variant_t(IntToString(j+1)));
			lpCell->setAttribute(_T("row"),_variant_t(IntToString(i+1)));
			int mergeRowCount = 1;
			int mergeColumnCount = 1;
			if(pTable->isMergedCell(i,j,&minRow,&maxRow,&minCol,&maxCol))
			{
				mergeRowCount=maxRow-minRow+1;
				mergeColumnCount=maxCol-minCol+1;
				for (int k=minRow;k<=maxRow;k++)
				{
					for(int l=minCol;l<=maxCol;l++)
					{
						if(k==i&&l==j)
						{
							continue;
						}
						strId.Format(_T("%d,%d"),k,l);
						if(IsStringInArray(insertRangeArray,strId))
							continue;
						insertRangeArray.Add(strId);
					}
				}
			}
			lpCell->setAttribute(_T("mergecolumn"),_variant_t(IntToString(mergeColumnCount)));
			lpCell->setAttribute(_T("mergerow"),_variant_t(IntToString(mergeRowCount)));
			lpCell->setAttribute(_T("height"),_variant_t(DoubleToString(pTable->rowHeight(i))));
			lpCell->setAttribute(_T("width"),_variant_t(DoubleToString(pTable->columnWidth(j))));
			AcDb::CellAlignment align=pTable->alignment(i,j);
			CString strHAlign="center";
			CString strVAlign="center";
			if(align==AcDb::kTopLeft)
			{
				strHAlign=_T("left");
				strVAlign=_T("top");
			}
			else if(align==AcDb::kBottomLeft)
			{
				strHAlign=_T("left");
				strVAlign=_T("bottom");
			}
			else if(align==AcDb::kMiddleLeft)
			{
				strHAlign=_T("left");
			}
			else if(align==AcDb::kTopRight)
			{
				strHAlign=_T("right");
				strVAlign=_T("top");
			}
			else if(align==AcDb::kBottomRight)
			{
				strHAlign=_T("right");
				strVAlign=_T("bottom");
			}
			else if(align==AcDb::kMiddleRight)
			{
				strHAlign=_T("right");
			}
			else if(align==AcDb::kTopCenter)
			{
				strVAlign=_T("top");
			}
			else if(align==AcDb::kBottomCenter)
			{
				strVAlign=_T("bottom");
			}

			lpCell->setAttribute(_T("verticalalignment"),_variant_t(strVAlign));
			lpCell->setAttribute(_T("horizontalalignment"),_variant_t(strHAlign));
			lpCells->appendChild(lpCell);

		}
	}
}
void TableToXml(MSXML2::IXMLDOMElementPtr lpSheet,AcDbTable* pTable)
{
	MSXML2::IXMLDOMElementPtr lpValues =lpSheet->ownerDocument->createElement(LPCTSTR(_T("values")));
	ReadValues(lpValues,pTable);
	lpSheet->appendChild(lpValues);
	MSXML2::IXMLDOMElementPtr lpCells =lpSheet->ownerDocument->createElement(LPCTSTR(_T("cells")));
	ReadCells(lpCells,pTable);
	lpSheet->appendChild(lpCells);
}
//导出Excel表格
void ToExcel()
{
	Acad::ErrorStatus es=eOk;
	resbuf* rb = acutBuildList(RTDXF0, _T("ACAD_TABLE"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择需要导出的表格:"),_T("\n移除需要导出的表格:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids1;
	SSToIds(ids1,ssName);
	acedSSFree(ssName);
	if(ids1.logicalLength()==0)
	{
		return;
	}
	CString lpDwgFile;
	CFileDialog fileDlg(FALSE, _T(".xls"), NULL, OFN_LONGNAMES | OFN_PATHMUSTEXIST,
		_T("Excel文件 (*.xls)|*.xls||"), acedGetAcadFrame());
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	lpDwgFile=fileDlg.GetPathName();
	TCHAR lpFile1[MAX_PATH+1]={0};
	GetTempPath(MAX_PATH,lpFile1);
	CString strTmp=NewGuid()+".xml";
	PathAppend(lpFile1,strTmp);
	MSXML2::IXMLDOMDocumentPtr lpDocument;
	HRESULT hr = lpDocument.CreateInstance(__uuidof(DOMDocument));

	if ( FAILED(hr) ) 
	{
		acutPrintf(_T("\n创建XMLDocument对象失败"));
		return;
	}
	MSXML2::IXMLDOMProcessingInstructionPtr lpInstruction = lpDocument->createProcessingInstruction(_T("xml"),_T("version='1.0' encoding='UTF-8'"));
	lpDocument->appendChild(lpInstruction);

	MSXML2::IXMLDOMElementPtr lpRoot = lpDocument->createElement(LPCTSTR(_T("root")));
	for(int i=0;i<ids1.logicalLength();i++)
	{
		AcDbObjectId id=ids1.at(i);
		AcDbObjectPointer<AcDbTable> spTable(id,AcDb::kForRead);
		if(spTable.openStatus()!=Acad::eOk)
		{
			continue;
		}
		MSXML2::IXMLDOMElementPtr lpSheet = lpDocument->createElement(LPCTSTR(_T("sheet")));
		TableToXml(lpSheet,spTable.object());
		lpRoot->appendChild(lpSheet);

	}
	lpDocument->appendChild(lpRoot);
	lpDocument->save((LPCTSTR)lpFile1);
	TCHAR lpFile[MAX_PATH+1];
	GetModuleFileName(_hdllInstance, lpFile, MAX_PATH);
	PathRemoveFileSpec(lpFile);
	PathRemoveFileSpec(lpFile);
	PathAppend(lpFile,_T("bin\\XlsToXml.exe"));
	if(!PathFileExists(lpFile))
	{
		acedAlert(_T("未找到XlsToXml.exe，请联系软件开发人员"));
		return;
	}
	CString strArg;
	strArg.Format(_T("\"%s\" \"1\" \"%s\" \"%s\""),lpFile,lpFile1,lpDwgFile);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(NULL,   // No module name (use command line). 
		strArg.GetBuffer(), // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 

	{
		//todo log
		//
		acedAlert(_T("调用XlsToXml.exe出错，请联系软件开发人员"));
		strArg.ReleaseBuffer();
		return;
	}
	strArg.ReleaseBuffer();
	acutPrintf(_T("\n正在打开EXCEL写入数据，请您稍后，如果内容较多可能需要一些时间，请耐心等待..."));
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if(!PathFileExists(lpDwgFile))
	{
		CString strInfo;
		strInfo.Format(_T("读取EXCEL文件内容失败，可能是由于下面三个原因照成的\r\n1:您的机器未安装.Net FrameWork\r\n2:您的机器未安装Microsoft Office\r\n3:EXCEL文件内容或格式不正确"));
		acedAlert(strInfo);
		return;
	}
	DeleteFile(lpFile1);
	if(AfxMessageBox(_T("生成EXCEL文件成功，是否打开查看?"),MB_YESNO)==IDYES)
	{
		ShellExecute(NULL, _T("open"), lpDwgFile, NULL,NULL, SW_SHOW);
	}


}
#endif