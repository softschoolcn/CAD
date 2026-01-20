#pragma once

class CByteString
{
public:
	CByteString(void);
public:
	~CByteString(void);
	CString toString(BYTE* data,const long nLen);
	CString toString(CArray<BYTE>& byteArray);
	inline void toString(char& char1,char& char2,BYTE& bByte);
	long toByte(BYTE*& data,long& byteLen,CString& str);
	inline void toByte(BYTE& bByte,char& char1,char& char2);
	inline int GetInt(char& char1);
private:
	char innerStr[62];
};
