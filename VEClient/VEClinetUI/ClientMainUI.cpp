#include "StdAfx.h"
#include "ui_MainUI.h"
#include "ClientMainUI.h"
#include "Client.h"
#include "ModifyCustomGroupActions.h"
#include "SetStateActions.h"
#include "OperateActions.h"
#include "UserActions.h"
#include "UserInformationDialog.h"

extern Client* g_pClient;

ClientMainUI::ClientMainUI() : m_TrayIcon(new QSystemTrayIcon(this))
	, m_SaveProgressDialog(this)
	, m_SendProgressDialog(this)
	, m_pMainActionGroup(this)
	, m_OrganizationModel(new QStandardItemModel())
	, m_CustomGroupsModel(new QStandardItemModel())
{
	m_ClientUI.setupUi(this);

	InitLoginUI();
	//m_LoginUI = QSharedPointer<LoginUI>::create();
	
	this->setCentralWidget(m_LoginUI.data());
	
	connect(this, SIGNAL(signal_Login(int)), SLOT(slot_Login(int)), Qt::QueuedConnection);
	qRegisterMetaType<P_MESSSAGE_DATA_UI>("P_MESSSAGE_DATA_UI");
	qRegisterMetaType<MESSSAGE_DATA_UI>("MESSSAGE_DATA_UI");
	qRegisterMetaType<P_USER_INFO_LIST>("P_USER_INFO_LIST");
	qRegisterMetaType<P_CUSTOM_GROUPS>("P_CUSTOM_GROUPS");
	qRegisterMetaType<std::string>("std::string&");
	qRegisterMetaType<P_USER_DETAIL_INFO>("P_USER_DETAIL_INFO");

	//连接消息接收槽
	connect(this, SIGNAL(signal_ReceiveMessage(P_MESSSAGE_DATA_UI)), SLOT(slot_ReceiveMessage(P_MESSSAGE_DATA_UI)), Qt::QueuedConnection);
	//连接登录槽
	connect(this, SIGNAL(signal_LogoutReply(int)), SLOT(slot_LogoutReply(int)), Qt::QueuedConnection);
	//消息派送定时器
	connect(&m_PatchMessageTimer, SIGNAL(timeout()), this, SLOT(slot_PatchMessage()));
	//未度消息提示定时器
	connect(&m_LookMessageTimer, SIGNAL(timeout()), this, SLOT(slot_lookMessage()));
	connect(&m_loginTimer, SIGNAL(timeout()), this, SLOT(slot_autoLogin()));
	//组织结构推送槽
	connect(this, SIGNAL(signal_OrganizationStructure(std::vector<ORG_NODE>*)), this, SLOT(slot_OrganizationStructure(std::vector<ORG_NODE>*)));
	connect(this, SIGNAL(signal_MyCustomGroup(P_CUSTOM_GROUPS)), this, SLOT(slot_MyCustomGroup(P_CUSTOM_GROUPS)));
	m_PatchMessageTimer.setInterval(800);
	m_LookMessageTimer.setInterval(300);
	m_FlushUserStateTimer.setInterval(1500);
	m_heartbeatTimer.setInterval(30000);
	m_loginTimer.setInterval(3);

	m_LookMessageTimer.start();
	//////////////////////设置系统托盘信息/////////////////////////////////
	m_TrayIcon->setIcon(QIcon("./image/TrayIcon.ico"));
	connect(m_TrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
		, this, SLOT(slot_activated(QSystemTrayIcon::ActivationReason)));
	////////////////////////////跟用户状态////////////////////////////////
	connect(&m_FlushUserStateTimer, SIGNAL(timeout()), this, SLOT(slot_FlushAllState()));
	connect(this, SIGNAL(signal_FlushAllState(P_USER_INFO_LIST)), this
		, SLOT(slot_UserStateHandle(P_USER_INFO_LIST)), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_heartbeat(int)), this, SLOT(slot_heartbeat(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(signal_setStateResult(int)), this, SLOT(slot_setStateResult(int)), Qt::QueuedConnection);
	
	connect(this, SIGNAL(signal_TransmitFileRequest(int, std::string&, std::string&, std::string&, int))
		, this, SLOT(slot_TransmitFileRequest(int, std::string&, std::string&, std::string&, int))
		, Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(signal_SendFileProgress(const std::string&, int, int)), this
	, SLOT(slot_SendFileProgress(const std::string&, int, int)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(signal_ReceiveFileProgress(const std::string&, int, int)), this
	, SLOT(slot_ReceiveFileProgress(const std::string&, int, int)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(signal_QueryAccountResult(int, std::string&)), this
		, SLOT(slot_QueryAccountResult(int, std::string&)));
	connect(this, SIGNAL(signal_UserDetailInformation(P_USER_DETAIL_INFO))
		, this, SLOT(slot_UserDetailInformation(P_USER_DETAIL_INFO)));
	connect(&m_pMainActionGroup, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_MainMenuToggled(QAction*)));

	connect(m_CustomGroupsModel, SIGNAL(itemChanged(QStandardItem*)),
		this, SLOT(slot_CustomGroupNameItemChanged(QStandardItem*)));

	connect(m_ClientUI.UserMenu, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_UserStateMenuTriggered(QAction*)));

	connect(m_ClientUI.OperatorMenu, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_UserStateMenuTriggered(QAction*)));
	//重新替换ACTION
	m_ActionMap.insert(m_ClientUI.action_3
		, new SetOnlineStateAction(m_ClientUI.UserMenu));
	m_ActionMap.insert(m_ClientUI.action_4
		, new SetHideStateAction(m_ClientUI.UserMenu));
	m_ActionMap.insert(m_ClientUI.action_5
		, new SetAwayStateAction(m_ClientUI.UserMenu));


	m_LogoutAction = new LogoutAction(m_ClientUI.UserMenu);
	m_LogoutAction->m_pClientUainUI = this;

	m_ActionMap.insert(m_ClientUI.action_O, m_LogoutAction);

	m_pSendFileAction = new SendFileAction(m_ClientUI.OperatorMenu);
	m_pSendOfflineFileAction = new SendFileAction(m_ClientUI.OperatorMenu);
	m_pSendOfflineFileAction->m_iState = 1;

	m_ActionMap.insert(m_ClientUI.action_10, m_pSendFileAction);
	m_ActionMap.insert(m_ClientUI.action, m_pSendOfflineFileAction);
	m_ActionMap.insert(m_ClientUI.action_6, new SetUserInfoAction(m_ClientUI.UserMenu));

	m_pSendMessageAction = new SendMessageAction(m_ClientUI.UserMenu);
	m_pSendMessageAction->m_ClientMainUI = this;
	m_ActionMap.insert(m_ClientUI.action_9, m_pSendMessageAction);


	///////////////////////////////////初始化菜单///////////////////////////////////////
	CreateUserStateMenu();
	connect(m_UserStateMenu, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_UserStateMenuTriggered(QAction*)));
	//////////////////////////////////////////////////////////////////////////

	m_loginTimer.start();
}

void ClientMainUI::InitMainUI()
{
	m_MainTab.value = new MainTab;
	connect(this->m_MainTab.data(), SIGNAL(customContextMenuRequested(const QPoint&))
		, this, SLOT(slot_ContactPeopleTreeView_CustomContextMenuRequested(const QPoint&)));
	//会话槽
	//connect(m_MainTab.value->m_MainTab.ContactPeopleTreeView, SIGNAL(itemDoubleClicked( QTreeWidgetItem*, int)), SLOT(slot_itemDoubleClicked( QTreeWidgetItem*, int)));
	//双击事件
	connect(m_MainTab.value->m_MainTab.ContactPeopleTreeView
		, SIGNAL(doubleClicked(const QModelIndex &))
		, SLOT(slot_doubleClicked ( const QModelIndex &)));

	connect(m_MainTab.value->m_MainTab.OrganizationTreeView
		, SIGNAL(doubleClicked(const QModelIndex &))
		, SLOT(slot_doubleClicked ( const QModelIndex &)));
	
	//单击事件
	connect(m_MainTab.value->m_MainTab.ContactPeopleTreeView
		, SIGNAL(clicked(const QModelIndex &))
		, SLOT(slot_clicked ( const QModelIndex &)));

	connect(m_MainTab.value->m_MainTab.OrganizationTreeView
		, SIGNAL(clicked(const QModelIndex &))
		, SLOT(slot_clicked ( const QModelIndex &)));

	connect(this->m_MainTab->m_MainTab.UserStateToolButton, SIGNAL(clicked(bool))
		, this, SLOT(slot_setUserStateMenu(bool)));
	connect(this->m_MainTab->m_MainTab.ContactPeopleTreeView
		, SIGNAL(customContextMenuRequested ( const QPoint)), this
		, SLOT(slot_ContactPeopleTreeView_CustomContextMenuRequested (const QPoint)));

	this->m_MainTab->m_MainTab.ContactPeopleTreeView
		->setContextMenuPolicy(Qt::CustomContextMenu);


	CreateModifyCustomGroupMenu();

	m_pSendFileAction->m_TreeView = m_MainTab->m_MainTab.ContactPeopleTreeView;
	m_pSendOfflineFileAction->m_TreeView = m_MainTab->m_MainTab.ContactPeopleTreeView;

	m_pSendFileAction->m_TreeView1 = m_MainTab->m_MainTab.OrganizationTreeView;
	m_pSendOfflineFileAction->m_TreeView1 = m_MainTab->m_MainTab.OrganizationTreeView;

	m_ClientUI.UserMenu->setEnabled(true);
	m_ClientUI.OperatorMenu->setEnabled(true);
}

void ClientMainUI::InitLoginUI()
{
	m_LoginUI.value = new LoginUI();
	QString strName = m_LoginUI->objectName();
	m_ClientUI.UserMenu->setEnabled(false);
	m_ClientUI.OperatorMenu->setEnabled(false);
	//m_LoginUI = QSharedPointer<LoginUI>::create();
}

ClientMainUI::~ClientMainUI(void)
{
	//m_TrayIcon->
	m_TrayIcon->hide();
}

void ClientMainUI::Show()
{
	
}

void ClientMainUI::Login( int iState, P_USER_INFO_LIST pUserInfo )
{
	std::string stdstrname;
	g_pClient->GetUserName(stdstrname);
	m_strUserName = QString::fromStdString(stdstrname);
	m_UserInfoList = *pUserInfo;
	m_MySate.iCurState = iState;

	emit signal_Login(iState);
}

void ClientMainUI::ReceiveMessage( int iType, std::string& strUser, unsigned char* pBuffer, int len )
{
	P_MESSSAGE_DATA_UI pData = new MESSSAGE_DATA_UI;
	
	pData->iUIType = 0;
	pData->iLen = len;
	pData->pReceiveBuffer = new char[len + 1];
	memset(pData->pReceiveBuffer, 0, len + 1);
	pData->iType = iType;
	pData->iState = 0;
	pData->m_struser = QString::fromStdString(strUser);

	memcpy_s(pData->pReceiveBuffer, len + 1, pBuffer, len);
	m_strSendedUserName = QString::fromStdString(strUser);
	emit signal_ReceiveMessage(/*iType, pBuffer, len*/pData);
}

void ClientMainUI::LogoutReply( int iState )
{
	emit signal_LogoutReply(iState);
}

void ClientMainUI::slot_Login( int iState )
{
	if(iState >= 0)
	{
		InitMainUI();
		//size_t iS = m_UserInfoList.size();
		//for(size_t i = 0; i < iS; i++)
		//{
		//	MessageCallback::USER_INFO ui = m_UserInfoList.at(i);
		//	QString strId = "%1";
		//	
		//	if(m_strUserName != QString::fromStdString(ui.name))
		//	{
		//		QTreeWidgetItem* pItem = new QTreeWidgetItem;
		//		pItem->setIcon(0, QIcon("./image/offline.ico"));
		//		//pItem->setText(0, strId.arg(ui.id - 1));
		//		pItem->setText(1, QString::fromStdString(ui.name));

		//		m_MainTab.value->SetUserList(pItem);
		//	}
		//}

		this->setCentralWidget(m_MainTab.value);
		m_TrayIcon->show();
		//获得组织结构信息
		g_pClient->GetOrganizationStructure();
		m_FlushUserStateTimer.start();
	}
	else
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("用户名或者密码错误。"));
		m_strUserName = "";
	}
}

void ClientMainUI::slot_ReceiveMessage( P_MESSSAGE_DATA_UI pData/*int iType, unsigned char* pBuffer, int len*/ )
{
#if 1
	InserMessageToPool(pData->m_struser, *pData);	
#else
	m_MessagePool.push_back(*pData);
#endif
	delete pData;
}

void ClientMainUI::slot_LogoutReply( int iS )
{
	m_FlushUserStateTimer.stop();
}

void ClientMainUI::CreateDialogFromName(QString& strAccount, QString& strName)
{
	USER_MAPTO_DIALOG::iterator iFinded = m_DialogPool.find(strAccount);
	if(iFinded != m_DialogPool.end())
	{
		iFinded.value()->show();
	}
	else
	{
		QString strText = QString::fromLocal8Bit("与%1的对话");
		strText = strText.arg(strName);

		MessageUI* pMessageUI = new MessageUI(this);
		pMessageUI->SetUserAccount(strAccount);
		pMessageUI->setWindowTitle(strText);
		pMessageUI->show();
		m_DialogPool.insert(strAccount, pMessageUI);
	}
}

void ClientMainUI::slot_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
	
}

void ClientMainUI::slot_PatchMessage()
{
#if 1
	//看看对话池能否找到相同用户的信息，能找到就显示消息。
	USER_MAPTO_MESSAGE::iterator iterBegin;
	

	for(iterBegin = m_MessagePool.begin(); iterBegin != m_MessagePool.end(); iterBegin++)
	{
		MESSAGE_VECTOR mv = iterBegin.value();
		int iCount = mv.count();

		QString strName = iterBegin.key();

		USER_MAPTO_DIALOG::iterator findResult = m_DialogPool.find(strName);

		if(findResult != m_DialogPool.end())
		{
			if(true/*findResult.value()->isVisible()*/)
			{
				QString strPlainText;
				size_t jNum = iterBegin.value().size();
				size_t startNum = 0;
				if(jNum > 10)
				{
					startNum = jNum - 10;
				}
				for(size_t j = startNum; j < jNum; j++)
				{
					iterBegin.value()[j].iState = 1;
					QString strSender;
					if(iterBegin.value()[j].iUIType == 1)
					{
						strSender = QString::fromLocal8Bit("我说");
					}
					else
					{
						QStandardItem* pItem = m_MainTab->FindUserNodeFromAccount(iterBegin.value()[j].m_struser);
						strSender = pItem->text() + QString::fromLocal8Bit("说");
					}
					QString strContent = QString::fromLocal8Bit(iterBegin.value()[j].pReceiveBuffer);
					strPlainText += strSender + " : \r\n  " + strContent + "\r\n";
				}
				findResult.value()->m_MessageUI.ReceiveTextEdit->setPlainText(strPlainText);
			}
		}
		else
		{
			m_NameStack.push(strName);
		}
	}
#else
	int iStartNum = 0;
	if(m_MessagePool.size() > 10)
	{
		iStartNum = m_MessagePool.size() - 10;
	}
	
	for(int i = iStartNum; i < m_MessagePool.size(); i++)
	{
		MESSSAGE_DATA_UI myMessage = m_MessagePool[i];

		QString strName = myMessage.m_struser;

		USER_MAPTO_DIALOG::iterator findResult = m_DialogPool.find(strName);

		if(findResult != m_DialogPool.end())
		{
			if(findResult.value()->isVisible())
			{
				//findResult.value()->m_MessageUI.ReceiveTextEdit->setPlainText(strPlainText);
			}
		}
		else
		{
			m_LookMessageTimer.start();
		}
	}
#endif
	m_PatchMessageTimer.stop();
}

void ClientMainUI::slot_activated( QSystemTrayIcon::ActivationReason reason )
{
	if(reason == QSystemTrayIcon::DoubleClick)
	{
		if(m_NameStack.size() == 0) return;
		QString strName = m_NameStack.pop();
		QStandardItem* pItem = m_MainTab->FindUserNodeFromAccount(strName);
		
		CreateDialogFromName(strName, pItem->text());

		m_PatchMessageTimer.start();
	}
}

void ClientMainUI::slot_lookMessage()
{
	static bool m_lsb = false;

	if(m_NameStack.size())
	{
		if(m_lsb)
		{
			m_TrayIcon->setIcon(QIcon("./image/TrayIcon.ico"));
			m_lsb = false;
		}
		else
		{
			m_TrayIcon->setIcon(QIcon("./image/onlin.ico"));
			m_lsb = true;
		}
	}
}

void ClientMainUI::closeEvent( QCloseEvent *event )
{
	g_pClient->Logout();
}

void ClientMainUI::InserMessageToPool(QString& strname, MESSSAGE_DATA_UI& pMessageData )
{
	USER_MAPTO_MESSAGE::iterator iFinded = m_MessagePool.find(strname);
	//插入消息池
	if(iFinded != m_MessagePool.end())
	{
		iFinded.value().push_back(pMessageData);
	}
	else
	{
		MESSAGE_VECTOR mv;
		mv.push_back(pMessageData);
		m_MessagePool.insert(strname, mv);
	}

	m_PatchMessageTimer.start();
}

void ClientMainUI::DeleteDialogFromPool( QString& strname )
{
	m_DialogPool.remove(strname);
}

void ClientMainUI::UserState( P_USER_INFO_LIST pUserState )
{
	emit signal_FlushAllState(pUserState);
}

void ClientMainUI::slot_FlushAllState()
{
	g_pClient->GetFriendState(-1, std::string(""));
}

void ClientMainUI::slot_UserStateHandle(P_USER_INFO_LIST pUIL)
{
	USER_INFO_LIST::iterator iter = pUIL->begin();

	for(iter; iter != pUIL->end(); iter++)
	{
		QString strName = QString::fromStdString(iter->name);

		m_MainTab->SetUserState(strName, iter->state);
	}

	delete pUIL;
}

void ClientMainUI::Heartbeat( int iBeat )
{
	emit signal_heartbeat(iBeat);
}

void ClientMainUI::slot_heartbeat( int iBeat )
{

}

void ClientMainUI::SetStateResult( int iState )
{
	emit signal_setStateResult(iState);
}

void ClientMainUI::slot_setStateResult( int iState )
{
	switch(iState)
	{
	case Client::ON_LINE:
		break;
	case Client::OFF_LINE:
		break;
	case Client::AWAY_TIME:
		break;
	case Client::HIDED:
		break;
	}
}

void ClientMainUI::slot_setUserStateMenu(bool)
{
	m_UserStateMenu->popup(cursor().pos());
}

void ClientMainUI::slot_ContactPeopleTreeView_CustomContextMenuRequested( const QPoint& pp )
{
	QModelIndex sil = m_MainTab->m_MainTab.ContactPeopleTreeView->currentIndex();
	
	QStandardItemModel* peopleModel = dynamic_cast<QStandardItemModel*>
		(m_MainTab->m_MainTab.ContactPeopleTreeView->model());
	QStandardItem* pItem = peopleModel->itemFromIndex(sil);
	if(!pItem)
	{
		pMgc->setEnabled(false);
	}
	else
	{
		if((!pItem->parent()))
		{
			pMgc->setEnabled(true);
		}
		else
			pMgc->setEnabled(false);
	}
	

	m_ModifyCustomGroupsMenu->popup(cursor().pos());
	
}

MessageCallback::REQUEST_RESULT ClientMainUI::TransmitFileRequest( int iState, std::string& srcName, std::string& fileName, std::string& strOutPath, int iType )
{
	return (emit signal_TransmitFileRequest(iState, srcName, fileName, strOutPath, iType));
}

int ClientMainUI::SendFileProgress( const std::string& strSendFileName, int iBlockCount, int iBlockID )
{
	return (emit signal_SendFileProgress(strSendFileName, iBlockCount, iBlockID));
}

int ClientMainUI::ReceiveFileProgress( const std::string& strReceiveFileName, int iBlockCount, int iBlockID )
{
	return (emit signal_ReceiveFileProgress(strReceiveFileName, iBlockCount, iBlockID));
}

MessageCallback::REQUEST_RESULT ClientMainUI::slot_TransmitFileRequest( int iState, std::string& srcName 
	, std::string& fileName, std::string& strOutPath, int iType )
{
	QString strInfo;
	QString Whose = QString::fromStdString(srcName);
	QString FileName = QString::fromStdString(fileName);

	QFileInfo fileInfo(FileName);

	QString localfileName = fileInfo.fileName();
	//if(iType == 2 || iType == 3)
		//localfileName = fileInfo.baseName();

	QMessageBox::StandardButton button;
	if(iState == 0)
	{
		//接收文件
		strInfo = 
			QString::fromLocal8Bit("接收") + Whose + 
			QString::fromLocal8Bit("发送的") + localfileName + 
			QString::fromLocal8Bit("文件吗？\r\n“确定”表示同意并接收，“取消表示拒绝接收。”");

		
		button = QMessageBox::information(this, QString::fromLocal8Bit("当前有文件接收")
			, strInfo, QMessageBox::Save | QMessageBox::Cancel);


	}
	else if(iState == 1)
	{
		//续传文件
		strInfo = 
			QString::fromLocal8Bit("继续接收从") + Whose + 
			QString::fromLocal8Bit("发送的") + localfileName + 
			QString::fromLocal8Bit("文件吗？\r\n“确定”表示同意并接收，“取消表示拒绝接收。”");

		button = QMessageBox::information(this, QString::fromLocal8Bit("当前有文件接收")
			, strInfo, QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Reset);
	}

	switch(button)
	{
	case QMessageBox::Save:
		{
			QString saveFile = QFileDialog::getSaveFileName(this
				, QString::fromLocal8Bit("保存文件"), localfileName);

			if(iState == 1)
			{
				QFileInfo saveInfo(saveFile);
				if(saveInfo.exists())
				{
					strOutPath = saveFile.toStdString();
					return MessageCallback::ACEPENT_CONTINUE;
				}
				else
				{
					QMessageBox::StandardButton re;
					re = QMessageBox::warning(this
						, QString::fromLocal8Bit("警告")
						, QString::fromLocal8Bit("续传时，请选择原来保存的文件。"));

					return MessageCallback::REFUSE;
				}
			}
			else if(iState == 0)
			{
				strOutPath = saveFile.toStdString();
				return MessageCallback::ACCEPT_RECEIVE;
			}
		}
		break;
	case QMessageBox::Cancel:
		{
			return MessageCallback::REFUSE;
		}
		break;
	case QMessageBox::Reset:
		{
			return MessageCallback::ACCEPT_RETRANSFER;
		}
		break;
	}
	
	return MessageCallback::REFUSE;
}

int ClientMainUI::slot_SendFileProgress( const std::string& strSendFileName , int iBlockCount, int iBlockID )
{
	m_SaveProgressDialog.show();
	m_SaveProgressDialog.setWindowTitle(QString::fromLocal8Bit("发送进度"));
	QLabel* pLabel = new QLabel(QString::fromStdString(strSendFileName));
	m_SaveProgressDialog.setLabel(pLabel);
	m_SaveProgressDialog.setMaximum(iBlockCount);
	m_SaveProgressDialog.setValue(iBlockID + 1);
	if(iBlockCount == iBlockID + 1)
		m_SaveProgressDialog.hide();
	return 0;
}

int ClientMainUI::slot_ReceiveFileProgress( const std::string& strReceiveFileName , int iBlockCount, int iBlockID )
{
	m_SaveProgressDialog.show();
	m_SendProgressDialog.setWindowTitle(QString::fromLocal8Bit("接收进度"));
	QLabel* pLabel = new QLabel(QString::fromStdString(strReceiveFileName));
	m_SaveProgressDialog.setLabel(pLabel);
	m_SaveProgressDialog.setMaximum(iBlockCount);
	m_SaveProgressDialog.setValue(iBlockID + 1);
	if(iBlockCount == iBlockID + 1)
		m_SaveProgressDialog.hide();
	return 0;
}

void ClientMainUI::slot_MainMenuToggled( QAction* pQA )
{
	if(pQA == m_ClientUI.action_2)
	{
		//g_pClient->ModifyGroup(0, )
	}
	else if(pQA == m_ClientUI.action_8)
	{

	}
	/*if(pQA == m_ClientUI.action_10)
	{
		QList<QTreeWidgetItem *> itemList = m_MainTab.value->m_MainTab.ContactPeopleTreeView->selectedItems();

		if(itemList.size() > 0)
		{
			QString strUserName = itemList.at(0)->text(1);
			QString strFileName = QFileDialog::getOpenFileName();
			g_pClient->SendFile(strFileName.toStdString(), strUserName.toStdString());
		}

	}*/
}

void ClientMainUI::OrganizationStructure( std::vector<ORG_NODE>* pNodes )
{
	std::vector<ORG_NODE>* myNodes = new std::vector<ORG_NODE>;
	*myNodes = *pNodes;
	emit signal_OrganizationStructure(myNodes);
}

void FindTreeNodeFromID(QStandardItem* pItem, int iID, QStandardItem **pFindedItem)
{
	QVariant varTemp = pItem->data();
	NODE_PROPERTY nodeProperty = varTemp.value<NODE_PROPERTY>();
	
	if(nodeProperty.value("id").toInt() == iID)
	{
		*pFindedItem = pItem;
		return;
	}

	if(pItem->hasChildren())
	{
		int rowCount = pItem->rowCount();
		for(int i = 0; i < rowCount; i++)
		{
			QStandardItem* pChildItem = pItem->child(i);
			FindTreeNodeFromID(pChildItem, iID, pFindedItem);
		}
	}
	else
	{
		return;
	}
}

void ClientMainUI::slot_OrganizationStructure( std::vector<ORG_NODE>* pNodes )
{
	int iRNum = 0;
	QStandardItem* pCurrentRoot = 0;
	int pParentID = -1;
	for(int i = 0; i < pNodes->size(); i++)
	{
		MessageCallback::ORG_NODE nodeData = pNodes->at(i);

		int iLevel = nodeData.iNodeId;
		int iParentNum = nodeData.iParentID;

		if(iParentNum == -1)
		{
			QStandardItem* pRoot = new QStandardItem(
				QIcon("./image/department.ico")
				, QString::fromStdString(nodeData.strName));

			NODE_PROPERTY nodeProperty;
			nodeProperty.insert("type", nodeData.iNodeType);
			nodeProperty.insert("id", nodeData.iNodeId);

			pRoot->setData(nodeProperty);
			//pRoot->setData(nodeData.iNodeId);
			m_OrganizationModel->setItem(iRNum, pRoot);
			pCurrentRoot = pRoot;
			iRNum++;
		}
		else
		{
			QStandardItem* pParentItem = 0;
			QIcon icon;
			if(nodeData.iNodeType == 0)
				icon.addFile("./image/department.ico");
			else
			{
				icon.addFile("./image/onlin.ico");
			}
			QStandardItem* pChildtItem = new QStandardItem(icon
				, QString::fromStdString(nodeData.strName));

			NODE_PROPERTY nodeProperty;
			nodeProperty.insert("type", nodeData.iNodeType);
			nodeProperty.insert("id", nodeData.iNodeId);

			pChildtItem->setData(nodeProperty);
			//pChildtItem->setData(nodeData.iNodeId);

			if(nodeData.iNodeType == 1)
			{
				m_AccountVector.push_back(nodeData.iNodeId);
				m_AccountNode.insert(nodeData.iNodeId, pChildtItem);
			}
			FindTreeNodeFromID(pCurrentRoot, nodeData.iParentID, &pParentItem);
			if(pParentItem)
				pParentItem->setChild(pParentItem->rowCount(), pChildtItem);
		}
	}

	this->m_MainTab->m_MainTab.OrganizationTreeView->setModel(m_OrganizationModel);

	//获得成员id;
	int iAccountNum = m_AccountNode.size();
	int iBufferNum = 0;
	int iModeNum = 0;
	int i50Count = 0;
	iBufferNum = iAccountNum / 50;
	iModeNum = iAccountNum % 50;
	if(iModeNum > 0)
	{
		i50Count = iBufferNum + 1;
	}
	else
	{
		i50Count = iBufferNum;
	}

	/*if(iBufferNum > 0 && i)
	{

	}*/
	for(int i = 0; i < i50Count; i++)
	{
		int iPreBufferSize = 0;
		int iBufferSize = 0;
		if(iBufferNum > 0 && iModeNum == 0)
		{
			iBufferSize = 50;
		}
		else if(iBufferNum == 0 && iModeNum > 0)
		{
			iBufferSize = iModeNum;
		}
		else if(iBufferNum > 0 && iModeNum > 0)
		{
			if(i == i50Count - 1)
			{
				iBufferSize = iModeNum;
			}
			else
			{
				iPreBufferSize = 50;
				iBufferSize = 50;
			}
		}
		else
		{
			return;
		}
		
		int* buffer = new int[iBufferSize];
		for(int j = 0; j < iBufferSize; j++)
		{
			int iNodeID = m_AccountVector.at(i * iPreBufferSize + j);
			buffer[j] = iNodeID;
		}
		g_pClient->QueryAccountFromNodeId(buffer, iBufferSize);
	}

	//获得自定义组
	g_pClient->GetCustomGroups();
}

int ClientMainUI::MyCustomGroup( P_CUSTOM_GROUPS pGroups )
{
	P_CUSTOM_GROUPS pMyGroups = new CUSTOM_GROUPS;
	*pMyGroups = *pGroups;
	emit signal_MyCustomGroup(pMyGroups);
	return 1;
}

int ClientMainUI::ModifyCustomGroupResult( int* iGroupID, int* iResult )
{
	return 0;
}

extern void addItem(QStandardItem* pSourceItem, QStandardItem* pDestItem);

void ClientMainUI::slot_MyCustomGroup( P_CUSTOM_GROUPS pGroups )
{
	for(int i = 0; i < pGroups->size(); i++)
	{
		QString strGroupName = QString::fromStdString(pGroups->at(i).name);
		int iType = pGroups->at(i).type;
		int iGroupID = pGroups->at(i).goupid;

		QIcon icon;
		if(iType == 0)
			icon.addFile("./image/department.ico");
		else
			icon.addFile("./image/onlin.ico");

		QStandardItem* pRoot = new QStandardItem(icon, strGroupName);
		NODE_PROPERTY nodeProperty;
		nodeProperty.insert("type", iType);
		nodeProperty.insert("id", iGroupID);
		nodeProperty.insert("not change", 1);
		pRoot->setData(nodeProperty);

		for(int j = 0; j < pGroups->at(i).nodes.size(); j++)
		{
			QStandardItem* pFindedItem = 0;
			FindTreeNodeFromID(m_OrganizationModel->item(0)
				, pGroups->at(i).nodes[j], &pFindedItem);
			if(pFindedItem != 0)
			{
				QStandardItem* pChild = pFindedItem->clone();
				if(iType == 0)
				{
					addItem(pFindedItem, pChild);
				}
				pRoot->setChild(j, pChild);
			}
		}

		m_CustomGroupsModel->setItem(i, pRoot);
	}

	this->m_MainTab->m_MainTab.ContactPeopleTreeView->setModel(m_CustomGroupsModel);
}

void ClientMainUI::CreateUserStateMenu()
{
	m_UserStateMenu = new QMenu(this->m_MainTab.data());
	//m_pActionGroup = new QActionGroup(m_UserStateMenu.data());

	QAction* pAction1 = new QAction(m_pActionGroup);
	QAction* pAction2 = new QAction(m_pActionGroup);
	QAction* pAction3 = new QAction(m_pActionGroup);

	pAction1->setText(QString::fromLocal8Bit("上线（&L）"));
	pAction2->setText(QString::fromLocal8Bit("隐身（&F）"));
	pAction3->setText(QString::fromLocal8Bit("离开（&A）"));

// 	m_pActionGroup->addAction(pAction1);
// 	m_pActionGroup->addAction(pAction2);
// 	m_pActionGroup->addAction(pAction3);

	m_UserStateMenu->addAction(pAction1);
	m_UserStateMenu->addAction(pAction2);
	m_UserStateMenu->addAction(pAction3);

	m_ActionMap.insert(pAction1, m_ActionMap.value(m_ClientUI.action_3));
	m_ActionMap.insert(pAction2, m_ActionMap.value(m_ClientUI.action_4));
	m_ActionMap.insert(pAction3, m_ActionMap.value(m_ClientUI.action_5));
}

void ClientMainUI::CreateModifyCustomGroupMenu()
{
	m_ModifyCustomGroupsMenu = new QMenu(this->m_MainTab.data());
	pAdd = new AddGroupAction(QString::fromLocal8Bit("添加部门组(&G)")
		, m_ModifyCustomGroupsMenu);
	pAddPeople = new AddPeopleGroupAction(QString::fromLocal8Bit(
		"添加联系人组(&P)"), m_ModifyCustomGroupsMenu);
	pDel = new DelGroupAction(QString::fromLocal8Bit("删除组(&D)")
		, m_ModifyCustomGroupsMenu);

	pMgc = new ModifyGroupContentAction(
		QString::fromLocal8Bit("编辑组内容(&M)")
		, m_ModifyCustomGroupsMenu);

	((AddGroupAction*)pAdd)->setTreeView(this->m_MainTab->m_MainTab.ContactPeopleTreeView);

	((AddPeopleGroupAction*)pAddPeople)->setTreeView(this->m_MainTab->m_MainTab.ContactPeopleTreeView);

	((DelGroupAction*)pDel)->setTreeView(this->m_MainTab->m_MainTab.ContactPeopleTreeView);

	((ModifyGroupContentAction*)pMgc)->setTreeView(this->m_MainTab->m_MainTab.ContactPeopleTreeView);
	((ModifyGroupContentAction*)pMgc)->m_SourceModel = m_OrganizationModel;
	((ModifyGroupContentAction*)pMgc)->m_DestinationModel = m_CustomGroupsModel;

	m_ModifyCustomGroupsMenu->addAction(pAdd);
	m_ModifyCustomGroupsMenu->addAction(pAddPeople);
	m_ModifyCustomGroupsMenu->addAction(pDel);
	m_ModifyCustomGroupsMenu->addSeparator();
	m_ModifyCustomGroupsMenu->addAction(pMgc);

	m_ActionMap.insert(m_ClientUI.action_2, pAdd);
	m_ActionMap.insert(m_ClientUI.action_8, pAddPeople);

	connect(this->m_ModifyCustomGroupsMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_ModifyGroup(QAction*)));
	/*m_ModifyCustomGroupsMenu->addAction(
	new AddGroupAction(QString::fromLocal8Bit("添加部门组"), m_ModifyCustomGroupsMenu));*/
	//QAction* pAction1 = new QAction;
	//m_ModifyCustomGroupsMenu->addAction(pAction1);
}

void ClientMainUI::slot_ModifyGroup( QAction* pAction )
{
	MyActionInterface* pMAI = dynamic_cast<MyActionInterface*>(pAction);
	if(pMAI)
		pMAI->MyAction();
}

void ClientMainUI::slot_CustomGroupNameItemChanged( QStandardItem * item )
{
	NODE_PROPERTY nodeProperty = item->data().value<NODE_PROPERTY>();
	int iNodeID = nodeProperty.value("id").toInt();
	if(nodeProperty.find("not change") != nodeProperty.end())
	{
		nodeProperty.remove("not change");
		item->setData(nodeProperty);
	}
	else
	{
		g_pClient->ModifyGroup(2, iNodeID, -1, item->text().toStdString());
	}
	
}

void ClientMainUI::slot_UserStateMenuTriggered( QAction* pAction)
{
	QMap<QAction*, QAction*>::iterator finded = m_ActionMap.find(pAction);

	if(finded != m_ActionMap.end())
	{
		MyActionInterface* pMAI = dynamic_cast<MyActionInterface*>(finded.value());
		if(pMAI)
			pMAI->MyAction();
	}
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(LogoutAction)

void ClientMainUI::setLoginUI()
{
	InitLoginUI();
	setCentralWidget(m_LoginUI.data());
}

int ClientMainUI::QueryAccountResult( int iNodeId, std::string& username )
{
	int iLocNodeId = iNodeId;
	std::string locName = username;
	emit signal_QueryAccountResult(iNodeId, username);
	return 1;
}

void ClientMainUI::slot_QueryAccountResult( int iNodeId, std::string& username )
{
	QStandardItem* pItem = m_AccountNode[iNodeId];
	if(pItem)
	{
		NODE_PROPERTY nodeProperty = pItem->data().value<NODE_PROPERTY>();
		nodeProperty.insert("account", QString::fromStdString(username));
		pItem->setData(nodeProperty);
	}
}

void ClientMainUI::slot_doubleClicked( const QModelIndex & index)
{
	//m_MainTab->m_MainTab.ContactPeopleTreeView
	QTreeView* pTreeView = dynamic_cast<QTreeView*>(sender());
	if(!pTreeView)
	{
		pTreeView = m_pSender;
	}
	QStandardItemModel* peopleModel = dynamic_cast<QStandardItemModel*>
		(pTreeView->model());
	QStandardItem* pItem = peopleModel->itemFromIndex(index);

	if(pItem)
	{
		NODE_PROPERTY nodeProperty = pItem->data().value<NODE_PROPERTY>();
		QString strAccount = nodeProperty.value("account").toString();
		if(strAccount.length() != 0)
			CreateDialogFromName(strAccount, pItem->text());
	}
	
	m_PatchMessageTimer.start();
}

void ClientMainUI::ClearPool()
{
	m_MessagePool.clear();
	m_DialogPool.clear();
}

void ClientMainUI::slot_clicked( const QModelIndex & index )
{
	m_pSender = dynamic_cast<QTreeView*>(sender());
	m_CurrentIndex = index;
}

void ClientMainUI::ToSendMessage()
{
	if(m_CurrentIndex.isValid())
		slot_doubleClicked(m_CurrentIndex);
}

int ClientMainUI::UserDetailInformation( P_USER_DETAIL_INFO info )
{
	P_USER_DETAIL_INFO localInfo = new USER_DETAIL_INFO;
	//memcpy_s(localInfo, sizeof(USER_DETAIL_INFO), info, sizeof(USER_DETAIL_INFO));

	localInfo->length	= info->length;
	localInfo->name		= info->name;
	localInfo->address	= info->address;
	localInfo->email	= info->email;
	localInfo->mobile	= info->mobile;
	localInfo->qq		= info->qq;
	localInfo->phone	= info->phone;
	memcpy_s(localInfo->icon, sizeof(localInfo->icon), info->icon
		, sizeof(info->icon));

	emit signal_UserDetailInformation(localInfo);
	return 1;
}

void ClientMainUI::slot_UserDetailInformation( P_USER_DETAIL_INFO info )
{
	UserInformationDialog uid;
	uid.m_pMyWidget->NamelineEdit->setText(QString::fromStdString(info->name));
	uid.m_pMyWidget->AddresslineEdit->setText(QString::fromStdString(info->address));
	uid.m_pMyWidget->EMaillineEdit->setText(QString::fromStdString(info->email));
	uid.m_pMyWidget->MobilelineEdit->setText(QString::fromStdString(info->mobile));
	uid.m_pMyWidget->QQlineEdit->setText(QString::fromStdString(info->qq));
	uid.m_pMyWidget->PhonelineEdit->setText(QString::fromStdString(info->phone));
	uid.m_pMyWidget->HeaderImageComboBox->setCurrentIndex(*(char*)(info->icon));


	int i = uid.exec();

	if(i == 1)
	{
		USER_DETAIL_INFO setedInfo;
		setedInfo.length = 0;
		memset(setedInfo.icon, 0, sizeof(setedInfo.icon));
		setedInfo.name = uid.m_pMyWidget->NamelineEdit->text().toAscii().data();
		setedInfo.address = uid.m_pMyWidget->AddresslineEdit->text().toAscii().data();
		setedInfo.email = uid.m_pMyWidget->EMaillineEdit->text().toAscii().data();
		setedInfo.mobile = uid.m_pMyWidget->MobilelineEdit->text().toAscii().data();
		setedInfo.qq = uid.m_pMyWidget->QQlineEdit->text().toAscii().data();
		setedInfo.phone = uid.m_pMyWidget->PhonelineEdit->text().toAscii().data();
		*(char*)(setedInfo.icon) = uid.m_pMyWidget->HeaderImageComboBox->currentIndex();

		g_pClient->SetUserInformation((char*)&setedInfo, sizeof(USER_DETAIL_INFO));
	}
}

void ClientMainUI::slot_autoLogin()
{
	m_LoginUI->AutoLogin();
	m_loginTimer.stop();
}

void LogoutAction::MyAction()
{
	if(m_pClientUainUI)
	{
		g_pClient->Logout();
		m_pClientUainUI->setLoginUI();
		m_pClientUainUI->ClearPool();
	}
}

void LogoutAction::InitMyAction()
{
	m_pClientUainUI = 0;
}