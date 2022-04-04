#pragma once
#include "MessageProtocol.h"

class MessagePacker
{
public:
	static int PackLoginReply(int iState, int iUserNum, user_list* pList
		, ACE_Message_Block* pMB);
	static int PackSendMessageReply(int iState, ACE_Message_Block* pMB);
	static int SendMessage(const std::string& strAddrAndProt
		, const std::string strName, int iMessageType, char* buffer
		, int iLen, ACE_Message_Block* pMB );
	static int PackLogoutReply(int iState, ACE_Message_Block* pMB);
	static int PackHeartbeat(const std::string& strUserName, int iState
		, const unsigned beatNum, ACE_Message_Block* pMB);
	static int PackUserStateReply(int iNum, user_state* pStateBuffer
		, ACE_Message_Block* pMB);
	static int PackSetUserStateReply( int iType, int iStateReply
		, ACE_Message_Block* pMB );
	static int PackOrgInformation(node_data* pNodeDatas, int iNodeNum
		, ACE_Message_Block* pMB);
	static int PackFileBlockAddition( const std::string& strAddrAndProt 
		, int iMyID, std::string& myName, const std::string& dstName
		, const std::string& fileName , int iBlockID, int iSendState
		, int iRecieveState, const std::string& strUUID, ACE_Message_Block* pMB);
	static int PackSendFileBlock( const std::string& strAddrAndProt 
		, int iMyID, std::string& myName, const std::string& dstName
		, const std::string& fileNamePath , int iBlockID, int iBlockSize
		, char* pBuffer, std::string& strUUID, ACE_Message_Block* pMB, char iType);
	static int PackRequestSendFileReply( const std::string& strAddrAndProt 
		, int iMyID, std::string& myName, const std::string& srcName
		, const std::string& dstAddress , const std::string& fileName
		, int iState, const std::string& strUUID, ACE_Message_Block* pMB );
	static int PackRequestSendFile( const std::string& strAddrAndProt, int iMyID 
		, std::string& myName, const std::string& dstName
		, const std::string& fileName, int iFileBlockCount , int iBlockSize
		, std::string strUUID, int iState, ACE_Message_Block* pMB 
		, send_file_reqest* result, char iType);

	static int PackModifyCustomGroupReply(char cState, int iGroupID, ACE_Message_Block* pMB);
	static int PackNodeIdToAccount(nodeid_to_account* pnta, ACE_Message_Block* pMB);
	static int PackUserInformation(user_information* pUi, ACE_Message_Block* pMB);
};

