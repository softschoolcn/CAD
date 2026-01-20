#pragma once

class GQRCodeManager
{
public:
	GQRCodeManager(void);
	~GQRCodeManager(void);
public:
	CString MakeQRCode(CString dat);
};
