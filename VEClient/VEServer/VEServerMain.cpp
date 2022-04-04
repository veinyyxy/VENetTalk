// VEServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UserInformationDemo.h"
#include "MessageServer_UDP.h"
#include "OfflineFileServer.h"
#include "ServerContext.h"
#include "UserInformationDemo.h"
#include "OffLineMessageDemo.h"
#include "TransmitFileTableDemo.h"
#include "OrganizationStructureInfoDemo.h"
#include "OffLineFile.h"
#include "OffLineFileDemo.h"

#if 0
int StartServer(u_short uPort)
{
	ACE_SOCK_Stream sockStream;
	ACE_SOCK_Acceptor sockAccepter;
	ACE_INET_Addr inetAddr(uPort);
	ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);
	ACE_Handle_Set handle_set;
	

	if(sockAccepter.open(inetAddr) == -1)
	{
		THROW_ERROR
		return -1;
	}
	handle_set.set_bit(sockAccepter.get_handle());

	while(1)
	{
		ACE_Handle_Set temp = handle_set;
		int result = ACE_OS::select(int(sockAccepter.get_handle()), temp.fdset(), 0, 0, timeout);

		if (result == -1)
		{
			THROW_ERROR;
		}
		else if (result == 0)
		{
			THROW_ERROR;
		}
		else
		{
			if(temp.is_set(sockAccepter.get_handle()))
			{
				result = sockAccepter.accept(sockStream);
				
				//sockStream.recv()
			}
		}
	}
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
	u_short port = 27015;

	if (argc > 1)
		port = ACE_OS::atoi (argv[1]);

	return StartServer(port);
}
#endif

ServerContext* g_pServerContext = new ServerContext;

int /*_tmain*/ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
	g_pServerContext->SetUserInfomation(new UserInformationDemo);
	g_pServerContext->SetOffLineMessage(new OffLineMessageDemo);
	g_pServerContext->SetTransmitFileTable(new TransmitFileTableDemo);
	g_pServerContext->SetOrganizationStructureInfo(new OrganizationStructureInfoDemo);
	g_pServerContext->SetOffLineFile(new OffLineFileDemo);
	//ACE::init();
	std::wstring wstrCommand;
	char wcBuffer[15];
	CMessageServer* pMS = new CMessageServer_UDP;
	OfflineFileServer* pOfflineFileServer = new OfflineFileServer;
	g_pServerContext->SetOfflineFileServer(pOfflineFileServer);
	if(argc == 2)
	{
		ACE_INET_Addr* addr;
		int iR = 0;
		size_t count = 0;
		iR = ACE::get_ip_interfaces(count, addr);
		
		int iPort = atoi(argv[1]);
		addr->set_port_number(iPort);
		
		ACE_TCHAR charAddr[20] = {0};
		iR = addr->addr_to_string(charAddr, sizeof(charAddr));
		pMS->SetAddr(*addr);

		g_pServerContext->SetServerAddress(charAddr);
		//建立离线文件服务器
		
		ACE_INET_Addr offAddr(iPort + 20, addr->get_host_addr());
		pOfflineFileServer->SetAddr(offAddr);

	}
	else if(argc == 3)
	{
		ACE_INET_Addr addr((u_short)atoi(argv[2]), argv[1]);
		pMS->SetAddr(addr);
		ACE_TCHAR charAddr[20] = {0};
		int iR = addr.addr_to_string(charAddr, sizeof(charAddr));
		g_pServerContext->SetServerAddress(charAddr);
		ACE_INET_Addr offAddr((u_short)atoi(argv[2]) + 20, argv[1]);
		pOfflineFileServer->SetAddr(offAddr);
	}
	else
	{
		return 0;
	}

	pMS->Initialze();
	pMS->Start();

	//建立离线文件服务器
	pOfflineFileServer->Initialze();
	pOfflineFileServer->Start();

	g_pServerContext->SetReseiveOfflineFileThread(
		pOfflineFileServer->GetReseiveOfflineFileThread());
	g_pServerContext->SetSendOfflineFileThread(
		pOfflineFileServer->GetSendOfflineFileThread());


	//////////////////////////////////////////////////////////////////////////
	while(1)
	{
		std::cout << "->";
		std::cin >> wcBuffer;
		
		if(strcmp(wcBuffer, "quit") == 0)
		{
			break;
		}
		//ACE_OS::sleep(2);
	}

	pMS->Stop();
	pOfflineFileServer->Stop();
	//ACE::fini();
	return 0;
}