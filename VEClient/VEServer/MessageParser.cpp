#include "StdAfx.h"
#include "MessageProtocol.h"
#include "MessageParser.h"
#include "ServerContext.h"
#include "UserInformation.h"
#include "MessagePacker.h"
#include "SenderThread.h"
#include "OffLineMessage.h"
#include "TransmitFileTable.h"
#include "OrganizationStructureInfo.h"
#include "SendFileThread.h"
#include "WriteFileThread.h"
#include "RecieveFileThread.h"
#include "OffLineFile.h"
#include "MessageServer.h"
#include "OfflineFileServer.h"

#define COPY_STRING_TO_ARRAY(a, b) strcpy_s(a\
	, sizeof(a), b.c_str())

#define COPY_DATA_TO_BLOCK(a, b, c) memcpy_s(a->wr_ptr(), a->size()\
	, b, sizeof(c));

extern ServerContext* g_pServerContext;

MessageParser::MessageParser() : m_LoginCallback(0)
{

}

void MessageParser::ParserLogin( ACE_Message_Block* pMB, std::string& name, std::string& pass )
{
	if(!pMB) return;
	message_header* pMH = (message_header*)(pMB->rd_ptr());

	pMB->rd_ptr(sizeof(message_header));
	login* pLoginBlock = (login*)(pMB->rd_ptr());
	
	name = pLoginBlock->_username;
	pass = pLoginBlock->_password;
}

void MessageParser::ParserMessageBlock( ACE_Message_Block* pMB, ACE_Message_Block* pReplyBlock )
{
	if(!pMB) return;
	message_header* pHeader = (message_header*)(pMB->rd_ptr());
	

	switch(pHeader->_type)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		OkToLogin(pMB, pReplyBlock);
		break;
	case 4:
		break;
	case 5:
		SendMessage(pMB, pReplyBlock);
		break;
	case 7:
		Logout(pMB, pReplyBlock);
		break;
	case 10:
		HeartheatHandle(pMB);
		break;
	case 11:
		PostUserState(pMB, pReplyBlock);
		break;
	case 13:
		SetUserState(pMB, pReplyBlock);
		break;
	case 15:
		RequestOrganization(pMB, pReplyBlock);
		break;
	case 17:
		RequestCustomGroup(pMB, pReplyBlock);
		break;
	case 19:
		GetCustomGroups(pMB, pReplyBlock);
		break;
	case 21:
		ModifyGroup(pMB, pReplyBlock);
		break;
	case 23:
		ModifyGroupNodes(pMB, pReplyBlock);
		break;
	case 25:
		QueryAccountFromNodeId(pMB, pReplyBlock);
		break;
	case 27:
		GetUserInfo(pMB, pReplyBlock);
		break;
	case 6000:
		SendFileRequest(pMB, pReplyBlock);
		break;
	case 6030:
		SendFileRequestReply(pMB, pReplyBlock);
		break;
	case 6020:
		SendFileBlockAddition(pMB, pReplyBlock);
		break;
	default:
		break;
	}
}

void MessageParser::OkToLogin(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock)
{
	std::string strName, strPass;
	ParserLogin(pAMB, strName, strPass);

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();

	std::string strOldAddress;
	if(m_LoginCallback && pUserInfo)
	{
		if(m_LoginCallback->Login(strName, strPass, pReplyBlock) > 0)
		{
			//发送消息的地址和端口
			//ACE_Message_Block* pAddressCount = new ACE_Message_Block(100);
			std::string strAddr;
			message_header* pMHSrc = (message_header*)(pAMB->base());
			strAddr = pMHSrc->_address;

			CopyAddressAndPort(pReplyBlock, pAMB);
			//memset(pAddressCount->wr_ptr(), 0, pAddressCount->size());
			//memcpy_s(pAddressCount->wr_ptr(), pAddressCount->size(), pMH, sizeof(pMH->_address));
			//pAddressCount->wr_ptr(sizeof(pMH->_address));

			pUserInfo->SetUserAddress(strName, strAddr);
			pUserInfo->SetUserState(strName, 1);

			//设置地址和端口为队头
			//pReplyBlock->next(pAddressCount);

			/////////////////////////////给此用户发送离线消息////////////////////////////
			OffLineMessage* pOLM = g_pServerContext->GetOffLineMessage();

			OffLineMessage::OFFLINE_MESSAGE_VECTOR offLineMessage;
			pOLM->GetMessage(0, &strName, &offLineMessage);

			for(size_t i = 0; i < offLineMessage.size(); i++)
			{
				send_message sendMessage = offLineMessage[i];
				std::string strUserAddr;
				
				ACE_Message_Block* pSendBlock = new ACE_Message_Block;

				MessagePacker::SendMessage(strUserAddr, sendMessage.username
					, 0, sendMessage.pBuffer, sizeof(sendMessage.pBuffer), pSendBlock);

				
				CopyAddressAndPort(pSendBlock, pAMB);

				g_pServerContext->GetSenderThread()->putq(pSendBlock);

			}

			OfflineFileServer* pOFS = g_pServerContext->GetOfflineFileServer();
			pOFS->SendOfflineFileToUser(strName);
			//////////////////////////////////////////////////////////////////////////
		}
	}
}

void MessageParser::SendMessage( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{

	if(!pAMB) return;

	//ACE_Message_Block* pRetransmissionBlock = new ACE_Message_Block(1024);

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	
	///////克隆要转发的消息//////
	ACE_Message_Block* pBlock = 0;
	pBlock = pAMB->clone();

	send_message* pSM = (send_message*)(pBlock->base() + sizeof(message_header));
	strcpy_s(pSM->username, sizeof pSM->username, pMH->_username);

	////////////////////
	pAMB->rd_ptr(sizeof(message_header));

	send_message* SendedMessage = (send_message*)(pAMB->rd_ptr());
	std::string strName = SendedMessage->username;

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
	std::string strAddr;
	pUserInfo->GetUserAddress(strName, strAddr);

	//如果可以取出用户的地址，说明用户登录了。如果不能则存储消息，等用户上线后发送。
	if(strAddr.size() > 0)
	{
		message_header* pDstHeader = (message_header*)(pBlock->base());

		strcpy_s(pDstHeader->_address, sizeof(pDstHeader->_address), strAddr.c_str());

		g_pServerContext->GetSenderThread()->putq(pBlock);

		MessagePacker::PackSendMessageReply(1, pReplyBlock);

		CopyAddressAndPort(pReplyBlock, pAMB);
	}
	else
	{
		OffLineMessage* pOLM = g_pServerContext->GetOffLineMessage();

		message_header* pMH = (message_header*)(pBlock->rd_ptr());
		
		send_message saveMessage;

		strcpy_s(saveMessage.username, sizeof(saveMessage.username), pSM->username);
		saveMessage.type = 1;
		saveMessage.length = pSM->length;
		strcpy_s(saveMessage.pBuffer, sizeof(saveMessage.pBuffer), pSM->pBuffer);

		pOLM->SaveMessage(0, strName, &saveMessage);

		CopyAddressAndPort(pReplyBlock, pAMB);
		MessagePacker::PackSendMessageReply(0, pReplyBlock);

		pBlock->release();
	}
	/*int iType = SendedMessage->type;
	int iLen = SendedMessage->length;
	std::string strMessage;
	if(iType == 0)
	{
	strMessage = SendedMessage->pBuffer;
	}*/
}

void MessageParser::ParseMessage( ACE_Message_Block* pAMB, int iUserID, const std::string& strName, char* pBuffer, int len )
{
	if(!pAMB) return;
	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	
}

void MessageParser::Logout(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock)
{
	if(!pAMB) return;

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	//ACE_Message_Block* pAddition = new ACE_Message_Block(100);
	//memset(pAddition->base(), 0, pAddition->size());

	//strcpy_s(pAddition->wr_ptr(), sizeof(pMH->_address), pMH->_address);
	//pAddition->wr_ptr(sizeof(pMH->_address));

	pAMB->rd_ptr(sizeof(message_header));

	logout* pout = (logout*)(pAMB->rd_ptr());

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
	
	pUserInfo->SetUserAddress(pout->username, "");
	pUserInfo->SetUserHeartbeat(pout->username, 0);
	pUserInfo->SetUserState(pout->username, 0);

	/*ACE_Message_Block* pAddrBlock = pAMB->cont();
	char* chAddr = pAddrBlock->rd_ptr();
	std::string strAddr = chAddr;*/

	//pReplyBlock->prev(pAddition);

	MessagePacker::PackLogoutReply(1, pReplyBlock);

	CopyAddressAndPort(pReplyBlock, pAMB);
}

void MessageParser::HeartheatHandle( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock /*= 0*/ )
{
	if(!pAMB && g_pServerContext) return;

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));
	heartbeat* pBeat = (heartbeat*)(pAMB->rd_ptr());
	
	if(strName.length() > 0)
	{
		UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();

		if(pUserInfo)
		{
			pUserInfo->SetHeartState(strName, 2);
		}
	}
}

void MessageParser::PostUserState( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strName = pMH->_username;

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();

	std::string strAddr;
	pUserInfo->GetUserAddress(strName, strAddr);
#if 1
	if(strAddr.length() > 6)
	{
		pAMB->rd_ptr(sizeof(message_header));
		user_state_request* pusr = (user_state_request*)(pAMB->rd_ptr());

		std::string requestName = pusr->_requestname;
		if(pusr->_requestid == -1 && requestName.length() <= 0)
		{
			UserInformation::USER_LIST ul;
			pUserInfo->GetUserList(&ul);

			user_state* pUserStateBuffer = new user_state[ul.size()];
			memset(pUserStateBuffer, 0, ul.size() * sizeof(user_state));

			size_t ii = 0;
			UserInformation::USER_LIST::iterator i = ul.begin();
			for(i, ii; ii < ul.size(), i != ul.end(); i++, ii++)
			{
				(pUserStateBuffer + ii)->state = i->second.user_state;
				(pUserStateBuffer + ii)->user_id = i->second.id;
				memcpy_s((pUserStateBuffer + ii)->_username, 255, i->second.name.c_str(), i->second.name.length());
			}
			
			//ACE_Message_Block* pAddition = new ACE_Message_Block(100);
			//memset(pAddition->base(), 0, pAddition->size());

			//strcpy_s(pAddition->base(), pAddition->size(), strAddr.c_str());
			//pAddition->wr_ptr(strAddr.length());

			//pReplyBlock->prev(pAddition);

			MessagePacker::PackUserStateReply(ul.size(), pUserStateBuffer, pReplyBlock);

			CopyAddressAndPort(pReplyBlock, pAMB);

			delete[] pUserStateBuffer;
		}
		else
		{
			/*user_state us = {0};
			pUserInfo->GetUserState(requestName, us.state);
			us.user_id = 0;
			memcpy_s(&us._username, 255, requestName.c_str(), requestName.length());;
			MessagePacker::PackUserStateReply(1, &us, pReplyBlock);*/
		}
		
	}
	else
	{

	}
#endif
}

void MessageParser::CopyAddressAndPort( ACE_Message_Block* pDst, ACE_Message_Block* pSrc )
{
	message_header* pMHSrc = (message_header*)(pSrc->base());
	message_header* pMHDst = (message_header*)(pDst->base());

	strcpy_s(pMHDst->_address, sizeof(pMHDst->_address), pMHSrc->_address);
}

void MessageParser::CopyAddressAndPort(ACE_Message_Block* pDst
	, const std::string& strAddr, const std::string& strName)
{
	message_header* pMHDst = (message_header*)(pDst->base());

	strcpy_s(pMHDst->_address, sizeof(pMHDst->_address), strAddr.c_str());
	strcpy_s(pMHDst->_username, sizeof(pMHDst->_username), strName.c_str());
}

void MessageParser::SetUserState( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	message_header* pHeader = (message_header*)(pAMB->base());
	pAMB->rd_ptr(sizeof(message_header));
	
	std::string strAddr = pHeader->_address;
	std::string strName = pHeader->_username;
	set_user_state* pSUS = (set_user_state*)(pAMB->rd_ptr());

	std::string strname = pSUS->_username;
	int iState = pSUS->state;

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();

	//std::string strAddr;
	pUserInfo->SetUserState(strname, iState);

	MessagePacker::PackSetUserStateReply(iState, 1, pReplyBlock);
	
	CopyAddressAndPort(pReplyBlock, strAddr, strName);
	//CopyAddressAndPort(pAMB, pReplyBlock);
}

void MessageParser::SendFileRequest( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	ACE_Message_Block* pRetransmitBlock = pAMB->clone();

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;
	std::string strClientAddr = pMH->_address;

	pAMB->rd_ptr(sizeof(message_header));

	send_file_reqest* pSFR = (send_file_reqest*)(pAMB->rd_ptr());

	
	pSFR->block_count;
	std::string strFilePath = pSFR->src_file_path;
	std::string strUserToUser;
	std::string strDstName = pSFR->username;

	//strUserToUser += strSrcName;
	//strUserToUser += "-";
	//strUserToUser += strDstName;

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
	std::string strUserAddr;
	pUserInfo->GetUserAddress(pSFR->username, strUserAddr);

	//修改克隆头信息
	message_header* pCloneHeader = (message_header*)(pRetransmitBlock->base());
	memset(pCloneHeader->_address, 0, sizeof(pCloneHeader->_address));
	memset(pCloneHeader->_username, 0, sizeof(pCloneHeader->_username));
	pCloneHeader->_user_id = 0;

	strcpy_s(pCloneHeader->_address, sizeof(pCloneHeader->_address), strUserAddr.c_str());
	strcpy_s(pCloneHeader->_username, sizeof(pCloneHeader->_username), strDstName.c_str());

	//修改接收文件的用户名为---发送文件的用户名
	pSFR = (send_file_reqest*)(pRetransmitBlock->base() + sizeof(message_header));
	COPY_STRING_TO_ARRAY(pSFR->username, strSrcName);
	//////////////////////////////////////////////////////////////////////////
	//客户端要求发送离线文件 type == 0x1
	if(pSFR->type == 0x0)
	{
		g_pServerContext->GetSenderThread()->putq(pRetransmitBlock);
	}
	else
	{
		send_file_reqest* pSendFileRequest = (send_file_reqest*)(pAMB->rd_ptr());
		std::string strSrcFilePath = pSendFileRequest->src_file_path;
		std::string strUUID = pSendFileRequest->uuid;
		int iRequestState = pSendFileRequest->state;

		//获得目的地用户名，当用户上线时发送给用户。
		char path_buffer[_MAX_PATH] = {0};
		char drive[_MAX_DRIVE] = {0};
		char dir[_MAX_DIR] = {0};
		char fname[_MAX_FNAME] = {0};
		char ext[_MAX_EXT] = {0};
		_splitpath(pSFR->src_file_path, drive, dir, fname, ext);

		std::string strFileDir;
		pUserInfo->GetOffLineFileDir("", strFileDir);
		sprintf_s(path_buffer, _MAX_PATH, "%s\\%s%s", strFileDir.c_str(), fname, ext);

		std::string strDstName = pSendFileRequest->username;

		OffLineFile* pOLF = g_pServerContext->GetOffLineFile();
		OffLineFile::OFF_LINE_FILE_INFO olfi;
		olfi.strSrcName = strSrcName;
		olfi.strDstName = strDstName;
		olfi.strUUID = strUUID;
		olfi.strFileDir = path_buffer;
		pOLF->SaveFileInfo(strDstName, strUUID, olfi);
		//服务器此时为接收，记录一些信息。
		pSendFileRequest = pSFR;

		if(true)//回调逻辑被删除，用true代替
		{
			//ACE_Message_Block* pSendBlock = new ACE_Message_Block;
			std::string MyName = "VEServer";// = m_pClientContext->GetUserName();
			//std::string MyAddr = m_pClientContext->GetClientAddress();

			std::string MyReceiveAddr;// = m_pClientContext->GetRecievFileAddress();
			g_pServerContext->GetReseiveOfflineFileThread()->GetRecieveFileAddress(&MyReceiveAddr);
			//std::string strOutPath;
			//int iReuslt = m_pMessageCallBack->TransmitFileRequest(iRequestState, strSrcName, strSrcFilePath, strOutPath);

			WriteFileThread::RECEIVE_FILE_DISCRIBE rfd;
			strcpy_s(rfd.savePath, sizeof(rfd.savePath), path_buffer);
			rfd.iState = 1;
			memcpy_s(&(rfd.sendRequest), sizeof(send_file_reqest)
				, pSendFileRequest, sizeof(send_file_reqest));

			ACE_Message_Block* pControlBlock = new ACE_Message_Block(sizeof(
				WriteFileThread::RECEIVE_FILE_DISCRIBE));
			memcpy_s(pControlBlock->wr_ptr(), pControlBlock->size(), &rfd
				, sizeof(WriteFileThread::RECEIVE_FILE_DISCRIBE));
			pControlBlock->wr_ptr(sizeof(WriteFileThread::RECEIVE_FILE_DISCRIBE));

			pControlBlock->msg_type(WriteFileThread::MB_USER_DISCRIBE);
			RecieveFileThread* pRFT = g_pServerContext->GetReseiveOfflineFileThread();
			WriteFileThread* pWFT = pRFT->GetWriteThread();
			pWFT->putq(pControlBlock);

			MessagePacker::PackRequestSendFileReply(strClientAddr, 0, strDstName
				, MyName, MyReceiveAddr, strSrcFilePath, 1, strUUID
				, pReplyBlock);

			/*SenderThread* pST = g_pServerContext->GetSenderThread();
			pST->putq(pSendBlock);*/
		}

		pRetransmitBlock->release();
	}
#if 0
	TransmitFileTable* pTFT = g_pServerContext->GetTransmitFileTable();
	int iResult = pTFT->CreateFileBlockTable(strFilePath, strUserToUser);

	if(iResult == 0)
	{
		//有传输记录，发送续传指令
		g_pServerContext->GetSenderThread()->putq(pRetransmitBlock);
	}
	else if(iResult == 1)
	{
		//转发请求
		g_pServerContext->GetSenderThread()->putq(pRetransmitBlock);
	}
#endif
}

void MessageParser::SendFileRequestReply( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	ACE_Message_Block* pFileRequsetReplyBlock = pAMB->clone();

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strDstName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));

	send_file_request_reply* pSFRR = (send_file_request_reply*)(pAMB->rd_ptr());
	//std::string strDstName = pSFRR->username;
	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
	std::string strUserAddr;
	pUserInfo->GetUserAddress(strDstName, strUserAddr);


	//如果目标机同意接收，设置目标机地址
#if 0
	if(pSFRR->reply_state != 0)
	{
		UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
		std::string strUserAddr;
		pUserInfo->GetUserAddress(pSFRR->username, strUserAddr);

		send_file_request_reply* pSFRRClone 
			= (send_file_request_reply*)(pRetransmitBlock->base() 
			+ sizeof(message_header));

		COPY_STRING_TO_ARRAY(pSFRRClone->useraddr, strUserAddr);
	}
#endif

	message_header* pCloneHeader = (message_header*)(pFileRequsetReplyBlock->base());
	memset(pCloneHeader->_address, 0, sizeof(pCloneHeader->_address));
	memset(pCloneHeader->_username, 0, sizeof(pCloneHeader->_username));
	pCloneHeader->_user_id = 0;

	strcpy_s(pCloneHeader->_address, sizeof(pCloneHeader->_address), strUserAddr.c_str());
	strcpy_s(pCloneHeader->_username, sizeof(pCloneHeader->_username), strDstName.c_str());

	if(pSFRR->type == 0x00)
		g_pServerContext->GetSenderThread()->putq(pFileRequsetReplyBlock);
	else
	{
		ACE_Message_Block* pControlBlock = new ACE_Message_Block(sizeof(
			SendFileThread::SEND_FILE_DISCRIBE));

		SendFileThread::SEND_FILE_DISCRIBE SFD;

		memcpy_s(SFD.address, sizeof(SFD.address), pSFRR->useraddr
			, sizeof(pSFRR->useraddr));
		memcpy_s(SFD.sendReqest.uuid, sizeof(SFD.sendReqest.uuid), pSFRR->uuid, sizeof(pSFRR->uuid));
		SFD.iState = pSFRR->reply_state;

		memcpy_s(pControlBlock->wr_ptr(), pControlBlock->size(), &SFD
			, sizeof(SendFileThread::SEND_FILE_DISCRIBE));
		pControlBlock->wr_ptr(sizeof(SendFileThread::SEND_FILE_DISCRIBE));

		if(pSFRR->reply_state == 0)
		{
			//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
		}
		else if(pSFRR->reply_state == 1)
		{
			//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
		}
		else
		{
			//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
		}

		pControlBlock->msg_type(SendFileThread::MB_USER_DISCRIBE);
		g_pServerContext->GetSendOfflineFileThread()->putq(pControlBlock);

		pFileRequsetReplyBlock->release();
	}
}

void MessageParser::SendFileBlockAddition( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	ACE_Message_Block* pAdditionInfo = pAMB->clone();

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));

	send_file_block_addition* pSFBA = (send_file_block_addition*)(pAMB->rd_ptr());
	std::string strUserName = pSFBA->username;

	UserInformation* pUserInfo = g_pServerContext->GetUserInfomation();
	std::string strUserAddr;
	pUserInfo->GetUserAddress(strUserName, strUserAddr);

	message_header* pMH2 = (message_header*)(pAdditionInfo->base());
	memset(pMH2->_address, 0, sizeof(pMH2->_address));
	memset(pMH2->_username, 0, sizeof(pMH2->_username));
	COPY_STRING_TO_ARRAY(pMH2->_address, strUserAddr);
	COPY_STRING_TO_ARRAY(pMH2->_username, strUserName);

	if(pSFBA->type == 0x00)
		g_pServerContext->GetSenderThread()->putq(pAdditionInfo);
	else
	{
		SendFileThread::SEND_BLOCK_ADDITION addition;
		addition.iState = 0;

		memcpy_s(&(addition.addition), sizeof(send_file_block_addition)
			, pSFBA, sizeof(send_file_block_addition));


		ACE_Message_Block* addBlock = new ACE_Message_Block(
			sizeof(SendFileThread::SEND_BLOCK_ADDITION));

		memcpy_s(addBlock->base(), addBlock->size(), &addition
			, sizeof(SendFileThread::SEND_BLOCK_ADDITION));

		addBlock->msg_type(SendFileThread::MB_USER_BLOCK_REPLY);
		g_pServerContext->GetSendOfflineFileThread()->putq(addBlock);

		pAdditionInfo->release();
	}
}

void MessageParser::RequestOrganization( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	OrganizationStructureInfo* pOSI = g_pServerContext->GetOrganizationStructureInfo();
	std::vector<OrganizationStructureInfo::ORG_NODE> OrgData;
	pOSI->GetWholeOrganization(&OrgData);

	node_data* pNodeData = new node_data[OrgData.size()];
	memset(pNodeData, 0, sizeof(node_data) * OrgData.size());

	for(size_t i = 0; i < OrgData.size(); i++)
	{
		pNodeData[i].levelid = OrgData[i].iLevel;
		pNodeData[i].nodenum = OrgData[i].iNodeId;
		pNodeData[i].nodetype = OrgData[i].iNodeType;
		pNodeData[i].parentnum = OrgData[i].iParentID;
		memcpy_s(pNodeData[i].nodename, sizeof(pNodeData[i].nodename)
			, OrgData[i].strName.data(), OrgData[i].strName.length());
		
	}

	MessagePacker::PackOrgInformation(pNodeData, OrgData.size(), pReplyBlock);
	CopyAddressAndPort(pReplyBlock, pAMB);

	delete[] pNodeData;
}

void MessageParser::RequestCustomGroup( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	ACE_Message_Block* pRetransmitBlock = pAMB->clone();

	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	UserInformation::CUSTOM_GROUPS groups;
	int iResult = g_pServerContext->GetUserInfomation()->GetCustomGroup(strSrcName, groups);

	if(iResult == 0) return;

	UserInformation::CUSTOM_GROUPS::iterator be = groups.begin();

	//////////////////////////////////////////////////////////////////////////
	pReplyBlock->init(1024*64);
	memset(pReplyBlock->base(), 0, pReplyBlock->size());
	message_header header = {0};
	header._type = 18;

	COPY_DATA_TO_BLOCK(pReplyBlock, &header, message_header);
	pReplyBlock->wr_ptr(sizeof(message_header));
	
	int iGroupCount = groups.size();

	COPY_DATA_TO_BLOCK(pReplyBlock, &iGroupCount, int);
	pReplyBlock->wr_ptr(sizeof(int));

	for(int i = 0; i < iGroupCount; i++)
	{
		custom_group_info cgi = {0};
		cgi.groupid = i;
		COPY_STRING_TO_ARRAY(cgi.groupname, groups[i].name);//copy
		//pReplyBlock->wr_ptr(sizeof(cgi.groupname));

		cgi.nodecout = groups[i].nodes_id.size();
		cgi.type = groups[i].type;

		memcpy_s(pReplyBlock->wr_ptr(), pReplyBlock->size(), &cgi
			, sizeof(custom_group_info) - sizeof(cgi.nodeid));//copy
		pReplyBlock->wr_ptr(sizeof(custom_group_info) - sizeof(cgi.nodeid));
		
		for(int j = 0; j < cgi.nodecout; j++)
		{
			*(((int*)(pReplyBlock->wr_ptr())) +  j) = groups[i].nodes_id[j];
		}
		pReplyBlock->wr_ptr(sizeof(int) * cgi.nodecout);

	}
	//////////////////////////////////////////////////////////////////////////
	CopyAddressAndPort(pReplyBlock, pAMB);
}

void MessageParser::GetCustomGroups( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{

}

void MessageParser::ModifyGroup( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));

	modify_custom_group* pModifyCustomGroup = (modify_custom_group*)(pAMB->rd_ptr());
	char cOperate = pModifyCustomGroup->operate;

	switch(cOperate)
	{
	case 0:
		{
			UserInformation::CustomGroup cg;
			cg.groupid = pModifyCustomGroup->groupinfo.groupid;
			cg.type = pModifyCustomGroup->groupinfo.type;
			//cg.nodes_id = pModifyCustomGroup->groupinfo.groupid;
			cg.name = pModifyCustomGroup->groupinfo.groupname;
			int iGroupID = pModifyCustomGroup->groupinfo.groupid;
			g_pServerContext->GetUserInfomation()->AddCustomGroup(strSrcName, cg, iGroupID);

			MessagePacker::PackModifyCustomGroupReply(1, iGroupID, pReplyBlock);
			CopyAddressAndPort(pReplyBlock, pAMB);
		}
		break;
	case 1:
		{
			int iGroupID = pModifyCustomGroup->groupinfo.groupid;
			g_pServerContext->GetUserInfomation()->DelCustomGroup(strSrcName, iGroupID);
		}
		break;
	case 2:
		{
			int iGroupID = pModifyCustomGroup->groupinfo.groupid;
			UserInformation::CustomGroup cg;
			cg.groupid = pModifyCustomGroup->groupinfo.groupid;
			cg.type = pModifyCustomGroup->groupinfo.type;
			//cg.nodes_id = pModifyCustomGroup->groupinfo.groupid;
			cg.name = pModifyCustomGroup->groupinfo.groupname;
			g_pServerContext->GetUserInfomation()->UpdCustomGroup(strSrcName, cg, iGroupID);
		}
		break;
	default:
		break;
	}
}

void MessageParser::ModifyGroupNodes( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));

	modify_custom_group_node* pModifyCustomGroupNode = (modify_custom_group_node*)(pAMB->rd_ptr());
	char cOperate = pModifyCustomGroupNode->operate;

	pAMB->rd_ptr(sizeof(modify_custom_group_node) 
		- sizeof(pModifyCustomGroupNode->nodeid));

	int* pIDBuffer = (int*)(pAMB->rd_ptr());

	for(int i = 0; i < pModifyCustomGroupNode->nodecout; i++)
	{
		switch(cOperate)
		{
		case 0:
			g_pServerContext->GetUserInfomation()->AddNodeToGroup(
				strSrcName, pModifyCustomGroupNode->groupid
				, *(pIDBuffer + i));
			break;
		case 1:
			g_pServerContext->GetUserInfomation()->DelNodeFromGroup(
				strSrcName, pModifyCustomGroupNode->groupid, *(pIDBuffer + i));
			break;
		case 2:
			break;
		default:
			break;
		}
		//MessagePacker::PackModifyCustomGroupReply(1, iGroupID, pReplyBlock);
		//CopyAddressAndPort(pReplyBlock, pAMB);
	}

}

void MessageParser::QueryAccountFromNodeId( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pAMB->rd_ptr(sizeof(message_header));

	nodeid_to_account* pNTA = (nodeid_to_account*)(pAMB->rd_ptr());
	for(int i = 0; i < pNTA->count && i < sizeof(pNTA->nodeid); i++)
	{
		std::string strName = "error";
		g_pServerContext->GetUserInfomation()->GetAccountFromNodeID(pNTA->nodeid[i], strName);

		strcpy_s(pNTA->username + 255 * i, 255, strName.c_str());
	}

	nodeid_to_account newnta;
	memcpy_s(&newnta, sizeof(newnta), pNTA, sizeof(nodeid_to_account));

	MessagePacker::PackNodeIdToAccount(&newnta, pReplyBlock);
	CopyAddressAndPort(pReplyBlock, pAMB);
}

void MessageParser::GetUserInfo( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock )
{
	message_header* pMH = (message_header*)(pAMB->rd_ptr());
	std::string strSrcName = pMH->_username;
	std::string strAccr = pMH->_address;

	pAMB->rd_ptr(sizeof(message_header));

	user_information* pui = (user_information*)(pAMB->rd_ptr());
	

	if(pui->direct == 1)//写个人信息
	{
		UserInformation::USER_DETAIL_INFO info;
		info.length = 0;
		memset(info.icon, 0, sizeof(info.icon));
		info.name = pui->name;
		info.address = pui->address;
		info.email = pui->email;
		info.mobile = pui->mobile;
		info.qq = pui->qq;
		info.phone =pui->phone;
		memcpy_s(info.icon, sizeof(info.icon), pui->icon, sizeof(pui->icon));
		info.length = pui->length;

		g_pServerContext->GetUserInfomation()->GetUserInformation(strSrcName, &info, pui->direct);
	}
	else
	{
		UserInformation::USER_DETAIL_INFO info;
		user_information readInfo;
		g_pServerContext->GetUserInfomation()->GetUserInformation(strSrcName, &info, pui->direct);
		strcpy_s(readInfo.name, sizeof(readInfo.name), info.name.c_str());
		strcpy_s(readInfo.mobile, sizeof(readInfo.mobile), info.mobile.c_str());
		strcpy_s(readInfo.phone, sizeof(readInfo.phone), info.phone.c_str());
		strcpy_s(readInfo.email, sizeof(readInfo.email), info.email.c_str());
		strcpy_s(readInfo.address, sizeof(readInfo.address), info.address.c_str());
		strcpy_s(readInfo.qq, sizeof(readInfo.qq), info.qq.c_str());
		memcpy_s(readInfo.icon, sizeof(readInfo.icon), info.icon, sizeof(info.icon));
		readInfo.length = info.length;
		MessagePacker::PackUserInformation(&readInfo, pReplyBlock);

		CopyAddressAndPort(pReplyBlock, strAccr, strSrcName);
	}
}
