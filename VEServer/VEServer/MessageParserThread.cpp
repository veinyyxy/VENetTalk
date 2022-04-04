#include "StdAfx.h"
#include "LoginNewImp.h"
#include "MessageParser.h"
#include "MessageParserThread.h"
#include "SenderThread.h"
#include "ServerContext.h"

extern ServerContext* g_pServerContext;

MessageParserThread::MessageParserThread(void) : m_ParserSize(ACE_MESSAGE_BLOCK_SIZE), m_ReplySize(ACE_MESSAGE_BLOCK_SIZE)
{
	g_pServerContext->SetParserThread(this);

	//m_pReplyMessage = new ACE_Message_Block(m_ParserSize);
	//m_pParserMessage = new ACE_Message_Block(m_ReplySize);
	m_Parser.SetLoginCallback(new LoginNowImp);
}

MessageParserThread::MessageParserThread( size_t ParserBufferSize, size_t ReplyBufferSize )
	: m_ParserSize(ParserBufferSize)
	, m_ReplySize(ReplyBufferSize)
{
	g_pServerContext->SetParserThread(this);
	//m_pReplyMessage = new ACE_Message_Block(m_ParserSize);
	//m_pParserMessage = new ACE_Message_Block(m_ReplySize);
	m_Parser.SetLoginCallback(new LoginNowImp);
}

MessageParserThread::~MessageParserThread(void)
{
}

int MessageParserThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* pParserMessage = 0;
		this->getq(pParserMessage);
		ACE_Message_Block* pReplyMessage = new ACE_Message_Block(m_ReplySize);
		memset(pReplyMessage->base(), 0, pReplyMessage->size());

		m_Parser.ParserMessageBlock(pParserMessage, pReplyMessage);

		SenderThread* pSenderThread = g_pServerContext->GetSenderThread();
		pSenderThread->putq(pReplyMessage);
		
	}
	return 0;
}

