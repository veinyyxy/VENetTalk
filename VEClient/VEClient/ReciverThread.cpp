#include "StdAfx.h"
#include "Receiver.h"
#include "ReciverThread.h"
#include "ClientContext.h"

#define DELETE_MESSAGE_BLOCK 0

ReceiverThread::ReceiverThread(ClientContext* pCC, MessageCallback* pMC, size_t sRBS, ACE_Thread_Manager *thr_man) 
	: ACE_Task<ACE_MT_SYNCH>(thr_man)
	, m_bStopFlag(false)
	, m_ReceivedBuffSize(sRBS)
	, m_pClientContext(pCC)
	, m_MessageParser(pCC, pMC)
{
	m_pReciver = new Receiver;
	//m_pReceivedMessageBlock = new ACE_Message_Block(sRBS);
	m_pClientContext->SetReceiverThread(this);
}

ReceiverThread::~ReceiverThread(void)
{
}

int ReceiverThread::svc( void )
{
	while(1)
	{
		m_ControlMutex.lock();
		if(m_bStopFlag) break;
		m_ControlMutex.release();
		
		ACE_Message_Block* pReceivedMessageBlock = new ACE_Message_Block(ACE_MESSAGE_BLOCK_SIZE);
		int iR = m_pReciver->asynch_receive(pReceivedMessageBlock);

		m_MessageParser.ParserMessageBlock(pReceivedMessageBlock, 0);
	}
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
