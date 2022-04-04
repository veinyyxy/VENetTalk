#include "StdAfx.h"
#include "MessageUI.h"
#include "Client.h"
#include "ClientMainUI.h"

extern Client* g_pClient;

MessageUI::MessageUI(QWidget* pParent) : QMainWindow(pParent), m_pParentWindow(pParent)
{
	m_MessageUI.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(m_MessageUI.SendMessagePushButton, SIGNAL(clicked(bool)), SLOT(slot_clicked(bool)));
	connect(m_MessageUI.CleanPushButton, SIGNAL(clicked(bool)), SLOT(slot_clear_screen(bool)));
	
	m_MessageUI.pushButton_8->setIcon(QIcon("./image/onlin.ico"));
	m_MessageUI.pushButton_8->setText("");
	m_MessageUI.pushButton_8->setIconSize(m_MessageUI.pushButton_8->size());

	connect(m_MessageUI.menu, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_menu_action(QAction*)));

	connect(m_MessageUI.menu_E, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_menu_action(QAction*)));

	m_MessageUI.ReceiveTextEdit->setReadOnly(true);
	m_MessageUI.pushButton_3->setHidden(true);

	m_menu = new QMenu(this);
	m_pAction1 = new QAction(QString::fromLocal8Bit("Ctrl+回车"), m_menu);
	m_pAction2 = new QAction(QString::fromLocal8Bit("回车"), m_menu);
	m_menu->addAction(m_pAction1);
	m_menu->addAction(m_pAction2);

	connect(m_menu, SIGNAL(triggered(QAction*)), this
		, SLOT(slot_menu_action(QAction*)));

	connect(m_MessageUI.pushButton_4, SIGNAL(clicked(bool))
		, this, SLOT(slot_setButtonMode(bool)));
}

 
MessageUI::~MessageUI(void)
{
}

void MessageUI::slot_clicked( bool bc )
{
	std::string stdstrName = m_strUserAccount.toStdString();
	QString strSendText = m_MessageUI.SendTextEdit->toPlainText();
	std::string strText = strSendText.toStdString();
	g_pClient->SetMessage(0, stdstrName, (char*)(strText.c_str()), strText.length());

	ClientMainUI* pCMUI = dynamic_cast<ClientMainUI*>(m_pParentWindow);
	ClientMainUI::MESSSAGE_DATA_UI messageData;

	messageData.iUIType = 1;
	messageData.iLen = strText.length();
	messageData.pReceiveBuffer = new char[messageData.iLen + 1];
	memset(messageData.pReceiveBuffer, 0, messageData.iLen + 1);
	memcpy_s(messageData.pReceiveBuffer, messageData.iLen + 1, strText.c_str(), strText.length());
	messageData.iType = 0;
	messageData.iState = 0;
	messageData.m_struser = QString::fromStdString(stdstrName);
	messageData.m_strName = m_strUserName;

	pCMUI->InserMessageToPool(messageData.m_struser, messageData);

	/*QString strSendContent = QString::fromStdString(stdstrName) + QString::fromLocal8Bit("：\r\n") + strSendText;
	m_MessageUI.ReceiveTextEdit->setPlainText(strSendContent);*/

	m_MessageUI.SendTextEdit->clear();
}

void MessageUI::closeEvent( QCloseEvent *event )
{
	ClientMainUI* pCMUI = dynamic_cast<ClientMainUI*>(m_pParentWindow);
	pCMUI->DeleteDialogFromPool(m_strUserAccount);
}

void MessageUI::slot_menu_action( QAction* pA )
{
	if(pA == m_MessageUI.action_4)
	{
		m_MessageUI.ReceiveTextEdit->cut();
	}
	else if(pA == m_MessageUI.action_5)
	{
		m_MessageUI.ReceiveTextEdit->copy();
	}
	else if(pA == m_MessageUI.action_6)
	{
		m_MessageUI.ReceiveTextEdit->paste();
	}
	else if(pA == m_MessageUI.action_7)
	{
		//m_MessageUI.ReceiveTextEdit->d
	}
	else if(pA == m_MessageUI.action_9)
	{
		m_MessageUI.ReceiveTextEdit->selectAll();
	}
	else if(pA == m_MessageUI.action_10)
	{
		slot_clear_screen( true );
	}
	else if(pA == m_MessageUI.action_3)
	{
		this->close();
	}
	else if(pA == m_pAction1)
	{
		QKeySequence keyseq(Qt::CTRL + Qt::Key_Enter);
		m_MessageUI.SendMessagePushButton->setFocus();
		m_MessageUI.SendMessagePushButton->setShortcut(keyseq);
	}
	else if(pA == m_pAction2)
	{
		m_MessageUI.SendMessagePushButton->setFocus();
		m_MessageUI.SendMessagePushButton->setShortcut(Qt::Key_Enter);
	}
}

void MessageUI::slot_clear_screen( bool )
{
	m_MessageUI.ReceiveTextEdit->clear();
}

void MessageUI::slot_setButtonMode( bool )
{
	m_menu->popup(cursor().pos());
}
