#pragma once
#include <vector>
#include <string>
#include "MessageProtocol.h"

class MessageCallback
{
public:
	typedef struct __userInfo__
	{
		int id;
		std::string name;
		int state;
	} USER_INFO, *P_USER_INFO;

	typedef enum {REFUSE = 0, ACCEPT_RECEIVE = 1, ACCEPT_RETRANSFER = 2
		,ERROR_CONTINUE_TRANSFER_FAILURE = 3, ACEPENT_CONTINUE = 4
	} REQUEST_RESULT;

	typedef std::vector<USER_INFO> USER_INFO_LIST, *P_USER_INFO_LIST;

	typedef struct node_info
	{
		int iLevel;
		int iNodeId;
		int iParentID;
		int iNodeType;
		std::string strName;
	} ORG_NODE, *P_ORG_NODE;

	typedef struct __Custom_Group___
	{
		int type;
		int goupid;
		std::string name;
		std::vector<int> nodes;
	} CUSTOM_GROUP, *P_CUSTOM_GROUP;

	typedef std::vector<CUSTOM_GROUP> CUSTOM_GROUPS, *P_CUSTOM_GROUPS;

	typedef struct __UserInformation________________
	{
		std::string name;
		std::string  mobile;
		std::string phone;
		std::string email;
		std::string address;
		std::string qq;
		char icon[1 + 1024 * 32];
		int length;
	} USER_DETAIL_INFO, *P_USER_DETAIL_INFO;

public:
	virtual void Login(int iState, P_USER_INFO_LIST pUserInfo) = 0;
	virtual void ReceiveMessage(int iType, std::string& strUser
		, unsigned char* pBuffer, int len) = 0;
	virtual void LogoutReply(int iState) = 0;
	virtual void UserState(P_USER_INFO_LIST pUserState) = 0;
	virtual void OrganizationStructure(std::vector<ORG_NODE>* pNodes) = 0;
	virtual void Heartbeat(int iBeat) = 0;
	virtual void SetStateResult(int iState) = 0;
	virtual MessageCallback::REQUEST_RESULT TransmitFileRequest(int iState, std::string& srcName, std::string& fileName, std::string& strOutPath, int iType) = 0;
	virtual int SendFileProgress(const std::string& strSendFileName, int iBlockCount, int iBlockID) = 0;
	virtual int ReceiveFileProgress(const std::string& strReceiveFileName, int iBlockCount, int iBlockID) = 0;
	virtual int MyCustomGroup(P_CUSTOM_GROUPS pGroups) = 0;
	virtual int ModifyCustomGroupResult(int* iGroupID, int* iResult) = 0;
	virtual int QueryAccountResult(int iNodeId, std::string& username) = 0;
	virtual int UserDetailInformation(P_USER_DETAIL_INFO info) = 0;
};

