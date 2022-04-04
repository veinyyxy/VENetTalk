/********************************************************************************
** Form generated from reading UI file 'MainUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientMainUI
{
public:
    QAction *action_L;
    QAction *action_O;
    QAction *action_3;
    QAction *action_4;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QAction *action_Q;
    QAction *action_9;
    QAction *action_10;
    QAction *action_11;
    QAction *action_B;
    QAction *action_13;
    QAction *action_2;
    QAction *action_8;
    QAction *action;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *UserMenu;
    QMenu *menu;
    QMenu *OperatorMenu;
    QMenu *menu_2;
    QMenu *HelpMenu;

    void setupUi(QMainWindow *ClientMainUI)
    {
        if (ClientMainUI->objectName().isEmpty())
            ClientMainUI->setObjectName(QString::fromUtf8("ClientMainUI"));
        ClientMainUI->resize(199, 600);
        action_L = new QAction(ClientMainUI);
        action_L->setObjectName(QString::fromUtf8("action_L"));
        action_L->setEnabled(false);
        action_O = new QAction(ClientMainUI);
        action_O->setObjectName(QString::fromUtf8("action_O"));
        action_3 = new QAction(ClientMainUI);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_4 = new QAction(ClientMainUI);
        action_4->setObjectName(QString::fromUtf8("action_4"));
        action_5 = new QAction(ClientMainUI);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(ClientMainUI);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_7 = new QAction(ClientMainUI);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        action_Q = new QAction(ClientMainUI);
        action_Q->setObjectName(QString::fromUtf8("action_Q"));
        action_9 = new QAction(ClientMainUI);
        action_9->setObjectName(QString::fromUtf8("action_9"));
        action_10 = new QAction(ClientMainUI);
        action_10->setObjectName(QString::fromUtf8("action_10"));
        action_11 = new QAction(ClientMainUI);
        action_11->setObjectName(QString::fromUtf8("action_11"));
        action_B = new QAction(ClientMainUI);
        action_B->setObjectName(QString::fromUtf8("action_B"));
        action_13 = new QAction(ClientMainUI);
        action_13->setObjectName(QString::fromUtf8("action_13"));
        action_2 = new QAction(ClientMainUI);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_8 = new QAction(ClientMainUI);
        action_8->setObjectName(QString::fromUtf8("action_8"));
        action = new QAction(ClientMainUI);
        action->setObjectName(QString::fromUtf8("action"));
        centralwidget = new QWidget(ClientMainUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ClientMainUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ClientMainUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 199, 23));
        UserMenu = new QMenu(menubar);
        UserMenu->setObjectName(QString::fromUtf8("UserMenu"));
        menu = new QMenu(UserMenu);
        menu->setObjectName(QString::fromUtf8("menu"));
        OperatorMenu = new QMenu(menubar);
        OperatorMenu->setObjectName(QString::fromUtf8("OperatorMenu"));
        menu_2 = new QMenu(OperatorMenu);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        HelpMenu = new QMenu(menubar);
        HelpMenu->setObjectName(QString::fromUtf8("HelpMenu"));
        ClientMainUI->setMenuBar(menubar);

        menubar->addAction(UserMenu->menuAction());
        menubar->addAction(OperatorMenu->menuAction());
        menubar->addAction(HelpMenu->menuAction());
        UserMenu->addAction(action_L);
        UserMenu->addAction(action_O);
        UserMenu->addAction(menu->menuAction());
        UserMenu->addAction(action_6);
        UserMenu->addAction(action_7);
        UserMenu->addSeparator();
        UserMenu->addAction(action_Q);
        menu->addSeparator();
        menu->addAction(action_3);
        menu->addAction(action_4);
        menu->addAction(action_5);
        OperatorMenu->addAction(action_9);
        OperatorMenu->addAction(action_10);
        OperatorMenu->addAction(action);
        OperatorMenu->addAction(action_11);
        OperatorMenu->addAction(menu_2->menuAction());
        menu_2->addAction(action_2);
        menu_2->addAction(action_8);
        HelpMenu->addAction(action_B);
        HelpMenu->addSeparator();
        HelpMenu->addAction(action_13);

        retranslateUi(ClientMainUI);

        QMetaObject::connectSlotsByName(ClientMainUI);
    } // setupUi

    void retranslateUi(QMainWindow *ClientMainUI)
    {
        ClientMainUI->setWindowTitle(QApplication::translate("ClientMainUI", "VE_\345\256\242\346\210\267\347\253\257", 0, QApplication::UnicodeUTF8));
        action_L->setText(QApplication::translate("ClientMainUI", "\347\231\273\345\275\225(&L)", 0, QApplication::UnicodeUTF8));
        action_O->setText(QApplication::translate("ClientMainUI", "\346\263\250\351\224\200(&O)", 0, QApplication::UnicodeUTF8));
        action_3->setText(QApplication::translate("ClientMainUI", "\345\234\250\347\272\277", 0, QApplication::UnicodeUTF8));
        action_4->setText(QApplication::translate("ClientMainUI", "\351\232\220\350\272\253", 0, QApplication::UnicodeUTF8));
        action_5->setText(QApplication::translate("ClientMainUI", "\347\246\273\345\274\200", 0, QApplication::UnicodeUTF8));
        action_6->setText(QApplication::translate("ClientMainUI", "\344\270\252\344\272\272\350\256\276\347\275\256(&P)...", 0, QApplication::UnicodeUTF8));
        action_7->setText(QApplication::translate("ClientMainUI", "\347\263\273\347\273\237\350\256\276\347\275\256(&S)...", 0, QApplication::UnicodeUTF8));
        action_Q->setText(QApplication::translate("ClientMainUI", "\351\200\200\345\207\272(Q)...", 0, QApplication::UnicodeUTF8));
        action_9->setText(QApplication::translate("ClientMainUI", "\345\217\221\351\200\201\345\217\212\346\227\266\346\266\210\346\201\257(&M)...", 0, QApplication::UnicodeUTF8));
        action_10->setText(QApplication::translate("ClientMainUI", "\345\217\221\351\200\201\346\226\207\344\273\266(&F)...", 0, QApplication::UnicodeUTF8));
        action_11->setText(QApplication::translate("ClientMainUI", "\345\217\221\351\200\201\345\271\277\346\222\255\346\266\210\346\201\257(&T)...", 0, QApplication::UnicodeUTF8));
        action_B->setText(QApplication::translate("ClientMainUI", "\345\270\256\345\212\251(&H)", 0, QApplication::UnicodeUTF8));
        action_13->setText(QApplication::translate("ClientMainUI", "\345\205\263\344\272\216...", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("ClientMainUI", "\346\267\273\345\212\240\351\203\250\351\227\250\345\210\206\347\273\204...", 0, QApplication::UnicodeUTF8));
        action_8->setText(QApplication::translate("ClientMainUI", "\346\267\273\345\212\240\350\201\224\347\263\273\344\272\272\345\210\206\347\273\204...", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("ClientMainUI", "\345\217\221\351\200\201\346\226\207\344\273\266\345\244\271(&O)...", 0, QApplication::UnicodeUTF8));
        UserMenu->setTitle(QApplication::translate("ClientMainUI", "\347\224\250\346\210\267(&U)", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("ClientMainUI", "\346\210\221\347\232\204\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        OperatorMenu->setTitle(QApplication::translate("ClientMainUI", "\346\223\215\344\275\234(A)", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("ClientMainUI", "\350\207\252\345\256\232\344\271\211\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8));
        HelpMenu->setTitle(QApplication::translate("ClientMainUI", "\345\270\256\345\212\251(&H)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ClientMainUI: public Ui_ClientMainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
