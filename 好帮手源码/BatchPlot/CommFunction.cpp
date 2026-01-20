#include "StdAfx.h"
#include "CommFunction.h"
//功能:判断字符串是否在字符串数组中
//参数:	array:字符串数组
//			string:字符串
//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
//返回:	true:在数组中；false:不在
bool IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase/*=true*/)
{
	for (int i=0;i<array.GetCount();i++) 
	{
		CString tmp=array.GetAt(i);
		if(bNoCase)
		{
			if(stringX.CompareNoCase(tmp)==0)
			{
				return true;
			}
		}
		else
		{
			if(stringX.Compare(tmp)==0) 
			{
				return true;
			}
		}
	}
	return false;
}
// 功能：将选择集转换为实体ID数组
// 参数：	  ssName,选择集
//			  ObjIds,实体ID数组		
// 返回： 
void SSToIds(ads_name ssName,AcDbObjectIdArray& ObjIds)
{
	AcDbObjectId ObjId;
	ads_name EntName;
#if _MSC_VER>=1900
	Adesk::Int32 nLength = 0;
#else
	long nLength = 0;
#endif
	acedSSLength(ssName,&nLength);

	for(int nLen=0; nLen<nLength; nLen++)
	{
		acedSSName(ssName,nLen,EntName);
		acdbGetObjectId(ObjId,EntName);
		ObjIds.append(ObjId);
	}
}
BOOL GetCurDwgFileName(CString& strDwgFileName)
{
	if(curDoc()==NULL)
		return FALSE;
	if(PathFileExists(curDoc()->fileName()))
	{
		TCHAR lpFile[MAX_PATH];
		lstrcpy(lpFile,curDoc()->fileName());
		if(!PathRemoveFileSpec(lpFile))
		{
			return FALSE;
		}
		if(!PathFileExists(lpFile))
		{
			//寻找所在目录
			return FALSE;
		}
		strDwgFileName=curDoc()->fileName();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//功能:	指定的字符串是否是合理的长整数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回长整数型结果
//返回: 是否是合理的长整数
BOOL IsValidLong(const CString value,long &lValue)
{
	bool bNegative=false;//是否是负数
	CString tmpValue=value;
	try
	{
		tmpValue.Trim();
	}
	catch(...)
	{

	}
	int nLen=tmpValue.GetLength();
	if(nLen==0)
		return FALSE;
	TCHAR c=tmpValue.GetAt(0);
	if(c=='-')
	{
		//负数
		bNegative=true;
		if(nLen==1)
		{
			//只有一个负号是不合理的
			return FALSE;
		}
		tmpValue=tmpValue.Right(nLen-1);
		nLen--;
	}
	long lTmpValue=0L;
	c=tmpValue.GetAt(0);
	if(c=='0')
	{
		if(nLen==1)
		{
			lValue=0L;
			return TRUE;
		}
		else
		{
			//第一位是0并且后面还有其它数字认为是不合理的
			return FALSE;
		}
	}
	else if(c>='1'&&c<='9')
	{
		lTmpValue+=lTmpValue*10+c-'0';
		for(int i=1;i<nLen;i++)
		{
			c=tmpValue.GetAt(i);
			if(c>='0'&&c<='9')
			{
				lTmpValue=lTmpValue*10+(c-'0');
			}
			else
			{
				//出现了非数字认为是不合理的
				return FALSE;
			}
		}
	}
	else
	{
		//出现了非数字认为是不合理的
		return FALSE;
	}
	lValue=bNegative?-lTmpValue:lTmpValue;
	return TRUE;
}
//功能:	指定的字符串是否是合理的整数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回整数型结果
//返回: 是否是合理的整数
BOOL IsValidInt(const CString value,int &nValue)
{

	long lValue=0L;
	BOOL bValid=IsValidLong(value,lValue);
	if(!bValid)
		return FALSE;
	nValue=lValue;
	long lValue1=nValue;
	if(lValue!=lValue1)
	{
		//说明数值被截断了，不在int的有效值范围内
		return FALSE;
	}
	return TRUE;
}
//功能:	指定的字符串是否是合理的浮点数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回浮点数结果
//返回: 是否是合理的浮点数
BOOL IsValidDouble(const CString value,double& dValue)
{
	if(value.IsEmpty())
	{
		return FALSE;
	}
	CString tmpValue=value;
	try
	{
		tmpValue=tmpValue.Trim();
	}
	catch(...)
	{

	}
	bool bNegative=false;//是否是负数
	int nIndex=tmpValue.Find('.');//寻找小数点
	int nLen=tmpValue.GetLength();
	BOOL bValid=FALSE;
	if(nIndex<0)
	{
		//没有小数点，为整数
		long lValue=0L;
		bValid=IsValidLong(tmpValue,lValue);
		if(!bValid)
			return FALSE;
		dValue=lValue;
		return TRUE;
	}
	long part_lValue=0L;//整数部分
	double part_Decimals=0.0;//小数部分
	//处理整数部分
	if(nIndex==0)
	{
		//第一个就是小数点，认为前面就是0是合理的
		part_lValue=0L;
	}
	else if(nIndex==1)
	{
		if(tmpValue.GetAt(0)=='-')
		{
			//第一个是负号也是合理的-.123是一个合理的浮点数
			part_lValue=-0L;
			bNegative=true;
		}
		else
		{
			CString strPart1=tmpValue.Left(1);
			bValid=IsValidLong(strPart1,part_lValue);
			if(!bValid)
				return FALSE;

		}
	}
	else
	{
		CString strPart1=tmpValue.Left(nIndex);
		bValid=IsValidLong(strPart1,part_lValue);
		if(!bValid)
			return FALSE;
	}
	//处理小数部分
	if(nIndex==nLen-1)
	{
		//只有小数点，但是没有小数部分认为是不合理的
		return FALSE;
	}
	CString strPart2=tmpValue.Right(nLen-1-nIndex);
	int nLen1=strPart2.GetLength();
	TCHAR c;
	double dDecimalsValue=1;//当前小数点上的1表示的数值
	for(int i=0;i<nLen1;i++)
	{
		//提高精度，但是貌似没有效果，在小数点9位数以后还是有不精确，对CAD系统影响很小
		if(i==0)
		{
			dDecimalsValue=0.10000000000000000000000000000000000000000000000000;
		}
		else if(i==1)
		{
			dDecimalsValue=0.01000000000000000000000000000000000000000000000000;
		}
		else if(i==2)
		{
			dDecimalsValue=0.00100000000000000000000000000000000000000000000000;
		}
		else if(i==3)
		{
			dDecimalsValue=0.00010000000000000000000000000000000000000000000000;
		}
		else if(i==4)
		{
			dDecimalsValue=0.00001000000000000000000000000000000000000000000000;
		}
		else if(i==5)
		{
			dDecimalsValue=0.00000100000000000000000000000000000000000000000000;
		}

		else
		{
			dDecimalsValue*=0.100000000000000000000000000000000000000000000000;
		}

		c=strPart2.GetAt(i);
		if(c>='0'&&c<='9')
		{
			part_Decimals+=(c-'0')*dDecimalsValue;
		}
		else
		{
			//出现了非数字认为是不合理的
			return FALSE;
		}

	}
	bNegative=bNegative||part_lValue<0;
	dValue=abs(part_lValue)+part_Decimals;
	if(bNegative)
	{
		dValue*=-1;
	}
	return TRUE;


}
BOOL getViewExtent(AcGePoint3d &leftBottom , AcGePoint3d &rightTop,CRect& Region)
{
	CView	*pView;
	pView	= acedGetAcadDwgView();
	if(pView == NULL) return FALSE;

	//获取当前视图的客户区范围
	CRect rc;
	pView->GetClientRect(&Region);
	CPoint pt1,pt2;

	//	pt1.x	= 0;
	pt1.x	= Region.left;
	pt1.y	= Region.bottom;
	pt2.x	= Region.right;
	//	pt2.y	= 0;
	pt2.y	= Region.top;

	//象素坐标到世界坐标的转换
	acedDwgPoint outPt1,outPt2;
	acedCoordFromPixelToWorld(pt1 , outPt1);
	acedCoordFromPixelToWorld(pt2 , outPt2);

	//世界坐标的格式转换
	leftBottom	= asPnt3d(outPt1);
	rightTop	= asPnt3d(outPt2);

	//	acutPrintf("A%d,%d,%d,%d\n", pt1.x, pt1.y, pt2.x, pt2.y);
	//	acutPrintf("B%d,%d,%d,%d\n", leftBottom.x, leftBottom.y, rightTop.x, rightTop.y);

	return TRUE;
}
void SplitString(CString string,char marker,CStringArray  &stringArray)
{
	if(string.IsEmpty())return ;

	CString tmp;
	int nIndex=string.Find(marker);
	while(nIndex!=-1)
	{
		tmp=string.Left(nIndex);
		stringArray.Add(tmp);
		string=string.Right(string.GetLength()-nIndex-1);
		nIndex=string.Find(marker);
	}
	stringArray.Add(string);
}
void CombineArrayToString(const CStringArray& array,CString &string,CString linkSymbol)
{
	int n=array.GetSize();
	//zhangshl BUG修改
	if(n==0)
	{
		string +=_T("");
		return;
	}
	//zhangshl BUG修改
	for(int i=0;i<n-1;i++)
	{
		string += array.GetAt(i);
		string += linkSymbol;
	}
	string += array.GetAt(n-1);
}
CString CombineArrayToString(const CStringArray& array,CString linkSymbol)
{
	CString tmp;
	CombineArrayToString(array,tmp,linkSymbol);
	return tmp;
}
Acad::ErrorStatus OpenAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	Acad::ErrorStatus es=Acad::eOk;

	AcDbLayerTable *pLayerTable=NULL;
	if((es=acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable,AcDb::kForRead))!=Acad::eOk)
	{
		pLayerTable->close();
		return es;
	}
	//创建一个层表迭代器
	AcDbLayerTableIterator *pLayerTableIterator;
	if ( (es =pLayerTable->newIterator (pLayerTableIterator) ) != Acad::eOk ) 
	{
		//acutPrintf (_T("\n不能创建表迭代器")) ;
		pLayerTable->close () ;
		return es;
	}
	pLayerTable->close () ;

	//取得图层
	for(pLayerTableIterator->start ();!pLayerTableIterator->done();pLayerTableIterator->step())
	{
		AcDbLayerTableRecord *pLayerTableRecord=NULL;
		es=pLayerTableIterator->getRecord(pLayerTableRecord,AcDb::kForRead);
		if(es!=Acad::eOk)
			continue;
		bool bLock=pLayerTableRecord->isLocked();
		bool bFrozen=pLayerTableRecord->isFrozen();
		if(bLock||bFrozen)
		{
			es=pLayerTableRecord->upgradeOpen();
			if(es!=eOk)
			{
				pLayerTableRecord->close();
				continue;
			}
			pLayerTableRecord->setIsLocked(false);
			pLayerTableRecord->setIsFrozen(false);
			ids.append(pLayerTableRecord->objectId());
			locks.append(bLock);
			frozens.append(bFrozen);

		}
		pLayerTableRecord->close();
	}
	delete pLayerTableIterator;
	pLayerTableIterator=NULL;
	return Acad::eOk;
}
Acad::ErrorStatus ResetAllLayer(AcDbObjectIdArray& ids,AcArray<bool>& locks,AcArray<bool>& frozens)
{
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbLayerTableRecordPointer spLayer(ids.at(i),AcDb::kForWrite);
		if(spLayer.openStatus()==Acad::eOk)
		{
			spLayer->setIsLocked(locks.at(i));
			spLayer->setIsFrozen(frozens.at(i));
		}
	}
	return Acad::eOk;
}
