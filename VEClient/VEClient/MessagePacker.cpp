#include "StdAfx.h"
#include "MessageProtocol.h"
#include "MessagePacker.h"

#define STRING_TO_CHAR_ARRAY(a, b) strcpy_s(a\
	, sizeof(a), b.c_str())

#define COPY_DATA_TO_BLOCK(a, b, c) memcpy_s(a->wr_ptr(), a->size()\
	, b, sizeof(c));pMB->wr_ptr(sizeof(c));

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

void MessagePacker::PackLogin( const std::string& strAddrAndProt
	, const std::string& name
	, const std::string& password
	, ACE_Message_Block* pMB )
{
	if(!pMB) return;

	pMB->init(sizeof(message_header) + sizeof(login));

	login login_messgae;
	message_header header;
	
	memset(&header, 0, sizeof header);
	memset(&login_messgae, 0, sizeof login_messgae);

	STRING_TO_CHAR_ARRAY(login_messgae._username, name);
	STRING_TO_CHAR_ARRAY(login_messgae._password, password);

	header._id = 3;
	header._type = 3;
	header._user_id = -1;
	STRING_TO_CHAR_ARRAY(header._address, strAddrAndProt);
	strcpy_s(header._time, "2014.7.25");

	memcpy(pMB->wr_ptr(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));
	memcpy(pMB->wr_ptr(), &login_messgae, sizeof(login));
	pMB->wr_ptr(sizeof(login));
}

void MessagePacker::SendMessage(const std::string& myName
	, const std::string& strAddrAndProt, const std::string strName
	, int iMessageType, char* buffer, int iLen, ACE_Message_Block* pMB )
{
	if(!pMB) return;

	pMB->init(sizeof(message_header) + sizeof(send_message) + iLen);

	message_header header = {0};
	send_message messageBogdy = {0};

	header._id = 0;
	header._type = 5;
	header._user_id = 0;
	
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	strcpy_s(header._address, strAddrAndProt.c_str());

	memcpy(pMB->wr_ptr(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));
	
	memcpy(messageBogdy.username, strName.c_str(), sizeof(messageBogdy.username));
	messageBogdy.length = iLen;
	messageBogdy.type = 0;
	memcpy(messageBogdy.pBuffer, buffer, iLen);

	memcpy(pMB->wr_ptr(), &messageBogdy, sizeof(send_message));
	pMB->wr_ptr(sizeof(send_message));
}

void MessagePacker::PackLogout( const std::string& strAddrAndProt 
	, const std::string& name , ACE_Message_Block* pMB )
{
	if(!pMB) return;

	pMB->init(sizeof(message_header) + sizeof(logout));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	logout gout = {0};

	header._id = 0;
	header._type = 7;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	//header._time;
	header._body_size = sizeof(logout);
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	gout.id = 0;
	memcpy_s(gout.username, sizeof(gout.username), name.c_str(), name.length());

	memcpy_s(pMB->wr_ptr(), pMB->size(), &gout, sizeof(logout));
	pMB->wr_ptr(sizeof(logout));

}

void MessagePacker::PackGetUserState(const std::string& strAddrAndProt
	, int iMyID, std::string& myName, int iGetID, std::string& getName
	, ACE_Message_Block* pMB)
{
	if(!pMB) return;

	pMB->init(sizeof(message_header) + sizeof(user_state_request));
	memset(pMB->base(), 0, pMB->size());

	user_state_request usr = {0};
	usr._requestid = iGetID;
	memcpy_s(usr._requestname, 255, getName.c_str(), getName.length());

	message_header header = {0};

	header._type = 11;

	memcpy_s(header._username, 255, myName.c_str(), myName.length());
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &usr, sizeof(user_state_request));
	pMB->wr_ptr(sizeof(user_state_request));
}

void MessagePacker::PackSetState(const std::string& strAddrAndProt
	, int iMyID, std::string& myName, int iState, ACE_Message_Block* pMB)
{
	if(!pMB) return;

	pMB->init(sizeof(message_header) + sizeof(set_user_state));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 13;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());

	set_user_state setUserState = {0};
	setUserState.user_id = iMyID;
	strcpy_s(setUserState._username, sizeof(setUserState._username), myName.c_str());
	setUserState.state = iState;

	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &setUserState, sizeof(set_user_state));
	pMB->wr_ptr(sizeof(set_user_state));


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

int MessagePacker::PackSendFileBlock( const std::string& strAddrAndProt
	, int iMyID, std::string& myName, const std::string& dstName, const std::string& fileNamePath
	, int iBlockID, int iBlockSize, char* pBuffer, std::string& strUUID, ACE_Message_Block* pMB, char cType )
{
	pMB->init(sizeof(message_header) + sizeof(send_file_block) + iBlockSize);
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};
	header._id = iMyID;
	header._type = 6010;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());

	send_file_block FileBlock = {0};
	FileBlock.userid = iMyID;
	FileBlock.type = cType;
	strcpy_s(FileBlock.username, sizeof(FileBlock.username)
		, strAddrAndProt.c_str());//源文件发送用户的ID
	strcpy_s(FileBlock.src_file_path, sizeof(FileBlock.src_file_path)
		, fileNamePath.c_str());//文件源的位置
	
	STRING_TO_CHAR_ARRAY(FileBlock.uuid, strUUID);
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

int MessagePacker::PackFileBlockAddition( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, const std::string& dstName, const std::string& fileName 
	, int iBlockID, int iSendState, int iRecieveState, const std::string& strUUID
	, ACE_Message_Block* pMB, char cType)
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
	BlockAddition.type = cType;
	STRING_TO_CHAR_ARRAY(BlockAddition.username, dstName);
	STRING_TO_CHAR_ARRAY(BlockAddition.uuid, strUUID);
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	memcpy_s(pMB->wr_ptr(), pMB->size(), &BlockAddition, sizeof(send_file_block_addition));
	pMB->wr_ptr(sizeof(send_file_block_addition));

	return 1;
}

int MessagePacker::PackRequestSendFileReply( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, const std::string& srcName, const std::string& dstAddress 
	, const std::string& fileName, int iState, const std::string& strUUID, ACE_Message_Block* pMB
	, char iType)
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
	fileReply.type = iType;

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	//pMB->wr_ptr(sizeof(message_header));

	COPY_DATA_TO_BLOCK(pMB, &fileReply, send_file_request_reply);
	//pMB->wr_ptr(sizeof(send_file_request_reply));

	return 1;
}

int MessagePacker::PackOrgStruction( const std::string& strAddrAndProt , int iMyID, std::string& myName, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 15;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = 0;
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));
	return 1;
}

int MessagePacker::PackCustomGroups( const std::string& strAddrAndProt , int iMyID, std::string& myName, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 17;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = 0;
	memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	pMB->wr_ptr(sizeof(message_header));

	return 1;
}

int MessagePacker::PackModifyGroup( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, int iOperate, int GroupID , int iType
	, std::string& strGroupName, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(modify_custom_group));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 21;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = sizeof(modify_custom_group);

	modify_custom_group mcg = {0};
	mcg.operate = iOperate;
	mcg.groupinfo.groupid = GroupID;
	mcg.groupinfo.type = iType;
	STRING_TO_CHAR_ARRAY(mcg.groupinfo.groupname, strGroupName);
	
	//memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	//pMB->wr_ptr(sizeof(message_header));
	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	COPY_DATA_TO_BLOCK(pMB, &mcg, modify_custom_group);
	//pMB->wr_ptr(sizeof(modify_custom_group));
}

int MessagePacker::PackModifyGroupNodes( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, int iOperate, int GroupID 
	, int NodeCount, int* iNodeBuffer, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(modify_custom_group_node) + sizeof(int) * NodeCount);
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 23;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = sizeof(modify_custom_group_node);

	modify_custom_group_node mcgn = {0};
	mcgn.operate = iOperate;
	mcgn.groupid = GroupID;
	mcgn.nodecout = NodeCount;
	
	COPY_DATA_TO_BLOCK(pMB, &header, message_header);

	memcpy_s(pMB->wr_ptr(), pMB->size(), &mcgn, sizeof(modify_custom_group_node) - sizeof(mcgn.nodeid));
	pMB->wr_ptr(sizeof(modify_custom_group_node) - sizeof(mcgn.nodeid));

	memcpy_s(pMB->wr_ptr(), pMB->size(), iNodeBuffer, sizeof(int) * NodeCount);
	pMB->wr_ptr(NodeCount * sizeof(int));
	//memcpy_s(pMB->wr_ptr(), pMB->size(), &header, sizeof(message_header));
	//pMB->wr_ptr(sizeof(message_header));
}

int MessagePacker::PackQueryAccountFromNodeId( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, int iNodeid[], int iNodeCount, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(nodeid_to_account));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 25;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = sizeof(nodeid_to_account);

	nodeid_to_account nta = {0};
	nta.count = iNodeCount;
	if(iNodeCount > sizeof(nta.nodeid)) return 0;
	for(int i = 0; i < iNodeCount && iNodeCount < sizeof(nta.nodeid); i++)
	{
		nta.nodeid[i] = iNodeid[i];
	}

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);
	COPY_DATA_TO_BLOCK(pMB, &nta, nodeid_to_account);

	return 1;
}

int MessagePacker::PackGetSetUserInformation( const std::string& strAddrAndProt 
	, int iMyID, std::string& myName, user_information* pUi 
	, ACE_Message_Block* pMB )
{
	if(!pMB) return 0;

	pMB->init(sizeof(message_header) + sizeof(user_information));
	memset(pMB->base(), 0, pMB->size());

	message_header header = {0};

	header._id = 0;
	header._type = 27;
	strcpy_s(header._address, sizeof(header._address), strAddrAndProt.c_str());
	strcpy_s(header._username, sizeof(header._username), myName.c_str());
	//header._time;
	header._body_size = sizeof(user_information);

	COPY_DATA_TO_BLOCK(pMB, &header, message_header);

	memcpy_s(pMB->wr_ptr(), pMB->size(), pUi, sizeof(user_information));
	pMB->wr_ptr(sizeof(user_information));
}

