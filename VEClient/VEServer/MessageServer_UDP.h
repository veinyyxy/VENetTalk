#pragma once
#include "MessageServer.h"

class ReceiverThread;
class MessageParserThread;
class SenderThread;
class HeartbeatMoniterThread;

class CMessageServer_UDP :
	public CMessageServer
{
public:
	CMessageServer_UDP(void);
	virtual ~CMessageServer_UDP(void);

	virtual int Initialze();

	virtual int Start();

	virtual int Stop();

	virtual int Pause();

	virtual int Resume();
private:
	ACE_Thread_Manager* m_pThreadManager;

	MessageParserThread* m_pMessageParserThread;
	ReceiverThread* m_pReciverThread;
	SenderThread* m_pSenderThread;

	HeartbeatMoniterThread* m_pHeartbeatMoniterThread;
};

