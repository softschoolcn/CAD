#pragma once
//打开对比文档辅助类
class GOpenCmdDocHelper
{
public:
	GOpenCmdDocHelper(void);
public:
	~GOpenCmdDocHelper(void);
	//交换两个文档的窗体位置
	void SwapDocPosition(AcApDocument* pDoc1,AcApDocument* pDoc2);
	//确保两个文档的左右位置
	void MakeSureDocLeftRight(AcApDocument* pDoc1,AcApDocument* pDoc2);
	//打开对比文档
	Acad::ErrorStatus OpenCmpDoc(CmpResult* pRc);
private:
	//获取文档对应的窗口
	HWND GetDocWindow(AcApDocument* pDoc);
	//打开对比文档
	Acad::ErrorStatus OpenDoc(AcApDocument*& pDoc,const CString strFile);
	//设置文档的显示方式
	void ShowDocWindow(AcApDocument* pDoc,int nCmdShow);
};
