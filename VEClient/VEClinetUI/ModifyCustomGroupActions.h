#pragma once
#include "MyActionInterface.h"

class GroupAction
{
public:
	inline void setTreeView(QTreeView* pTV){m_pTreeView = pTV;}
protected:
	QTreeView* m_pTreeView;
};

class AddPeopleGroupAction : public MyActionInterface, public GroupAction
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(AddPeopleGroupAction)

	virtual void MyAction();

	virtual void InitMyAction();

};

class AddGroupAction : public MyActionInterface, public GroupAction
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(AddGroupAction)

	virtual void MyAction();

	virtual void InitMyAction();

};

class UptGroupAction : public MyActionInterface, public GroupAction
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(UptGroupAction)

	virtual void MyAction();

	virtual void InitMyAction();

};

class DelGroupAction : public MyActionInterface, public GroupAction
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DelGroupAction)

		virtual void MyAction();

	virtual void InitMyAction();

};

class ModifyGroupContentAction : public MyActionInterface, public GroupAction
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ModifyGroupContentAction)

	virtual void MyAction();

	virtual void InitMyAction();

	QStandardItemModel* m_SourceModel;
	QStandardItemModel* m_DestinationModel;
};