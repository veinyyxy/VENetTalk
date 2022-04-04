#include "StdAfx.h"
#include "ServerContext.h"

ServerContext::ServerContext(void)
	: m_pReceierThread(0)
	, m_pSenderThread(0)
	, m_pMessageParser(0)
	, m_pUserInfo(0)
	, m_pOffLineMessage(0)
	, m_pTransmitFileTable(0)
	, m_pOrganizationStructureInfo(0)
	, m_pRecieveFileThread(0)
	, m_pSendFileThread(0)
	, m_pWriteFileThread(0)
	, m_pOffLineFile(0)
	, m_pOfflineFileServer(0)
	, m_MessageID(0)
{
	m_strServerAddress = "255.255.255.255:65535";
}


ServerContext::~ServerContext(void)
{
}
