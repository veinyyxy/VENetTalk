#pragma once
#include <map>
#include "FileBlockMirror.h"
#include "MessageProtocol.h"

class Sender;
class ClientContext;

class SendFileThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		MB_USER_DISCRIBE = ACE_Message_Block::MB_USER + 10,
		MB_USER_BLOCK_REPLY = ACE_Message_Block::MB_USER + 11
	};
	typedef struct ____file_describe_____
	{
		int iState;
		int iReplyState;
		send_file_reqest sendReqest;
		char address[100];
		FileBlockMirror m_FileMirror;
	} SEND_FILE_DISCRIBE, *P_SEND_FILE_DISCRIBE;
	typedef struct ____file_block_addition_____
	{
		int iState;
		send_file_block_addition addition;
	} SEND_BLOCK_ADDITION, *P_SEND_BLOCK_ADDITION;
	SendFileThread(ACE_Task<ACE_MT_SYNCH>* pSFT = 0, ClientContext* pCC = 0);
	virtual ~SendFileThread(void);
	inline void SetSendThread(ACE_Task<ACE_MT_SYNCH>* pAT){m_pSendThread = pAT;}
	inline void SetClientContext(ClientContext* pCC){m_ClientContext = pCC;}
	virtual int svc( void );
	void SendFileBlock(const std::string& strFileName, int iBlockID, int iBlockSize
		, int iBlockCount, int iFileSize, const std::string& strAddr
		, const std::string& strUUID);
	void Init();

private:
	std::map<std::string, SEND_FILE_DISCRIBE> m_SendPool;
	ACE_Task<ACE_MT_SYNCH>* m_pSendThread;
	Sender* m_pSender;
	ClientContext* m_ClientContext;
};

