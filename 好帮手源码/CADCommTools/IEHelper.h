#pragma once
//打开指定后缀的文件
HINSTANCE ShellOpenFile(LPCTSTR ext,LPCTSTR file,int showcmd=SW_NORMAL);
//访问网页
HINSTANCE GotoURL(LPCTSTR url, int showcmd=SW_NORMAL);
