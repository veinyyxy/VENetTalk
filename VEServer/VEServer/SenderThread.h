#pragma once

class Sender;

class SenderThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	SenderThread(size_t sendBuffSize = ACE_MESSAGE_BLOCK_SIZE);
	virtual ~SenderThread(void);

	virtual int svc( void );
	int InitSender();

	int InitSender(const ACE_TCHAR *host, 
		u_short port);
	int RestSender(const ACE_TCHAR *host,
		u_short port);

private:
	Sender* m_pSender;
	size_t m_SendBuffSize;
};

