
#include "StdAfx.h"
#include <Rpc.h>
#include "zip.h"
#include "Receiver.h"
#include "SenderThread.h"
#include "ReciverThread.h"
#include "MessagePacker.h"
#include "ClientUdp.h"
#include "ClientContext.h"
#include "SendFileThread.h"
#include "MessageProtocol.h"
#include "MessageCallBack.h"
#include "MakeDirToZip.h"

extern int IsDir(const char* filename);

ClientUdp::ClientUdp(void) : m_iMessageID(0)
{
	m_pClientContext = new ClientContext;
	m_pSenderThread = new SenderThread(m_pClientContext);
	m_pReciverThread = new ReceiverThread(m_pClientContext, 0, ACE_MESSAGE_BLOCK_SIZE);
}


ClientUdp::~ClientUdp(void)
{
}

int ClientUdp::Login(const char* cName, const char* cPw )
{
	ACE_Message_Block* pMB = new ACE_Message_Block;
	std::string strName, strPass, strAddr;
	strName = cName;
	m_pClientContext->SetUserName(cName);
	m_pClientContext->SetIsLogin(false);
	strAddr = m_pClientContext->GetClientAddress();
	strPass = cPw;
	MessagePacker::PackLogin(strAddr, strName, strPass, pMB);

	m_pSenderThread->putq(pMB);

	return 0;
}

int ClientUdp::Logout()
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	
	if(name.length() != 0)
	{
		ACE_Message_Block* pMB = new ACE_Message_Block;
		MessagePacker::PackLogout(strAddr, name, pMB);
		m_pSenderThread->putq(pMB);
		return 1;
	}
	else
		return 0;
}

int ClientUdp::SetMessage( int iUserID, const std::string& strName, char* pBuffer, int iSize )
{
	if(m_pClientContext->GetIsLogin())
	{
		std::string strAddr = m_pClientContext->GetClientAddress();
		ACE_Message_Block* pMB = new ACE_Message_Block;
		MessagePacker::SendMessage(m_pClientContext->GetUserName(), strAddr, strName, 0, pBuffer, iSize, pMB);

		m_pSenderThread->putq(pMB);
	}
	else
	{
		std::cout<<std::endl<<"还没有登陆。"<<std::endl;
	}
	return 0;
}

int ClientUdp::GetFriendList()
{
	return 0;
}

int ClientUdp::StartClient(const std::string& cHost, unsigned short usPort, const std::string& strClient, unsigned short usRevPort)
{
	ACE_INET_Addr* addr = new ACE_INET_Addr;
	int iR = 0;

	if(strClient.length() == 0)
	{
		size_t count = 0;
		iR = ACE::get_ip_interfaces(count, addr);
		addr->set_port_number(usRevPort);
		
	}
	else
	{
		addr->set(usRevPort, strClient.c_str());
		//iR = addr->set_address(strClient.c_str(), 4);
		//addr->set_port_number(usRevPort);
	}

	ACE_TCHAR charAddr[20] = {0};
	iR = addr->addr_to_string(charAddr, sizeof(charAddr));

	if(iR != 0) return 0;

	m_pClientContext->Init();

	m_pSenderThread->InitSender(cHost.c_str(), usPort);
	m_pSenderThread->activate();

	m_pReciverThread->InitReciver(*addr);
	m_pReciverThread->activate();

	std::string strAddrAndPort = charAddr;
	m_pClientContext->SetClientAddress(strAddrAndPort);
	
	return 0;
}

int ClientUdp::StopClient()
{
	m_pReciverThread->StopReciverThread();
	m_pSenderThread->wait();
	m_pReciverThread->wait();
	return 0;
}

int ClientUdp::SetMessageCallback( MessageCallback* pMC )
{
	m_pReciverThread->SetMessageCallback(pMC);
	m_pClientContext->SetMessageCallback(pMC);
	return 1;
}

int ClientUdp::GetUserName( std::string& strName )
{
	strName = m_pClientContext->GetUserName();
	return 1;
}

int ClientUdp::GetuserAddress( std::string& strAddrAndPort )
{
	strAddrAndPort = m_pClientContext->GetClientAddress();
	return 1;
}

int ClientUdp::GetFriendState( int iID, std::string& strName )
{
	std::string strAddrAndPort = m_pClientContext->GetClientAddress();
	std::string strMyName = m_pClientContext->GetUserName();
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackGetUserState(strAddrAndPort, 0, strMyName, iID, strName, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::SetMyState( ENUM_USER_STATE state )
{
	std::string strAddrAndPort = m_pClientContext->GetClientAddress();
	std::string strMyName = m_pClientContext->GetUserName();
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackSetState(strAddrAndPort, 0, strMyName, state, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::SendFile( std::string& filePath, std::string& strDstUserName, int iType )
{
	std::string strLocPath = filePath;
	char cDriver[5] = {0};
	char cDir[1024] = {0};
	char cFileName[1024] = {0};
	char cExt[100] = {0};

	_splitpath(filePath.c_str(), cDriver, cDir, cFileName, cExt);

	if(IsDir(filePath.c_str()) == 0)
	{
		std::string strZipName = std::string(cFileName)
			+ std::string(cExt);
		std::string strZipFilePath = std::string(cDriver)
			+ std::string(cDir)
			+ strZipName
			+ std::string(".folder");

		MakeDirToZip::CreateZipFromDir(filePath, strZipFilePath);
		strLocPath = strZipFilePath;

		iType = 0x2 + iType;
	}

	std::string strAddrAndPort = m_pClientContext->GetClientAddress();
	std::string strMyName = m_pClientContext->GetUserName();

	UUID uuid;
	RPC_STATUS rpcResult = UuidCreate(&uuid);

	if(rpcResult == RPC_S_OK)
	{
		printf("OK");
	}
	else
	{
		printf("NOT OK");
	}

	SendFileThread::SEND_FILE_DISCRIBE sendFileDiscribe;
	
	ACE_Message_Block* pRequest = new ACE_Message_Block;

	//判断是否存在未传输完成的可能
	std::string strBlockFile;
	strBlockFile = strLocPath + ".rbd";
	int iResult = access(strBlockFile.c_str(), F_OK);

	if(iResult == 0)
	{
		//文件存在
		std::string strUUID;
		
		sendFileDiscribe.m_FileMirror.Read(strBlockFile);
		sendFileDiscribe.m_FileMirror.GetUUID(&strUUID);
		sendFileDiscribe.iState = 3;
		MessagePacker::PackRequestSendFile(strAddrAndPort, 0, strMyName, 
			strDstUserName, strLocPath, 0, 1024 * 50, strUUID, 1, pRequest
			, &(sendFileDiscribe.sendReqest), iType);
	}
	else
	{
		//文件不存在
		sendFileDiscribe.iState = -1;
		RPC_CSTR charUUID;
		rpcResult = UuidToStringA(&uuid, &charUUID);
		char buffer[37] = {0};
		strcpy_s(buffer, 37, (char*)charUUID);

		std::string strUUID = buffer;

		const char* pBuffer = strUUID.c_str();

		MessagePacker::PackRequestSendFile(strAddrAndPort, 0, strMyName, 
			strDstUserName, strLocPath, 0, 1024 * 50, strUUID, 0, pRequest
			, &(sendFileDiscribe.sendReqest), iType);
	}
	

	size_t BlockSize = sizeof(SendFileThread::SEND_FILE_DISCRIBE);
	ACE_Message_Block* pSendDiscribe = new ACE_Message_Block(BlockSize);

	memset(pSendDiscribe->base(), 0, pSendDiscribe->size());

	BlockSize = sizeof sendFileDiscribe;
	//pBuffer = sendFileDiscribe.uuid.c_str();
	memcpy_s(pSendDiscribe->wr_ptr(), pSendDiscribe->size()
		, &sendFileDiscribe, sizeof(SendFileThread::SEND_FILE_DISCRIBE));

	pSendDiscribe->wr_ptr(sizeof(SendFileThread::SEND_FILE_DISCRIBE));

	pSendDiscribe->msg_type(SendFileThread::MB_USER_DISCRIBE);
	m_pClientContext->GetSendFileThread()->putq(pSendDiscribe);

	//发送请求
	m_pSenderThread->putq(pRequest);
	return 1;
}

int ClientUdp::GetOrganizationStructure()
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackOrgStruction(strAddr, 0, name, pMB);

	m_pSenderThread->putq(pMB);


	return 1;
}

int ClientUdp::GetCustomGroups()
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackCustomGroups(strAddr, 0, name, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::ModifyGroup( int iOperate, int GroupID, int iType, std::string& strGroupName )
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackModifyGroup(strAddr, 0, name, iOperate, GroupID, iType, strGroupName, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::ModifyGroupNodes( int iOperate, int GroupID, int NodeCount, int* iNodeBuffer )
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackModifyGroupNodes(strAddr, 0, name, iOperate, GroupID, NodeCount, iNodeBuffer, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::QueryAccountFromNodeId( int iNodeId[], int iCount)
{
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;
	MessagePacker::PackQueryAccountFromNodeId(strAddr, 0, name, iNodeId, iCount, pMB);
	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::GetUserInformation()
{
	user_information ui = {0};
	ui.direct = 0;
	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;

	MessagePacker::PackGetSetUserInformation(strAddr, 0, name, &ui, pMB);

	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::SetUserInformation( char* pBuffer, int iLength )
{
	if(iLength != sizeof(MessageCallback::USER_DETAIL_INFO)) return 0;

	MessageCallback::P_USER_DETAIL_INFO info 
		= (MessageCallback::P_USER_DETAIL_INFO)pBuffer;
	user_information ui = {0};
	ui.direct = 1;
	user_information readInfo;

	strcpy_s(ui.name, sizeof(readInfo.name), info->name.c_str());
	strcpy_s(ui.mobile, sizeof(readInfo.mobile), info->mobile.c_str());
	strcpy_s(ui.phone, sizeof(readInfo.phone), info->phone.c_str());
	strcpy_s(ui.email, sizeof(readInfo.email), info->email.c_str());
	strcpy_s(ui.address, sizeof(readInfo.address), info->address.c_str());
	strcpy_s(ui.qq, sizeof(readInfo.qq), info->qq.c_str());
	memcpy_s(ui.icon, sizeof(readInfo.icon), info->icon, sizeof(info->icon));
	ui.length = info->length;

	std::string strAddr(m_pClientContext->GetClientAddress()), name(m_pClientContext->GetUserName());
	ACE_Message_Block* pMB = new ACE_Message_Block;

	MessagePacker::PackGetSetUserInformation(strAddr, 0, name, &ui, pMB);
	m_pSenderThread->putq(pMB);
	return 1;
}

int ClientUdp::GetMessageID( int* id )
{
	*id = ++m_iMessageID;
	return 1;
}
