#pragma once
namespace GLog
{
	//功能:	输出一行日志
	//参数:	fmt:待输出内容，最多输出4096个字符
	//返回:	无
	void WriteLine(CString fmt, ...);
}
#ifndef GLOG_FILELINE
#define GLOG_FILELINE GLog::WriteLine(_T("XXXXXXXXXXXXXXX ::文件: %s; 行: %d"),CString(__FILE__),__LINE__)
#endif
