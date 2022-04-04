#pragma once

class ReceiverThread;
class SenderThread;
class SendFileThread;
class RecieveFileThread;
class MessageCallback;

class ClientContext
{
public:
	ClientContext(void);
	virtual ~ClientContext(void);
	inline ReceiverThread* GetReceiverThread(){return m_pReceierThread;}
	inline SenderThread*   GetSenderThread(){return m_pSenderThread;}
	inline void SetReceiverThread(ReceiverThread* pRT){m_pReceierThread = pRT;}
	void SetSenderThread(  SenderThread*  pST);
	void SetClientAddress(const std::string& strAddr);
	inline std::string& GetClientAddress(){return m_strClientAddress;}
	inline void SetUserName(const std::string& strUsername){m_strUserName = strUsername;}
	inline const std::string& GetUserName(){return m_strUserName;}
	inline void SetIsLogin(bool bLogin){m_bLogin = bLogin;}
	inline bool GetIsLogin(){return m_bLogin;}
	inline void SetSendFileThread(SendFileThread* pSFT){m_pSendFileThread = pSFT;}
	inline SendFileThread* GetSendFileThread(){return m_pSendFileThread;}
	inline void SetRecievThread(RecieveFileThread* pRFT){m_pReceiveFileThread = pRFT;}
	inline RecieveFileThread* GetRecieveFileThread(){return m_pReceiveFileThread;}
	std::string& GetRecievFileAddress();
	inline MessageCallback* GetMessageCallback(){return m_pMessageCallback;}
	inline void SetMessageCallback(MessageCallback* pMC){m_pMessageCallback = pMC;}
	void Init();
private:
	ReceiverThread* m_pReceierThread;
	SenderThread* m_pSenderThread;
	SendFileThread* m_pSendFileThread;
	RecieveFileThread* m_pReceiveFileThread;
	MessageCallback* m_pMessageCallback;
	std::string m_strClientAddress, m_RecievefileAddr;
	std::string m_strUserName;
	bool m_bLogin;
};

