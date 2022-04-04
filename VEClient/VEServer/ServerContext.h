#pragma once

class ReceiverThread;
class SenderThread;
class MessageParserThread;
class UserInformation;
class OffLineMessage;
class TransmitFileTable;
class OrganizationStructureInfo;
class RecieveFileThread;
class WriteFileThread;
class SendFileThread;
class OffLineFile;
class OfflineFileServer;

class ServerContext
{
public:
	ServerContext(void);
	virtual ~ServerContext(void);
	inline ReceiverThread* GetReceiverThread(){return m_pReceierThread;}
	inline SenderThread*   GetSenderThread(){return m_pSenderThread;}
	inline MessageParserThread*  GetParserThread(){return m_pMessageParser;}
	inline UserInformation* GetUserInfomation(){return m_pUserInfo;}
	inline OffLineMessage* GetOffLineMessage(){return m_pOffLineMessage;}
	inline TransmitFileTable* GetTransmitFileTable(){return m_pTransmitFileTable;}
	inline OrganizationStructureInfo* GetOrganizationStructureInfo(){return m_pOrganizationStructureInfo;}
	inline WriteFileThread* GetWriteOfflineFileThread(){return m_pWriteFileThread;}
	inline RecieveFileThread* GetReseiveOfflineFileThread(){return m_pRecieveFileThread;}
	inline SendFileThread* GetSendOfflineFileThread(){return m_pSendFileThread;}
	inline OffLineFile* GetOffLineFile(){return m_pOffLineFile;}
	inline OfflineFileServer* GetOfflineFileServer(){return m_pOfflineFileServer;}
	inline void GetServerAddress(std::string* pStrAddr){*pStrAddr = m_strServerAddress;}

	inline void SetReceiverThread(ReceiverThread* pRT){m_pReceierThread = pRT;}
	inline void SetSenderThread(  SenderThread*  pST){m_pSenderThread = pST;}
	inline void SetParserThread(  MessageParserThread* pMP){m_pMessageParser = pMP;}
	inline void SetUserInfomation(UserInformation* pUI){m_pUserInfo = pUI;}
	inline void SetOffLineMessage(OffLineMessage* pOM){m_pOffLineMessage = pOM;}
	inline void SetTransmitFileTable(TransmitFileTable* pTF){m_pTransmitFileTable = pTF;}
	inline void SetOrganizationStructureInfo(OrganizationStructureInfo* pOSI){m_pOrganizationStructureInfo = pOSI;}
	inline void SetWriteOfflineFileThread(WriteFileThread* pWFT){m_pWriteFileThread = pWFT;}
	inline void SetReseiveOfflineFileThread(RecieveFileThread* pRFT){m_pRecieveFileThread = pRFT;}
	inline void SetSendOfflineFileThread(SendFileThread* pSFT){m_pSendFileThread = pSFT;}
	inline void SetServerAddress(const std::string strAddr){m_strServerAddress = strAddr;}
	inline void SetOffLineFile(OffLineFile* pOLF){m_pOffLineFile = pOLF;}
	inline void SetOfflineFileServer(OfflineFileServer* pOFS){m_pOfflineFileServer = pOFS;}

private:
	ReceiverThread* m_pReceierThread;
	SenderThread* m_pSenderThread;
	MessageParserThread* m_pMessageParser;
	UserInformation* m_pUserInfo;
	OffLineMessage* m_pOffLineMessage;
	TransmitFileTable* m_pTransmitFileTable;
	OrganizationStructureInfo* m_pOrganizationStructureInfo;
	RecieveFileThread* m_pRecieveFileThread;
	SendFileThread* m_pSendFileThread;
	WriteFileThread* m_pWriteFileThread;
	std::string m_strServerAddress;
	OffLineFile* m_pOffLineFile;
	OfflineFileServer* m_pOfflineFileServer;
};

