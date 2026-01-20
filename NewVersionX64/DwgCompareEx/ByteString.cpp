#include "StdAfx.h"
#include "ByteString.h"
CByteString::CByteString(void)
{
	int i=0;
	for(i=0;i<=9;i++)
		innerStr[i]='0'+i;
	for(i=0;i<=25;i++)
		innerStr[i+10]='A'+i;
	for(i=0;i<=25;i++)
		innerStr[i+36]='a'+i;
}

CByteString::~CByteString(void)
{
}
CString CByteString::toString(BYTE* data,const long nLen)
{
	if(nLen==0)
		return _T("");
	char* szBuf=new char[2*nLen+1];
	char char1;
	char char2;
	for(long i=0;i<nLen;i++)
	{
		toString(char1,char2,data[i]);
		szBuf[2*i]=char1;
		szBuf[2*i+1]=char2;
	}
	szBuf[2*nLen]='\0';
	CString strTmp(szBuf);
	delete[] szBuf;
	szBuf=NULL;
	return strTmp;
}
CString CByteString::toString(AcArray<BYTE>& byteArray)
{
	if(byteArray.logicalLength()==0)
		return _T("");
	char* szBuf=new char[2*byteArray.logicalLength()+1];
	char char1;
	char char2;
	for(long i=0;i<byteArray.logicalLength();i++)
	{
		toString(char1,char2,byteArray[i]);
		szBuf[2*i]=char1;
		szBuf[2*i+1]=char2;
	}
	szBuf[2*byteArray.logicalLength()]='\0';
	CString strTmp(szBuf);
	delete[] szBuf;
	szBuf=NULL;
	return strTmp;
}
void CByteString::toString(char& char1,char& char2,BYTE& bByte)
{
	int n1=bByte%62;
	int n2=(bByte-n1)/62;
	char1=innerStr[n2];
	char2=innerStr[n1];
}
long CByteString::toByte(BYTE*& data,long& byteLen,CString& str)
{
	byteLen=0;
	if(str.IsEmpty())
		return 0;
	int nLen=str.GetLength();
	if(nLen%2!=0)
		return 0;
	nLen=nLen/2;
	byteLen=nLen;
	data=new BYTE[nLen];
	char char1;
	char char2;
	for(long i=0;i<nLen;i++)
	{
		char1=str.GetAt(2*i);
		char2=str.GetAt(2*i+1);
		toByte(data[i],char1,char2);

	}

	return nLen;

	
}
void CByteString::toByte(BYTE& bByte,char& char1,char& char2)
{
	int n1=GetInt(char1);
	int n2=GetInt(char2);
	bByte=n1*62+n2;
}
int CByteString::GetInt(char& char1)
{
	if(char1>='0'&&char1<='9')
	{
		return char1-'0';
	}
	if(char1>='A'&&char1<='Z')
	{
		return char1-'A'+10;
	}
	if(char1>='a'&&char1<='z')
	{
		return char1-'a'+36;
	}
	return 0;

}