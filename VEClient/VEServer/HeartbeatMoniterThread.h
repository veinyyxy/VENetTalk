#pragma once

class UserInformation;
class SenderThread;

class HeartbeatMoniterThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	HeartbeatMoniterThread(void);
	virtual ~HeartbeatMoniterThread(void);

	virtual int svc( void );
private:
	UserInformation* m_pUserInfo;
	SenderThread* m_pSenderThread;
};

