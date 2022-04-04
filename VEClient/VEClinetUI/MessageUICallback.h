#pragma once
#include "MessageCallBack.h"

class MessageUICallback : public QObject, public MessageCallback
{
	Q_OBJECT
public:
	MessageUICallback(void);
	~MessageUICallback(void);

	virtual void Login( int iState, P_USER_INFO_LIST pUserInfo );

	virtual void ReceiveMessage( int iType, std::string strUser, unsigned char* pBuffer, int len );

	virtual void LogoutReply( int iState );
signals:
	void signal_Login( int iState, MessageCallback::P_USER_INFO_LIST pUserInfo );
	void signal_ReceiveMessage( int iType, std::string strUser, unsigned char* pBuffer, int len );
	void signal_LogoutReply( int iState );
};

