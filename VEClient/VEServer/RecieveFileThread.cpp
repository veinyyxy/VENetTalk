#include "StdAfx.h"
#include "RecieveFileThread.h"
#include "Receiver.h"
#include "WriteFileThread.h"
#include "MessageProtocol.h"
RecieveFileThread::RecieveFileThread(ACE_INET_Addr* pAddr)
{
	m_pReceiver = new Receiver;
	m_pReceiver->open_addr(*pAddr);
	char addrBuffer[100] = {0};
	pAddr->addr_to_string(addrBuffer, 100);
	m_strAddr = addrBuffer;
	m_pWriteFileThread = new WriteFileThread(0, 0);
	m_pWriteFileThread->activate();
}


RecieveFileThread::~RecieveFileThread(void)
{
}

int RecieveFileThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* pFileBlock = new ACE_Message_Block(1024 * 54);
		int iRes = m_pReceiver->asynch_receive(pFileBlock);

		send_file_block* pBlock = (send_file_block*)(pFileBlock->base() + sizeof(message_header));

		std::string strUUID = pBlock->uuid;
		int iBlockID = pBlock->block_id;
		int iBlockSize = pBlock->file_block_size;

		if(pFileBlock)
		{
			m_pWriteFileThread->putq(pFileBlock);
			/*message_header* pHeader = (message_header*)(pFileBlock->base());
			int iType = pHeader->_type;*/
		}
	}
}
