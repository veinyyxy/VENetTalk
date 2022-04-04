#include "StdAfx.h"
#include "LoginUI.h"
#include "Client.h"
#include "ServerConfigDialog.h"

extern Client* g_pClient;

LoginUI::LoginUI(void)
{
	this->setObjectName(QString::fromLocal8Bit("LoginUI"));
	/*if (this->objectName().isEmpty())
	this->setStyleSheet(QString::fromUtf8("QWidget#LoginUI\n"
	"{\n"
	"	border: 1px solid transparent;\n"
	"	border-radius: 5px;\n"
	"	background: qradialgradient(\n"
	"		cx: 0.5, cy: -1.8,\n"
	"		fx: 0.5, fy: 0,\n"
	"		radius: 2, \n"
	"		stop: 0 #FFFFFF,\n"
	"		stop: 1 #CDDBE6);\n"
	"	font: bold;\n"
	"}\n"
	""));*/
	m_LoginUI.setupUi(this);
	//setObjectName(QString::fromLocal8Bit("LoginUI"));
	
	QSettings reg("./Info.ini"
		, QSettings::IniFormat);

	int iSavePass = 0;
	int iAutoLogin = 0;
	QString currentAccount;
	char* cPass = 0;

	iSavePass = reg.value("/IM/savePass").toInt();
	if(iSavePass == 1)
		m_LoginUI.SavePasswordCheckBox->setChecked(true);

	iAutoLogin = reg.value("/IM/autoLogin").toInt();
	if(iAutoLogin == 1)
		m_LoginUI.AutoLoginCheckBox->setChecked(true);

	currentAccount = reg.value("/IM/currentAccount").toString();
	m_LoginUI.AccountLineEdit->setText(currentAccount);

	QByteArray pass = reg.value("/IM/pass").toByteArray();
			
	QByteArray byteNewArray;
	for(int i = 0; i < pass.size(); i++)
	{
		char newPass = pass[i] - 2;
		byteNewArray.push_back(newPass);
	}

	QString strPass = QString::fromAscii(byteNewArray.data());

	m_LoginUI.PasswordLineEdit->setText(strPass);


	m_LoginUI.LoginButton->setShortcut(QKeySequence::InsertParagraphSeparator);
	connect(m_LoginUI.LoginButton, SIGNAL(clicked(bool)), SLOT(slot_loginButtonClick(bool)));
	connect(m_LoginUI.commandLinkButton, SIGNAL(clicked(bool)), SLOT(slot_setServer(bool)));
	
	//#LoginUI
	/*
	QString strName = objectName();
	this->setStyleSheet("QWidget{background:white;}");
	QFile qss("./login_ui.qss");
	qss.open(QFile::ReadOnly);
	setStyleSheet(qss.readAll());
	qss.close();*/
}


LoginUI::~LoginUI(void)
{
}

void LoginUI::slot_loginButtonClick(bool bc)
{
	QString strAccount = m_LoginUI.AccountLineEdit->text();
	QString strPassword = m_LoginUI.PasswordLineEdit->text();
	std::string strACC = strAccount.toStdString();
	std::string strPass = strPassword.toStdString();


	QSettings reg("./Info.ini"
		, QSettings::IniFormat);

	if(m_LoginUI.SavePasswordCheckBox->isChecked())
		reg.setValue("/IM/savePass", 1);
	else
		reg.setValue("/IM/savePass", 0);

	if(m_LoginUI.AutoLoginCheckBox->isChecked())
		reg.setValue("/IM/autoLogin", 1);
	else
		reg.setValue("/IM/autoLogin", 0);

	reg.setValue("/IM/currentAccount", strAccount);

	QByteArray byteArray = strPassword.toAscii();
	QByteArray byteNewArray;
	for(int i = 0; i < byteArray.size(); i++)
	{
		char newPass = byteArray[i] + 2;
		byteNewArray.push_back(newPass);
	}
	QString strCCC = QString::fromAscii(byteNewArray.data(), byteNewArray.count());
	reg.setValue("/IM/pass", strCCC);

	if(g_pClient)
	{
		g_pClient->Login(strACC.c_str(), strPass.c_str());
	}
}

void LoginUI::AutoLogin()
{
	if(m_LoginUI.AutoLoginCheckBox->isChecked())
		slot_loginButtonClick(true);
}

void LoginUI::slot_setServer( bool bc )
{
	QSettings configIni("./Config.ini"
		, QSettings::IniFormat);

	QVariant varValue;
	varValue = configIni.value("/SERVER_CONFIG/IMSERVER_IP");
	QString m_qstrServerIP = varValue.toString();
	varValue = configIni.value("/SERVER_CONFIG/IMSERVER_PORT");
	QString m_qstrServerPort = varValue.toString();
	varValue = configIni.value("/CLIENT_CONFIG/IMCLIENT_IP");
	QString m_qstrClientIP = varValue.toString();
	varValue = configIni.value("/CLIENT_CONFIG/IMCLIENT_PORT");
	QString m_qstrClientPort = varValue.toString();


	ServerConfigDialog scd;
	scd.m_myWidget.serverIP->setText(m_qstrServerIP);
	scd.m_myWidget.serverPort->setText(m_qstrServerPort);
	scd.m_myWidget.ClientIP->setText(m_qstrClientIP);
	scd.m_myWidget.ClientPort->setText(m_qstrClientPort);


	int iR = scd.exec();

	if(iR == 1)
	{
		m_qstrServerIP   = scd.m_myWidget.serverIP->text();
		m_qstrServerPort = scd.m_myWidget.serverPort->text();
		m_qstrClientIP   = scd.m_myWidget.ClientIP->text();
		m_qstrClientPort = scd.m_myWidget.ClientPort->text();

		configIni.setValue("/SERVER_CONFIG/IMSERVER_IP" ,m_qstrServerIP  );
		configIni.setValue("/SERVER_CONFIG/IMSERVER_PORT",m_qstrServerPort);
		configIni.setValue("/CLIENT_CONFIG/IMCLIENT_IP"  ,m_qstrClientIP  );
		configIni.setValue("/CLIENT_CONFIG/IMCLIENT_PORT",m_qstrClientPort);

		QMessageBox::warning(0, QString::fromLocal8Bit("及时通信客户端")
			, QString::fromLocal8Bit("由于修改了配置，必须重新启动客户端，才能加载新的配置。"));
	}
}
