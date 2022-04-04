#pragma once
#include "offlinemessage.h"
#include <map>
#include <vector>

class OffLineMessageDemo :
	public OffLineMessage
{
public:
	typedef std::map<std::string, OffLineMessage::OFFLINE_MESSAGE_VECTOR> OFFLINE_MESSAGE_NAME_TO_MESSAGE_MAP, *P_OFFLINE_MESSAGE_NAME_TO_MESSAGE_MAP;
	OffLineMessageDemo(void);
	virtual ~OffLineMessageDemo(void);


	virtual void SaveMessage( int DstID, const std::string& strName, send_message* pSendMessage);
	virtual int GetMessage( int* DestID, std::string* strName, OffLineMessage::P_OFFLINE_MESSAGE_VECTOR pSendMessage);

private:
	OFFLINE_MESSAGE_NAME_TO_MESSAGE_MAP m_MessageMap;
	//std::vector<send_message*> m_OffLineMesaageMap;
};

