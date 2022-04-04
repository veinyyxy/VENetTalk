#pragma once

class Sender;
class ClientContext;

class SenderThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	SenderThread(ClientContext*);
	virtual ~SenderThread(void);

	virtual int svc( void );

	int InitSender(const ACE_TCHAR *host, 
		u_short port);
	int RestSender(const ACE_TCHAR *host,
		u_short port);

private:
	Sender* m_pSender;
	ClientContext* m_pClientContext;
};

