#include "StdAfx.h"
#include "MessageServer.h"
#include "OfflineFileServer.h"
#include "RecieveFileThread.h"
#include "SendFileThread.h"
#include "MessagePacker.h"
#include "ServerContext.h"
#include "UserInformation.h"
#include "SenderThread.h"
#include "OffLineFile.h"
#include <string>
//#include <Rpc.h>

extern ServerContext* g_pServerContext;

OfflineFileServer::OfflineFileServer(void)
{
	
}


OfflineFileServer::~OfflineFileServer(void)
{
}

int OfflineFileServer::Initialze()
{
	m_pRecieveFileThread = new RecieveFileThread(&m_Addr);
	m_pSendFileThread = new SendFileThread();
	m_pSendFileThread->Init();
	return 1;
}

int OfflineFileServer::Start()
{
	m_pRecieveFileThread->activate();
	m_pSendFileThread->activate();
	return 1;
}

int OfflineFileServer::Stop()
{
	return -1;
}

int OfflineFileServer::Pause()
{
	return -1;
}

int OfflineFileServer::Resume()
{
	return -1;
}

void OfflineFileServer::SendOfflineFileToUser( const std::string strUser )
{
	OffLineFile* pOLF = g_pServerContext->GetOffLineFile();
	OffLineFile::OFF_LINE_FILE_INFO infoFile;
	OffLineFile::OFF_LINE_FILES Files;
	if(pOLF->GetFileInfo(strUser, Files) == 0) return;
	
	/*infoFile.strSrcName = "wanglei";
	infoFile.strDstName = "lili";
	infoFile.strUUID = "735F3ADC-3E98-4D88-8E4D-132E7C73FB7B";
	infoFile.strFileDir = "E:\\cc.xps";*/
	OffLineFile::OFF_LINE_FILES::iterator iFileBegin = Files.begin();
	while(iFileBegin!=Files.end())
	{
		infoFile = iFileBegin->second;
		SendFile(infoFile.strFileDir, infoFile.strUUID, infoFile.strSrcName
			, infoFile.strDstName, infoFile.iFileType);

		iFileBegin++;
	}
}

int OfflineFileServer::SendFile( std::string& filePath, const std::string&
	strUUID, const std::string& strSrcUserName, const std::string& 
	strDstUserName, int iType )
{
	UserInformation* pUI = g_pServerContext->GetUserInfomation();
	std::string strAddrAndPort;// = m_pClientContext->GetClientAddress();
	pUI->GetUserAddress(strDstUserName, strAddrAndPort);
	std::string strMyName = strSrcUserName;// = m_pClientContext->GetUserName();
	SendFileThread::SEND_FILE_DISCRIBE sendFileDiscribe;
	ACE_Message_Block* pRequest = new ACE_Message_Block;
	//判断是否存在未传输完成的可能
	std::string strBlockFile;
	strBlockFile = filePath + ".rbd";
	int iResult = access(strBlockFile.c_str(), F_OK);

	if(iResult == 0)
	{
		//文件存在
		std::string strUUID;
		sendFileDiscribe.m_FileMirror.Read(strBlockFile);
		sendFileDiscribe.m_FileMirror.GetUUID(&strUUID);
		sendFileDiscribe.iState = 3;

		/*源发送者与目标发送者调换，现在请求头里面表示的是文件的来源，客户端发送这个消息的时候表示的是文件的目标.
		因为发送给客户端后，消息头可以不必注明发送者（发送者为服务器）*/
		MessagePacker::PackRequestSendFile(strAddrAndPort, 0, (std::string)strDstUserName, 
			strMyName, filePath, 0, 1024 * 50, strUUID, 1, pRequest
			, &(sendFileDiscribe.sendReqest), iType);
	}
	else
	{
		//文件不存在
		sendFileDiscribe.iState = -1;

		MessagePacker::PackRequestSendFile(strAddrAndPort, 0, (std::string)strDstUserName, 
			strMyName, filePath, 0, 1024 * 50, strUUID, 0, pRequest
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
	m_pSendFileThread->putq(pSendDiscribe);

	//发送请求
	g_pServerContext->GetSenderThread()->putq(pRequest);
	return 1;
}