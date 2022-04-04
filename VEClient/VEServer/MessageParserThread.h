#pragma once
#include "MessageParser.h"

class LoginNowImp;

class MessageParserThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	MessageParserThread();
	MessageParserThread(size_t ParserBufferSize, size_t ReplyBufferSize);
	virtual ~MessageParserThread(void);

	virtual int svc( void );
private:
	MessageParser m_Parser;
	//ACE_Message_Block* m_pReplyMessage;
	//ACE_Message_Block* m_pParserMessage;
	size_t m_ParserSize;
	size_t m_ReplySize;
};

