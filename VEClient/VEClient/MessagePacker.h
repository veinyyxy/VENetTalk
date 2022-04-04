#pragma once
struct send_file_reqest;

struct user_information;

class MessagePacker
{
public:
	static void PackLogin(const std::string& strAddrAndProt
		, const std::string& name
		, const std::string& password
		, ACE_Message_Block* pMB);

	static void SendMessage(const std::string& myName, const std::string& strAddrAndProt
		, const std::string strName
		, int iMessageType
		, char* buffer
		, int iLen
		, ACE_Message_Block* pMB);

	static void PackLogout(const std::string& strAddrAndProt
		, const std::string& name
		, ACE_Message_Block* pMB);
	static void PackGetUserState(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, int iGetID, std::string& getName
		, ACE_Message_Block* pMB);
	static void PackSetState(const std::string& strAddrAndProt, int iMyID
		, std::string& myName, int iState, ACE_Message_Block* pMB);

	static int PackRequestSendFile(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, const std::string& dstName
		, const std::string& fileName, int iFileBlockCount, int iBlockSize
		, std::string strUUID, int iState, ACE_Message_Block* pMB
		, send_file_reqest* result = 0, char iType = 0x0);
	static int PackSendFileBlock(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, const std::string& dstName, const std::string& fileName
		, int iBlockID, int iBlockSize, char* pBuffer, std::string& strUUID, ACE_Message_Block* pMB, char cType = 0x00);

	static int PackFileBlockAddition(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, const std::string& dstName, const std::string& fileName
		, int iBlockID, int iSendState, int iRecieveState, const std::string& strUUID
		, ACE_Message_Block* pMB, char cType = 0x00);

	static int PackRequestSendFileReply(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, const std::string& srcName, const std::string& dstAddress 
		, const std::string& fileName, int iState, const std::string& strUUID, ACE_Message_Block* pMB, char iType = 0x0);

	static int PackOrgStruction(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, ACE_Message_Block* pMB);

	static int PackCustomGroups(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, ACE_Message_Block* pMB);

	static int PackModifyGroup(const std::string& strAddrAndProt
		, int iMyID, std::string& myName,  int iOperate, int GroupID
		, int iType, std::string& strGroupName, ACE_Message_Block* pMB);

	static int PackModifyGroupNodes(const std::string& strAddrAndProt
		, int iMyID, std::string& myName, int iOperate, int GroupID
		, int NodeCount, int* iNodeBuffer, ACE_Message_Block* pMB);

	static int PackQueryAccountFromNodeId( const std::string& strAddrAndProt 
		, int iMyID, std::string& myName, int iNodeid[], int iNodeCount
		, ACE_Message_Block* pMB );


	static int PackGetSetUserInformation(const std::string& strAddrAndProt 
		, int iMyID, std::string& myName, user_information* pUi
		, ACE_Message_Block* pMB);
};

