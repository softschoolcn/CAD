#include "StdAfx.h"
#include "GQrCodeCommand.h"
#include "GQrCodeMgr.h"
#include <AtlImage.h>
#include "GQrCodeJig.h"
#include "GQrCodeJig.h"
#include "GQrCodeDialog.h"
struct ImagePointIndex
{
	int X;
	int Y;
};
CString GetSuffix()
{
	time_t now;
	time(&now);
	CString suffix;
	suffix.Format(_T("%x"), now);
	return suffix;
}
#include "QR_Encode.h"
#include "GQRCodeManager.h"
//获取二维码LOGO范围
bool GetQrCodeLogoExtent(AcGePoint3d& ptMin, AcGePoint3d& ptMax)
{
	HMODULE hModule=_hdllInstance;
	TCHAR m_lpFile[MAX_PATH+1]={0};
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("二维码LOGO.dwg"));
	if(!PathFileExists(m_lpFile))
	{
		return false;
	}
	bool rc=false;
	AcDbDatabase* pDbTmp=new AcDbDatabase(false);
	try
	{
		Acad::ErrorStatus es=pDbTmp->readDwgFile(m_lpFile);
		if(es!=eOk)
		{
			acutPrintf(_T("\n读取DWG文件:%s，发生错误:%s\n"),m_lpFile,acadErrorStatusText(es));
		}
		else
		{
			AcDbObjectIdArray ids;
			EnumAllEntityInModelSpace(ids,pDbTmp);
			if(GetMaxExtPoint(ids,ptMin,ptMax))
			{
				rc=true;
			}
		}
	}
	catch(...)
	{
		if(pDbTmp!=NULL)
		{
			delete pDbTmp;
			pDbTmp=NULL;
		}
	}
	if(pDbTmp!=NULL)
	{
		delete pDbTmp;
		pDbTmp=NULL;
	}
	return rc;
}
//增加二维码的LOGO
void AddQrCodeLogo(AcDbObjectIdArray& EntIds,double LogoScale,AcGePoint3d ptBase,AcGePoint3d pt)
{
	HMODULE hModule=_hdllInstance;
	TCHAR m_lpFile[MAX_PATH+1]={0};
	GetModuleFileName(hModule,m_lpFile,MAX_PATH);
	PathRemoveFileSpec(m_lpFile);
	PathRemoveFileSpec(m_lpFile);
	PathAppend(m_lpFile,_T("UserConfig"));
	PathAppend(m_lpFile,_T("二维码LOGO.dwg"));
	if(!PathFileExists(m_lpFile))
	{
		return;
	}
	AcDbDatabase* pDbTmp=new AcDbDatabase(false);
	try
	{
		Acad::ErrorStatus es=pDbTmp->readDwgFile(m_lpFile);
		if(es!=eOk)
		{
			acutPrintf(_T("\n读取DWG文件:%s，发生错误:%s\n"),m_lpFile,acadErrorStatusText(es));
		}
		else
		{
			AcDbObjectIdArray ids;
			EnumAllEntityInModelSpace(ids,pDbTmp);
			AcDbIdMapping idMap;
			AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
			es=pDb->wblockCloneObjects(ids,pDb->currentSpaceId(),idMap,AcDb::kDrcReplace);
			if(es==Acad::eOk)
			{
				ids.setLogicalLength(0);
				AcDbIdMappingIter iter(idMap);
				AcDbIdPair idPair;
				for (iter.start(); !iter.done(); iter.next())
				{
					iter.getMap(idPair);
					ids.append(idPair.value());
				}
				AcGePoint3d ptMin;
				AcGePoint3d ptMax;
				if(GetMaxExtPoint(ids,ptMin,ptMax))
				{
					AcGePoint3d ptMid=GCADUtils::MidPoint(ptMin,ptMax);
					AcGePoint3d ptMinEnt;
					AcGePoint3d ptMaxEnt;
					if(GetMaxExtPoint(EntIds,ptMinEnt,ptMaxEnt))
					{
						AcGePoint3d ptMidEnt=GCADUtils::MidPoint(ptMinEnt,ptMaxEnt);
						AcGeMatrix3d xform1;
						xform1.setToTranslation(ptMidEnt-ptMid);



						//double dx=(ptMaxEnt.x-ptMinEnt.x)/maxX;
						//double dy=(ptMaxEnt.y-ptMinEnt.y)/maxY;
						double dx=(ptMaxEnt.x-ptMinEnt.x)*2*LogoScale/(ptMax.x-ptMin.x);
						double dy=(ptMaxEnt.y-ptMinEnt.y)*2*LogoScale/(ptMax.x-ptMin.x);
						double dScale=max(dx,dy);//应该是一样的
						AcGeMatrix3d xform2;
						xform2.setToScaling(dScale,ptMidEnt);
						for(int i=0;i<ids.logicalLength();i++)
						{
							AcDbEntityPointerEx<AcDbEntity> spEnt(ids.at(i),AcDb::kForWrite,false,true);
							if(spEnt.openStatus()!=Acad::eOk)
							{
								continue;
							}
							spEnt->transformBy(xform1);
							spEnt->transformBy(xform2);
						}
						AcDbExtents ext_logo(ptMin,ptMax);//LOGO的范围
						ext_logo.transformBy(xform1);
						ext_logo.transformBy(xform2);
						for(int j=EntIds.logicalLength()-1;j>=0;j--)
						{
							AcDbEntityPointerEx<AcDbEntity> spEnt(EntIds.at(j),AcDb::kForWrite,false,true);
							if(spEnt.openStatus()!=Acad::eOk)
							{
								continue;
							}
							AcDbExtents ext;
							spEnt->getGeomExtents(ext);
							if(!GCADUtils::IsExtIntersect(ext,ext_logo))
							{
								continue;
							}
							spEnt->erase();
							EntIds.removeAt(j);
						}
						EntIds.append(ids);
					}
					else
					{
						acutPrintf(_T("\n计算二维码范围失败\n"));
					}
				}
				else
				{
					acutPrintf(_T("\n计算二维码LOGO范围失败\n"));
				}
			}
			else
			{
				acutPrintf(_T("\n拷贝二维码LOGO失败，发生错误:%s\n"),acadErrorStatusText(es));
			}
		}

	}
	catch(...)
	{
		if(pDbTmp!=NULL)
		{
			delete pDbTmp;
			pDbTmp=NULL;
		}
	}
	if(pDbTmp!=NULL)
	{
		delete pDbTmp;
		pDbTmp=NULL;
	}

}
//插入二维码
void QrCode()
{
	{
		if(!CHECK_REG_BOOL())
		return;
	}
	CAcModuleResourceOverride res;
	GQrCodeDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	CString strText=dlg.m_strText;
	ads_point result;
	int rc=acedGetPoint(NULL,_T("\n>>请选择插入点\n"),result);
	if(rc!=RTNORM)
		return;
	GQRCodeManager mag;
	CString strResult=mag.MakeQRCode(strText);
	if(strResult.IsEmpty())
	{
		AfxMessageBox(_T("生成二维码失败，您可以先减少二维码文字的长度，如果还是失败，请联系作者\r\nQQ:245804736 E-Mail:wdzhangsl@126.com"));
		return;
	}
	int firstIndex=strResult.Find(_T(';'));
	if(firstIndex<=0)
	{
		AfxMessageBox(_T("生成无效二维码，未找到宽度，请联系作者\r\nQQ:245804736\r\nE-Mail:wdzhangsl@126.com"));
		return;
	}
	CString strWidth=strResult.Left(firstIndex);
	int maxX=_ttoi(strWidth);
	strResult=strResult.Right(strResult.GetLength()-firstIndex-1);
	firstIndex=strResult.Find(_T(';'));
	if(firstIndex<=0)
	{
		AfxMessageBox(_T("生成无效二维码，未找到长度，请联系作者\r\nQQ:245804736\r\nE-Mail:wdzhangsl@126.com"));
		return;
	}
	
	CString strHeight=strResult.Left(firstIndex);
	int maxY=_ttoi(strHeight);

	AcGePoint3d ptLogoMin;
	AcGePoint3d ptLogoMax;
	bool bAddLogo=GetQrCodeLogoExtent(ptLogoMin,ptLogoMax);
	//double dLogoWidth=0.0;
	//double dLogoHeight=0.0;
	double dLogoWidthStart=0.0;
	double dLogoWidthEnd=0.0;
	double dLogoHeightStart=0.0;
	double dLogoHeightEnd=0.0;
	double LogoScale=0.1;
	if(bAddLogo)
	{
		
	//dLogoWidth=maxX*LogoScale*2;
	dLogoWidthStart=maxX*(0.5-LogoScale);
	dLogoWidthEnd=maxX*(0.5+LogoScale);
	//dLogoHeight=maxY*LogoScale*2;
	dLogoHeightStart=maxY*(0.5-LogoScale);
	dLogoHeightEnd=maxY*(0.5+LogoScale);
	}
	strResult=strResult.Right(strResult.GetLength()-firstIndex-1);
	bool* s=new bool[maxY*maxX];
	int nIndex=0;
	for(int X=0;X<maxX;X++)
	{
		for(int Y=0;Y<maxY;Y++)
		{
			TCHAR c=_T('0');
			if(nIndex<strResult.GetLength())
			{
				c=strResult.GetAt(nIndex);
			}
			if(c==_T('1'))
			{
				s[Y*maxX+X]=true;
				if(bAddLogo)
				{
					bool bXin=(X>=dLogoWidthStart&&X<=dLogoWidthEnd)||(X+1>=dLogoWidthStart&&X+1<=dLogoWidthEnd);
					bool bYin=(Y>=dLogoHeightStart&&Y<=dLogoHeightEnd)||(Y+1>=dLogoHeightStart&&Y+1<=dLogoHeightEnd);
					if(bXin&&bYin)
					{
						s[Y*maxX+X]=false;
					}
				}
			}
			else
			{
				s[Y*maxX+X]=false;
			}
			nIndex++;
		}
	}
	GQrCodeJIg jig;
	jig.ptBase.x=result[0];
	jig.ptBase.y=result[1];
	jig.ptBase.z=result[2];
	jig.ptBase=GCADUtils::UCS2WCS(jig.ptBase);
	jig.maxY=maxY;
	jig.maxX=maxX;
	jig.s=s;
	jig.InitEnt();
	int ret=jig.Do();
	if(RTNORM==ret)
	{
		AcDbObjectIdArray EntIds;
		EntIds.append(jig.m_ids);
		if(bAddLogo)
		{
		AddQrCodeLogo(EntIds,LogoScale,jig.ptBase,jig.m_pointTopLast);
		}

		CString strName=_T("QrCode_")+GetSuffix();
		//makeGroup(acdbHostApplicationServices()->workingDatabase(),strName,true,jig.m_ids);
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable* pTable = NULL;
		Acad::ErrorStatus es = pDb->getBlockTable(pTable, AcDb::kForWrite);
		if(es!=Acad::eOk)
		{
			return;
		}
		AcDbObjectPointer<AcDbBlockTable> spTable;
		spTable.acquire(pTable);
		AcDbObjectId recordId;
		if(spTable->has(strName))
		{

		}
		AcDbBlockTableRecord* pRec = new AcDbBlockTableRecord();
		es=pRec->setName(strName);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return;
		}
		es=spTable->add(recordId,pRec);
		if(es!=eOk)
		{
			delete pRec;
			pRec=NULL;
			return;
		}
		pRec->close();
		bool bValidExt=false;
		AcDbExtents ext;
		AcDbObjectIdArray ids1;
		for(long i=0;i<EntIds.logicalLength();i++)
		{
			AcDbEntityPointer spEnt1(EntIds.at(i),AcDb::kForRead);
			if(spEnt1.openStatus()!=Acad::eOk)
			{
				continue;
			}
			ids1.append(EntIds.at(i));
			AcDbExtents tmp_ext;
			if(spEnt1->getGeomExtents(tmp_ext)==Acad::eOk)
			{
				bValidExt=true;
				ext.addExt(tmp_ext);
			}
		}
		if(!bValidExt)
		{
			ext.set(AcGePoint3d(0,0,0),AcGePoint3d(0,0,0));
		}
		AcGePoint3d ptCenter(0.0,0.0,0.0);
		if(bValidExt)
		{
			ptCenter.x=0.5*(ext.minPoint().x+ext.maxPoint().x);
			ptCenter.y=0.5*(ext.minPoint().y+ext.maxPoint().y);
			AcDbObjectPointer<AcDbBlockTableRecord> spRec(recordId,AcDb::kForWrite);
			if(spRec.openStatus()==Acad::eOk)
			{
				spRec->setOrigin(ptCenter);
			}
		}
		AcDbIdMapping idMap;
		es=pDb->deepCloneObjects(ids1,recordId,idMap);
		//插入一个块参照
		AcDbBlockReference *pBlkRef=new AcDbBlockReference;
		pBlkRef->setBlockTableRecord(recordId);//指定所引用的图块表记录的对象ID
		resbuf to,from;
		from.restype=RTSHORT;//插入图块要进行用户坐标与世界坐标的转换
		from.resval.rint=1;//UCS
		to.restype=RTSHORT;
		to.resval.rint=0;//WCS
		AcGeVector3d normal(0.0,0.0,1.0);
		acedTrans(&(normal.x),&from,&to,Adesk::kTrue,&(normal.x));//转换函数
		pBlkRef->setPosition(ptCenter); //指定基点
		pBlkRef->setRotation(0.0);//旋转角度
		pBlkRef->setNormal(normal);
		pBlkRef->setDatabaseDefaults(pDb);

		AcGeScale3d dGeScale(1.0,1.0,1.0);
		pBlkRef->setScaleFactors(dGeScale);//放大比例

		AcDbBlockTableRecord *pBlockTableRecord;
		if(Acad::eOk == acdbOpenObject(pBlockTableRecord, pDb->currentSpaceId(), AcDb::kForWrite))
		{
			AcDbObjectId resultId;
			pBlockTableRecord->appendAcDbEntity(resultId,pBlkRef);
			pBlkRef->close();
			pBlockTableRecord->close();
		}
		else
		{
			delete pBlkRef;
			pBlkRef=NULL;
		}
		//删除旧的实体对象
		for(long i=0;i<EntIds.logicalLength();i++)
		{
			AcDbEntityPointer spEnt2(EntIds.at(i),AcDb::kForWrite);
			if(spEnt2.openStatus()!=Acad::eOk)
			{
				continue;
			}
			spEnt2->erase();
		}
	}
	delete[] s;
	s=NULL;
}
