#include "StdAfx.h"
#include "UserActions.h"
#include "Client.h"
#include "UserInformationDialog.h"

extern Client* g_pClient;

ADD_MYACTION_CONTSTRUCTED_DECLARATION(LoginAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SetUserInfoAction)

void LoginAction::InitMyAction()
{
	
}

void LoginAction::SetNameAndPassword( const QString strName, const QString Password )
{
	m_strName = strName;
	m_strPassword = Password;
}

void LoginAction::MyAction()
{
	std::string strstdName = m_strName.toStdString();
	std::string strstdPassword = m_strPassword.toStdString();
//g_pClient->Login();
}
//////////////////////////////////////////////////////////////////////////


void SetUserInfoAction::MyAction()
{
	g_pClient->GetUserInformation();
}

void SetUserInfoAction::InitMyAction()
{
	
}
