#pragma once
#include "ui_UserInformation.h"

class UserInformationDialog :
	public QDialog
{
	//Q_OBJECT
public:
	UserInformationDialog(void);
	~UserInformationDialog(void);
	Ui::UserInfoDialog* m_pMyWidget;
//public slots:

};

