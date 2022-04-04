#pragma once
#include "ui_MainTab.h"

class MainTab :
	public QFrame
{
	Q_OBJECT
public:
	MainTab(void);
	virtual ~MainTab(void);
	Ui::MainTab m_MainTab;
	void SetUserAccount(const QString& strAccount);
	void SetUserName(const QString& strName);
	void SetUserList(QTreeWidgetItem* pItemModel);
	void SetUserState(const QString& strName, int iState);
	QStandardItem* FindUserNodeFromAccount(const QString& strAccount);
	inline void SetModelState(QStandardItemModel* pModel, const QString& strName, int iState);
public slots:
	void aaa();
	
};

