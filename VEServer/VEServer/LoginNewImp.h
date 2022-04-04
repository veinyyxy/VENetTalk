#pragma once
#include "messageparser.h"
class LoginNowImp :
	public LoginNow
{
public:
	LoginNowImp(void);
	virtual ~LoginNowImp(void);

	virtual int Login( std::string& strName, std::string& pass, ACE_Message_Block* pMB = 0 );

};

