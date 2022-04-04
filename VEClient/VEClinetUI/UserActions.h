#pragma once

#include "MyActionInterface.h"
class Client;

class LoginAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(LoginAction)
	virtual void MyAction();
	virtual void InitMyAction();

	void SetNameAndPassword(const QString strName, const QString Password);
private:
	QString m_strName;
	QString m_strPassword;
};

class SetUserInfoAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SetUserInfoAction)
	virtual void MyAction();
	virtual void InitMyAction();
};