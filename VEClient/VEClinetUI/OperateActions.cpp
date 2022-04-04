#include "StdAfx.h"
#include "OperateActions.h"
#include "Client.h"
#include "ClientMainUI.h"

ADD_MYACTION_CONTSTRUCTED_DECLARATION(SendFileAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SendMessageAction)
extern Client* g_pClient;

void SendFileAction::MyAction()
{
	bool bTest = m_TreeView->isVisible();
	bool bTest1 = m_TreeView1->isVisible();

	QStandardItem* pItem = 0;

	if(bTest)
	{
		QModelIndex index = m_TreeView->currentIndex();
		QStandardItemModel* peopleModel = dynamic_cast<QStandardItemModel*>
			(m_TreeView->model());
		pItem = peopleModel->itemFromIndex(index);
	}
	else if(bTest1)
	{
		QModelIndex index = m_TreeView1->currentIndex();
		QStandardItemModel* peopleModel = dynamic_cast<QStandardItemModel*>
			(m_TreeView1->model());
		pItem = peopleModel->itemFromIndex(index);
	}
	
	if(pItem == 0) return;
	

	NODE_PROPERTY nodeProperty = pItem->data().value<NODE_PROPERTY>();
	QString strAccount = nodeProperty.value("account").toString();
	int iUserState = nodeProperty.value("state").toInt();

	if(strAccount.size() > 0)
	{
		QString strFileName;
		if(m_iState == 0)
			strFileName = QFileDialog::getOpenFileName();
		else
			strFileName = QFileDialog::getExistingDirectory();

		if(strFileName.length() > 0)
		{
			if(iUserState == 1)
				g_pClient->SendFile(strFileName.toStdString(), strAccount.toStdString());
			else
			{
				QString strInfo 
					= QString::fromLocal8Bit("%1不在线，系统自动转为发送离线文件。")
					.arg(pItem->text());
				QMessageBox::StandardButton res 
					= QMessageBox::warning((QWidget*)0
					, QString::fromLocal8Bit("警告")
					, strInfo
					, QMessageBox::Ok|QMessageBox::Cancel);

				if(res == QMessageBox::Ok)
					g_pClient->SendFile(strFileName.toStdString()
					, strAccount.toStdString(), 1);
				else
					return;
			}
		}
	}
}

void SendFileAction::InitMyAction()
{
	m_TreeView = 0;
	m_iState = 0;
}

void SendMessageAction::MyAction()
{
	m_ClientMainUI->ToSendMessage();
}

void SendMessageAction::InitMyAction()
{
	//m_CurrentIndex = 0;
	m_ClientMainUI = 0;
}
