#pragma once
#include "myactioninterface.h"

class Client;
class ClientMainUI;

class SendFileAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SendFileAction)
	virtual void MyAction();

	virtual void InitMyAction();
	QTreeView* m_TreeView;
	QTreeView* m_TreeView1;
	int m_iState;
};

class SendMessageAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SendMessageAction)

	virtual void MyAction();

	virtual void InitMyAction();
	ClientMainUI* m_ClientMainUI;
	//QModelIndex* m_CurrentIndex;
};

