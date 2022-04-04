#pragma once
class ClientContext;
class MessageCallback;
#include <map>

class MessageParser
{
public:
	MessageParser(ClientContext*, MessageCallback*);
	~MessageParser(void);
	void LoginReply(ACE_Message_Block* pMB);
	void ParserMessageBlock( ACE_Message_Block* pMB, ACE_Message_Block* pReplyBlock );
	void RecieverMessage(ACE_Message_Block* pMB);
	void LogoutReply(ACE_Message_Block* pMB);
	void SendHeartbeat(ACE_Message_Block* pMB);
	inline void SetMessageCallback(MessageCallback* pMC){m_pMessageCallBack = pMC;}
	void RecieverUserState(ACE_Message_Block* pMB);
	void SetUserStateReply(ACE_Message_Block* pMB);
	void TransmitFileRequest(ACE_Message_Block* pMB);
	void TransmitFileReqestReply(ACE_Message_Block* pMB);
	void TranmitBlockReply(ACE_Message_Block* pMB);
	void HandleOrganization(ACE_Message_Block* pMB);
	void GetCustomGroups(ACE_Message_Block* pMB);
	void ModifyGroup(ACE_Message_Block* pMB);
	void ModifyGroupNodes(ACE_Message_Block* pMB);
	void QueryAccountFromNodeId(ACE_Message_Block* pMB);
	void GetSetUserInforamtion(ACE_Message_Block* pMB);
private:
	ClientContext* m_pClientContext;
	MessageCallback* m_pMessageCallBack;

	std::map<int, ACE_Message_Block*> m_MessagePoll;
};

