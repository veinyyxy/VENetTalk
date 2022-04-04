#include "StdAfx.h"
#include "MessagePacker.h"
#include "ServerContext.h"
#include "UserInformation.h"

#define STRING_TO_CHAR_ARRAY(a, b) strcpy_s(a\
	, sizeof(a), b.c_str())

#define COPY_DATA_TO_BLOCK(a, b, c) memcpy_s(a->wr_ptr(), a->size()\
	, b, sizeof(c));

extern ServerContext* g_pServerContext;

int GetBlockCount(int iFileSize, int iBlockSize)
{
	int BlockCount(0), iP1(0), iP2(0);

	iP1 = iFileSize / iBlockSize;
	iP2 = iFileSize % iBlockSize;

	if(iP2 == iFileSize)
	{
		BlockCount = 1;
	}
	else if(iP2 == 0)
	{
		BlockCount = iP1;
	}
	else
	{
		BlockCount  = iP1 + 1;
	}

	return BlockCount;
}

int MessagePacker::PackLoginReply( int iState, int iUserNum, user_list* pList, ACE_Message_Block* pMB )
{
	if(!pMB) return -1;

	if(iState == 1)
	{
		message_header header = {0};
		header._id = 0;
		header._type = 4;
		header._user_id = 0;
		header._body_size = sizeof(login_reply) + iUserNum * sizeof(user_list);

		//pMB->init(sizeof(message_header) + header._body_size);

		memcpy(pMB->wr_ptr(), &header, sizeof(message_header));
		pMB->wr_ptr(sizeof(message_header));

		login_reply reply = {0};
		reply.state = iState;
		reply.user_num = iUserNum;
		//只拷贝前两个int型数据
		memcpy(pMB->wr_ptr(), &reply, sizeof(login_reply));
		pMB->wr_ptr(sizeof(login_reply));

		memcpy(pMB->wr_ptr(), pList, iUserNum * sizeof(user_list));
		pMB->wr_ptr(iUserNum * sizeof(user_list));
	}
	else if(iState == -1)
	{
		message_header header = {0};
		header._id = 0;
		header._type = 4;
		header._user_id = 0;
		header._body_size = sizeof(login_reply);
		size_t sA = sizeof(message_header);
		memcpy(pMB->wr_ptr(), &header, sA);
		pMB->wr_ptr(sizeof(message_header));

		login_reply reply;
		reply.state = iState;
		reply.user_num = 0;
		reply.pUserBuff = 0;

		memcpy(pMB->wr_ptr(), &reply, sizeof(int) * 2);
		pMB->wr_ptr(sizeof(int) * 2);
	}

	return 1;
}

int MessagePacker::PackSendMessageReply( int iState, ACE_Message_Block* pMB )
{
	message_header header = {0};
	header._id = 0;
	header._type = 6;
	header._user_id = 0;
	header._body_size = sizeof(send_message_reply);

	memcpy(pMB->wr_ptr(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	send_message_reply replySend = {0};
	replySend.state = iState;
	memcpy_s(pMB->wr_ptr(), pMB->size(), &replySend, sizeof(send_message_reply));
	pMB->wr_ptr(sizeof(send_message_reply));
	return 1;
}

int MessagePacker::SendMessage(const std::string& strAddrAndProt, const std::string strName, int iMessageType, char* buffer, int iLen, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(send_message) + iLen);

	message_header header = {0};
	send_message messageBogdy = {0};

	header._id = 0;
	header._type = 5;
	header._user_id = 0;

	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());

	memcpy(pMB->wr_ptr(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy(messageBogdy.username, strName.c_str(), sizeof(messageBogdy.username));
	messageBogdy.length = iLen;
	messageBogdy.type = 0;
	memcpy(messageBogdy.pBuffer, buffer, iLen);

	memcpy(pMB->wr_ptr(), &messageBogdy, sizeof(send_message));
	pMB->wr_ptr(sizeof(send_message));

	return 1;
}

int MessagePacker::PackLogoutReply( int iState, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(logout_reply));

	message_header header = {0};
	logout_reply outReply = {0};

	header._type = 8;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	outReply.state = iState;
	memcpy_s(pMB->wr_ptr(), pMB->size(), &outReply, sizeof(logout_reply));
	pMB->wr_ptr(sizeof logout_reply);

	return 1;
}

int MessagePacker::PackHeartbeat(const std::string& strUserName, int iState, const unsigned beatNum, ACE_Message_Block* pMB)
{
	if(!pMB) return 0;
	unsigned int iHeartbeatNum = 0;

	if(g_pServerContext == 0) return 0;
	UserInformation*  pUI = g_pServerContext->GetUserInfomation();
	
	if(pUI == 0) return 0;
	if(pUI->GetUserHeartbeat(strUserName, iHeartbeatNum) <=0) return 0;
	
	message_header header = {0};
	heartbeat heart = {0};

	strcpy_s(header._username, sizeof(header._username), strUserName.c_str());

	header._type = 9;
	heart._state = iState;
	
	heart._beat_num = iHeartbeatNum;

	pMB->init(sizeof(message_header) + sizeof(heartbeat));
	memset(pMB->base(), 0, pMB->size());
	
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof message_header);

	memcpy_s(pMB->wr_ptr(), pMB->size() - sizeof(message_header), &heart, sizeof heartbeat);
	pMB->wr_ptr(sizeof heartbeat);

	return 1;
}

int MessagePacker::PackUserStateReply(int iNum, user_state* pStateBuffer, ACE_Message_Block* pMB)
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(user_state_list) + iNum * sizeof(user_state));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 12;
	
	memcpy_s(pMB->wr_ptr(), sizeof(message_header), &header, sizeof(message_header));

	user_state_list usl;
	usl.num = iNum;
	usl.list_start = 0;

	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &usl, sizeof(user_state_list));
	pMB->wr_ptr(sizeof(user_state_list));

	memcpy_s(pMB->wr_ptr(), pMB->size(), pStateBuffer, iNum * sizeof(user_state));

	pMB->wr_ptr(iNum * sizeof(user_state));
	return 1;
}

int MessagePacker::PackSetUserStateReply( int iType, int iStateReply, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(set_user_state_reply));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 14;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	set_user_state_reply re;

	re.iState = iStateReply;
	re.iType = iType;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &re, sizeof(set_user_state_reply));
	pMB->wr_ptr(sizeof(set_user_state_reply));
	
	return 1;
}

int MessagePacker::PackOrgInformation( node_data* pNodeDatas, int iNodeNum, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(int) + sizeof(node_data) * iNodeNum);
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 16;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	org_struct ost;

	ost.nodecount = iNodeNum;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &ost.nodecount, sizeof(ost.nodecount));
	pMB->wr_ptr(sizeof(ost.nodecount));
	
	memcpy_s(pMB->wr_ptr(), pMB->size(), pNodeDatas, sizeof(node_data) * iNodeNum);
	pMB->wr_ptr(sizeof(node_data) * iNodeNum);

	return 1;
}

int MessagePacker::PackFileBlockAddition( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, const std::string& dstName, const std::string& fileName 
	, int iBlockID, int iSendState, int iRecieveState, const std::string& strUUID, ACE_Message_Block* pMB)
{
	pMB->init(sizeof(message_header) + sizeof(send_file_block_addition));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 6020;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());

	send_file_block_addition BlockAddition = {0};
	BlockAddition.block_id = iBlockID;
	BlockAddition.send_state = iSendState;
	BlockAddition.reciver_state = iRecieveState;
	strcpy_s(BlockAddition.username, sizeof(BlockAddition.username)
		, dstName.c_str());
	strcpy_s(BlockAddition.uuid, sizeof(BlockAddition.uuid)
		, strUUID.c_str());
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &BlockAddition, sizeof(send_file_block_addition));
	pMB->wr_ptr(sizeof(send_file_block_addition));

	return 1;
}

int MessagePacker::PackSendFileBlock( const std::string& strAddrAndProt
	, int iMyID, std::string& myName
	, const std::string& dstName
	, const std::string& fileNamePath
	, int iBlockID, int iBlockSize
	, char* pBuffer
	, std::string& strUUID
	, ACE_Message_Block* pMB
	, char iType)
{
	pMB->init(sizeof(message_header) + sizeof(send_file_block) + iBlockSize);
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 6010;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), strAddrAndProt.c_str());

	send_file_block FileBlock = {0};
	FileBlock.userid = iMyID;
	FileBlock.type = iType;
	strcpy_s(FileBlock.username, sizeof(FileBlock.username)
		, strAddrAndProt.c_str());//源文件发送用户的ID
	strcpy_s(FileBlock.src_file_path, sizeof(FileBlock.src_file_path)
		, fileNamePath.c_str());//文件源的位置

	strcpy_s(FileBlock.uuid, sizeof(FileBlock.uuid), strUUID.c_str());
	FileBlock.block_id = iBlockID;//块的数量
	FileBlock.file_block_size = iBlockSize;//文件大小

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &FileBlock, sizeof(send_file_block));
	pMB->wr_ptr(sizeof(send_file_block));

	memcpy_s(pMB->wr_ptr(), pMB->size(), pBuffer, iBlockSize);
	pMB->wr_ptr(iBlockSize);

	return 1;
}

int MessagePacker::PackRequestSendFileReply( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, const std::string& srcName, const std::string& dstAddress 
	, const std::string& fileName, int iState, const std::string& strUUID, ACE_Message_Block* pMB )
{
	pMB->init(sizeof(message_header) + sizeof(send_file_request_reply));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 6030;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());

	send_file_request_reply fileReply = {0};

	fileReply.userid = 0;
	STRING_TO_CHAR_ARRAY(fileReply.username, srcName);
	STRING_TO_CHAR_ARRAY(fileReply.dst_file_path, fileName);
	STRING_TO_CHAR_ARRAY(fileReply.uuid, strUUID);
	STRING_TO_CHAR_ARRAY(fileReply.useraddr, dstAddress);
	fileReply.reply_state = iState;

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	pMB->wr_ptr(sizeof(message_header));

	COPY_DATA_TO_BLOCK(pMB, &fileReply, send_file_request_reply);
	pMB->wr_ptr(sizeof(send_file_request_reply));

	return 1;
}

int MessagePacker::PackRequestSendFile( const std::string& strAddrAndProt, int iMyID
	, std::string& myName, const std::string& dstName, const std::string& fileName, int iFileBlockCount
	, int iBlockSize, std::string strUUID, int iState, ACE_Message_Block* pMB
	, send_file_reqest* result, char iType)
{
	long FileSize = 0;
	FILE* file = fopen(fileName.c_str(), "rb");
	if (file)
	{
		FileSize = filelength(fileno(file));
		fclose(file);
	}

	if(FileSize <= 0) return -1;
	if(!pMB) return -2;

	pMB->init(sizeof(message_header) + sizeof(send_file_reqest));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 6000;

	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());

	send_file_reqest SendFileReqest = {0};

	SendFileReqest.state = iState;
	SendFileReqest.file_size = FileSize;//文件大小
	SendFileReqest.file_block_size = iBlockSize;
	SendFileReqest.type = iType;
	int BlockCount = GetBlockCount(FileSize, iBlockSize);

	SendFileReqest.userid = iMyID;
	STRING_TO_CHAR_ARRAY(SendFileReqest.uuid, strUUID);
	strcpy_s(SendFileReqest.username, sizeof(SendFileReqest.username)
		, dstName.c_str());
	SendFileReqest.block_count =  BlockCount;//文件块数
	strcpy_s(SendFileReqest.src_file_path, sizeof(SendFileReqest.src_file_path)
		, fileName.c_str());

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &SendFileReqest, sizeof(send_file_reqest));
	pMB->wr_ptr(sizeof(send_file_reqest));

	if(result)
	{
		memcpy_s(result, sizeof(send_file_reqest), &SendFileReqest, sizeof(send_file_reqest));
	}
	return 1;

}

int MessagePacker::PackModifyCustomGroupReply( char cState, int iGroupID, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(custom_group_reply));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 20;

	memcpy_s(pMB->wr_ptr(), sizeof(message_header), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	custom_group_reply reply = {0};
	reply.groupid = iGroupID;
	reply.state = cState;

	COPY_DATA_TO_BLOCK(pMB, &reply, custom_group_reply);
	pMB->wr_ptr(sizeof(custom_group_reply));

	return 1;
}

int MessagePacker::PackNodeIdToAccount( nodeid_to_account* pnta, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(nodeid_to_account));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 24;

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	pMB->wr_ptr(sizeof(message_header));
	COPY_DATA_TO_BLOCK(pMB, pnta, nodeid_to_account);
	pMB->wr_ptr(sizeof(nodeid_to_account));
}

int MessagePacker::PackUserInformation( user_information* pUi, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;
	pMB->init(sizeof(message_header) + sizeof(user_information));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._type = 26;

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	pMB->wr_ptr(sizeof(message_header));
	COPY_DATA_TO_BLOCK(pMB, pUi, user_information);
	pMB->wr_ptr(sizeof(user_information));
}
