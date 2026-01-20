#include "stdafx.h"
#ifndef ACAD_2004
#if _MSC_VER<1400
#include "dbtable.h"
#endif
#endif
#include "GDimCommand.h"
//获取两点间距离
double GetDistPoints(AcGePoint3d pt1, AcGePoint3d pt2)
{
	return sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y));
}

//获取圆弧亮点的半径
double GetRPoints(AcGePoint3d pt1, AcGePoint3d pt2, double dBugle)
{
	return fabs(GetDistPoints(pt1, pt2)/(2*sin(2*atan(dBugle))));
}
// 功能：根据指针添加实体到模型空间
// 参数：resultId：返回的实体id
//			  pEnt：待添加的实体指针
//			  pDB：接收实体的数据库		
// 返回：1：成功；0：失败
long AddToCurrentSpace(AcDbObjectId& resultId,AcDbEntity* pEnt,AcDbDatabase* pDB)
{	
	resultId.setNull();

	AcDbBlockTableRecord* pBlkRec = NULL;//打开表
	Acad::ErrorStatus es = acdbOpenObject(pBlkRec, pDB->currentSpaceId(), AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\n 打开模型空间失败！"));
		return 0;
	}

	es = pBlkRec->appendAcDbEntity(resultId,pEnt);//添加实体
	if (es != Acad::eOk) 
	{
		acutPrintf(_T("\n 添加对象到模型空间失败！%s"),acadErrorStatusText(es));
		pBlkRec->close();
		return 0;
	}
	es=pBlkRec->close();

	return 1;
}
//标注点
AcDbObjectId MarkPoint(const AcGePoint3d &pt,const CString &strContent,int nColorIndex,double dHeight)
{
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	if (NULL == pDB)
		return AcDbObjectId::kNull;
	
	AcDbText *pText=new AcDbText(AcGePoint3d(pt.x+3,pt.y+3,0),strContent,pDB->textstyle(),dHeight);
	pText->setColorIndex(nColorIndex);

	AcDbObjectId id;
	//添加实体到模型空间
	AddToCurrentSpace(id,pText,pDB);
	pText->close();
	pText=NULL;
	return id;
}
#ifdef ACAD_2004
// 坐标点标注
void DimCoords()
{
	acutPrintf(_T("\n当前CAD版本为AUTOCAD2004,不支持Table功能，如需实现标注点坐标，请更换至AUTOCAD2005或者AUTOCAD2006"));
}
#else
// 坐标点标注
void DimCoords()
{
		//获取CAD版本
#ifdef VC7
		//CAD2007以上均支持表格
		if(acdbHostAppMinorVersion()<1)
		{
			acutPrintf(_T("\n当前CAD版本为AUTOCAD2004,不支持Table功能，如需实现标注点坐标，请更换至AUTOCAD2005或者AUTOCAD2006"));
			return;
		}
#endif
		TCHAR* promptPtrs[2]={_T("\n选择需要标注的对象:"),_T("\n移除需要标注的对象:")};
		AcDbObjectId objId;
		AcDbObjectIdArray objIds;
		AcDbObjectIdArray textIds;
		AcDbObjectId textId;
		ads_name ssName;
		while (RTNORM == acedSSGet(_T(":$:S"),promptPtrs,NULL,NULL,ssName))//每次只选取一个，如果之前已有选择集包含多个实体，此处将得到多个实体
		{
			int nCount1=objIds.logicalLength();
			AcDbObjectIdArray objIdsSwap;
			SSToIds(objIdsSwap,ssName);
			ads_free(ssName);
			int nCount2=objIdsSwap.logicalLength();
			for (int i = 0; i < objIdsSwap.logicalLength(); i++)
			{
				int nFind = -1;
				objIds.find(objIdsSwap.at(i), nFind);
				if (nFind < 0)
				{
					objIds.append(objIdsSwap.at(i));
					AcDbEntity *pEnt = NULL;
					if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIdsSwap.at(i), AcDb::kForWrite))
					{
						pEnt->highlight();
					}
					if (pEnt != NULL)
					{
						pEnt->close();
						pEnt = NULL;
					}
				}
			}
			int nCount3=objIds.logicalLength();
			if(nCount3>nCount1)
			{
				if(nCount3==nCount1+nCount2)
				{
					//全部都是新选择的对象
					acutPrintf(_T("\n选择了%d个对象，总共选择了%d个对象"),nCount2,nCount3);
				}
				else
				{
					//有不是新选择的对象
					acutPrintf(_T("\n选择了%d个对象(%d个重复)，总共选择了%d个对象"),nCount2,nCount1+nCount2-nCount3,nCount3);
				}
			}
		}

		if(objIds.logicalLength()<=0)
			return;//不创建图层

		CString strMSG;

		int num = 0;//记录所有选中实体中所处理点的索引位置

		AcDbTable *pTable=new AcDbTable();
		pTable->setColumnWidth(0,20);
		pTable->insertColumns(1,100);
		pTable->setTextString(0,1,_T("坐标"));
		AcGePoint3d ptSwap;
		double bulgeSwap = 0.0;

		pTable->insertColumns(2,80);
		pTable->setTextString(0,2,_T("距离或者半径"));

		AcDbExtents objExts;
		for(int i=0;i<objIds.logicalLength();i++)//也可能有多个实体，先选择多个实体，再执行命令
		{

			AcDbCurve *pCurve=NULL;//只处理AcDbCurve派生类中的AcDb2dPolyline,AcDb3dPolyline,AcDbLine,AcDbPolyline,AcDbSPline
			if(acdbOpenObject(pCurve,objIds.at(i),AcDb::kForRead)==Acad::eOk)
			{			
				if(pCurve->isKindOf(AcDb2dPolyline::desc()))
				{
					AcDb2dPolyline *pl=AcDb2dPolyline::cast(pCurve);
					AcDbObjectIterator *pIterator=pl->vertexIterator();
					if(pIterator!=NULL)//读取顶点
					{
						int nVertex=0;
						for(pIterator->start();!pIterator->done();pIterator->step())
						{
							AcDb2dVertex *pVertex=NULL;
							if(acdbOpenObject(pVertex,pIterator->objectId(),AcDb::kForRead)==Acad::eOk)
							{
								pTable->insertRows(num+1,10);

								strMSG.Format(_T("%d"),num);
								pTable->setTextString(num+1,0,strMSG);
								strMSG.Format(_T("(%.3f,%.3f)"),pVertex->position().x,pVertex->position().y);
								pTable->setTextString(num+1,1,strMSG);
								if (nVertex != 0)
								{
									if (bulgeSwap == 0.0)
									{
										strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pVertex->position()));
										
									}
									else
									{
										strMSG.Format(_T("r:%.3f"), GetRPoints(ptSwap, pVertex->position(), bulgeSwap));
									}
									pTable->setTextString(num,2,strMSG);
									textId=MarkPoint(AcGePoint3d((pVertex->position().x+ptSwap.x)/2, (pVertex->position().y+ptSwap.y)/2, 0.0), strMSG,7,12);
									if(textId.isValid())
									{
										textIds.append(textId);
									}
								}
								ptSwap = pVertex->position();//记录当时点作为临时点
								bulgeSwap =pVertex->bulge();//记录当时点的凸度作为临时凸度

								//标注顶点
								strMSG.Format(_T("%d"),num);
								textId=MarkPoint(pVertex->position(),strMSG,7,12);
								if(textId.isValid())
								{
									textIds.append(textId);
								}

								pVertex->close();
								pVertex=NULL;
							}
							nVertex++;
							num++;
						}
						//画初始点和终点的距离或者半径
						if (pl->isClosed())
						{
							pIterator->start();
							AcDb2dVertex *pVertex=NULL;
							if(acdbOpenObject(pVertex,pIterator->objectId(),AcDb::kForRead)==Acad::eOk)
							{
								if (bulgeSwap == 0.0)
								{
									strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pVertex->position()));

								}
								else
								{
									strMSG.Format(_T("r:%.3f"), GetRPoints(ptSwap, pVertex->position(), bulgeSwap));
								}
								pTable->setTextString(num,2,strMSG);
								textId=MarkPoint(AcGePoint3d((pVertex->position().x+ptSwap.x)/2, (pVertex->position().y+ptSwap.y)/2, 0.0), strMSG,7,12);
								if(textId.isValid())
								{
									textIds.append(textId);
								}
							}
						}
						//#######画初始点和终点的距离或者半径###########
					}
				}
				else if(pCurve->isKindOf(AcDb3dPolyline::desc()))
				{//三维顶点无法处理凸度
					AcDb3dPolyline *pl=AcDb3dPolyline::cast(pCurve);
					AcDbObjectIterator *pIterator=pl->vertexIterator();
					if(pIterator!=NULL)//读取顶点
					{
						int nVertex=0;
						for(pIterator->start();!pIterator->done();pIterator->step())
						{
							AcDb3dPolylineVertex *pVertex=NULL;
							if(acdbOpenObject(pVertex,pIterator->objectId(),AcDb::kForRead)==Acad::eOk)
							{
								pTable->insertRows(num+1,10);

								strMSG.Format(_T("%d"),num);
								pTable->setTextString(num+1,0,strMSG);
								strMSG.Format(_T("(%.3f,%.3f)"),pVertex->position().x,pVertex->position().y);
								pTable->setTextString(num+1,1,strMSG);

								if (nVertex != 0)
								{
									strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pVertex->position()));

									pTable->setTextString(num,2,strMSG);
									textId=MarkPoint(AcGePoint3d((pVertex->position().x+ptSwap.x)/2, (pVertex->position().y+ptSwap.y)/2, 0.0), strMSG,7,12);
									if(textId.isValid())
									{
										textIds.append(textId);
									}
								}
								ptSwap = pVertex->position();//记录当时点作为临时点 
								
								strMSG.Format(_T("%d"),num);
								textId=MarkPoint(pVertex->position(),strMSG,7,12);
								if(textId.isValid())
								{
									textIds.append(textId);
								}

								pVertex->close();
								pVertex=NULL;
							}
							nVertex++;
							num++;
						}
						//画初始点和终点的距离或者半径
						if (pl->isClosed())
						{
							pIterator->start();
							AcDb2dVertex *pVertex=NULL;
							if(acdbOpenObject(pVertex,pIterator->objectId(),AcDb::kForRead)==Acad::eOk)
							{
								strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pVertex->position()));

								pTable->setTextString(num,2,strMSG);
								textId=MarkPoint(AcGePoint3d((pVertex->position().x+ptSwap.x)/2, (pVertex->position().y+ptSwap.y)/2, 0.0), strMSG,7,12);
								if(textId.isValid())
								{
									textIds.append(textId);
								}
							}
						}
						//#######画初始点和终点的距离或者半径###########
					}
				}
				else if(pCurve->isKindOf(AcDbLine::desc()))
				{
					AcDbLine *pl=AcDbLine ::cast(pCurve);

					AcGePoint3d pt1,pt2;
					pl->getStartPoint(pt1);
					pl->getEndPoint(pt2);

					pTable->insertRows(num+1,10);
					strMSG.Format(_T("%d"),num);
					pTable->setTextString(num+1,0,strMSG);
					strMSG.Format(_T("(%.3f,%.3f)"),pt1.x,pt1.y);
					pTable->setTextString(num+1,1,strMSG);
					strMSG.Format(_T("%d"),num);
					textId=MarkPoint(pt1,strMSG,7,12);
					if(textId.isValid())
					{
						textIds.append(textId);
					}
					num++;

					pTable->insertRows(num+1,10);
					strMSG.Format(_T("%d"),num);
					pTable->setTextString(num+1,0,strMSG);
					strMSG.Format(_T("(%.3f,%.3f)"),pt2.x,pt2.y);
					pTable->setTextString(num+1,1,strMSG);

					strMSG.Format(_T("%d"),num);
					MarkPoint(pt2,strMSG,7,12);

					strMSG.Format(_T("d:%.3f"),GetDistPoints(pt1, pt2));
					pTable->setTextString(num,2,strMSG);
					textId=MarkPoint(AcGePoint3d((pt1.x+pt2.x)/2, (pt1.y+pt2.y)/2, 0.0), strMSG,7,12);
					if(textId.isValid())
					{
						textIds.append(textId);
					}
					num++;
				}
				else if(pCurve->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pl=AcDbPolyline::cast(pCurve);								
					//if(pl->hasBulges()==Adesk::kTrue)
					//{//有凸度
					//	pTable->insertColumns(2,50);
					//	pTable->setTextString(0,2,"凸度");
					//}
					pTable->insertRows(num+1,10,pl->numVerts());

					for(int i=0;i<pl->numVerts();i++)
					{
						AcGePoint3d pt;
						pl->getPointAt(i,pt);

						strMSG.Format(_T("%d"),num+i);
						pTable->setTextString(num+i+1,0,strMSG);
						strMSG.Format(_T("(%.3f,%.3f)"),pt.x,pt.y);
						pTable->setTextString(num+i+1,1,strMSG);

						if(pl->hasBulges()==Adesk::kTrue)
						{
							double bugle;
							pl->getBulgeAt(i,bugle);
							strMSG.Format(_T("%.3f"),bugle);
							pTable->setTextString(num+i+1,2,strMSG);
						}

						if (i != 0)
						{
							if (bulgeSwap == 0.0)
							{
								strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pt));

							}
							else
							{
								strMSG.Format(_T("r:%.3f"), GetRPoints(ptSwap, pt, bulgeSwap));
							}
							pTable->setTextString(num+i,2,strMSG);
							textId=MarkPoint(AcGePoint3d((pt.x+ptSwap.x)/2, (pt.y+ptSwap.y)/2, 0.0), strMSG,7,12);
							if(textId.isValid())
							{
								textIds.append(textId);
							}
						}
						ptSwap = pt;//记录当时点作为临时点
						pl->getBulgeAt(i, bulgeSwap);//记录当时点的凸度作为临时凸度

						strMSG.Format(_T("%d"),num+i);
						textId=MarkPoint(pt,strMSG,7,12);
						if(textId.isValid())
						{
							textIds.append(textId);
						}
					}
					//画初始点和终点的距离或者半径
					if (pl->isClosed())
					{
						AcGePoint3d pt;
						pl->getPointAt(0,pt);
						if (bulgeSwap == 0.0)
						{
							strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pt));

						}
						else
						{
							strMSG.Format(_T("r:%.3f"), GetRPoints(ptSwap, pt, bulgeSwap));
						}
						pTable->setTextString(num+pl->numVerts(),2,strMSG);
						textId=MarkPoint(AcGePoint3d((pt.x+ptSwap.x)/2, (pt.y+ptSwap.y)/2, 0.0), strMSG,7,12);
						if(textId.isValid())
						{
							textIds.append(textId);
						}
					
					}
					//#######画初始点和终点的距离或者半径###########
					num = num + pl->numVerts();
				}
				else if(pCurve->isKindOf(AcDbSpline::desc()))
				{
					AcDbSpline *pl=AcDbSpline::cast(pCurve);

					pTable->insertRows(num+1,10,pl->numControlPoints());
					for(int i=0;i<pl->numControlPoints();i++)
					{
						AcGePoint3d pt;
						pl->getControlPointAt(i,pt);

						strMSG.Format(_T("%d"),num+i);
						pTable->setTextString(num+i+1,0,strMSG);
						strMSG.Format(_T("(%.3f,%.3f)"),pt.x,pt.y);
						pTable->setTextString(num+i+1,1,strMSG);

						strMSG.Format(_T("%d"),num+i);
						textId=MarkPoint(pt,strMSG,7,12);
						if(textId.isValid())
						{
							textIds.append(textId);
						}
						if (i != 0)
						{
							strMSG.Format(_T("d:%.3f"),GetDistPoints(ptSwap, pt));

							pTable->setTextString(num+i,2,strMSG);
							MarkPoint(AcGePoint3d((pt.x+ptSwap.x)/2, (pt.y+ptSwap.y)/2, 0.0), strMSG,7,12);
						}
						ptSwap = pt;//记录当时点作为临时点
					}
				}
				else
				{
					acutPrintf(_T("\n无法处理的实体类型!"));

					pCurve->close();
					pCurve=NULL;
					/*
					//zhangshl bug修改
					//这个时候关闭Table则下面的函数执行会崩溃
					//为什么将整个的都绘制在一个表格里面?
					pTable->close();
					pTable=NULL;
					*/

					continue;//跳出循环
				}

				//设置颜色
				pTable->setColorIndex(7);

				AcDbExtents objExt;
				pCurve->getGeomExtents(objExt);
				objExts.addExt(objExt);
				pCurve->close();
				pCurve=NULL;	
			}
		}
		//插入位置
		pTable->setPosition(AcGePoint3d(objExts.minPoint().x,objExts.minPoint().y+pTable->height(),0));

		pTable->generateLayout(); // Very very important, else expect crashes later on
		AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
		if (NULL != pDB)
		{	
			AcDbObjectId id;
			//CDwgOpr::AddEntToDb(pTable,id);//添加完成后自动关闭实体
			AddToCurrentSpace(id,pTable,pDB);
			//所有的标注都必须放在辅助线图层上
			//否则保存会出问题
			/********************************************/
			pTable->close();
			pTable=NULL;
			/*********************************************/
		}
		else
		{
			pTable->close();
			pTable=NULL;
		}	
		//字体样式，否则可能乱码

		//使所有高亮的对象不高亮显示
		for (int i = 0; i < objIds.logicalLength(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (Acad::eOk == acdbOpenAcDbEntity(pEnt, objIds.at(i), AcDb::kForWrite))
			{
				pEnt->unhighlight();
			}
			if (pEnt != NULL)
			{
				pEnt->close();
				pEnt = NULL;
			}
		}
}
#endif