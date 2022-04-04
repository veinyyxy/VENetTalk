#include "StdAfx.h"
#include "HeartbeatMoniterThread.h"
#include "ServerContext.h"
#include "UserInformation.h"
#include "MessagePacker.h"
#include "SenderThread.h"

extern ServerContext* g_pServerContext;

HeartbeatMoniterThread::HeartbeatMoniterThread(void)
{
	m_pUserInfo = 0;
	m_pSenderThread = 0;
	if(g_pServerContext)
	{
		m_pUserInfo = g_pServerContext->GetUserInfomation();
		m_pSenderThread = g_pServerContext->GetSenderThread();
	}
}


HeartbeatMoniterThread::~HeartbeatMoniterThread(void)
{
}

int HeartbeatMoniterThread::svc( void )
{
#if 0
	while(1)
	{
		if(m_pUserInfo && m_pSenderThread)
		{
			UserInformation::USER_LIST userList;
			m_pUserInfo->GetUserList(&userList);

			UserInformation::USER_LIST::iterator ib = userList.begin();

			for(;ib != userList.end(); ib++)
			{
				int iUserState = 0;
				int iHeartState = 0;
				m_pUserInfo->GetUserState(ib->first, iUserState);
				if(m_pUserInfo->GetHeartState(ib->first, iHeartState))
				{
					if(iHeartState == 0)
					{
						std::string strAddress = ib->second.user_address;

						ACE_Message_Block* pMB = new ACE_Message_Block;

						if(MessagePacker::PackHeartbeat(ib->first, iUserState, 0, pMB))
						{
							//ACE_Message_Block* pAddressBlock = new ACE_Message_Block(strAddress.size()+1);
							//memset(pAddressBlock->wr_ptr(), 0, pAddressBlock->size());
							//memcpy_s(pAddressBlock->wr_ptr(), pAddressBlock->size(), strAddress.data(), strAddress.length());
							//pMB->prev(pAddressBlock);

							message_header* pDstHeader = (message_header*)(pMB->base());

							strcpy_s(pDstHeader->_address, sizeof(pDstHeader->_address), strAddress.c_str());

							m_pSenderThread->putq(pMB);

							/*if(iUserState == 6657)
							{
								m_pUserInfo->SetUserAddress();
								m_pUserInfo->SetUserState(ib->first, 0);
								m_pUserInfo->
							}*/

							m_pUserInfo->SetHeartState(ib->first, 1);
						}
						else
						{
							delete pMB;
						}
					}
					else
					{
						if(iHeartState == 1)
						{
							m_pUserInfo->SetUserAddress(ib->first, "");
							m_pUserInfo->SetHeartState(ib->first, 0);
							m_pUserInfo->SetUserHeartbeat(ib->first, 0);
							m_pUserInfo->SetUserState(ib->first, 0);
						}
						else
						{
							m_pUserInfo->SetHeartState(ib->first, 0);
						}
					}
				}

				
			}
		}
		Sleep(8000);
	}
#endif
	return 1;
}
