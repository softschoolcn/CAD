#pragma once
#include <vector>
using namespace std;
class GBarcodeMgr
{
public:
	//起始位
    enum Code128StartModes
    {
        CodeUnset = 0,
        CodeA = 103,
        CodeB = 104,
        CodeC = 105
    };
public:
	GBarcodeMgr(void);
	~GBarcodeMgr(void);
	//获取条码原始内容
	CString GetOriginCode() const;
	//设置条码原始内容
	void SetOriginCode(const CString strCode);
	//指定中心点位置和高度创建
	//bText,是否包含文字显示
	Acad::ErrorStatus Create(AcDbVoidPtrArray& entitySet,AcGePoint3d  position,double dHeight,bool bText=true);
private:
	//条码编码后的内容
	string GetBarcodeContents();
	vector<string> mCode128Encoding;  //条码编码规则
	string mCode128Stop;              //停止位
	double mBarWidth;                 //条码宽度
	CString mOriginCode;              //条码内容
};
