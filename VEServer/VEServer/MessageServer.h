#pragma once

class CMessageServer
{
public:
	CMessageServer(void);
	CMessageServer(ACE_INET_Addr& addr);
	CMessageServer(const char* pchAddr, u_short uPort);
	virtual ~CMessageServer(void);
	inline void SetAddr(ACE_INET_Addr& addr){m_Addr = addr;}

	inline void SetAddr(const char* pchAddr){memset(m_cAddr, 0, 512);strcpy_s(m_cAddr, 512, pchAddr);};
	inline void SetPort(u_short uPort){m_uPort = uPort;}
	
	virtual int Initialze() = 0;
	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int Pause() = 0;
	virtual int Resume() = 0;

protected:
	u_short m_uPort;
	ACE_INET_Addr m_Addr;
	char m_cAddr[512];
};

