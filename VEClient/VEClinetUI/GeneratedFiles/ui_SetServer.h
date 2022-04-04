/********************************************************************************
** Form generated from reading UI file 'SetServer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETSERVER_H
#define UI_SETSERVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ServerConfigDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QLineEdit *serverPort;
    QLineEdit *serverIP;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QLineEdit *ClientIP;
    QLineEdit *ClientPort;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(392, 141);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(130, 110, 121, 21));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 181, 81));
        serverPort = new QLineEdit(groupBox);
        serverPort->setObjectName(QString::fromUtf8("serverPort"));
        serverPort->setGeometry(QRect(50, 50, 71, 20));
        serverPort->setMaxLength(5);
        serverIP = new QLineEdit(groupBox);
        serverIP->setObjectName(QString::fromUtf8("serverIP"));
        serverIP->setGeometry(QRect(50, 20, 101, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 71, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 71, 16));
        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(200, 10, 181, 80));
        ClientIP = new QLineEdit(groupBox_2);
        ClientIP->setObjectName(QString::fromUtf8("ClientIP"));
        ClientIP->setGeometry(QRect(50, 20, 101, 20));
        ClientPort = new QLineEdit(groupBox_2);
        ClientPort->setObjectName(QString::fromUtf8("ClientPort"));
        ClientPort->setGeometry(QRect(50, 50, 71, 20));
        ClientPort->setMaxLength(5);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 71, 16));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 50, 71, 16));

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "\346\234\215\345\212\241\345\231\250\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Dialog", "\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        serverPort->setInputMask(QApplication::translate("Dialog", "99999; ", 0, QApplication::UnicodeUTF8));
        serverIP->setInputMask(QApplication::translate("Dialog", "999.999.999.999; ", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "\347\253\257\345\217\243\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Dialog", "\345\256\242\346\210\267\347\253\257", 0, QApplication::UnicodeUTF8));
        ClientIP->setInputMask(QApplication::translate("Dialog", "999.999.999.999; ", 0, QApplication::UnicodeUTF8));
        ClientPort->setInputMask(QApplication::translate("Dialog", "99999; ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "\347\253\257\345\217\243\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServerConfigDialog: public Ui_ServerConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSERVER_H
