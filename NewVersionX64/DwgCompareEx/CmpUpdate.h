#pragma once
struct CmpVersionInfo
{
	CString m_strCurrentVersion;
	CString m_strVersiuonURL;
	CString m_strURL;
	CString m_UpdatDescribe;
	CString m_strLastUpdateTime;
};
class CmpUpdate
{
public:
	CmpUpdate(void);
	~CmpUpdate(void);
	CmpVersionInfo m_LocalInfo;
	CmpVersionInfo m_ServerInfo;
	BOOL ReadLocal();
	BOOL ReadLocalServerInfo();
	BOOL UpdateServerInfo();
	BOOL WriteTryTime();
	BOOL NeedTryUpdate();
	bool m_bHaveNewVersion;
	bool m_bFirstTime;
	bool m_bNeedReadServer;
};
