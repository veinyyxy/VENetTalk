#include "StdAfx.h"
#include "MessageUICallback.h"


MessageUICallback::MessageUICallback(void)
{
}


MessageUICallback::~MessageUICallback(void)
{
}

void MessageUICallback::Login( int iState, P_USER_INFO_LIST pUserInfo )
{
	emit signal_Login(iState, pUserInfo);
}

void MessageUICallback::ReceiveMessage( int iType, std::string strUser, unsigned char* pBuffer, int len )
{
	emit signal_ReceiveMessage(iType, strUser, pBuffer, len);
}

void MessageUICallback::LogoutReply( int iState )
{
	emit signal_LogoutReply(iState);
}
