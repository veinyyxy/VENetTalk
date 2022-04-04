#pragma once
#include <string>
#include <vector>
#include "MessageProtocol.h"

class OffLineMessage
{
public:
	typedef std::vector<send_message> OFFLINE_MESSAGE_VECTOR, *P_OFFLINE_MESSAGE_VECTOR;
	virtual void SaveMessage(int DstID, const std::string& strName, send_message* pSendMessage) = 0;
	virtual int GetMessage(int* DestID, std::string* strName, OffLineMessage::P_OFFLINE_MESSAGE_VECTOR pSendMessage) = 0;
};

