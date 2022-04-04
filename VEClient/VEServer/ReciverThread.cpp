#include "StdAfx.h"
#include "Receiver.h"
#include "ReciverThread.h"
#include "ServerContext.h"
#include "MessageParserThread.h"

#define DELETE_MESSAGE_BLOCK 0

extern ServerContext* g_pServerContext;

ReceiverThread::ReceiverThread(size_t sRBS, ACE_Thread_Manager *thr_man) 
	: ACE_Task<ACE_MT_SYNCH>(thr_man)
	, m_bStopFlag(false)
	, m_ReceivedBuffSize(sRBS)
{
	m_pReciver = new Receiver;
	g_pServerContext->SetReceiverThread(this);
}


ReceiverThread::~ReceiverThread(void)
{
}

int ReceiverThread::svc( void )
{
#if DELETE_MESSAGE_BLOCK
	ACE_Message_Block* pCommandBlock = new ACE_Message_Block(4);
#endif
	while(1)
	{
#if DELETE_MESSAGE_BLOCK
		pCommandBlock->reset();
		this->getq(pCommandBlock, new ACE_Time_Value(0.1));
		char* pCommand = pCommandBlock->rd_ptr();

		if(strcmp(pCommand, "q") == 0)
			break;
#endif
		m_ControlMutex.lock();
		if(m_bStopFlag) break;
		m_ControlMutex.release();
		
		ACE_Message_Block* pReceivedMessageBlock = new ACE_Message_Block(m_ReceivedBuffSize);

		int iR = m_pReciver->asynch_receive(pReceivedMessageBlock);

		MessageParserThread* pParserThread =  g_pServerContext->GetParserThread();
		pParserThread->putq(pReceivedMessageBlock);
	}
#if DELETE_MESSAGE_BLOCK
	delete pCommandBlock;
#endif
	return 0;
}

int ReceiverThread::open( void *args /*= 0 */ )
{
	return 0;
}

int ReceiverThread::close( u_long flags /*= 0 */ )
{
	return 0;
	//return m_pReciver->close_recever();
}

int ReceiverThread::InitReciver(ACE_INET_Addr& addr)
{
	return m_pReciver->open_addr(addr);
}

int ReceiverThread::StopReciverThread()
{
	m_ControlMutex.lock();
	m_bStopFlag = true;
	m_ControlMutex.release();
	
	m_pReciver->close_recever();
	/*ACE_Message_Block* pStopCommand = new ACE_Message_Block(4);
	pStopCommand->copy("q");
	this->putq(pStopCommand);*/
	return /*ACE_Proactor::instance()->end_event_loop()*/1;
}
