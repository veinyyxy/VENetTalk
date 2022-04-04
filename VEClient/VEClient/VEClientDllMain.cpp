#include "stdafx.h"
#include "ClientUdp.h"

Client* g_pClient = new ClientUdp;

Client* GetClient()
{
	if(g_pClient)
		return g_pClient;
	else
	{
		g_pClient = new ClientUdp;
		return g_pClient;
	}
}

void ReleaseClient()
{
	if(g_pClient)
	{
		delete g_pClient;
		g_pClient = nullptr;
	}
}

BOOLEAN WINAPI DllMain( IN HINSTANCE hDllHandle, 
	IN DWORD     nReason, 
	IN LPVOID    Reserved )
{
	BOOLEAN bSuccess = TRUE;


	//  Perform global initialization.

	switch ( nReason )
	{
	case DLL_PROCESS_ATTACH:

		//  For optimization.

		DisableThreadLibraryCalls( hDllHandle );
		ACE::init();
		break;

	case DLL_PROCESS_DETACH:
		
		break;
	}

	return bSuccess;

}
