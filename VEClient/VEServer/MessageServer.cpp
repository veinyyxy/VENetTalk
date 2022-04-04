#include "StdAfx.h"
#include "MessageServer.h"


CMessageServer::CMessageServer(void) : m_uPort(41873)
{
	m_Addr.set_port_number(m_uPort);
}

CMessageServer::CMessageServer(const char* pchAddr, u_short uPort) 
	: m_uPort(uPort)
{
	SetAddr(pchAddr);

	m_Addr.set_address(m_cAddr, strlen(m_cAddr));
	m_Addr.set_port_number(m_uPort);
}

CMessageServer::CMessageServer( ACE_INET_Addr& addr ) 
	: m_Addr(addr)
{

}


CMessageServer::~CMessageServer(void)
{
}
