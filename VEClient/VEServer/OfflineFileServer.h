#pragma once

class RecieveFileThread;
class WriteFileThread;
class SendFileThread;

class OfflineFileServer : public CMessageServer
{
public:
	OfflineFileServer(void);
	~OfflineFileServer(void);

	virtual int Initialze();

	virtual int Start();

	virtual int Stop();

	virtual int Pause();

	virtual int Resume();

	inline RecieveFileThread* GetReseiveOfflineFileThread(){return m_pRecieveFileThread;}
	inline SendFileThread* GetSendOfflineFileThread(){return m_pSendFileThread;}
	void SendOfflineFileToUser(const std::string strUser);
private:
	int SendFile( std::string& filePath, const std::string& strUUID, const std::string& strSrcUserName, const std::string& strDstUserName, int iType );
	RecieveFileThread* m_pRecieveFileThread;
	SendFileThread* m_pSendFileThread;
	WriteFileThread* m_pWriteFileThread;
};

