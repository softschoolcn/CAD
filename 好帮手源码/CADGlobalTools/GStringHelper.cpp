#include "StdAfx.h"
#include "GStringHelper.h"

GStringHelper::GStringHelper(void)
{
}

GStringHelper::~GStringHelper(void)
{
}
//功能:	指定的字符串是否是合理的长整数（只能处理十进制的）
//参数:	value,指定的字符串
//      nValue,返回长整数型结果
//返回: 是否是合理的长整数
BOOL GStringHelper::IsValidLong(const CString value,long &lValue)
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
BOOL GStringHelper::IsValidInt(const CString value,int &nValue)
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
BOOL GStringHelper::IsValidDouble(const CString value,double& dValue)
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
		if(part_lValue<0)
		{
			bNegative=true;
			part_lValue=-part_lValue;
		}
		else if(part_lValue==0)
		{
			//如:-0.5
			if(strPart1.GetAt(0)=='-')
			{
				bNegative=true;
			}
		}
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
//功能:	将CString转换成char*,自动区分UNICODE和ASCII
//参数:	swString,需要转化的字符串
//      szBuffer,char*指针，使用完后记得使用free释放
//返回: 
void GStringHelper::ConvertToLPCSTR(char*& szBuf,CString swString)
{
#ifdef _UNICODE
	szBuf = NULL;
	UINT  nLen  = NULL;

	nLen = WideCharToMultiByte(CP_ACP, 0, swString/*wszString*/, -1, NULL, 0, NULL, NULL);

	szBuf = (char*)malloc(nLen + 1);

	int nRet = WideCharToMultiByte(CP_ACP, 0, swString, -1, szBuf, nLen, NULL, NULL);

	if (0 == nRet)
	{
		free(szBuf); 
		szBuf = NULL;
	}
#else
	szBuf = NULL;
	int nLen=swString.GetLength()+1;
	szBuf = (char*)malloc(nLen + 1);
	strcpy(szBuf,swString);
#endif
}
std::string GStringHelper::ConvertToLPCSTR(CString swString)
{
	char* szBuf=NULL;
	ConvertToLPCSTR(szBuf,swString);
	std::string tmp=std::string(szBuf);
	free(szBuf);
	return tmp;
}
/*!
     @zhangshl 生成GUID
     */
	CString GStringHelper::NewGuid()
	{
		OLECHAR szGuid[128] =OLESTR("");
		GUID g_guid;
		CoCreateGuid(&g_guid);	
		StringFromGUID2(g_guid,szGuid,sizeof(szGuid)/sizeof(OLECHAR));
		USES_CONVERSION;
		CString m_GuidPrt =W2T(szGuid);
		return m_GuidPrt;
	}
	/*!
     @brief 根据token拆分字符串src，放到result里
     */
    bool GStringHelper::Split(CString& src, CString token, CStringArray& resultArray)
	{
		return Split(src,token,resultArray,false);
	}
    bool GStringHelper::Split(CString& src, CString token, CStringArray& resultArray,bool ingoreEmptyString)
    {
        int tokenSize = token.GetLength();
        CString result;
        CString srcouceStr = src;
        while (true)
        {
            int index = srcouceStr.Find(token);
            if (index == 0)
            {
                result = "";
            }
            else if (index < 0)
            {
				if((!ingoreEmptyString)||(!srcouceStr.IsEmpty()))
				{
                resultArray.Add(srcouceStr);
				}
                return true;
            }
            else
            {
                result = srcouceStr.Left(index);
            }
			if((!ingoreEmptyString)||(!result.IsEmpty()))
			{
				resultArray.Add(result);
			}

            int sourceLength = srcouceStr.GetLength();
            srcouceStr = srcouceStr.Right(sourceLength - index - tokenSize);
        }
    }
	//功能:判断字符串是否在字符串数组中
	//参数:	array:字符串数组
	//			string:字符串
	//			bNoCase:是否区分大小写，true:区分，默认；false:不区分
	//返回:	true:在数组中；false:不在
	bool GStringHelper::IsStringInArray(const CStringArray& array, CString stringX,bool bNoCase/*=true*/)
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
