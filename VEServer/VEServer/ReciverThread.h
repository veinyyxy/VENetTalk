#pragma once

class Receiver;

class ReceiverThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	ReceiverThread(size_t sRBS = 1024, ACE_Thread_Manager *thr_man = 0);
	~ReceiverThread(void);

	virtual int svc( void );

	virtual int open( void *args = 0 );

	virtual int close( u_long flags = 0 );

	/*virtual int put( ACE_Message_Block *, ACE_Time_Value * = 0 );*/

	int InitReciver(ACE_INET_Addr& addr);
	int StopReciverThread();
	inline Receiver* GetReceiver(){return m_pReciver;}
	
private:
	size_t m_ReceivedBuffSize;
	bool m_bStopFlag;
	Receiver* m_pReciver;
	ACE_Mutex m_ControlMutex;
};

