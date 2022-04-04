/********************************************************************************
** Form generated from reading UI file 'MessageUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEUI_H
#define UI_MESSAGEUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageUI
{
public:
    QAction *action;
    QAction *action_3;
    QAction *action_4;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QAction *action_9;
    QAction *action_10;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTextEdit *SendTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *CleanPushButton;
    QTextEdit *ReceiveTextEdit;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_6;
    QPushButton *SendMessagePushButton;
    QPushButton *pushButton_4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_8;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_E;

    void setupUi(QMainWindow *MessageUI)
    {
        if (MessageUI->objectName().isEmpty())
            MessageUI->setObjectName(QString::fromUtf8("MessageUI"));
        MessageUI->resize(618, 457);
        action = new QAction(MessageUI);
        action->setObjectName(QString::fromUtf8("action"));
        action_3 = new QAction(MessageUI);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_4 = new QAction(MessageUI);
        action_4->setObjectName(QString::fromUtf8("action_4"));
        action_4->setVisible(false);
        action_5 = new QAction(MessageUI);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(MessageUI);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_6->setVisible(false);
        action_7 = new QAction(MessageUI);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        action_7->setVisible(false);
        action_9 = new QAction(MessageUI);
        action_9->setObjectName(QString::fromUtf8("action_9"));
        action_10 = new QAction(MessageUI);
        action_10->setObjectName(QString::fromUtf8("action_10"));
        centralwidget = new QWidget(MessageUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setSpacing(1);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        SendTextEdit = new QTextEdit(centralwidget);
        SendTextEdit->setObjectName(QString::fromUtf8("SendTextEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SendTextEdit->sizePolicy().hasHeightForWidth());
        SendTextEdit->setSizePolicy(sizePolicy);
        SendTextEdit->setMinimumSize(QSize(0, 20));
        SendTextEdit->setMaximumSize(QSize(16777215, 80));

        gridLayout->addWidget(SendTextEdit, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        CleanPushButton = new QPushButton(centralwidget);
        CleanPushButton->setObjectName(QString::fromUtf8("CleanPushButton"));

        horizontalLayout_2->addWidget(CleanPushButton);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        ReceiveTextEdit = new QTextEdit(centralwidget);
        ReceiveTextEdit->setObjectName(QString::fromUtf8("ReceiveTextEdit"));

        gridLayout->addWidget(ReceiveTextEdit, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        horizontalLayout_3->addWidget(pushButton_5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        horizontalLayout_3->addWidget(pushButton_6);

        SendMessagePushButton = new QPushButton(centralwidget);
        SendMessagePushButton->setObjectName(QString::fromUtf8("SendMessagePushButton"));

        horizontalLayout_3->addWidget(SendMessagePushButton);

        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);
        pushButton_4->setMaximumSize(QSize(15, 16777215));

        horizontalLayout_3->addWidget(pushButton_4);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy2);
        pushButton_8->setMinimumSize(QSize(100, 250));

        verticalLayout_2->addWidget(pushButton_8);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MessageUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MessageUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 618, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_E = new QMenu(menubar);
        menu_E->setObjectName(QString::fromUtf8("menu_E"));
        MessageUI->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_E->menuAction());
        menu->addAction(action);
        menu->addSeparator();
        menu->addAction(action_3);
        menu_E->addAction(action_4);
        menu_E->addAction(action_5);
        menu_E->addAction(action_6);
        menu_E->addAction(action_7);
        menu_E->addSeparator();
        menu_E->addAction(action_9);
        menu_E->addAction(action_10);

        retranslateUi(MessageUI);

        QMetaObject::connectSlotsByName(MessageUI);
    } // setupUi

    void retranslateUi(QMainWindow *MessageUI)
    {
        MessageUI->setWindowTitle(QApplication::translate("MessageUI", "MainWindow", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("MessageUI", "\346\237\245\347\234\213\350\201\224\347\263\273\344\272\272\350\265\204\346\226\231(&V)", 0, QApplication::UnicodeUTF8));
        action_3->setText(QApplication::translate("MessageUI", "\345\205\263\351\227\255\345\257\271\350\257\235(&C)", 0, QApplication::UnicodeUTF8));
        action_4->setText(QApplication::translate("MessageUI", "\345\211\252\345\210\207(&T)", 0, QApplication::UnicodeUTF8));
        action_5->setText(QApplication::translate("MessageUI", "\346\213\267\350\264\235(&C)", 0, QApplication::UnicodeUTF8));
        action_6->setText(QApplication::translate("MessageUI", "\347\262\230\350\264\264(&V)", 0, QApplication::UnicodeUTF8));
        action_7->setText(QApplication::translate("MessageUI", "\345\210\240\351\231\244(&D)", 0, QApplication::UnicodeUTF8));
        action_9->setText(QApplication::translate("MessageUI", "\345\205\250\351\200\211(&A)", 0, QApplication::UnicodeUTF8));
        action_10->setText(QApplication::translate("MessageUI", "\346\270\205\345\261\217(&R)", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MessageUI", "\345\255\227\344\275\223...", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MessageUI", "\350\241\250\346\203\205", 0, QApplication::UnicodeUTF8));
        CleanPushButton->setText(QApplication::translate("MessageUI", "\346\270\205\345\261\217(&R)", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MessageUI", "\346\266\210\346\201\257\350\256\260\345\275\225(&H)...", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MessageUI", "\345\205\263\351\227\255(&C)", 0, QApplication::UnicodeUTF8));
        SendMessagePushButton->setText(QApplication::translate("MessageUI", "\345\217\221\351\200\201(&S)", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MessageUI", "\342\206\223", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("MessageUI", "\347\205\247\347\211\207", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MessageUI", "\346\266\210\346\201\257(&M)", 0, QApplication::UnicodeUTF8));
        menu_E->setTitle(QApplication::translate("MessageUI", "\347\274\226\350\276\221(&E)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MessageUI: public Ui_MessageUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEUI_H
