#include "StdAfx.h"
#include "Sender.h"
#include "SenderThread.h"
#include "ServerContext.h"
#include "MessageProtocol.h"

extern ServerContext* g_pServerContext;

#define MY_BLOCK_SIZE (4 * 1024)

SenderThread::SenderThread(size_t sendBuffSize)
	: m_SendBuffSize(sendBuffSize)
{
	m_pSender = new Sender;
	g_pServerContext->SetSenderThread(this);
}


SenderThread::~SenderThread(void)
{
}

int SenderThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* pSendBufferBlock = 0;
		this->getq(pSendBufferBlock);
		
		
		if(pSendBufferBlock == 0)
		{
			continue;
		}
		message_header* pMH = (message_header*)(pSendBufferBlock->base());
		char* chAddr = pMH->_address;
		std::string strAddr = chAddr;
		size_t sP = strAddr.find(':');

		std::string strAddr1 = strAddr.substr(0, sP);
		std::string portNum = strAddr.substr(sP + 1, strAddr.size() - sP);
		unsigned short shPort = atoi(portNum.c_str());
		ACE_INET_Addr addr(shPort, strAddr1.c_str());
		
		if(shPort == 0 || strAddr1.length() <= 0)
		{
			pSendBufferBlock->release();
			continue;
		}
		/*
		int iWriteLength = pSendBufferBlock->length();
		if(iWriteLength > MY_BLOCK_SIZE)
		{
			int iBlockCount = iWriteLength / MY_BLOCK_SIZE;
			int iFinalCount = iWriteLength % MY_BLOCK_SIZE;
			if(iFinalCount > 0) iBlockCount += 1;
			ACE_Message_Block* pPrevBlock = 0;
			ACE_Message_Block* pFirstBlock = 0;
			for(int i = 0; i < iBlockCount; i++)
			{
				ACE_Message_Block* pContinueBlock = 0;
				int iSize = 0;
				if(iBlockCount - 1 == i)
				{
					if(iFinalCount > 0)
					{
						iSize = iFinalCount;
					}
					else
					{
						break;
					}
				}
				else
				{
					iSize = MY_BLOCK_SIZE;
				}
				pContinueBlock = new ACE_Message_Block(MY_BLOCK_SIZE);
				memset(pContinueBlock->base(), 0, pContinueBlock->size());
				memcpy_s(pContinueBlock->wr_ptr(), pContinueBlock->size(), pSendBufferBlock->base() + MY_BLOCK_SIZE * i, iSize);
				pContinueBlock->wr_ptr(iSize);
				if(i==0) 
					pFirstBlock = pContinueBlock;
				else
					pPrevBlock->cont(pContinueBlock);
				pPrevBlock = pContinueBlock;
			}
			pSendBufferBlock->release();
			pSendBufferBlock = pFirstBlock;
		}*/
		m_pSender->asynch_send(addr, pSendBufferBlock);
	}

	return 0;
}

int SenderThread::InitSender( const ACE_TCHAR *host, u_short port )
{
	return m_pSender->Open(host, port);
}

int SenderThread::InitSender()
{
	return m_pSender->Open();
}

int SenderThread::RestSender( const ACE_TCHAR *host, u_short port )
{
	m_pSender->clean();
	return m_pSender->Open(host, port);
}
