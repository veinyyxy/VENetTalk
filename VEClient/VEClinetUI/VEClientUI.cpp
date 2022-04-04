// VEClinetUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VEClientUI.h"
#include "ClientMainUI.h"
#include "Client.h"

extern Client* GetClient();
extern void ReleaseClient();

Client* g_pClient = GetClient();

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	ClientMainUI MainUI;
	MainUI.show();

	////////////////////读取配置////////////////////////////
	g_pClient->SetMessageCallback(&MainUI);
	QSettings configIni("./Config.ini", QSettings::IniFormat);

	QVariant varValue;
	varValue = configIni.value("/SERVER_CONFIG/IMSERVER_IP");
	QString m_qstrServerIP = varValue.toString();
	varValue = configIni.value("/SERVER_CONFIG/IMSERVER_PORT");
	QString m_qstrServerPort = varValue.toString();
	varValue = configIni.value("/CLIENT_CONFIG/IMCLIENT_IP");
	QString m_qstrClientIP = varValue.toString();
	varValue = configIni.value("/CLIENT_CONFIG/IMCLIENT_PORT");
	QString m_qstrClientPort = varValue.toString();
	//////////////////////////使用上面的配置初始化客户端/////////////////////////////
	if(g_pClient)
	{
		g_pClient->StartClient(m_qstrServerIP.toStdString(), m_qstrServerPort.toUShort()
			, m_qstrClientIP.toStdString(), m_qstrClientPort.toUShort());
	}
	
	QFile qss("stylesheet.qss");
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());
	qss.close();

	return app.exec();
}