#pragma once
struct GQrCodeItem
{
	int X;
	int Y;
	bool bHatch;
	GQrCodeItem()
	{
		X=0;
		Y=0;
		bHatch=false;
	}
};
class GQrCodeMgr
{
public:
	GQrCodeMgr(void);
	~GQrCodeMgr(void);
private:
	char qrcodeErrorCorrect;
	char qrcodeEncodeMode;
	int qrcodeVersion;
	int qrcodeStructureappendN;
	int qrcodeStructureappendM;
	int qrcodeStructureappendParity;
	CString qrcodeStructureappendOriginaldata;

public:
	void	setQrcodeErrorCorrect(char c);
	char	getQrcodeErrorCorrect();

	void	setQrcodeVersion(int i);
	int		getQrcodeVersion();

	void	setQrcodeEncodeMode(char c);
	char	getQrcodeEncodeMode();

	void	setStructureappend(int i, int j, int k);
	void	TestMsg(char* myStr,int length);

	bool** calQrcode(char* abyte0,int iLen0,int& iLenRet);

	int calStructureappendParity(char* abyte0,int i);
	static BYTE* GenerateBarCode(const char *_pStrInfo, int *_pWidth, int *_pHeight);
	//-----------------------------------------------------------------------------------------
//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
static bool SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);


private: //私有方法
	char* divideDataBy8Bits(int* ai, char* abyte0, int i,int iLen,int& iLRet);//,,,返回长度
	char* calculateRSECC(char* abyte0, char byte0, char* abyte1, int i, int j,
		int iLen0,int iLen1);	
	char selectMask(char** abyte0, int i,int iLen0);
	char* calculateByteArrayBits(char* abyte0, char* abyte1, CString s,
		int iLen0,int iLen1,int& iLRet);
	CString GetResRootPath();
};
