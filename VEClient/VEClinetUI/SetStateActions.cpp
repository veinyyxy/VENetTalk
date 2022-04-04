#include "StdAfx.h"
#include "SetStateActions.h"

ADD_MYACTION_CONTSTRUCTED_DECLARATION(SetOfflineStateAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SetOnlineStateAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SetAwayStateAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SetHideStateAction)

extern Client* g_pClient;

void SetOfflineStateAction::MyAction()
{
	g_pClient->SetMyState(Client::OFF_LINE);
}

void SetOfflineStateAction::InitMyAction()
{
	
}

SetOfflineStateAction::~SetOfflineStateAction( void )
{

}

void SetOnlineStateAction::MyAction()
{
	g_pClient->SetMyState(Client::ON_LINE);
}

void SetOnlineStateAction::InitMyAction()
{
	
}

SetOnlineStateAction::~SetOnlineStateAction( void )
{

}

void SetAwayStateAction::MyAction()
{
	g_pClient->SetMyState(Client::AWAY_TIME);
}

void SetAwayStateAction::InitMyAction()
{
	
}

SetAwayStateAction::~SetAwayStateAction( void )
{

}


void SetHideStateAction::MyAction()
{
	g_pClient->SetMyState(Client::HIDED);
}

void SetHideStateAction::InitMyAction()
{
	
}
