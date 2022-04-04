#include "StdAfx.h"
#include "UserInformationDialog.h"


UserInformationDialog::UserInformationDialog(void)
{
	m_pMyWidget = new Ui::UserInfoDialog();
	m_pMyWidget->setupUi(this);
	
	QComboBox* pCB = m_pMyWidget->HeaderImageComboBox;
	pCB->setIconSize(QSize(40,40));
	pCB->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	for(int i = 0; i < 11; i++)
	{
		QIcon icon1(QString("./image/head/%1.ico").arg(i+1));
		m_pMyWidget->HeaderImageComboBox->addItem(icon1, NULL, i+1);
	}
	
	//m_myWidget.HeaderImageComboBox->addItem(icon2, NULL, 2);
}


UserInformationDialog::~UserInformationDialog(void)
{
	delete m_pMyWidget;
}
