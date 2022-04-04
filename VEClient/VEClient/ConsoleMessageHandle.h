#pragma once
#include "messagecallback.h"
class ConsoleMessageHandle :
	public MessageCallback
{
public:
	ConsoleMessageHandle(void);
	~ConsoleMessageHandle(void);

	virtual void Login( int iState, P_USER_INFO_LIST pUserInfo );

	virtual void ReceiveMessage( int iType, std::string& strUser, unsigned char* pBuffer, int len );

	virtual void LogoutReply( int iState );

	virtual void UserState( P_USER_INFO_LIST pUserState );

	virtual void Heartbeat( int iBeat );

	virtual void SetStateResult( int iState );

	virtual MessageCallback::REQUEST_RESULT TransmitFileRequest( int iState, std::string& srcName, std::string& fileName, std::string& strOutPath );

	virtual int SendFileProgress( const std::string& strSendFileName, int iBlockCount, int iBlockID );

	virtual int ReceiveFileProgress( const std::string& strReceiveFileName, int iBlockCount, int iBlockID );

	virtual void OrganizationStructure(std::vector<ORG_NODE>* pNodes);

	virtual int MyCustomGroup( P_CUSTOM_GROUPS pGroups );

	virtual int ModifyCustomGroupResult( int* iGroupID, int* iResult );

	virtual int QueryAccountResult( int iNodeId, std::string& username );

	virtual int UserDetailInformation( P_USER_DETAIL_INFO info );

};

