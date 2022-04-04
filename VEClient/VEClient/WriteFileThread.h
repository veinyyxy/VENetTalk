#pragma once
#include <map>
#include "FileBlockMirror.h"
#include "MessageProtocol.h"

class ClientContext;

class WriteFileThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		MB_USER_DISCRIBE = ACE_Message_Block::MB_USER + 11
	}; 
	typedef struct ____file_describe_____
	{
		int iState;
		char savePath[1024];
		send_file_reqest sendRequest;
		FileBlockMirror m_FileMirror;
	} RECEIVE_FILE_DISCRIBE, *P_RECEIVE_FILE_DISCRIBE;
	WriteFileThread(ACE_Task<ACE_MT_SYNCH>* pSFT, ClientContext* pCC);
	~WriteFileThread(void);
	//void WriteFileBlock(const std::string& fileName, int iSize, const char* buffer);
	virtual int svc( void );
	inline void SetSendThread(ACE_Task<ACE_MT_SYNCH>* pAT){m_pSendThread = pAT;}
	inline void SetClientContext(ClientContext* pCC){m_ClientContext = pCC;}
	void SendWriteFileResult(ClientContext* pCC 
		, const RECEIVE_FILE_DISCRIBE& RFD, int iBlockID, const std::string strUUID, char cType = 0x00);
	void WritBlankFile( const std::string strFileName, int iFileSize, int iBlockSize, int iBlockNum );
private:
	std::map<std::string, RECEIVE_FILE_DISCRIBE> m_ReceivePool;
	ACE_Task<ACE_MT_SYNCH>* m_pSendThread;
	ClientContext* m_ClientContext;
};

