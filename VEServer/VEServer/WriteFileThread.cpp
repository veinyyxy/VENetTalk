#include "StdAfx.h"
#include "WriteFileThread.h"
#include "MessageProtocol.h"
#include "MessagePacker.h"
#include "ServerContext.h"
#include "UserInformation.h"

#include <fstream>
#include <string>

extern ServerContext* g_pServerContext;

WriteFileThread::WriteFileThread(ACE_Task<ACE_MT_SYNCH>* pSFT, ServerContext* pCC)
	: m_pSendThread(pSFT), m_ServerContext(pCC)
{
	m_ServerContext = g_pServerContext;
	m_pSendThread = (ACE_Task<ACE_MT_SYNCH>*)(m_ServerContext->GetSenderThread());
}

WriteFileThread::~WriteFileThread(void)
{
}

int WriteFileThread::svc( void )
{
	while(1)
	{
		ACE_Message_Block* pBlock = 0;
		//ACE_Time_Value times(0, 3000);

		getq(pBlock/*, &times*/);

		if(pBlock)
		{
			if(pBlock->msg_type() == MB_USER_DISCRIBE)
			{
				//文件描述数据
				P_RECEIVE_FILE_DISCRIBE pReceiveBlock = (
					P_RECEIVE_FILE_DISCRIBE)(pBlock->rd_ptr());

				std::string strUUID = pReceiveBlock->sendRequest.uuid;

				std::map<std::string, RECEIVE_FILE_DISCRIBE>::iterator iFinded 
					= m_ReceivePool.find(strUUID);

				if(iFinded == m_ReceivePool.end())
				{
					if(pReceiveBlock->iState == 1)//同意，并接收新文件
					{
						m_ReceivePool[strUUID] = *pReceiveBlock;

						int iBlockNum = pReceiveBlock->sendRequest.block_count;
						//创建块表
						m_ReceivePool.at(strUUID).m_FileMirror.Create(iBlockNum);

						/////////////////////////////////写空文件////////////////////////////////////
						std::string strBlankFile = pReceiveBlock->savePath;
						int iBlockSize = pReceiveBlock->sendRequest.file_block_size;
						int iFileSize = pReceiveBlock->sendRequest.file_size;
						WritBlankFile(strBlankFile, iFileSize, iBlockSize, iBlockNum);
						//////////////////////////////////写块描述文件//////////////////////////////
						//iFinded->second.m_FileMirror.SetBlockState(iBlockID, 1);
						std::string strBlockFile;
						strBlockFile = m_ReceivePool.at(strUUID).savePath;
						strBlockFile += ".wbd";
						m_ReceivePool.at(strUUID).m_FileMirror.Save(strBlockFile, strUUID);
						//////////////////////////////////////////////////////////////////////////
					}
					else if(pReceiveBlock->iState == 2)//重传文件
					{
						if(pReceiveBlock->sendRequest.state == 1)
						{
							m_ReceivePool[strUUID] = *pReceiveBlock;

							int iBlockNum = pReceiveBlock->sendRequest.block_count;

							std::string strBlockFile;
							strBlockFile = m_ReceivePool.at(strUUID).savePath;
							strBlockFile += ".wbd";
							m_ReceivePool.at(strUUID).m_FileMirror.Read(strBlockFile);

							int iBreakPoint = -1;
							SendWriteFileResult(m_ServerContext, m_ReceivePool.at(strUUID)
								, iBreakPoint - 1, strUUID);
						}
					}
					else if(pReceiveBlock->iState == 4)//续传文件
					{
						m_ReceivePool[strUUID] = *pReceiveBlock;

						int iBlockNum = pReceiveBlock->sendRequest.block_count;

						std::string strBlockFile;
						strBlockFile = m_ReceivePool.at(strUUID).savePath;
						strBlockFile += ".wbd";
						m_ReceivePool.at(strUUID).m_FileMirror.Read(strBlockFile);

						int iBreakPoint = m_ReceivePool.at(strUUID).m_FileMirror.GetBreakPoint();
						if(iBreakPoint != -1)
						{
							SendWriteFileResult(m_ServerContext, m_ReceivePool.at(strUUID)
								, iBreakPoint - 1, strUUID);

						}
						else
						{
							SendWriteFileResult(m_ServerContext, m_ReceivePool.at(strUUID)
								, iBlockNum, strUUID);
							m_ReceivePool.erase(strUUID);
							remove(strBlockFile.c_str());
						}
					}
					else
					{

					}
				}
				else //有未完成的传输
				{

				}
				
			}
			else
			{
				//写文件块
				message_header* header = (message_header*)(pBlock->base());

				pBlock->rd_ptr(sizeof(message_header));
				send_file_block* pFileBlock = (send_file_block*)(pBlock->rd_ptr());
				
				std::string strUUID = pFileBlock->uuid;
				int iBlockID = pFileBlock->block_id;
				int iBlockSize = pFileBlock->file_block_size;

				std::map<std::string, RECEIVE_FILE_DISCRIBE>::iterator iFinded
					= m_ReceivePool.find(strUUID);


				if(iFinded != m_ReceivePool.end())
				{
					ofstream outFile(iFinded->second.savePath
						, std::ios::out|std::ios::in|std::ios::binary);

					pBlock->rd_ptr(sizeof(send_file_block));
					char* pBuffer = pBlock->rd_ptr();

					outFile.seekp(iBlockID * iFinded->second.sendRequest.file_block_size);
					outFile.write(pBuffer, iBlockSize);
#if 0
					/*if(iBlockID == iFinded->second.sendRequest.block_count - 1)
					{
						int iWrLen = 0;

						int iRemainder = iFinded->second.sendRequest.file_size 
							% iFinded->second.sendRequest.file_block_size;
						if(iRemainder == 0)
							iWrLen = iFinded->second.sendRequest.file_block_size;
						else
							iWrLen = iRemainder;
						
						outFile.seekp(iBlockID * iFinded->second.sendRequest.file_block_size);
						outFile.write(pBuffer, iWrLen);
					}
					else
					{
						outFile.seekp(iBlockID * iFinded->second.sendRequest.file_block_size);
						outFile.write(pBuffer
							, iFinded->second.sendRequest.file_block_size);
					}*/
#endif
					outFile.close();
					//修改块描述文件
					iFinded->second.m_FileMirror.SetBlockState(iBlockID, 1);
					std::string strBlockFile;
					strBlockFile += iFinded->second.savePath;
					strBlockFile += ".wbd";
					iFinded->second.m_FileMirror.Save(strBlockFile, strUUID);
					//发送写文件结果
					SendWriteFileResult(m_ServerContext, iFinded->second, iBlockID, strUUID);

					if(iBlockID ==  iFinded->second.sendRequest.block_count - 1)
					{
						m_ReceivePool.erase(strUUID);
						remove(strBlockFile.c_str());
					}
				}
			}
		}
		pBlock->release();
	}
}

void WriteFileThread::SendWriteFileResult(ServerContext* pCC
	, const RECEIVE_FILE_DISCRIBE& RFD, int iBlockID, const std::string strUUID)
{
	ACE_Message_Block* pSaveBlockResult = new ACE_Message_Block;
	std::string strServerAddr;// = m_ClientContext->GetClientAddress();
	m_ServerContext->GetServerAddress(&strServerAddr);
	std::string myName = "VEServer";// = m_ClientContext->GetUserName();
	std::string dstName = RFD.sendRequest.username;
	std::string fileName = RFD.sendRequest.src_file_path;


	MessagePacker::PackFileBlockAddition(strServerAddr, 0, myName
		, dstName, fileName, iBlockID, 1, 1, strUUID, pSaveBlockResult);

	std::string strDstUserAddr;
	UserInformation* pUI = m_ServerContext->GetUserInfomation();
	pUI->GetUserAddress(dstName, strDstUserAddr);

	message_header* pMH = (message_header*)(pSaveBlockResult->base());
	strcpy_s(pMH->_address, sizeof(pMH->_address), strDstUserAddr.c_str());

	m_pSendThread->putq(pSaveBlockResult);

	int iBlockCount = RFD.sendRequest.block_count;
}

void WriteFileThread::WritBlankFile( const std::string strFileName, int iFileSize, int iBlockSize, int iBlockNum )
{
	ofstream outFile(strFileName
		, std::ios::out/*|std::ios::in*/|std::ios::binary);


	char* pBuffer = new char[iBlockSize];
	memset(pBuffer, 0, iBlockSize);

	for(int i = 0; i < iBlockNum; i++)
	{
		if(iBlockNum - 1 == i)
		{
			int iWrLen = iFileSize % iBlockSize;

			outFile.write(pBuffer, iWrLen);
		}
		else
		{
			outFile.write(pBuffer, iBlockSize);
		}

	}

	outFile.close();
}
