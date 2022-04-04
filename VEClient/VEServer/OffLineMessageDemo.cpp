#include "StdAfx.h"
#include "OffLineMessageDemo.h"


OffLineMessageDemo::OffLineMessageDemo(void)
{
}


OffLineMessageDemo::~OffLineMessageDemo(void)
{
}

void OffLineMessageDemo::SaveMessage( int DstID, const std::string& strName, send_message* pSendMessage)
{
	OFFLINE_MESSAGE_NAME_TO_MESSAGE_MAP::iterator iFinded = m_MessageMap.find(strName);

	if(iFinded != m_MessageMap.end())
	{
		iFinded->second.push_back(*pSendMessage);
	}
	else
	{
		OFFLINE_MESSAGE_VECTOR MessageVector;
		MessageVector.push_back(*pSendMessage);
		m_MessageMap[strName] = MessageVector;
	}
	
}

int OffLineMessageDemo::GetMessage( int* DestID, std::string* strName, OffLineMessage::P_OFFLINE_MESSAGE_VECTOR pSendMessage )
{
	OFFLINE_MESSAGE_NAME_TO_MESSAGE_MAP::iterator iFinded = m_MessageMap.find(*strName);

	if(iFinded != m_MessageMap.end())
	{
		*pSendMessage = iFinded->second;
		m_MessageMap.erase(iFinded);
		return 1;
	}
	return 0;
}
