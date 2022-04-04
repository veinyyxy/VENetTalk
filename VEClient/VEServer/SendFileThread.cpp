#include "StdAfx.h"
#include "SendFileThread.h"
#include "MessageProtocol.h"
#include "MessagePacker.h"
#include "Sender.h"

#include <fstream>
#include <string>
extern int GetBlockCount(int iFileSize, int iBlockSize);

SendFileThread::SendFileThread(ACE_Task<ACE_MT_SYNCH>* pSFT, ServerContext* pCC)
	: m_pSendThread(pSFT), m_ClientContext(pCC)
{
	m_pSender = new Sender;
}


SendFileThread::~SendFileThread(void)
{
}

int SendFileThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* pBlock = 0;
		getq(pBlock);

		/*
		message_header* pMH = (message_header*)(pBlock->rd_ptr());
		std::string strSrcName = pMH->_username;

		pBlock->rd_ptr(sizeof(message_header));

		send_file_request_reply* pSendFileRequest = (send_file_request_reply*)(pBlock->rd_ptr());
		std::string strFileNamePath = pSendFileRequest->dst_file_path;
		std::string strUserAddr = pSendFileRequest->useraddr;
		std::string strUserName = pSendFileRequest->username;
		*/
#if 1
		if(pBlock)
		{
			if(pBlock->msg_type() == MB_USER_DISCRIBE)
			{
				P_SEND_FILE_DISCRIBE pSFD = (P_SEND_FILE_DISCRIBE)(pBlock->rd_ptr());
				std::string strUUID = pSFD->sendReqest.uuid;
				if(pSFD->iState == -1)//我检测到的状态为：文件没有传输记录，新建传输上下文
				{
					m_SendPool[strUUID] = *pSFD;
				}
				else if(pSFD->iState == 0)//对方回复我的状态：拒绝接收文件
				{
					m_SendPool.erase(strUUID);
					continue;
				}
				else if(pSFD->iState == 1)//对方回复我的状态：同意，并发送文件的第0块
				{
					std::map<std::string, SEND_FILE_DISCRIBE>::iterator 
						iFinded = m_SendPool.find(strUUID);

					memcpy_s(iFinded->second.address, 37, pSFD->address, 37);
					std::string strFile = iFinded->second.sendReqest.src_file_path;
					size_t BlockCount = iFinded->second.sendReqest.block_count;
					size_t BlockSize  = iFinded->second.sendReqest.file_block_size;
					size_t FileSize = iFinded->second.sendReqest.file_size;

					pSFD->m_FileMirror.Create(BlockCount);

					std::string strBlockFile = strFile + ".rbd";
					//对方接收了请求，发送文件的第0块
					std::string strAddr = iFinded->second.address;
					SendFileBlock(strFile, 0, BlockSize, BlockCount, FileSize, strAddr, strUUID);
					pSFD->m_FileMirror.SetBlockState(0, 1);

					pSFD->m_FileMirror.Save(strBlockFile, strUUID);
#if 0
					ifstream readFile;
					readFile.open(strFile, std::ios::in | ios::binary);

					/*readFile.seekg(0, std::ios_base::end);
					std::streampos fileSize = readFile.tellg();
					readFile.seekg(std::ios_base::beg);*/

					char* pBlock = new char[BlockSize];
					
					for(size_t i = 0; i < BlockCount; i++)
					{
						memset(pBlock, 0, BlockSize);
						readFile.read(pBlock, BlockSize);
						ACE_Message_Block* pMessageBlock = new ACE_Message_Block;
						memset(pMessageBlock->base(), 0, pMessageBlock->size());
						int iFileBlockSize = 0;
						if(i == BlockCount - 1)
						{
							iFileBlockSize = iFinded->second.sendReqest.file_size % BlockSize;
						}
						else
						{
							iFileBlockSize = BlockSize;
						}

						std::string strMyAddr, strMyName, dstName, strFileName;
						MessagePacker::PackSendFileBlock(strMyAddr, 0, strMyName
							, dstName, strFileName, i, iFileBlockSize
							, pBlock, strUUID, pMessageBlock);

						std::string strAddr = iFinded->second.address;
						size_t sP = strAddr.find(':');

						std::string strAddr1 = strAddr.substr(0, sP);
						std::string portNum = strAddr.substr(sP + 1, strAddr.size() - sP);
						unsigned short shPort = atoi(portNum.c_str());
						ACE_INET_Addr addr(shPort, strAddr1.c_str());
						Sleep(3000);
						m_pSender->asynch_send(pMessageBlock, addr);
					}
#endif
				}
				else if(pSFD->iState == 2)//对方回复我的状态：中断的文件，当时要重新传送
				{

				}
				else if(pSFD->iState == 3)//我检测到的状态为：文件存在，可续传文件
				{
					std::map<std::string, SEND_FILE_DISCRIBE>::iterator finded = m_SendPool.find(strUUID);
					if(finded == m_SendPool.end())
					{
						m_SendPool[strUUID] = *pSFD;
					}
					else
					{

					}
				}
				else if(pSFD->iState == 4)//对方回复我的状态：继续传送中断的文件
				{
					std::map<std::string, SEND_FILE_DISCRIBE>::iterator 
						iFinded = m_SendPool.find(strUUID);
					if(iFinded != m_SendPool.end())
					{
						memcpy_s(iFinded->second.address, 37, pSFD->address, 37);
						std::string strFileName = iFinded->second.sendReqest.src_file_path;
						std::string strBlockFile = strFileName + ".rbd";
						//由于FileBlockMirror类无法通过内存拷贝拷贝数据，所以再读一次
						iFinded->second.m_FileMirror.Read(strBlockFile);
					}
				}
				
			}
			else
			{
				SEND_BLOCK_ADDITION* pAdd = (SEND_BLOCK_ADDITION*)(pBlock->base());
				if(pAdd->addition.reciver_state)
				{
					std::string strUUID = pAdd->addition.uuid;
					std::map<std::string, SEND_FILE_DISCRIBE>::iterator 
						iFinded = m_SendPool.find(strUUID);

					if(iFinded != m_SendPool.end())
					{
						int iFileSize = iFinded->second.sendReqest.file_size;
						int iBlockSize = iFinded->second.sendReqest.file_block_size;
						int iBlockCount = iFinded->second.sendReqest.block_count;
						std::string strFileName = iFinded->second.sendReqest.src_file_path;
						std::string strAddr = iFinded->second.address;

						std::string strBlockFile = strFileName + ".rbd";

						if(pAdd->addition.block_id < iBlockCount)
						{
							if(pAdd->addition.block_id == iBlockCount - 1)
							{
								m_SendPool.erase(strUUID);
								remove(strBlockFile.c_str());
								continue;
							}
							int iBlockID = pAdd->addition.block_id + 1;
							SendFileBlock(strFileName, iBlockID, iBlockSize, iBlockCount, iFileSize, strAddr, strUUID);
							iFinded->second.m_FileMirror.SetBlockState(pAdd->addition.block_id, 1);
							//MessageCallback* pMC = m_ClientContext->GetMessageCallback();
							//pMC->SendFileProgress(strFileName, iBlockCount, iBlockID);
						}
						
						iFinded->second.m_FileMirror.Save(strBlockFile, strUUID);

						/*char path_buffer[_MAX_PATH] = {0};
						char drive[_MAX_DRIVE] = {0};
						char dir[_MAX_DIR] = {0};
						char fname[_MAX_FNAME] = {0};
						char ext[_MAX_EXT] = {0};
						_splitpath(strFileName.c_str(), drive, dir, fname, ext);

						std::string strBlockFile;
						strBlockFile += 
						strBlockFile += fname;
						strBlockFile += ext;
						strBlockFile += iFinded->second.sendReqest.username;
						strBlockFile += "rbd";*/

						
					}
					
				}
				
			}
		}

		pBlock->release();
#endif
	}
}

void SendFileThread::SendFileBlock( const std::string& strFileName, int iBlockID
	, int iBlockSize, int iBlockCount, int iFileSize, const std::string& strAddr, const std::string& strUUID )
{
	ifstream readFile;
	readFile.open(strFileName, std::ios::in | ios::binary);

	char* pReadBuffer = new char[iBlockSize];

	int iReadedSize = 0;

	int iRemainSize = iFileSize % iBlockSize;

	if(iBlockID == iBlockCount - 1)
	{
		if(iRemainSize != 0)
		{
			iReadedSize = iRemainSize;
		}
		else
		{
			iReadedSize = iBlockSize;
		}
	}
	else
	{
		iReadedSize = iBlockSize;
	}

	memset(pReadBuffer, 0, iBlockSize);
	readFile.seekg(iBlockSize * iBlockID);
	readFile.read(pReadBuffer, iReadedSize);
	ACE_Message_Block* pMessageBlock = new ACE_Message_Block();
	memset(pMessageBlock->base(), 0, pMessageBlock->size());
	
	std::string strMyAddr, strMyName, dstName;
	MessagePacker::PackSendFileBlock(strMyAddr, 0, strMyName
		, dstName, strFileName, iBlockID, iReadedSize
		, pReadBuffer, (std::string)strUUID, pMessageBlock, 1);

	size_t sP = strAddr.find(':');

	std::string strAddr1 = strAddr.substr(0, sP);
	std::string portNum = strAddr.substr(sP + 1, strAddr.size() - sP);
	unsigned short shPort = atoi(portNum.c_str());
	ACE_INET_Addr addr(shPort, strAddr1.c_str());
	m_pSender->asynch_send(addr, pMessageBlock);
}

void SendFileThread::Init()
{
	m_pSender->Open(0, 0);
}
