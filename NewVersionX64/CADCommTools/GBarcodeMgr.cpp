#include "StdAfx.h"
#include "GBarcodeMgr.h"
GBarcodeMgr::GBarcodeMgr(void)
{
	mCode128Stop = "1100011101011";
	mCode128Encoding.push_back("11011001100");
	mCode128Encoding.push_back("11001101100");
	mCode128Encoding.push_back("11001100110");
	mCode128Encoding.push_back("10010011000");
	mCode128Encoding.push_back("10010001100");
	mCode128Encoding.push_back("10001001100");
	mCode128Encoding.push_back("10011001000");
	mCode128Encoding.push_back("10011000100");
	mCode128Encoding.push_back("10001100100");
	mCode128Encoding.push_back("11001001000");
	mCode128Encoding.push_back("11001000100");
	mCode128Encoding.push_back("11000100100");
	mCode128Encoding.push_back("10110011100");
	mCode128Encoding.push_back("10011011100");
	mCode128Encoding.push_back("10011001110");
	mCode128Encoding.push_back("10111001100");
	mCode128Encoding.push_back("10011101100");
	mCode128Encoding.push_back("10011100110");
	mCode128Encoding.push_back("11001110010");
	mCode128Encoding.push_back("11001011100");
	mCode128Encoding.push_back("11001001110");
	mCode128Encoding.push_back("11011100100");
	mCode128Encoding.push_back("11001110100");
	mCode128Encoding.push_back("11101101110");
	mCode128Encoding.push_back("11101001100");
	mCode128Encoding.push_back("11100101100");
	mCode128Encoding.push_back("11100100110");
	mCode128Encoding.push_back("11101100100");
	mCode128Encoding.push_back("11100110100");
	mCode128Encoding.push_back("11100110010");
	mCode128Encoding.push_back("11011011000");
	mCode128Encoding.push_back("11011000110");
	mCode128Encoding.push_back("11000110110");
	mCode128Encoding.push_back("10100011000");
	mCode128Encoding.push_back("10001011000");
	mCode128Encoding.push_back("10001000110");
	mCode128Encoding.push_back("10110001000");
	mCode128Encoding.push_back("10001101000");
	mCode128Encoding.push_back("10001100010");
	mCode128Encoding.push_back("11010001000");
	mCode128Encoding.push_back("11000101000");
	mCode128Encoding.push_back("11000100010");
	mCode128Encoding.push_back("10110111000");
	mCode128Encoding.push_back("10110001110");
	mCode128Encoding.push_back("10001101110");
	mCode128Encoding.push_back("10111011000");
	mCode128Encoding.push_back("10111000110");
	mCode128Encoding.push_back("10001110110");
	mCode128Encoding.push_back("11101110110");
	mCode128Encoding.push_back("11010001110");
	mCode128Encoding.push_back("11000101110");
	mCode128Encoding.push_back("11011101000");
	mCode128Encoding.push_back("11011100010");
	mCode128Encoding.push_back("11011101110");
	mCode128Encoding.push_back("11101011000");
	mCode128Encoding.push_back("11101000110");
	mCode128Encoding.push_back("11100010110");
	mCode128Encoding.push_back("11101101000");
	mCode128Encoding.push_back("11101100010");
	mCode128Encoding.push_back("11100011010");
	mCode128Encoding.push_back("11101111010");
	mCode128Encoding.push_back("11001000010");
	mCode128Encoding.push_back("11110001010");
	mCode128Encoding.push_back("10100110000");
	mCode128Encoding.push_back("10100001100");
	mCode128Encoding.push_back("10010110000");
	mCode128Encoding.push_back("10010000110");
	mCode128Encoding.push_back("10000101100");
	mCode128Encoding.push_back("10000100110");
	mCode128Encoding.push_back("10110010000");
	mCode128Encoding.push_back("10110000100");
	mCode128Encoding.push_back("10011010000");
	mCode128Encoding.push_back("10011000010");
	mCode128Encoding.push_back("10000110100");
	mCode128Encoding.push_back("10000110010");
	mCode128Encoding.push_back("11000010010");
	mCode128Encoding.push_back("11001010000");
	mCode128Encoding.push_back("1110111010");
	mCode128Encoding.push_back("1000010100");
	mCode128Encoding.push_back("0001111010");
	mCode128Encoding.push_back("10100111100");
	mCode128Encoding.push_back("10010111100");
	mCode128Encoding.push_back("10010011110");
	mCode128Encoding.push_back("10111100100");
	mCode128Encoding.push_back("10011110100");
	mCode128Encoding.push_back("10011110010");
	mCode128Encoding.push_back("11110100100");
	mCode128Encoding.push_back("11110010100");
	mCode128Encoding.push_back("11110010010");
	mCode128Encoding.push_back("11011011110");
	mCode128Encoding.push_back("11011110110");
	mCode128Encoding.push_back("11110110110");
	mCode128Encoding.push_back("10101111000");
	mCode128Encoding.push_back("10100011110");
	mCode128Encoding.push_back("10001011110");
	mCode128Encoding.push_back("10111101000");
	mCode128Encoding.push_back("10111100010");
	mCode128Encoding.push_back("11110101000");
	mCode128Encoding.push_back("11110100010");
	mCode128Encoding.push_back("10111011110");
	mCode128Encoding.push_back("10111101110");
	mCode128Encoding.push_back("11101011110");
	mCode128Encoding.push_back("11110101110");
	mCode128Encoding.push_back("11010000100");
	mCode128Encoding.push_back("11010010000");
	mCode128Encoding.push_back("11010011100");

	mBarWidth = 0.5;
}

GBarcodeMgr::~GBarcodeMgr(void)
{
}
//获取条码原始内容
CString GBarcodeMgr::GetOriginCode() const
{
	return mOriginCode;
}
//设置条码原始内容
//设置条码原始内容
void GBarcodeMgr::SetOriginCode(const CString strCode)
{
	mOriginCode=strCode;
}
//指定中心点位置和高度创建
//bText,是否包含文字显示
Acad::ErrorStatus GBarcodeMgr::Create(AcDbVoidPtrArray& entitySet,AcGePoint3d  position,double dHeight,bool bText)
{
	string code = GStringHelper::ConvertToLPCSTR(GetOriginCode());
	int nCodeIndex=0;
	mBarWidth=dHeight*0.47769;//长宽比
	mBarWidth=mBarWidth/11.0;//共有11位
	AcGePoint3d pt=position;
	string encode = GetBarcodeContents();
	int len = encode.size();
	double dTextWidth=mBarWidth*len;
	pt.x-=dTextWidth/2.0;
	AcGePoint3d ptStart=pt;
	ptStart.y=pt.y-0.15*dHeight;
	AcGePoint3d ptEnd=pt;
	ptEnd.y=pt.y-0.5*dHeight;
	for (int i = 0; i < len; i++)
	{
		if (encode[i] == '1')
		{
			AcDbPolyline* pLine = new AcDbPolyline(2);
			pLine->addVertexAt(0, AcGePoint2d(pt.x,pt.y+0.5*dHeight));
			pLine->addVertexAt(1, AcGePoint2d(pt.x,bText?pt.y-0.1*dHeight:pt.y-0.5*dHeight));
			pLine->setConstantWidth(mBarWidth);
			pLine->setColorIndex(0);
			entitySet.append(pLine);
		}
		pt.x += mBarWidth;
	}
	if(bText)
	{
		dTextWidth=dTextWidth/code.length();
		for(int j=0;j<code.length();j++)
		{
			CString strText=_T("");
			strText+=code.at(j);
			//被11整除，且不是0
			if(j!=0)
			{
				ptStart.x+=dTextWidth;
			}
			ptEnd.x+=dTextWidth;
			//创建文本
			AcDbMText* pMText=new AcDbMText();
			pMText->setAttachment(AcDbMText::kMiddleCenter);
			pMText->setLocation(GCADUtils::MidPoint(ptStart,ptEnd));
			pMText->setTextHeight(dHeight*0.3);
			pMText->setContents(strText);
			entitySet.append(pMText);
		}
	}
	
	return Acad::eOk;
}
//条码编码后的内容
string GBarcodeMgr::GetBarcodeContents()
{
	string code = GStringHelper::ConvertToLPCSTR(GetOriginCode());
	vector<int> codeDigit;
	codeDigit.push_back(CodeB);
	int checkDigit = CodeB;
	int len = code.size();
	for (int i = 0; i < len; i++)
	{
		int digit = int(code[i] - 32);
		codeDigit.push_back(digit);
		checkDigit += digit * (i + 1);
	}
	codeDigit.push_back(checkDigit % 103);

	string code128;
	len = codeDigit.size();
	for (int i = 0; i < len; i++)
	{
		int index = codeDigit[i];
		code128 += mCode128Encoding[index];
	}

	code128 += mCode128Stop;
	return code128;
}