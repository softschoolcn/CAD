// GDimConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GDimConfigDialog.h"
#include "..\\CADGlobalTools\\GLockDocument.h"
#include "GCADFonts.h"
#include "ConfigToolsHeaders.h"
const COLORREF IndexedColors[] = {
	RGB(0, 0, 0),
	RGB(255, 0, 0),
	RGB(255, 255, 0),
	RGB(0, 255, 0),
	RGB(0, 255, 255),
	RGB(0, 0, 255),
	RGB(255, 0, 255),
	RGB(255, 255, 255),
	RGB(65, 65, 65),
	RGB(128, 128, 128),
	RGB(255, 0, 0),
	RGB(255, 170, 170),
	RGB(189, 0, 0),
	RGB(189, 126, 126),
	RGB(129, 0, 0),
	RGB(129, 86, 86),
	RGB(104, 0, 0),
	RGB(104, 69, 69),
	RGB(79, 0, 0),
	RGB(79, 53, 53),
	RGB(255, 63, 0),
	RGB(255, 191, 170),
	RGB(189, 46, 0),
	RGB(189, 141, 126),
	RGB(129, 31, 0),
	RGB(129, 96, 86),
	RGB(104, 25, 0),
	RGB(104, 78, 69),
	RGB(79, 19, 0),
	RGB(79, 59, 53),
	RGB(255, 127, 0),
	RGB(255, 212, 170),
	RGB(189, 94, 0),
	RGB(189, 157, 126),
	RGB(129, 64, 0),
	RGB(129, 107, 86),
	RGB(104, 52, 0),
	RGB(104, 86, 69),
	RGB(79, 39, 0),
	RGB(79, 66, 53),
	RGB(255, 191, 0),
	RGB(255, 234, 170),
	RGB(189, 141, 0),
	RGB(189, 173, 126),
	RGB(129, 96, 0),
	RGB(129, 118, 86),
	RGB(104, 78, 0),
	RGB(104, 95, 69),
	RGB(79, 59, 0),
	RGB(79, 73, 53),
	RGB(255, 255, 0),
	RGB(255, 255, 170),
	RGB(189, 189, 0),
	RGB(189, 189, 126),
	RGB(129, 129, 0),
	RGB(129, 129, 86),
	RGB(104, 104, 0),
	RGB(104, 104, 69),
	RGB(79, 79, 0),
	RGB(79, 79, 53),
	RGB(191, 255, 0),
	RGB(234, 255, 170),
	RGB(141, 189, 0),
	RGB(173, 189, 126),
	RGB(96, 129, 0),
	RGB(118, 129, 86),
	RGB(78, 104, 0),
	RGB(95, 104, 69),
	RGB(59, 79, 0),
	RGB(73, 79, 53),
	RGB(127, 255, 0),
	RGB(212, 255, 170),
	RGB(94, 189, 0),
	RGB(157, 189, 126),
	RGB(64, 129, 0),
	RGB(107, 129, 86),
	RGB(52, 104, 0),
	RGB(86, 104, 69),
	RGB(39, 79, 0),
	RGB(66, 79, 53),
	RGB(63, 255, 0),
	RGB(191, 255, 170),
	RGB(46, 189, 0),
	RGB(141, 189, 126),
	RGB(31, 129, 0),
	RGB(96, 129, 86),
	RGB(25, 104, 0),
	RGB(78, 104, 69),
	RGB(19, 79, 0),
	RGB(59, 79, 53),
	RGB(0, 255, 0),
	RGB(170, 255, 170),
	RGB(0, 189, 0),
	RGB(126, 189, 126),
	RGB(0, 129, 0),
	RGB(86, 129, 86),
	RGB(0, 104, 0),
	RGB(69, 104, 69),
	RGB(0, 79, 0),
	RGB(53, 79, 53),
	RGB(0, 255, 63),
	RGB(170, 255, 191),
	RGB(0, 189, 46),
	RGB(126, 189, 141),
	RGB(0, 129, 31),
	RGB(86, 129, 96),
	RGB(0, 104, 25),
	RGB(69, 104, 78),
	RGB(0, 79, 19),
	RGB(53, 79, 59),
	RGB(0, 255, 127),
	RGB(170, 255, 212),
	RGB(0, 189, 94),
	RGB(126, 189, 157),
	RGB(0, 129, 64),
	RGB(86, 129, 107),
	RGB(0, 104, 52),
	RGB(69, 104, 86),
	RGB(0, 79, 39),
	RGB(53, 79, 66),
	RGB(0, 255, 191),
	RGB(170, 255, 234),
	RGB(0, 189, 141),
	RGB(126, 189, 173),
	RGB(0, 129, 96),
	RGB(86, 129, 118),
	RGB(0, 104, 78),
	RGB(69, 104, 95),
	RGB(0, 79, 59),
	RGB(53, 79, 73),
	RGB(0, 255, 255),
	RGB(170, 255, 255),
	RGB(0, 189, 189),
	RGB(126, 189, 189),
	RGB(0, 129, 129),
	RGB(86, 129, 129),
	RGB(0, 104, 104),
	RGB(69, 104, 104),
	RGB(0, 79, 79),
	RGB(53, 79, 79),
	RGB(0, 191, 255),
	RGB(170, 234, 255),
	RGB(0, 141, 189),
	RGB(126, 173, 189),
	RGB(0, 96, 129),
	RGB(86, 118, 129),
	RGB(0, 78, 104),
	RGB(69, 95, 104),
	RGB(0, 59, 79),
	RGB(53, 73, 79),
	RGB(0, 127, 255),
	RGB(170, 212, 255),
	RGB(0, 94, 189),
	RGB(126, 157, 189),
	RGB(0, 64, 129),
	RGB(86, 107, 129),
	RGB(0, 52, 104),
	RGB(69, 86, 104),
	RGB(0, 39, 79),
	RGB(53, 66, 79),
	RGB(0, 63, 255),
	RGB(170, 191, 255),
	RGB(0, 46, 189),
	RGB(126, 141, 189),
	RGB(0, 31, 129),
	RGB(86, 96, 129),
	RGB(0, 25, 104),
	RGB(69, 78, 104),
	RGB(0, 19, 79),
	RGB(53, 59, 79),
	RGB(0, 0, 255),
	RGB(170, 170, 255),
	RGB(0, 0, 189),
	RGB(126, 126, 189),
	RGB(0, 0, 129),
	RGB(86, 86, 129),
	RGB(0, 0, 104),
	RGB(69, 69, 104),
	RGB(0, 0, 79),
	RGB(53, 53, 79),
	RGB(63, 0, 255),
	RGB(191, 170, 255),
	RGB(46, 0, 189),
	RGB(141, 126, 189),
	RGB(31, 0, 129),
	RGB(96, 86, 129),
	RGB(25, 0, 104),
	RGB(78, 69, 104),
	RGB(19, 0, 79),
	RGB(59, 53, 79),
	RGB(127, 0, 255),
	RGB(212, 170, 255),
	RGB(94, 0, 189),
	RGB(157, 126, 189),
	RGB(64, 0, 129),
	RGB(107, 86, 129),
	RGB(52, 0, 104),
	RGB(86, 69, 104),
	RGB(39, 0, 79),
	RGB(66, 53, 79),
	RGB(191, 0, 255),
	RGB(234, 170, 255),
	RGB(141, 0, 189),
	RGB(173, 126, 189),
	RGB(96, 0, 129),
	RGB(118, 86, 129),
	RGB(78, 0, 104),
	RGB(95, 69, 104),
	RGB(59, 0, 79),
	RGB(73, 53, 79),
	RGB(255, 0, 255),
	RGB(255, 170, 255),
	RGB(189, 0, 189),
	RGB(189, 126, 189),
	RGB(129, 0, 129),
	RGB(129, 86, 129),
	RGB(104, 0, 104),
	RGB(104, 69, 104),
	RGB(79, 0, 79),
	RGB(79, 53, 79),
	RGB(255, 0, 191),
	RGB(255, 170, 234),
	RGB(189, 0, 141),
	RGB(189, 126, 173),
	RGB(129, 0, 96),
	RGB(129, 86, 118),
	RGB(104, 0, 78),
	RGB(104, 69, 95),
	RGB(79, 0, 59),
	RGB(79, 53, 73),
	RGB(255, 0, 127),
	RGB(255, 170, 212),
	RGB(189, 0, 94),
	RGB(189, 126, 157),
	RGB(129, 0, 64),
	RGB(129, 86, 107),
	RGB(104, 0, 52),
	RGB(104, 69, 86),
	RGB(79, 0, 39),
	RGB(79, 53, 66),
	RGB(255, 0, 63),
	RGB(255, 170, 191),
	RGB(189, 0, 46),
	RGB(189, 126, 141),
	RGB(129, 0, 31),
	RGB(129, 86, 96),
	RGB(104, 0, 25),
	RGB(104, 69, 78),
	RGB(79, 0, 19),
	RGB(79, 53, 59),
	RGB(51, 51, 51),
	RGB(80, 80, 80),
	RGB(105, 105, 105),
	RGB(130, 130, 130),
	RGB(190, 190, 190),
	RGB(255, 255, 255)
};
CString GetDimConfigValue(CString strKey,CString strDefault)
{
	CString strValue=GUserDataRecord::getInstance()->GetProfileString(_T("GDimConfigDialog"),strKey,128,strDefault);
	return strValue;
}
Acad::ErrorStatus UpdateDimTextStyleByConfig(AcDbTextStyleTableRecord* pRec,AcDbDatabase* pDB=acdbHostApplicationServices()->workingDatabase())
{
	CString strValue=GetDimConfigValue(_T("TextWH"),_T("0.70"));
	pRec->setXScale(_tstof(strValue));
	strValue=GetDimConfigValue(_T("CommFont"),_T("txt.shx"));
	pRec->setFileName(strValue);
	strValue=GetDimConfigValue(_T("BigFont"),_T("gbcbig.shx"));
	pRec->setBigFontFileName(strValue);
	return Acad::eOk;
}
Acad::ErrorStatus UpdateDimTextStyleByConfig(AcDbObjectId&   rcId,AcDbDatabase* pDB)
{
	if(pDB==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbTextStyleTablePointer spTable(pDB->textStyleTableId(),AcDb::kForRead);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	AcDbObjectId recordId;
	Acad::ErrorStatus es=spTable->getAt(_T("cadgj_com_dimension"),recordId);
	if(es!=Acad::eOk)
	{
		AcDbTextStyleTableRecord   * pRec=new AcDbTextStyleTableRecord();
		pRec->setName(_T("cadgj_com_dimension"));
		es=UpdateDimTextStyleByConfig(pRec,pDB);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		if(spTable->upgradeOpen()!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return spTable->upgradeOpen();
		}
		es=spTable->add(rcId,pRec);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		pRec->close();
		return es;
		
	}
	else
	{
		AcDbTextStyleTableRecordPointer spRec(recordId,AcDb::kForWrite);
		if(spRec.openStatus()!=Acad::eOk)
		{
			return spRec.openStatus();
		}
		es=UpdateDimTextStyleByConfig(spRec.object(),pDB);
		if(es!=Acad::eOk)
		{
			return es;
		}
		rcId=recordId;
		return es;
	}
}
//根据配置修改标注样式
Acad::ErrorStatus UpdateDimStyleByConfig(AcDbDimStyleTableRecord   * pRec,AcDbDatabase* pDB)
{
	int nHTBL=1;
	int nCTBL=1;
	GCADUtils::GetScale(nHTBL,nCTBL);
	CString strValue=GetDimConfigValue(_T("Arrow"),_T("粗线"));
	CString strBLK=_T("_ARCHTICK");

	if(strValue.CompareNoCase(_T("粗线"))==0)
	{
		//粗线
		//建筑标记
		strBLK=_T("_ARCHTICK");
	}
	else if(strValue.CompareNoCase(_T("细线"))==0)
	{
		//细线
		//倾斜
		strBLK=_T("_OBLIQUE");
	}
	else if(strValue.CompareNoCase(_T("圆点"))==0)
	{
		//圆点
		//小点
		strBLK=_T("_DOTSMALL");
	}
	else if(strValue.CompareNoCase(_T("箭头"))==0)
	{
		//箭头
		//实心闭合
		strBLK=_T("");
	}
	pRec->setDimblk(strBLK);
	pRec->setDimblk1(strBLK);
	pRec->setDimblk2(strBLK);
	pRec->setDimtad(1);//垂直放置文字
	pRec->setDimtih(false);//文字界内对齐，打开


	strValue=GetDimConfigValue(_T("LCCTA"),_T("1.00"));
	pRec->setDimdle(_tstof(strValue)*nHTBL);

	strValue=GetDimConfigValue(_T("DIMEXE"),_T("1.00"));
	pRec->setDimexe(_tstof(strValue)*nHTBL);

	strValue=GetDimConfigValue(_T("DIMEX0"),_T("2.50"));
	pRec->setDimexo(_tstof(strValue)*nHTBL);

	strValue=GetDimConfigValue(_T("XSD"),_T("2"));
	pRec->setDimdec(_ttoi(strValue)*nHTBL);
	//文字偏移
	pRec->setDimgap(1.0*nHTBL);
	//箭头大小
	pRec->setDimasz(1.0*nHTBL);

	CString strColor=GUserDataRecord::getInstance()->GetProfileString(_T("GDimConfigDialog"),_T("TextColor"),128,_T("7"));
	int nColor=_ttoi(strColor);
	if(nColor<1||nColor>255)
	{
		nColor=7;
	}
	AcCmColor col;
	col.setColorIndex(nColor);
	pRec->setDimclrt(col);
	strValue=GetDimConfigValue(_T("TextHeight"),_T("3.00"));
	pRec->setDimtxt(_tstof(strValue)*nHTBL);

	AcDbObjectId   rcId;
	Acad::ErrorStatus es=UpdateDimTextStyleByConfig(rcId,pDB);
	if(es!=Acad::eOk)
	{
		return es;
	}
	//DIMTXSTY
	pRec->setDimtxsty(rcId);
	
	//DIMDEC:换算精度
	return Acad::eOk;
}
//根据配置修改标注样式,返回标注样式的ID
Acad::ErrorStatus UpdateDimStyleByConfig(AcDbObjectId&   rcId,bool bCreateNew,AcDbDatabase* pDB)
{
	if(pDB==NULL)
	{
		return Acad::eNullHandle;
	}
	AcDbDimStyleTablePointer spTable(pDB->dimStyleTableId(),AcDb::kForRead);
	if(spTable.openStatus()!=Acad::eOk)
	{
		return spTable.openStatus();
	}
	AcDbObjectId recordId;
	Acad::ErrorStatus es=spTable->getAt(_T("cadgj_com_dimension"),recordId);
	if(es!=Acad::eOk)
	{
		if(!bCreateNew)
		{
			return Acad::eOk;
		}
		AcDbDimStyleTableRecord   * pRec=new AcDbDimStyleTableRecord();
		pRec->setName(_T("cadgj_com_dimension"));
		es=UpdateDimStyleByConfig(pRec,pDB);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		if(spTable->upgradeOpen()!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return spTable->upgradeOpen();
		}
		es=spTable->add(rcId,pRec);
		if(es!=Acad::eOk)
		{
			delete pRec;
			pRec=NULL;
			return es;
		}
		pRec->close();
		return es;
		
	}
	else
	{
		AcDbDimStyleTableRecordPointer spRec(recordId,AcDb::kForWrite);
		if(spRec.openStatus()!=Acad::eOk)
		{
			return spRec.openStatus();
		}
		es=UpdateDimStyleByConfig(spRec.object(),pDB);
		if(es!=Acad::eOk)
		{
			return es;
		}
		rcId=recordId;
		return es;
	}


}
// GDimConfigDialog 对话框
IMPLEMENT_DYNAMIC(GDimConfigDialog, CDialog)

GDimConfigDialog::GDimConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GDimConfigDialog::IDD, pParent)
{
	CreateNewDim();
	m_nColorIndex=7;
}

GDimConfigDialog::~GDimConfigDialog()
{
	AcGsView* pView = mPreviewCtrl.view();
	pView->erase(m_pDim);
	mPreviewCtrl.clearAll();
	if(m_pDim!=NULL)
	{
		//暂时还不能释放，后面想办法
		delete m_pDim;
		m_pDim=NULL;
	}
}

void GDimConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_COLOR,m_ColorButton);
}


BEGIN_MESSAGE_MAP(GDimConfigDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ARROW, &GDimConfigDialog::OnCbnSelchangeComboArrow)
	ON_CBN_SELCHANGE(IDC_COMBO_LCCTA, &GDimConfigDialog::OnCbnSelchangeComboLccta)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTHEIGHT, &GDimConfigDialog::OnCbnSelchangeComboTextheight)
	ON_CBN_SELCHANGE(IDC_COMBO_DIMEXE, &GDimConfigDialog::OnCbnSelchangeComboDimexe)
	ON_CBN_SELCHANGE(IDC_COMBO_DIMEX0, &GDimConfigDialog::OnCbnSelchangeComboDimex0)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &GDimConfigDialog::OnBnClickedButtonColor)
	ON_CBN_SELCHANGE(IDC_COMBO_XSD, &GDimConfigDialog::OnCbnSelchangeComboXsd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEFONTS, &GDimConfigDialog::OnBnClickedButtonUpdatefonts)
END_MESSAGE_MAP()


// GDimConfigDialog 消息处理程序


BOOL GDimConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (!mPreviewCtrl.SubclassDlgItem(IDC_PREVIEW,this))
		return FALSE;
	mPreviewCtrl.init(_hdllInstance,true);
	InitPreview();
	InitDimPreview();
	ZoomAndUpdate();
	m_ColorButton.SetWindowText(_T("白色"));
	m_ColorButton.SetFillColor(RGB(255,255,255));
	UpdateFonts();
	RoolbackUserSet();
	UpdateDim();
	return TRUE;
}
void GDimConfigDialog::RoolbackUserSet()
{
	RoolbackUserComboSet(IDC_COMBO_ARROW,_T("Arrow"),_T("粗线"));
	RoolbackUserComboSet(IDC_COMBO_LCCTA,_T("LCCTA"),_T("1.00"));
	RoolbackUserComboSet(IDC_COMBO_DIMEXE,_T("DIMEXE"),_T("1.00"));
	RoolbackUserComboSet(IDC_COMBO_DIMEX0,_T("DIMEX0"),_T("2.50"));
	RoolbackUserComboSet(IDC_COMBO_XSD,_T("XSD"),_T("2"));

	RoolbackUserComboSet(IDC_COMBO_TEXTHEIGHT,_T("TextHeight"),_T("3.00"));
	RoolbackUserComboSet(IDC_COMBO_TEXTWH,_T("TextWH"),_T("0.70"));
	RoolbackUserComboSet(IDC_COMBO_COMMFONT,_T("CommFont"),_T("txt.shx"));
	RoolbackUserComboSet(IDC_COMBO_BIGFONT,_T("BigFont"),_T("gbcbig.shx"));
	CString strColor=GUserDataRecord::getInstance()->GetProfileString(_T("GDimConfigDialog"),_T("TextColor"),128,_T("7"));
	int nColor=_ttoi(strColor);
	if(nColor<1||nColor>255)
	{
		nColor=7;
	}
	m_ColorButton.SetWindowText(GetIndexColorName(nColor));
	m_ColorButton.SetFillColor(IndexedColors[nColor]);
	m_nColorIndex=nColor;
	m_ColorButton.Invalidate();
	m_ColorButton.UpdateWindow();
}
void GDimConfigDialog::RecordUserSet()
{
	RecordUserComboSet(IDC_COMBO_ARROW,_T("Arrow"));
	RecordUserComboSet(IDC_COMBO_LCCTA,_T("LCCTA"));
	RecordUserComboSet(IDC_COMBO_DIMEXE,_T("DIMEXE"));
	RecordUserComboSet(IDC_COMBO_DIMEX0,_T("DIMEX0"));
	RecordUserComboSet(IDC_COMBO_XSD,_T("XSD"));

	RecordUserComboSet(IDC_COMBO_TEXTHEIGHT,_T("TextHeight"));
	RecordUserComboSet(IDC_COMBO_TEXTWH,_T("TextWH"));
	RecordUserComboSet(IDC_COMBO_COMMFONT,_T("CommFont"));
	RecordUserComboSet(IDC_COMBO_BIGFONT,_T("BigFont"));

	CString strColor;
	strColor.Format(_T("%d"),m_nColorIndex);
	GUserDataRecord::getInstance()->WriteProfileString(_T("GDimConfigDialog"),_T("TextColor"),strColor);

}
void GDimConfigDialog::RecordUserComboSet(int nId,CString strKey)
{
	CComboBox* pBox=(CComboBox*)GetDlgItem(nId);
	if(pBox==NULL)
	{
		return;
	}
	if(pBox->GetCount()==0)
	{
		return;
	}
	if(pBox->GetCurSel()<0)
	{
		return;
	}
	CString strText;
	pBox->GetLBText(pBox->GetCurSel(),strText);
	GUserDataRecord::getInstance()->WriteProfileString(_T("GDimConfigDialog"),strKey,strText);
}
void GDimConfigDialog::UpdateFonts()
{
	GCADFonts* pFonts=GCADFonts::GetInstance();
	pFonts->Enum();
	CComboBox* pBox=(CComboBox*)GetDlgItem(IDC_COMBO_COMMFONT);
	if(pBox!=NULL)
	{
		for(int i=0;i<pFonts->m_strCommShxFontsArray.GetCount();i++)
		{
			pBox->AddString(pFonts->m_strCommShxFontsArray.GetAt(i));
		}
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_BIGFONT);
	if(pBox!=NULL)
	{
		for(int i=0;i<pFonts->m_strBigFontsArray.GetCount();i++)
		{
			pBox->AddString(pFonts->m_strBigFontsArray.GetAt(i));
		}
	}
}
void GDimConfigDialog::RoolbackUserComboSet(int nId,CString strKey,CString strDefault)
{
	CComboBox* pBox=(CComboBox*)GetDlgItem(nId);
	if(pBox==NULL)
	{
		return;
	}
	if(pBox->GetCount()==0)
	{
		return;
	}
	pBox->SetCurSel(0);
	CString strValue=GUserDataRecord::getInstance()->GetProfileString(_T("GDimConfigDialog"),strKey,128,strDefault);
	this->SelectComboxItem(pBox,strValue);
}
Acad::ErrorStatus GDimConfigDialog::InitPreview()
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if (pDb == NULL)
    {
        return Acad::eNoDatabase;
    }
    //取数据库的范围
    AcGePoint3d extMax = pDb->extmax();
    AcGePoint3d extMin = pDb->extmin();

    //取活动视图的信息
    ads_real height = 0.0, width = 0.0, viewTwist = 0.0;
    AcGePoint3d targetView;
    AcGeVector3d viewDir;
    GetActiveViewPortInfo(height, width, targetView, viewDir, viewTwist, true);

    //取观察变换矩阵
    viewDir = viewDir.normal();
    AcGeVector3d viewXDir = viewDir.perpVector().normal();
    viewXDir = viewXDir.rotateBy(viewTwist, -viewDir);
    AcGeVector3d viewYDir = viewDir.crossProduct(viewXDir);
    AcGePoint3d boxCenter = extMin + 0.5 * (extMax - extMin);
    AcGeMatrix3d viewMat;
    viewMat = AcGeMatrix3d::alignCoordSys(boxCenter, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis, AcGeVector3d::kZAxis,  
        boxCenter, viewXDir, viewYDir, viewDir).inverse();

    //转换观察范围
    //AcDbExtents wcsExtents
	AcDbExtents viewExtents(extMin, extMax);// = wcsExtents;
	viewExtents.transformBy(viewMat);

    //设置视图视点信息
	double xMax = fabs(viewExtents.maxPoint().x - viewExtents.minPoint().x);
	double yMax = fabs(viewExtents.maxPoint().y - viewExtents.minPoint().y);
	AcGePoint3d eye = boxCenter + viewDir;
	AcGsView* pView = mPreviewCtrl.view();
	pView->setView(eye, boxCenter, viewYDir, xMax, yMax);

    return Acad::eOk;
}
bool GDimConfigDialog::GetActiveViewPortInfo(ads_real& height, ads_real& width,
                                          AcGePoint3d& target, AcGeVector3d& viewDir,
                                          ads_real& viewTwist, bool getViewCenter)
{
    acedVports2VportTableRecords();
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if (pDb == NULL)
    {
        return false;
    }

    AcDbViewportTable* pVTable = NULL;
    Acad::ErrorStatus es = pDb->getViewportTable(pVTable, AcDb::kForRead);
    if (es != Acad::eOk)
    {
        return false;
    }
    pVTable->close();

    AcDbViewportTableRecord* pViewPortRec = NULL;
    es = pVTable->getAt(_T("*Active"), pViewPortRec, AcDb::kForRead);
    if (es != Acad::eOk)
    {
        return false;
    }
    pViewPortRec->close();

    height = pViewPortRec->height();
    width = pViewPortRec->width();
    if (getViewCenter == true)
    {
        struct resbuf rb;
        memset(&rb, 0, sizeof(struct resbuf));
        acedGetVar(_T("VIEWCTR"), &rb);
        target = AcGePoint3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]);
    }
    else
    {
        target = pViewPortRec->target();
    }
    viewDir = pViewPortRec->viewDirection();
    viewTwist = pViewPortRec->viewTwist();

    return true;
}
void GDimConfigDialog::InitDimPreview()
{
	//AcGsView* pView = mPreviewCtrl.view();
	//pView->add(m_pDim,mPreviewCtrl.model());
}
void GDimConfigDialog::ZoomAndUpdate()
{
	
	AcDbExtents extents(AcGePoint3d(-2,-3,0),AcGePoint3d(20,8,0));
	double height = extents.maxPoint().y-extents.minPoint().y;
	double width = extents.maxPoint().x-extents.minPoint().x;
	AcGePoint3d target=GCADUtils::MidPoint(extents.minPoint(),extents.maxPoint());
    target.z = 0;
	height*=2;
	width*=2;

    AcGePoint3d eye(target.x, target.y, 1);
	AcGsView* pView = mPreviewCtrl.view();
    pView->setView(eye, target, AcGeVector3d::kYAxis, width, height);
	pView->erase(m_pDim);
	pView->add(m_pDim,mPreviewCtrl.model());

	pView->invalidate();
	pView->update();
}
void GDimConfigDialog::UpdateDim()
{
	AcGsView* pView = mPreviewCtrl.view();
	pView->erase(m_pDim);
	delete m_pDim;
	m_pDim=NULL;

	CreateNewDim();
	Acad::ErrorStatus es=Acad::eOk;
	CComboBox* pBox=(CComboBox*)GetDlgItem(IDC_COMBO_LCCTA);
	if(pBox->GetCurSel()>=0)
	{
		CString strDle;
		pBox->GetLBText(pBox->GetCurSel(),strDle);
		es=m_pDim->setDimdle(_tstof(strDle));
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_ARROW);
	if(pBox->GetCurSel()>=0)
	{
		CString strBLK=_T("_ARCHTICK");

		if(pBox->GetCurSel()==0)
		{
			//粗线
			//建筑标记
			strBLK=_T("_ARCHTICK");
		}
		else if(pBox->GetCurSel()==1)
		{
			//细线
			//倾斜
			strBLK=_T("_OBLIQUE");
		}
		else if(pBox->GetCurSel()==2)
		{
			//圆点
			//小点
			strBLK=_T("_DOTSMALL");
		}
		else if(pBox->GetCurSel()==3)
		{
			//箭头
			//实心闭合
			strBLK=_T("");
		}
		m_pDim->setDimblk(strBLK);
	    m_pDim->setDimblk1(strBLK);
	    m_pDim->setDimblk2(strBLK);
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_TEXTHEIGHT);
	if(pBox->GetCurSel()>=0)
	{
		CString strTextHeight;
		pBox->GetLBText(pBox->GetCurSel(),strTextHeight);
		es=m_pDim->setDimtxt(_tstof(strTextHeight));
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_DIMEXE);
	if(pBox->GetCurSel()>=0)
	{
		CString strDimExe;
		pBox->GetLBText(pBox->GetCurSel(),strDimExe);
		es=m_pDim->setDimexe(_tstof(strDimExe));
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_DIMEX0);
	if(pBox->GetCurSel()>=0)
	{
		CString strDimExo;
		pBox->GetLBText(pBox->GetCurSel(),strDimExo);
		es=m_pDim->setDimexo(_tstof(strDimExo));
	}

	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_XSD);
	if(pBox->GetCurSel()>=0)
	{
		CString strDimTxt=_T("5");
		int nXSD=pBox->GetCurSel();
		if(nXSD==0)
		{
			strDimTxt=_T("5");
		}
		else
		{
			CString strFormat;
			strFormat.Format(_T(".%df"),nXSD);
			strFormat=_T("%")+strFormat;
			strDimTxt.Format(strFormat,5.4321);
		}
		m_pDim->setDimensionText(strDimTxt);
	}
	else
	{
		m_pDim->setDimensionText(_T("5.43"));
	}
	AcCmColor col;
	col.setColorIndex(m_nColorIndex);
	m_pDim->setDimclrt(col);

	ZoomAndUpdate();

	
}
void GDimConfigDialog::OnCbnSelchangeComboArrow()
{
	UpdateDim();
}

void GDimConfigDialog::OnCbnSelchangeComboLccta()
{
	UpdateDim();
}
//执行设置
BOOL GDimConfigDialog::ExcSet()
{
	RecordUserSet();
	GLockDocument _lock;
	AcDbObjectId   rcId;
	UpdateDimStyleByConfig(rcId);
	return TRUE;
}
Acad::ErrorStatus GDimConfigDialog::CreateNewDim()
{
	m_pDim=new AcDbAlignedDimension();
	m_pDim->setXLine1Point(AcGePoint3d(-5,-5,0));
	m_pDim->setXLine2Point(AcGePoint3d(25,-5,0));
	m_pDim->setDimtad(1);

	struct resbuf pResult;
	acedGetVar(_T("DIMSTYLE"),&pResult);//取得当前标注
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	m_pDim->setDatabaseDefaults(pDB);

	AcDbDimStyleTable *pDimStyleTbl;
	Acad::ErrorStatus es=pDB->getDimStyleTable(pDimStyleTbl, AcDb::kForWrite);
	if(es==Acad::eOk)
	{
		AcDbDimStyleTableRecord *pDimStyleTblRcd;
		AcDbObjectId ptt;
		es=pDimStyleTbl->getAt(pResult.resval.rstring,pDimStyleTblRcd,AcDb::kForWrite);
		if(es==Acad::eOk)
		{
			char pt='.';//小数分割符
			pDimStyleTblRcd->setDimdsep(pt);
			ptt=pDimStyleTblRcd->objectId();
			pDimStyleTblRcd->close();
			

			pDB->setDimstyleData(ptt);//防止修改后出现样式替代
			pDB->setDimstyle(ptt);
			m_pDim->setDimensionStyle(ptt);
		}

		pDimStyleTbl->close();
	}
	else
	{

	}
	return es;
}

void GDimConfigDialog::OnCbnSelchangeComboTextheight()
{
	UpdateDim();
}

void GDimConfigDialog::OnCbnSelchangeComboDimexe()
{
	UpdateDim();
}

void GDimConfigDialog::OnCbnSelchangeComboDimex0()
{
	UpdateDim();
}

void GDimConfigDialog::OnBnClickedButtonColor()
{
	int nColorIndex=7;
	if(acedSetColorDialog(nColorIndex,false,256)!=Adesk::kTrue)
	{
		return;
	}
	m_ColorButton.SetWindowText(GetIndexColorName(nColorIndex));
	m_ColorButton.SetFillColor(IndexedColors[nColorIndex]);
	m_nColorIndex=nColorIndex;
	m_ColorButton.Invalidate();
	m_ColorButton.UpdateWindow();
	UpdateDim();
}

void GDimConfigDialog::OnCbnSelchangeComboXsd()
{
	UpdateDim();
}

void GDimConfigDialog::OnBnClickedButtonUpdatefonts()
{
	//更新字体
	GCADFonts* pFonts=GCADFonts::GetInstance();
	pFonts->Update();
	CComboBox* pBox=(CComboBox*)GetDlgItem(IDC_COMBO_COMMFONT);
	if(pBox!=NULL)
	{
		for(int i=0;i<pFonts->m_strCommShxFontsArray.GetCount();i++)
		{
			pBox->AddString(pFonts->m_strCommShxFontsArray.GetAt(i));
		}
	}
	pBox=(CComboBox*)GetDlgItem(IDC_COMBO_BIGFONT);
	if(pBox!=NULL)
	{
		for(int i=0;i<pFonts->m_strBigFontsArray.GetCount();i++)
		{
			pBox->AddString(pFonts->m_strBigFontsArray.GetAt(i));
		}
	}
	RoolbackUserComboSet(IDC_COMBO_COMMFONT,_T("CommFont"),_T("txt.shx"));
	RoolbackUserComboSet(IDC_COMBO_BIGFONT,_T("BigFont"),_T("gbcbig.shx"));
}
