#pragma once
#include "MessageParser.h"

class Receiver;
class ClientContext;
class MessageCallback;

class ReceiverThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	ReceiverThread(ClientContext* PCC, MessageCallback* pMC, size_t sRBS = ACE_MESSAGE_BLOCK_SIZE, ACE_Thread_Manager *thr_man = 0);
	~ReceiverThread(void);

	virtual int svc( void );

	virtual int open( void *args = 0 );

	virtual int close( u_long flags = 0 );

	/*virtual int put( ACE_Message_Block *, ACE_Time_Value * = 0 );*/

	int InitReciver(ACE_INET_Addr& addr);
	int StopReciverThread();
	inline Receiver* GetReceiver(){return m_pReciver;}
	inline void SetMessageCallback(MessageCallback* pMC){m_MessageParser.SetMessageCallback(pMC);}

private:
	size_t m_ReceivedBuffSize;
	bool m_bStopFlag;
	ACE_Message_Block* m_pReceivedMessageBlock;
	Receiver* m_pReciver;
	ACE_Mutex m_ControlMutex;
	MessageParser m_MessageParser;
	ClientContext* m_pClientContext;
};

