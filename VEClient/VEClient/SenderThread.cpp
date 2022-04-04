#include "StdAfx.h"
#include "Sender.h"
#include "SenderThread.h"
#include "ClientContext.h"

extern ClientContext* p_gClientContext;

SenderThread::SenderThread(ClientContext* pCC) : m_pClientContext(pCC)
{
	m_pSender = new Sender;
	m_pClientContext->SetSenderThread(this);
}


SenderThread::~SenderThread(void)
{
}

int SenderThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* mb = new ACE_Message_Block;
		this->getq(mb);

		int iSize = mb->length();

		ACE_DEBUG((LM_DEBUG, "Block size = %d", iSize));
		m_pSender->asynch_send(mb);
		//ACE_Proactor::instance()->handle_events();

		//delete mb;
	}

	return 0;
}

int SenderThread::InitSender( const ACE_TCHAR *host, u_short port )
{
	return m_pSender->Open(host, port);
}

int SenderThread::RestSender( const ACE_TCHAR *host, u_short port )
{
	m_pSender->clean();
	return m_pSender->Open(host, port);
}
