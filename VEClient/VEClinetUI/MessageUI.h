#pragma once
#include "ui_MessageUI.h"


class MessageUI :
	public QMainWindow
{
	Q_OBJECT
public:
	MessageUI(QWidget* pParent = 0);
	virtual ~MessageUI(void);
	Ui::MessageUI m_MessageUI;
	inline void SetUserAccount(const QString& strName){m_strUserAccount = strName;}
	inline void SetuserName(const QString& strName){m_strUserName = strName;}
	void closeEvent(QCloseEvent *event);
public slots:
	void slot_clicked(bool bc);
	void slot_menu_action(QAction* pA);
	void slot_clear_screen(bool);
	void slot_setButtonMode(bool);
private:
	QString m_strUserName;
	QString m_strUserAccount;
	QWidget* m_pParentWindow;
	QMenu* m_menu;
	QAction* m_pAction1;
	QAction* m_pAction2;

};

