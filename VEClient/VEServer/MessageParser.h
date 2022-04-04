#pragma once

class MessageParser;

class LoginNow
{
public:
	virtual int Login(std::string& strName, std::string& pass, ACE_Message_Block* pMB = 0) = 0;
};

class MessageParser
{
public:
	MessageParser();
	void ParserMessageBlock(ACE_Message_Block* pMB, ACE_Message_Block* pReplyBlock = 0);
	inline void SetLoginCallback(LoginNow* pLN){m_LoginCallback = pLN;}
private:
	void Logout(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void ParserLogin(ACE_Message_Block* pMB, std::string& name, std::string& pass);
	void OkToLogin(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock = 0);
	void SendMessage(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void ParseMessage(ACE_Message_Block* pAMB, int iUserID, const std::string& strName, char* pBuffer, int len);
	//void HeartheatReplay(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock = 0);
	void HeartheatHandle( ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock = 0 );
	void PostUserState(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock = 0);
	void CopyAddressAndPort(ACE_Message_Block* pDst, ACE_Message_Block* pSrc);
	void CopyAddressAndPort(ACE_Message_Block* pDst , const std::string& strAddr, const std::string& strName);
	void SetUserState(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void SendFileRequest(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void SendFileRequestAddition(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void SendFileRequestReply(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void SendFileBlockAddition(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void RequestOrganization(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void RequestCustomGroup(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void GetCustomGroups(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void ModifyGroup(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void ModifyGroupNodes(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void QueryAccountFromNodeId(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);
	void GetUserInfo(ACE_Message_Block* pAMB, ACE_Message_Block* pReplyBlock);

	LoginNow* m_LoginCallback;
};

