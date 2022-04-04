#include "StdAfx.h"
#include "ConsoleMessageHandle.h"


ConsoleMessageHandle::ConsoleMessageHandle(void)
{
}


ConsoleMessageHandle::~ConsoleMessageHandle(void)
{
}

void ConsoleMessageHandle::Login( int iState, P_USER_INFO_LIST pUserInfo )
{
	if(iState >= 0)
	{
		std::cout<<"Login Successed."<<std::endl;
	}
	else
		std::cout<<"Login Failure."<<std::endl;
}

void ConsoleMessageHandle::ReceiveMessage( int iType, std::string& strUser, unsigned char* pBuffer, int len )
{
	if(iType == 0)
	{
	std::cout<<std::endl;
	std::cout<<"Receiver message from " <<strUser<<":"<<std::endl;
	std::cout<<pBuffer<<std::endl;
	}
	else
	{
	std::cout<<std::endl;
	std::cout<<"Don't Parse Format of Message."<<std::endl;
	}
}

void ConsoleMessageHandle::LogoutReply( int iState )
{
	if(iState)
	{
		std::cout<<std::endl<<"Logout!!!"<<std::endl;
	}
	else
	{
		std::cout<<std::endl<<"Login, Please!"<<std::endl;
	}
}

void ConsoleMessageHandle::UserState( P_USER_INFO_LIST pUserState )
{
	printf("user state");
}

void ConsoleMessageHandle::Heartbeat( int iBeat )
{
	printf("heartbeat");
}

void ConsoleMessageHandle::SetStateResult( int iState )
{
	printf("Set State Reslut %d", iState);
}

MessageCallback::REQUEST_RESULT ConsoleMessageHandle::TransmitFileRequest( int iState, std::string& srcName, std::string& fileName, std::string& strOutPath )
{
	if(iState == 0)
	{
		std::cout<<"Recieve file (" << fileName.c_str() <<")" << " from " 
			<<srcName.c_str()<<".(Y, n)";
	}
	else if(iState == 1)
	{
		std::cout<<"Continue to recieve file (" << fileName.c_str() <<")" << " from " 
			<<srcName.c_str()<<".(Y, n, r)";
	}
	
	char cBuff[10] = {0};
	std::cin>>cBuff;

	if(strcmp(cBuff, "Y") == 0)
	{
		if(iState == 1)
		{
			std::cout<<"Continue......"<<std::endl;
		}
		std::cout<<"Save Path:";
		char cBuff[1024] = {0};
		std::cin>>cBuff;
		strOutPath = cBuff;
		if(iState == 1)
		{
			return ACEPENT_CONTINUE;
		}
		else
		{
			return ACCEPT_RECEIVE;
		}
	}
	else if(strcmp(cBuff, "r") == 0)
	{
		std::cout<<"Save Path:";
		char cBuff[1024] = {0};
		std::cin>>cBuff;
		strOutPath = cBuff;
		return ACCEPT_RETRANSFER;
	}
	else
	{
		return REFUSE;
	}
}

int ConsoleMessageHandle::SendFileProgress( const std::string& strSendFileName, int iBlockCount, int iBlockID )
{
	//float fBB = iBlockID + 1/iBlockCount;
	//int iBB = fBB * 100;
	//std::cout<<strSendFileName.c_str()<<iBB<<"%"<<std::endl;
	return 1;
}

int ConsoleMessageHandle::ReceiveFileProgress( const std::string& strReceiveFileName, int iBlockCount, int iBlockID )
{
	//float fBB = iBlockID + 1/iBlockCount;
	//int iBB = fBB * 100;
	//std::cout<<strReceiveFileName.c_str()<<iBB<<"%"<<std::endl;
	return 1;
}

void ConsoleMessageHandle::OrganizationStructure(std::vector<ORG_NODE>* pNodes )
{
	for(size_t i = 0; i < pNodes->size(); i++)
	{
		int iLevel = pNodes->at(i).iLevel;
		while(iLevel-- > 0)
		{
			std::cout<<"     ";
		}
		if(pNodes->at(i).iNodeType == 0)
		{
			std::cout<<"+";
		}
		else
			std::cout<<"-";
		std::cout<<pNodes->at(i).strName.data()<<std::endl;
	}
}

int ConsoleMessageHandle::MyCustomGroup( P_CUSTOM_GROUPS pGroups )
{
	return 1;
}

int ConsoleMessageHandle::ModifyCustomGroupResult( int* iGroupID, int* iResult )
{
	return 1;
}

int ConsoleMessageHandle::QueryAccountResult( int iNodeId, std::string& username )
{
	return 1;
}

int ConsoleMessageHandle::UserDetailInformation( P_USER_DETAIL_INFO info )
{
	return 0;
}
