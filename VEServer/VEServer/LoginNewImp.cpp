#include "StdAfx.h"
#include "UserInformation.h"
#include "LoginNewImp.h"
#include "MessagePacker.h"
#include "ServerContext.h"
#include "OffLineMessage.h"
#include "SenderThread.h"

extern ServerContext* g_pServerContext;

LoginNowImp::LoginNowImp(void)
{
}

LoginNowImp::~LoginNowImp(void)
{
}

int LoginNowImp::Login( std::string& strName, std::string& pass, ACE_Message_Block* pMB )
{
	UserInformation* p_gUserInfo = g_pServerContext->GetUserInfomation();
	if(p_gUserInfo)
	{
		if(p_gUserInfo->IsRightfulUser(strName, pass))
		{
			/*UserInformation::USER_LIST userList;
			p_gUserInfo->GetUserList(&userList);

			UserInformation::USER_LIST::iterator be = userList.begin();
			user_list* pStructUserList = new user_list[userList.size()];

			for(int i = 0; be != userList.end(); (be++, i++))
			{
				memset(pStructUserList + i, 0, sizeof(user_list));
				(pStructUserList + i)->id = be->second.id;
				memcpy((pStructUserList + i)->_username, be->second.name.c_str(), be->second.name.length());
			}

			delete pStructUserList;*/
			MessagePacker::PackLoginReply(1, 0, 0, pMB);
			return 1;
		}
		else
		{
			MessagePacker::PackLoginReply(-1, 0, 0, pMB);
			return -1;
		}
	}

	return -1;
}
