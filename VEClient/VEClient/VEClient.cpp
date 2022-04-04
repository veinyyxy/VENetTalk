// VEClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ClientUdp.h"
#include "ClientContext.h"
#include "ConsoleMessageHandle.h"
//10.2.31.32 20002  10.12.10.225 2323

int /*_tmain*/ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	MessageCallback* pPC = new ConsoleMessageHandle;
	Client* g_pClient = new ClientUdp;
	g_pClient->SetMessageCallback(pPC);

	char cRight[] = "Copyright 'C 2005-2013 VEINY Ltd. All rights reserved.";
	char wcBuffer[1023];

	std::cout<<cRight<<std::endl;

	if(argc < 4)
	{
		delete g_pClient;
		return 0;
	}

	g_pClient->StartClient(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));

	/*std::string strAddr;
	strAddr += std::string(argv[3]);
	strAddr += std::string(":");
	strAddr += std::string(argv[4]);*/

	//p_gClientContext->SetClientAddress(strAddr);
	while(1)
	{
		std::cout << "->";
		std::cin >> wcBuffer;

		if(strcmp(wcBuffer, "quit") == 0)
		{
			g_pClient->StopClient();
			break;
		}
		else if(strcmp(wcBuffer, "ln") == 0)//login
		{
			std::string userName, password;

			memset(wcBuffer, 0, 15);
			std::cout << "user：";
			std::cin >> wcBuffer;

			userName = wcBuffer;

			std::cout << "password：";
			std::cin >> wcBuffer;

			password = wcBuffer;

			g_pClient->Login(userName.c_str(), password.c_str());
		}
		else if(strcmp(wcBuffer, "lt") == 0)
		{
			g_pClient->Logout();
		}
		else if(strcmp(wcBuffer, "fl") == 0)//friend_list
		{
			;
		}
		else if(strcmp(wcBuffer, "os") == 0)//organization_structure
		{
			;
		}
		else if(strcmp(wcBuffer, "send") == 0)
		{
			std::string strUser, messgae;
			memset(wcBuffer, 0, 1023);
			std::cout << "user：";
			std::cin >> wcBuffer;

			strUser = wcBuffer;

			std::cout << "text：";
			std::cin >> wcBuffer;

			messgae = wcBuffer;

			g_pClient->SetMessage(0, strUser, wcBuffer, messgae.length());
		}
		else if(strcmp(wcBuffer, "us") == 0)
		{
			std::string strName;
			g_pClient->GetFriendState(-1, strName);
		}
		else if(strcmp(wcBuffer, "sf") == 0)
		{
			std::cout<<"FILE:";
			memset(wcBuffer, 0, 1023);
			std::cin>>wcBuffer;
			std::string strFile = wcBuffer;

			memset(wcBuffer, 0, 1023);
			std::cout<<"NAME:";
			std::cin>>wcBuffer;
			std::string strName = wcBuffer;
			g_pClient->SendFile(strFile, strName);
		}
		else if(strcmp(wcBuffer, "sfo") == 0)
		{
			std::cout<<"FILE:";
			memset(wcBuffer, 0, 1023);
			std::cin>>wcBuffer;
			std::string strFile = wcBuffer;

			memset(wcBuffer, 0, 1023);
			std::cout<<"NAME:";
			std::cin>>wcBuffer;
			std::string strName = wcBuffer;
			g_pClient->SendFile(strFile, strName, 1);
		}
		else if(strcmp(wcBuffer, "org") == 0)
		{
			g_pClient->GetOrganizationStructure();
		}
		else if(strcmp(wcBuffer, "cg") == 0)
		{
			g_pClient->GetCustomGroups();
		}
		else
		{

		}

		std::cout<<std::endl;
	}

	return 0;
}

