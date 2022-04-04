// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char *buff = "ABBBBBBBBA";
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);

	int iResult = WSAStartup(wVersionRequested, &wsaData);
	SOCKET sockHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	DWORD dwError = GetLastError();

	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_port = htons(27015);

	//iResult = bind(sockHandle, (SOCKADDR*)&clientAddr, sizeof(clientAddr));

	iResult = connect(sockHandle, (SOCKADDR*)&clientAddr, sizeof(clientAddr));

	iResult = send(sockHandle, buff, sizeof buff, 0);
	iResult = shutdown(sockHandle, SD_SEND);
	closesocket(sockHandle);
	WSACleanup();
	return 0;
}

