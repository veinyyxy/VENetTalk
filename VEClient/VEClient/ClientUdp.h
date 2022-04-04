#pragma once
#include "Client.h"

class SenderThread;
class ReceiverThread;
class ClientContext;

class ClientUdp :
	public Client
{
public:
	ClientUdp(void);
	virtual ~ClientUdp(void);

	virtual int Login(const char* cName, const char* cPw );

	virtual int Logout();

	virtual int SetMessage( int iUserID, const std::string& strName, char* pBuffer, int iSize );

	virtual int GetFriendList();

	virtual int StartClient(const std::string& cHost, unsigned short usPort, const std::string& strClient, unsigned short usRevPort);

	virtual int StopClient();

	virtual int SetMessageCallback( MessageCallback* pMC );

	virtual int GetUserName( std::string& strName );

	virtual int GetuserAddress( std::string& strAddrAndPort );

	virtual int GetFriendState( int iID, std::string& strName );

	virtual int SetMyState( ENUM_USER_STATE state );

	virtual int SendFile( std::string& filePath, std::string& strDstUserName, int iType = 0 );

	virtual int GetOrganizationStructure();

	virtual int GetCustomGroups();

	virtual int ModifyGroup( int iOperate, int GroupID, int iType, std::string& strGroupName );

	virtual int ModifyGroupNodes( int iOperate, int GroupID, int NodeCount, int* iNodeBuffer );

	virtual int QueryAccountFromNodeId( int iNodeId[], int iCount);

	virtual int GetUserInformation();

	virtual int SetUserInformation( char* pBuffer, int iLength );

	virtual int GetMessageID( int* id );

private:
	SenderThread* m_pSenderThread;
	ReceiverThread* m_pReciverThread;

	ClientContext* m_pClientContext;
	int m_iMessageID;
};

