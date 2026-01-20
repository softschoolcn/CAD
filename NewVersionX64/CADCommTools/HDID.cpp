//用C++获取硬盘的序列号ID以及网卡的MAC地址的源代码，可以用此方式结合md5加密算法进行软件加密。    




//获取硬盘ID信息及网卡MAC地址的代码   

/*+++  
HDID.CPP  
Written by Lu Lin  
http://lu0.126.com  
2000.11.3  
---*/   
#include "stdafx.h"
#include "HDID.h"
#define DFP_GET_VERSION 0x00074080   
#define DFP_SEND_DRIVE_COMMAND 0x0007c084   
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088   

#pragma pack(1)   
typedef struct _GETVERSIONOUTPARAMS {   
	BYTE bVersion; // Binary driver version.   
	BYTE bRevision; // Binary driver revision.   
	BYTE bReserved; // Not used.   
	BYTE bIDEDeviceMap; // Bit map of IDE devices.   
	DWORD fCapabilities; // Bit mask of driver capabilities.   
	DWORD dwReserved[4]; // For future use.   
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;   

typedef struct _IDEREGS {   
	BYTE bFeaturesReg; // Used for specifying SMART "commands".   
	BYTE bSectorCountReg; // IDE sector count register   
	BYTE bSectorNumberReg; // IDE sector number register   
	BYTE bCylLowReg; // IDE low order cylinder value   
	BYTE bCylHighReg; // IDE high order cylinder value   
	BYTE bDriveHeadReg; // IDE drive/head register   
	BYTE bCommandReg; // Actual IDE command.   
	BYTE bReserved; // reserved for future use. Must be zero.   
} IDEREGS, *PIDEREGS, *LPIDEREGS;   

typedef struct _SENDCMDINPARAMS {   
	DWORD cBufferSize; // Buffer size in bytes   
	IDEREGS irDriveRegs; // Structure with drive register values.   
	BYTE bDriveNumber; // Physical drive number to send   
	// command to (0,1,2,3).   
	BYTE bReserved[3]; // Reserved for future expansion.   
	DWORD dwReserved[4]; // For future use.   
	//BYTE bBuffer[1]; // Input buffer.   
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;   

typedef struct _DRIVERSTATUS {   
	BYTE bDriverError; // Error code from driver,   
	// or 0 if no error.   
	BYTE bIDEStatus; // Contents of IDE Error register.   
	// Only valid when bDriverError   
	// is SMART_IDE_ERROR.   
	BYTE bReserved[2]; // Reserved for future expansion.   
	DWORD dwReserved[2]; // Reserved for future expansion.   
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;   

typedef struct _SENDCMDOUTPARAMS {   
	DWORD cBufferSize; // Size of bBuffer in bytes   
	DRIVERSTATUS DriverStatus; // Driver status structure.   
	BYTE bBuffer[512]; // Buffer of arbitrary length   
	// in which to store the data read from the drive.   
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;   

typedef struct _IDSECTOR {   
	USHORT wGenConfig;   
	USHORT wNumCyls;   
	USHORT wReserved;   
	USHORT wNumHeads;   
	USHORT wBytesPerTrack;   
	USHORT wBytesPerSector;   
	USHORT wSectorsPerTrack;   
	USHORT wVendorUnique[3];   
	CHAR sSerialNumber[20];   
	USHORT wBufferType;   
	USHORT wBufferSize;   
	USHORT wECCSize;   
	CHAR sFirmwareRev[8];   
	CHAR sModelNumber[40];   
	USHORT wMoreVendorUnique;   
	USHORT wDoubleWordIO;   
	USHORT wCapabilities;   
	USHORT wReserved1;   
	USHORT wPIOTiming;   
	USHORT wDMATiming;   
	USHORT wBS;   
	USHORT wNumCurrentCyls;   
	USHORT wNumCurrentHeads;   
	USHORT wNumCurrentSectorsPerTrack;   
	ULONG ulCurrentSectorCapacity;   
	USHORT wMultSectorStuff;   
	ULONG ulTotalAddressableSectors;   
	USHORT wSingleWordDMA;   
	USHORT wMultiWordDMA;   
	BYTE bReserved[128];   
} IDSECTOR, *PIDSECTOR;   

/*+++  
Global vars  
---*/   
GETVERSIONOUTPARAMS vers;   
SENDCMDINPARAMS in;   
SENDCMDOUTPARAMS out;   
HANDLE h;   
DWORD i;   
BYTE j;   

void CopyRight()
{   
	//cerr<<endl<<"HDD identifier v1.0 for WIN95/98/Me/NT/2000. written by Lu Lin"<<endl;   
	//cerr<<"For more information, please visit Inside Programming: http://lu0.126.com"<<endl;   
	//cerr<<"2000.11.3"<<endl<<endl;   
}   

VOID ChangeByteOrder(PCHAR szString, USHORT uscStrSize)   
{   

	USHORT i;   
	CHAR temp;   

	for (i = 0; i < uscStrSize; i+=2)   
	{   
		temp = szString[i];   
		szString[i] = szString[i+1];   
		szString[i+1] = temp;   
	}   
}   

void DetectIDE(BYTE bIDEDeviceMap){   
	if (bIDEDeviceMap&1){   
		if (bIDEDeviceMap&16){   
			//cout<<"ATAPI device is attached to primary controller, drive 0."<<endl;   
		}else{   
			//cout<<"IDE device is attached to primary controller, drive 0."<<endl;   
		}   
	}   
	if (bIDEDeviceMap&2){   
		if (bIDEDeviceMap&32){   
			//cout<<"ATAPI device is attached to primary controller, drive 1."<<endl;   
		}else{   
			//cout<<"IDE device is attached to primary controller, drive 1."<<endl;   
		}   
	}   
	if (bIDEDeviceMap&4){   
		if (bIDEDeviceMap&64){   
			//cout<<"ATAPI device is attached to secondary controller, drive 0."<<endl;   
		}else{   
			//cout<<"IDE device is attached to secondary controller, drive 0."<<endl;   
		}   
	}   
	if (bIDEDeviceMap&8){   
		if (bIDEDeviceMap&128){   
			//cout<<"ATAPI device is attached to secondary controller, drive 1."<<endl;   
		}else{   
			//cout<<"IDE device is attached to secondary controller, drive 1."<<endl;   
		}   
	}   
}
void AddToHardId(char hardId[20],const char* str,const int nSize)
{
	for(int i=0;i<nSize;i++)
	{
		if(str[i]=='\0')
		{
			break;
		}
		if((str[i]>='0'&&str[i]<='9')||
			(str[i]>='A'&&str[i]<='Z')||
			(str[i]>='a'&&str[i]<='z'))
		{
			for(int j=0;j<19;j++)
			{
				if(hardId[j]=='\0')
				{
					hardId[j]=str[i];
					break;
				}
			}
		}
	}
}
bool GetHardId_nt(char hardId[20])
{
	TCHAR hd[80]={0};   
	PIDSECTOR phdinfo;   
	char s[41];   

	ZeroMemory(&vers,sizeof(vers));   
	//We start in NT/Win2000   
	for (j=0;j<4;j++)
	{   
		wsprintf(hd,_T("\\\\.\\PhysicalDrive%d"),j);   
		h=CreateFile(hd,GENERIC_READ|GENERIC_WRITE,   
			FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);   
		if (!h)
		{   
			continue;   
		}   
		if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){   
			CloseHandle(h);   
			continue;   
		}   
		//If IDE identify command not supported, fails   
		if (!(vers.fCapabilities&1)){   
			//cout<<"Error: IDE identify command not supported.";   
			CloseHandle(h);   
			return false;   
		}   
		//Identify the IDE drives   
		ZeroMemory(&in,sizeof(in));   
		ZeroMemory(&out,sizeof(out));   
		if (j&1){   
			in.irDriveRegs.bDriveHeadReg=0xb0;   
		}else{   
			in.irDriveRegs.bDriveHeadReg=0xa0;   
		}   
		if (vers.fCapabilities&(16>>j)){   
			//We don't detect a ATAPI device.   
			//cout<<"Drive "<<(int)(j+1)<<" is a ATAPI device, we don't detect it"<<endl;   

			continue;   
		}else{   
			in.irDriveRegs.bCommandReg=0xec;   
		}   
		in.bDriveNumber=j;   
		in.irDriveRegs.bSectorCountReg=1;   
		in.irDriveRegs.bSectorNumberReg=1;   
		in.cBufferSize=512;   
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){   
			//cout<<"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl;   
			CloseHandle(h);   
			return false;   
		}   
		phdinfo=(PIDSECTOR)out.bBuffer;   
		memcpy(s,phdinfo->sModelNumber,40);   
		s[40]=0;   
		ChangeByteOrder(s,40);   
		//cout<<endl<<"Module Number:"<<s<<endl;   
		memcpy(s,phdinfo->sFirmwareRev,8);   
		s[8]=0;   
		ChangeByteOrder(s,8);   
		//cout<<"\tFirmware rev:"<<s<<endl;   
		memcpy(s,phdinfo->sSerialNumber,20);   
		s[20]=0;   
		ChangeByteOrder(s,20);
		AddToHardId(hardId,s,19);
		//cout<<"\tSerial Number:"<<s<<endl;   
		//cout<<"\tCapacity:"<<phdinfo->ulTotalAddressableSectors/2/1024<<"M"<<endl<<endl;   
		CloseHandle(h);   
	}   
	CopyRight(); 
	return true;
}
bool GetHardId_9x(char hardId[20])
{
	ZeroMemory(&vers,sizeof(vers));   
	//We start in 95/98/Me   
	h=CreateFile(_T("\\\\.\\Smartvsd"),0,0,0,CREATE_NEW,0,0);   
	if (!h)
	{  
		return false;
	}   

	if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0))
	{   
		CloseHandle(h);   
		return false;   
	}   
	//If IDE identify command not supported, fails   
	if (!(vers.fCapabilities&1))
	{   
		CloseHandle(h);   
		return false;   
	}   
	//Display IDE drive number detected   
	DetectIDE(vers.bIDEDeviceMap);   
	//Identify the IDE drives   
	for (j=0;j<4;j++)
	{   
		PIDSECTOR phdinfo;   
		char s[41];   

		ZeroMemory(&in,sizeof(in));   
		ZeroMemory(&out,sizeof(out));   
		if (j&1){   
			in.irDriveRegs.bDriveHeadReg=0xb0;   
		}else{   
			in.irDriveRegs.bDriveHeadReg=0xa0;   
		}   
		if (vers.fCapabilities&(16>>j)){   

			//We don't detect a ATAPI device.   
			//cout<<"Drive "<<(int)(j+1)<<" is a ATAPI device, we don't detect it"<<endl;   
			continue;   
		}else{   
			in.irDriveRegs.bCommandReg=0xec;   
		}   
		in.bDriveNumber=j;   
		in.irDriveRegs.bSectorCountReg=1;   
		in.irDriveRegs.bSectorNumberReg=1;   
		in.cBufferSize=512;   
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0))
		{   
			//cout<<"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl;   
			CloseHandle(h);
			continue;
		}   
		phdinfo=(PIDSECTOR)out.bBuffer;   
		memcpy(s,phdinfo->sModelNumber,40);   
		s[40]=0;   
		ChangeByteOrder(s,40);   
		//cout<<endl<<"Module Number:"<<s<<endl;   
		memcpy(s,phdinfo->sFirmwareRev,8);   
		s[8]=0;   
		ChangeByteOrder(s,8);   
		//cout<<"\tFirmware rev:"<<s<<endl;   
		memcpy(s,phdinfo->sSerialNumber,20);   
		s[20]=0;   
		ChangeByteOrder(s,20);
		AddToHardId(hardId,s,19);   
	}   

	//Close handle before quit   
	CloseHandle(h);   
	CopyRight();
	return true;
}
bool GetHardId(char hardId[20])
{
	OSVERSIONINFO VersionInfo;   

	ZeroMemory(&VersionInfo,sizeof(VersionInfo));   
	VersionInfo.dwOSVersionInfoSize=sizeof(VersionInfo);   
	GetVersionEx(&VersionInfo);
	bool rc=false;
	if(VersionInfo.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
	{
		rc=GetHardId_9x(hardId);
	}
	else if(VersionInfo.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{
		rc=GetHardId_nt(hardId);
	}
	else
	{
		return false;
	}
	if(rc)
	{
		if(hardId[19]!='\0')
		{
			hardId[19]='\0';
		}
		if(strlen(hardId)<5)
		{
			rc=false;
		}
	}
	return rc;
}
/*


void main(){   
	char szAddr[20];   
	GetMacAddress(szAddr);   
	cout<<"\nNet adapter MAC Address:"<<szAddr<<endl<<endl;   

	OSVERSIONINFO VersionInfo;   

	ZeroMemory(&VersionInfo,sizeof(VersionInfo));   
	VersionInfo.dwOSVersionInfoSize=sizeof(VersionInfo);   
	GetVersionEx(&VersionInfo);   

	switch (VersionInfo.dwPlatformId){   
	case VER_PLATFORM_WIN32s:   
		cout<<"Win32s is not supported by this programm."<<endl;   
		break;   
	case VER_PLATFORM_WIN32_WINDOWS:   
		hdid9x();   
		break;   
	case VER_PLATFORM_WIN32_NT:   
		hdidnt();   
		break;   
	}   

	// TODO: Add your control notification handler code here   
}
*/