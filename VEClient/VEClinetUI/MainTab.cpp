#include "StdAfx.h"
#include "MainTab.h"
#include "Client.h"

extern Client* g_pClient;

void FindTreeNodeFromIdAndAccount(QStandardItem* pItem
	, const QString& strAccount
	, int iID, QStandardItem **pFindedItem)
{
	QVariant varTemp = pItem->data();
	if(varTemp.isValid())
	{
		NODE_PROPERTY nodeProperty = varTemp.value<NODE_PROPERTY>();

		if(iID == -100)
		{
			if(nodeProperty.value("account").toString() == strAccount)
			{
				*pFindedItem = pItem;
				return;
			}
		}
		else
		{
			if(nodeProperty.value("id").toInt() == iID)
			{
				*pFindedItem = pItem;
				return;
			}
		}
	}

	if(pItem->hasChildren())
	{
		int rowCount = pItem->rowCount();
		for(int i = 0; i < rowCount; i++)
		{
			QStandardItem* pChildItem = pItem->child(i);
			FindTreeNodeFromIdAndAccount(pChildItem, strAccount, iID, pFindedItem);
		}
	}
	else
	{
		return;
	}
}

MainTab::MainTab(void)
{
	m_MainTab.setupUi(this);
	//m_MainTab.ContactPeopleTreeView->setHeaderHidden(true);
	QStringList nameList;
	nameList.push_back(QString::fromLocal8Bit(""));
	nameList.push_back(QString::fromLocal8Bit("ук╨е"));
	//m_MainTab.ContactPeopleTreeView->setHeaderLabels(nameList);
	m_MainTab.ContactPeopleTreeView->setHeaderHidden(true);
	m_MainTab.OrganizationTreeView->setHeaderHidden(true);

	m_MainTab.DoNetReadToolButton->setVisible(false);

	m_MainTab.UserImageToolButton->setIcon(QIcon("./image/onlin.ico"));
	m_MainTab.UserImageToolButton->setIconSize(QSize(40, 40));
	m_MainTab.ContactPeopleTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_MainTab.OrganizationTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


MainTab::~MainTab(void)
{
}

void MainTab::SetUserName( const QString& strName )
{
	/*QStandardItem* pItem = FindUserNodeFromAccount(strName);
	QString strMyName = pItem->text();*/
	m_MainTab.UserNameLable->setText(strName);
}

void MainTab::SetUserList(QTreeWidgetItem* pItemModel)
{
	//int iCount = m_MainTab.ContactPeopleTreeView->topLevelItemCount();
	//m_MainTab.ContactPeopleTreeView->insertTopLevelItem(iCount, pItemModel);
}

void MainTab::SetModelState(QStandardItemModel* pModel, const QString& strAccount, int iState)
{
	int iRowCount = pModel->rowCount();

	for(int i = 0; i < iRowCount; i++)
	{
		QStandardItem* pItem = pModel->item(i);
		QStandardItem* pFindResult = 0;
		FindTreeNodeFromIdAndAccount(pItem, strAccount, -100, &pFindResult);

		if(!pFindResult) continue;
		//QString strTT = pFindResult->text();

		NODE_PROPERTY nodeProperty = pFindResult->data().value<NODE_PROPERTY>();
		std::string stdAcc;
		g_pClient->GetUserName(stdAcc);
		if(strAccount == QString::fromStdString(stdAcc))
		{
			SetUserName(pFindResult->text());
		}
		//
		//QString strAccount = nodeProperty.value("")
		NODE_PROPERTY::iterator ifind = nodeProperty.find("state");
		if(ifind != nodeProperty.end())
		{
			nodeProperty["state"] = iState;
		}
		else
			nodeProperty.insert("state", iState);

		pFindResult->setData(nodeProperty);

		switch(iState)
		{
		case 0:
			pFindResult->setIcon(QIcon("./image/offline.ico"));

			break;
		case 1:
			pFindResult->setIcon(QIcon("./image/onlin.ico"));
			break;
		default:
			pFindResult->setIcon(QIcon("./image/offline.ico"));
			break;
		}

	}
}

void MainTab::SetUserState( const QString& strName, int iState )
{
	QStandardItemModel* pOrgModel = (QStandardItemModel*)(m_MainTab.OrganizationTreeView->model());
	QStandardItemModel* pCustomModel = (QStandardItemModel*)(m_MainTab.ContactPeopleTreeView->model());

	m_MainTab.ContactPeopleTreeView->show();
	SetModelState(pOrgModel, strName, iState);

	m_MainTab.OrganizationTreeView->show();
	SetModelState(pCustomModel, strName, iState);
}

void MainTab::aaa()
{

}

QStandardItem* MainTab::FindUserNodeFromAccount( const QString& strAccount )
{
	QStandardItemModel* pModel = (QStandardItemModel*)(m_MainTab.OrganizationTreeView->model());

	int iRowCount = pModel->rowCount();

	for(int i = 0; i < iRowCount; i++)
	{
		QStandardItem* pItem = pModel->item(i);
		QStandardItem* pFindResult = 0;
		FindTreeNodeFromIdAndAccount(pItem, strAccount, -100, &pFindResult);
		if(pFindResult)
			return pFindResult;
	}

	return 0;
}

void MainTab::SetUserAccount( const QString& strAccount )
{

}

