#pragma once
#include "myactioninterface.h"
#include "Client.h"

class ClientMainUI;

class SetStateInterface
{
public:
	Client* m_pClient;
	ClientMainUI* m_pMainUI;
};

class SetOfflineStateAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SetOfflineStateAction)
	virtual ~SetOfflineStateAction(void);

	virtual void MyAction();

	virtual void InitMyAction();

};


class SetOnlineStateAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SetOnlineStateAction)
	virtual ~SetOnlineStateAction(void);

	virtual void MyAction();

	virtual void InitMyAction();

};

class SetAwayStateAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SetAwayStateAction)
	virtual ~SetAwayStateAction(void);

	virtual void MyAction();

	virtual void InitMyAction();

};

class SetHideStateAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SetHideStateAction)

		virtual void MyAction();

	virtual void InitMyAction();

};