#include "StdAfx.h"
#include "ClientContext.h"
#include "SendFileThread.h"
#include "RecieveFileThread.h"
#include "WriteFileThread.h"

ClientContext::ClientContext(void) : m_bLogin(false)
{
	m_pSendFileThread = new SendFileThread;
	m_pSendFileThread->activate();

	m_pSendFileThread->SetClientContext(this);
}


ClientContext::~ClientContext(void)
{
}

std::string& ClientContext::GetRecievFileAddress()
{
	return m_RecievefileAddr;
}

void ClientContext::SetClientAddress( const std::string& strAddr )
{
	m_strClientAddress = strAddr;
	size_t sP = m_strClientAddress.find(':');

	std::string strAddr1 = m_strClientAddress.substr(0, sP);
	std::string portNum = m_strClientAddress.substr(sP + 1
		, m_strClientAddress.size() - sP);
	unsigned short shPort = atoi(portNum.c_str());

	ACE_INET_Addr Addr(shPort + 1, strAddr1.c_str());

	char buffer[100] = {0};
	sprintf_s(buffer, 100, "%s:%d", strAddr1.c_str(), shPort + 1);
	m_RecievefileAddr = buffer;


	m_pReceiveFileThread = new RecieveFileThread(&Addr);
	m_pReceiveFileThread->activate();

	m_pReceiveFileThread->GetWriteThread()->SetSendThread((ACE_Task<ACE_MT_SYNCH>*)m_pSenderThread);
	m_pReceiveFileThread->GetWriteThread()->SetClientContext(this);
}

void ClientContext::SetSenderThread( SenderThread* pST )
{
	m_pSenderThread = pST;
	m_pSendFileThread->SetSendThread((ACE_Task<ACE_MT_SYNCH>*)pST);
}

void ClientContext::Init()
{
	m_pSendFileThread->Init();
}
