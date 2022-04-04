#pragma once
#include <map>
#include <string>

class MessageCallback;

class Client
{
public:
	enum ENUM_USER_STATE{OFF_LINE, ON_LINE, AWAY_TIME, HIDED};
	virtual int Login(const char* cName, const char* cPw) = 0;
	virtual int Logout() = 0;
	virtual int SetMessage(int iUserID, const std::string& strName, char* pBuffer, int iSize) = 0;
	virtual int GetFriendList() = 0;
	virtual int GetFriendState(int iID, std::string& strName) = 0;
	virtual int GetOrganizationStructure() = 0;
	virtual int StartClient(const std::string& strHost, unsigned short usPort, const std::string& strClient, unsigned short usRevPort) = 0;
	virtual int StopClient() = 0;
	virtual int SetMessageCallback(MessageCallback* pMC) = 0;
	virtual int GetUserName(std::string& strName) = 0;
	virtual int GetuserAddress(std::string& strAddrAndPort) = 0;
	virtual int SetMyState(ENUM_USER_STATE state) = 0;
	virtual int SendFile(std::string& filePath, std::string& strDstUserName, int iType = 0 ) = 0;
	virtual int GetCustomGroups() = 0;
	virtual int ModifyGroup(int iOperate, int GroupID, int iType, std::string& strGroupName) = 0;
	virtual int ModifyGroupNodes(int iOperate, int GroupID, int NodeCount, int* iNodeBuffer) = 0;
	virtual int QueryAccountFromNodeId( int iNodeId[], int iCount) = 0;
	virtual int GetUserInformation() = 0;
	virtual int SetUserInformation(char* pBuffer, int iLength) = 0;
	virtual int GetMessageID(int* id) = 0;
};

