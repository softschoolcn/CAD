// GMTextDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GMTextDialog.h"


// GMTextDialog 对话框

IMPLEMENT_DYNAMIC(GMTextDialog, CDialog)

GMTextDialog::GMTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GMTextDialog::IDD, pParent)
{

}

GMTextDialog::~GMTextDialog()
{
}

void GMTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GMTextDialog, CDialog)
END_MESSAGE_MAP()


// GMTextDialog 消息处理程序
