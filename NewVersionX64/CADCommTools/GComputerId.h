#pragma once

class GComputerId
{
public:
	GComputerId(void);
public:
	~GComputerId(void);
public:
	//获取CPUID
	//CString GetCpuId();
	//获取MAC
	CString GetMac();
	//获取机器码
	CString GetComputerId();
};
