#pragma once
#include "ui_LoginUI.h"

class LoginUI :
	public QFrame
{
	Q_OBJECT
public:
	LoginUI(void);
	virtual ~LoginUI(void);
	Ui::LoginUI m_LoginUI;
	void AutoLogin();

public slots:
	void slot_loginButtonClick(bool bc);
	void slot_setServer(bool bc);
//signals:
//	void signal_Login(int iState);
	
};

