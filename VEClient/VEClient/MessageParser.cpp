#include "StdAfx.h"
#include "MessageProtocol.h"
#include "MessageParser.h"
#include "ClientContext.h"
#include "SenderThread.h"
#include "MessageCallBack.h"
#include "MessagePacker.h"
#include "SendFileThread.h"
#include "RecieveFileThread.h"
#include "WriteFileThread.h"

MessageParser::MessageParser(ClientContext* pCC, MessageCallback* pMC) : m_pClientContext(pCC), m_pMessageCallBack(pMC)
{
	
}


MessageParser::~MessageParser(void)
{
}

void MessageParser::LoginReply( ACE_Message_Block* pMB )
{
	if(!pMB) return;
	pMB->rd_ptr(sizeof(message_header));

	login_reply* pLR = (login_reply*)(pMB->rd_ptr());
	char* pBuff = (char*)pLR;
	pLR->pUserBuff = (user_list*)(pBuff + sizeof(int) * 3);

	MessageCallback::USER_INFO_LIST UserList;
	if(pLR->state >= 0)
	{
		for(int i = 0; i < pLR->user_num; i++)
		{
			MessageCallback::USER_INFO userInfo;
			userInfo.id = (pLR->pUserBuff + i)->id;
			userInfo.name = (pLR->pUserBuff + i)->_username;

			UserList.push_back(userInfo);
		}

		m_pClientContext->SetIsLogin(true);
	}
	else
	{
		m_pClientContext->SetIsLogin(false);
	}

	if(m_pMessageCallBack)
		m_pMessageCallBack->Login(pLR->state, &UserList);
}

void MessageParser::ParserMessageBlock( ACE_Message_Block* pMB, ACE_Message_Block* pReplyBlock )
{
	if(!pMB) return;
	message_header* pHeader = (message_header*)(pMB->rd_ptr());

	if(pHeader == nullptr) return;

	int iMessageID = pHeader->_id;
	int iTotal = pHeader->_totalnum;
	int iCurrentID = pHeader->_ordernum;

	if(iTotal > 1)
	{
		std::map<int, ACE_Message_Block*>::iterator fined 
			= m_MessagePoll.find(iMessageID);

		if(fined != m_MessagePoll.end())
		{
			message_header* pH = (message_header*)(fined->second->base());
			if(iTotal - 1 != iCurrentID)
			{
				pMB->rd_ptr(sizeof(message_header));
				fined->second->cont(pMB);
				return;
			}
			else
			{
				pMB->rd_ptr(sizeof(message_header));
				fined->second->cont(pMB);
				pMB = fined->second;
			}
		}
		else
		{
			m_MessagePoll[iMessageID] = pMB;
			return;
		}

	}
	//int iCurrentId = pHeader->_ordernum;

	switch(pHeader->_type)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		LoginReply(pMB);
		break;
	case 5:
		RecieverMessage(pMB);
		break;
	case 8:
		LogoutReply(pMB);
		break;
	case 9:
		SendHeartbeat(pMB);
		break;
	case 12:
		RecieverUserState(pMB);
		break;
	case 14:
		SetUserStateReply(pMB);
	case 16:
		HandleOrganization(pMB);
		break;
	case 18:
		GetCustomGroups(pMB);
		break;
	case 20:
		ModifyGroup(pMB);
		break;
	case 22:
		ModifyGroupNodes(pMB);
		break;
	case 24:
		QueryAccountFromNodeId(pMB);
		break;
	case 26:
		GetSetUserInforamtion(pMB);
		break;
	case 6000:
		TransmitFileRequest(pMB);
		break;
	case 6030:
		TransmitFileReqestReply(pMB);
		break;
	case 6020:
		TranmitBlockReply(pMB);
		break;
	default:
		break;
	}
}

void MessageParser::RecieverMessage( ACE_Message_Block* pMB )
{
	if(!pMB) return;
	pMB->rd_ptr(sizeof(message_header));

	send_message* pSendMessage = (send_message*)(pMB->rd_ptr());

	std::string strname = pSendMessage->username;
	if(m_pMessageCallBack)
		m_pMessageCallBack->ReceiveMessage(pSendMessage->type, strname, (unsigned char*)(pSendMessage->pBuffer), pSendMessage->length);
}

void MessageParser::LogoutReply( ACE_Message_Block* pMB )
{
	if(!pMB) return;
	pMB->rd_ptr(sizeof(message_header));

	logout_reply* outReply = (logout_reply*)(pMB->wr_ptr());

	if(m_pMessageCallBack)
	{
		m_pClientContext->SetIsLogin(false);
		m_pMessageCallBack->LogoutReply(outReply->state);
	}
}

void MessageParser::SendHeartbeat( ACE_Message_Block* pMB )
{
	if(!pMB) return;

	ACE_Message_Block* pReplayBeat = pMB->clone();
	message_header* pHeader = (message_header*)(pReplayBeat->base());
	pHeader->_type = 10;
	
	heartbeat* pBeat = (heartbeat*)(pReplayBeat->base() + sizeof(message_header));

	pBeat->_state = 1;

	SenderThread* pST = m_pClientContext->GetSenderThread();
	pST->putq(pReplayBeat);


}

void MessageParser::RecieverUserState( ACE_Message_Block* pMB )
{
	if(!pMB) return;

	pMB->rd_ptr(sizeof(message_header));

	user_state_list* pUSL = (user_state_list*)(pMB->rd_ptr());
	
	int iNum = pUSL->num;

	pMB->rd_ptr(sizeof(user_state_list));
	user_state* pUserList = (user_state*)(pMB->rd_ptr());

	MessageCallback::USER_INFO_LIST* pUserInfoList = new MessageCallback::USER_INFO_LIST;
	for(int i = 0; i < iNum; i++)
	{
		MessageCallback::USER_INFO userInfo;
		userInfo.id = (pUserList + i)->user_id;
		userInfo.name = (pUserList + i)->_username;
		userInfo.state = (pUserList + i)->state;

		pUserInfoList->push_back(userInfo);
	}
	m_pMessageCallBack->UserState(pUserInfoList);
}

void MessageParser::SetUserStateReply( ACE_Message_Block* pMB )
{
	if(!pMB) return;
	pMB->rd_ptr(sizeof(message_header));

	set_user_state_reply* outReply = (set_user_state_reply*)(pMB->rd_ptr());

	if(m_pMessageCallBack)
	{
		m_pMessageCallBack->SetStateResult(outReply->iState);
	}
}

//别人发送文件给我，向我询问，是否接收文件
void MessageParser::TransmitFileRequest( ACE_Message_Block* pMB )
{
	message_header* pMH = (message_header*)(pMB->rd_ptr());
	//std::string strSrcName = pMH->_username;
	//std::string strSrcAddr = "";//pMH->_address;

	pMB->rd_ptr(sizeof(message_header));

	send_file_reqest* pSendFileRequest = (send_file_reqest*)(pMB->rd_ptr());
	std::string strSrcFilePath = pSendFileRequest->src_file_path;
	std::string strUUID = pSendFileRequest->uuid;
	int iRequestState = pSendFileRequest->state;
	int iType = pSendFileRequest->type;

	std::string strSrcName = pSendFileRequest->username;
	char cType = pSendFileRequest->type;

	if(m_pMessageCallBack)
	{
		ACE_Message_Block* pSendBlock = new ACE_Message_Block;
		std::string MyName = m_pClientContext->GetUserName();
		//std::string MyAddr = m_pClientContext->GetClientAddress();
		
		std::string MyReceiveAddr = m_pClientContext->GetRecievFileAddress();
		std::string strOutPath;
		int iReuslt = m_pMessageCallBack->TransmitFileRequest(iRequestState
			, strSrcName, strSrcFilePath, strOutPath, iType);

		WriteFileThread::RECEIVE_FILE_DISCRIBE rfd;
		strcpy_s(rfd.savePath, sizeof(rfd.savePath), strOutPath.c_str());
		rfd.iState = iReuslt;
		memcpy_s(&(rfd.sendRequest), sizeof(send_file_reqest)
			, pSendFileRequest, sizeof(send_file_reqest));

		ACE_Message_Block* pControlBlock = new ACE_Message_Block(sizeof(
			WriteFileThread::RECEIVE_FILE_DISCRIBE));
		memcpy_s(pControlBlock->wr_ptr(), pControlBlock->size(), &rfd
			, sizeof(WriteFileThread::RECEIVE_FILE_DISCRIBE));
		pControlBlock->wr_ptr(sizeof(WriteFileThread::RECEIVE_FILE_DISCRIBE));

		pControlBlock->msg_type(WriteFileThread::MB_USER_DISCRIBE);
		RecieveFileThread* pRFT = m_pClientContext->GetRecieveFileThread();
		WriteFileThread* pWFT = pRFT->GetWriteThread();
		pWFT->putq(pControlBlock);

		MessagePacker::PackRequestSendFileReply("", 0, strSrcName
			, MyName, MyReceiveAddr, strSrcFilePath, iReuslt, strUUID
			, pSendBlock, cType);

		if(iReuslt == MessageCallback::ACCEPT_RECEIVE)
		{
			//同意接收文件
		}
		else if(iReuslt == MessageCallback::REFUSE)
		{
			//撤销接收文件
		}
		else if(iReuslt == MessageCallback::ACCEPT_RETRANSFER)
		{
			//重新传送文件

		}
		else
		{
			//请求续传失败
		}

		SenderThread* pST = m_pClientContext->GetSenderThread();
		pST->putq(pSendBlock);
	}
}
//发送文件给对方，对方反馈我的信息。回复：1-同意，0-拒接，2-重新传送文件，4续传
void MessageParser::TransmitFileReqestReply( ACE_Message_Block* pMB )
{
	message_header* pMH = (message_header*)(pMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pMB->rd_ptr(sizeof(message_header));

	send_file_request_reply* pSendFileRequest = (send_file_request_reply*)(
		pMB->rd_ptr());

	ACE_Message_Block* pControlBlock = new ACE_Message_Block(sizeof(
		SendFileThread::SEND_FILE_DISCRIBE));

	SendFileThread::SEND_FILE_DISCRIBE SFD;

	memcpy_s(SFD.address, sizeof(SFD.address), pSendFileRequest->useraddr
		, sizeof(pSendFileRequest->useraddr));
	memcpy_s(SFD.sendReqest.uuid, 37, pSendFileRequest->uuid, 37);
	SFD.iState = pSendFileRequest->reply_state;

	memcpy_s(pControlBlock->wr_ptr(), pControlBlock->size(), &SFD
		, sizeof(SendFileThread::SEND_FILE_DISCRIBE));
	pControlBlock->wr_ptr(sizeof(SendFileThread::SEND_FILE_DISCRIBE));

	if(pSendFileRequest->reply_state == 0)
	{
		//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
	}
	else if(pSendFileRequest->reply_state == 1)
	{
		//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
	}
	else
	{
		//m_pClientContext->GetSendFileThread()->putq(pControlBlock);
	}

	pControlBlock->msg_type(SendFileThread::MB_USER_DISCRIBE);
	m_pClientContext->GetSendFileThread()->putq(pControlBlock);

	pMB->release();
}

void MessageParser::TranmitBlockReply( ACE_Message_Block* pMB )
{
	message_header* pMH = (message_header*)(pMB->rd_ptr());
	std::string strSrcName = pMH->_username;

	pMB->rd_ptr(sizeof(message_header));

	send_file_block_addition* pSendFileRequest = (send_file_block_addition*)(
		pMB->rd_ptr());

	SendFileThread::SEND_BLOCK_ADDITION addition;
	addition.iState = 0;

	memcpy_s(&(addition.addition), sizeof(send_file_block_addition)
		, pSendFileRequest, sizeof(send_file_block_addition));


	ACE_Message_Block* addBlock = new ACE_Message_Block(
		sizeof(SendFileThread::SEND_BLOCK_ADDITION));

	memcpy_s(addBlock->base(), addBlock->size(), &addition
		, sizeof(SendFileThread::SEND_BLOCK_ADDITION));

	addBlock->msg_type(SendFileThread::MB_USER_BLOCK_REPLY);
	m_pClientContext->GetSendFileThread()->putq(addBlock);

	pMB->release();
}

void MessageParser::HandleOrganization( ACE_Message_Block* pMB )
{
	pMB->rd_ptr(sizeof(message_header));
	int iNodeCount = *(int*)pMB->rd_ptr();
	pMB->rd_ptr(sizeof(int));
	node_data* pNodeDatas = (node_data*)(pMB->rd_ptr());
	std::vector<MessageCallback::ORG_NODE>* 
		pNodes = new std::vector<MessageCallback::ORG_NODE>;
	for(int i = 0; i < iNodeCount; i++)
	{
		MessageCallback::ORG_NODE node;
		node.iLevel = pNodeDatas[i].levelid;
		node.iNodeId = pNodeDatas[i].nodenum;
		node.iNodeType = pNodeDatas[i].nodetype;
		node.iParentID = pNodeDatas[i].parentnum;
		node.strName = pNodeDatas[i].nodename;

		pNodes->push_back(node);
	}
	MessageCallback* pMC = m_pClientContext->GetMessageCallback();
	pMC->OrganizationStructure(pNodes);

	delete pNodes;
}

void MessageParser::GetCustomGroups( ACE_Message_Block* pMB )
{
	pMB->rd_ptr(sizeof(message_header));

	int iGroupCount = *(int*)(pMB->rd_ptr());
	pMB->rd_ptr(sizeof(int));

	//custom_group_info* mcg = (custom_group_info*)(pMB->rd_ptr());
	MessageCallback::CUSTOM_GROUPS customGroups;	
	for(int i = 0; i < iGroupCount; i++)
	{
		MessageCallback::CUSTOM_GROUP group;
		group.nodes.clear();

		custom_group_info* thisMCG = (custom_group_info*)(pMB->rd_ptr());
		int iGroupID = thisMCG->groupid;
		std::string strGroupName = thisMCG->groupname;
		int iType = thisMCG->type;
		int iNodeCount = thisMCG->nodecout;
		
		//////////////////////////////////////
		group.name = strGroupName;
		group.type = iType;
		group.goupid = thisMCG->groupid;
		//////////////////////////////////////
		pMB->rd_ptr(sizeof(custom_group_info) - sizeof(thisMCG->nodeid));
		int* pNodeIds = (int*)(pMB->rd_ptr());

		for(int j = 0; j < iNodeCount;j++)
		{
			int iNodeID = *(int*)(pNodeIds + j);
			group.nodes.push_back(iNodeID);
		}

		pMB->rd_ptr(sizeof(int) * iNodeCount);

		customGroups.push_back(group);
	}

	MessageCallback* pMC = m_pClientContext->GetMessageCallback();
	pMC->MyCustomGroup(&customGroups);
}

void MessageParser::ModifyGroup( ACE_Message_Block* pMB )
{
	pMB->rd_ptr(sizeof(message_header));

	custom_group_reply* pReply = (custom_group_reply*)(pMB->rd_ptr());
	int iGroupid = pReply->groupid;
	int iState = pReply->state;

	MessageCallback* pMC = m_pClientContext->GetMessageCallback();
	pMC->ModifyCustomGroupResult(&iGroupid, &iState);
}

void MessageParser::ModifyGroupNodes( ACE_Message_Block* pMB )
{

}

void MessageParser::QueryAccountFromNodeId( ACE_Message_Block* pMB )
{
	pMB->rd_ptr(sizeof(message_header));

	nodeid_to_account* pReply = (nodeid_to_account*)(pMB->rd_ptr());
	MessageCallback* pMC = m_pClientContext->GetMessageCallback();
	
	for(int i = 0; i < pReply->count; i++)
	{
		std::string strName = (pReply->username) + (i*255);
		pMC->QueryAccountResult(pReply->nodeid[i], strName);
	}
}

void MessageParser::GetSetUserInforamtion( ACE_Message_Block* pMB )
{
	pMB->rd_ptr(sizeof(message_header));

	user_information* pReply = (user_information*)(pMB->rd_ptr());
	
	MessageCallback::USER_DETAIL_INFO info;
	info.name = pReply->name;
	info.mobile = pReply->mobile;
	info.phone = pReply->phone;
	info.email = pReply->email;
	info.address = pReply->address;
	info.qq = pReply->qq;
	info.length = pReply->length;
	memcpy_s(info.icon, sizeof(info.icon)
		,pReply->icon, sizeof(pReply->icon));
	
	MessageCallback* pMC = m_pClientContext->GetMessageCallback();
	pMC->UserDetailInformation(&info);

}
