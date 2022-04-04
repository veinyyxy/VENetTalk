/********************************************************************************
** Form generated from reading UI file 'LoginUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINUI_H
#define UI_LOGINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginUI
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *AccountLineEdit;
    QLabel *label_2;
    QLineEdit *PasswordLineEdit;
    QLabel *label_3;
    QCheckBox *SavePasswordCheckBox;
    QCheckBox *AutoLoginCheckBox;
    QPushButton *LoginButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QCommandLinkButton *commandLinkButton;
    QCommandLinkButton *commandLinkButton_2;

    void setupUi(QWidget *LoginUI)
    {
        if (LoginUI->objectName().isEmpty())
            LoginUI->setObjectName(QString::fromUtf8("LoginUI"));
        LoginUI->resize(198, 543);
        LoginUI->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(LoginUI);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(LoginUI);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        AccountLineEdit = new QLineEdit(LoginUI);
        AccountLineEdit->setObjectName(QString::fromUtf8("AccountLineEdit"));

        verticalLayout->addWidget(AccountLineEdit);

        label_2 = new QLabel(LoginUI);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_2);

        PasswordLineEdit = new QLineEdit(LoginUI);
        PasswordLineEdit->setObjectName(QString::fromUtf8("PasswordLineEdit"));
        PasswordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(PasswordLineEdit);

        label_3 = new QLabel(LoginUI);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_3);

        SavePasswordCheckBox = new QCheckBox(LoginUI);
        SavePasswordCheckBox->setObjectName(QString::fromUtf8("SavePasswordCheckBox"));

        verticalLayout->addWidget(SavePasswordCheckBox);

        AutoLoginCheckBox = new QCheckBox(LoginUI);
        AutoLoginCheckBox->setObjectName(QString::fromUtf8("AutoLoginCheckBox"));

        verticalLayout->addWidget(AutoLoginCheckBox);

        LoginButton = new QPushButton(LoginUI);
        LoginButton->setObjectName(QString::fromUtf8("LoginButton"));

        verticalLayout->addWidget(LoginButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        commandLinkButton = new QCommandLinkButton(LoginUI);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(commandLinkButton->sizePolicy().hasHeightForWidth());
        commandLinkButton->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("Segoe UI"));
        font.setPointSize(10);
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        commandLinkButton->setFont(font);
        commandLinkButton->setIconSize(QSize(15, 15));

        horizontalLayout->addWidget(commandLinkButton);

        commandLinkButton_2 = new QCommandLinkButton(LoginUI);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(100);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(commandLinkButton_2->sizePolicy().hasHeightForWidth());
        commandLinkButton_2->setSizePolicy(sizePolicy2);
        commandLinkButton_2->setFont(font);
        commandLinkButton_2->setIconSize(QSize(15, 15));
        commandLinkButton_2->setAutoRepeatDelay(283);

        horizontalLayout->addWidget(commandLinkButton_2);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(LoginUI);

        QMetaObject::connectSlotsByName(LoginUI);
    } // setupUi

    void retranslateUi(QWidget *LoginUI)
    {
        LoginUI->setWindowTitle(QApplication::translate("LoginUI", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LoginUI", "\350\264\246\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LoginUI", "\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LoginUI", "\347\231\273\345\275\225\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        SavePasswordCheckBox->setText(QApplication::translate("LoginUI", "\350\256\260\344\275\217\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        AutoLoginCheckBox->setText(QApplication::translate("LoginUI", "\350\207\252\345\212\250\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        LoginButton->setText(QApplication::translate("LoginUI", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        commandLinkButton->setText(QApplication::translate("LoginUI", "\346\234\215\345\212\241\345\231\250\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        commandLinkButton_2->setText(QApplication::translate("LoginUI", "\347\224\263\350\257\267\350\264\246\345\217\267", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoginUI: public Ui_LoginUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINUI_H
