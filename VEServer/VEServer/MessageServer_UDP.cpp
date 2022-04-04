#include "StdAfx.h"
#include "Receiver.h"
#include "ReciverThread.h"
#include "MessageParserThread.h"
#include "SenderThread.h"
#include "MessageServer_UDP.h"
#include "HeartbeatMoniterThread.h"

class ReceiverParser : public ReceiverCallback
{
public:
	ReceiverParser(MessageParserThread* pMPT) : m_pMessageParserThread(pMPT){}
	virtual void ReciverComplete( ACE_Message_Block* pMB )
	{
		if(m_pMessageParserThread)
			m_pMessageParserThread->putq(pMB);
	}
public:
	MessageParserThread* m_pMessageParserThread;
};

CMessageServer_UDP::CMessageServer_UDP(void)
{
	//m_pThreadManager = ACE_Thread_Manager::instance();
	m_pReciverThread = new ReceiverThread(ACE_MESSAGE_BLOCK_SIZE);
	//Receiver* pReceiver = m_pReciverThread->GetReceiver();
	m_pMessageParserThread = new MessageParserThread(ACE_MESSAGE_BLOCK_SIZE, ACE_MESSAGE_BLOCK_SIZE);
	//pReceiver->SetReceiverCallback(new ReceiverParser(m_pMessageParserThread));
	m_pSenderThread = new SenderThread(ACE_MESSAGE_BLOCK_SIZE);

	m_pHeartbeatMoniterThread = new HeartbeatMoniterThread;
}


CMessageServer_UDP::~CMessageServer_UDP(void)
{
}

int CMessageServer_UDP::Initialze()
{
	m_pReciverThread->InitReciver(m_Addr);
	m_pSenderThread->InitSender();
	
	
	return 1;
}

int CMessageServer_UDP::Start()
{
	m_pReciverThread->activate();
	m_pSenderThread->activate();
	m_pMessageParserThread->activate(0, 3);
	m_pHeartbeatMoniterThread->activate();

	return 0;
}

int CMessageServer_UDP::Stop()
{
	//m_pThreadManager->cancel_all();

	m_pReciverThread->StopReciverThread();
	//m_pSenderThread->close();
	m_pReciverThread->wait();
	//m_pThreadManager->close();
	//m_pThreadManager->close_singleton();
	return 1;
}

int CMessageServer_UDP::Pause()
{
	return -1;
}

int CMessageServer_UDP::Resume()
{
	return -1;
}
