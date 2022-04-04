#pragma once
#include "ui_SetServer.h"

class ServerConfigDialog :
	public QDialog
{
public:
	ServerConfigDialog(void);
	~ServerConfigDialog(void);
	Ui::ServerConfigDialog m_myWidget;
};

