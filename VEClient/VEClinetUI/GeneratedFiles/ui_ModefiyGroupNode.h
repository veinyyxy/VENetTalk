/********************************************************************************
** Form generated from reading UI file 'ModefiyGroupNode.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEFIYGROUPNODE_H
#define UI_MODEFIYGROUPNODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ModifyGroupNodeDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QSpacerItem *horizontalSpacer;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *AddButton;
    QPushButton *DelButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *FindButton;
    QTreeView *treeView_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *OkButton;
    QPushButton *CancelButton;

    void setupUi(QDialog *ModifyGroupNodeDialog)
    {
        if (ModifyGroupNodeDialog->objectName().isEmpty())
            ModifyGroupNodeDialog->setObjectName(QString::fromUtf8("ModifyGroupNodeDialog"));
        ModifyGroupNodeDialog->resize(582, 450);
        ModifyGroupNodeDialog->setLayoutDirection(Qt::LeftToRight);
        ModifyGroupNodeDialog->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(ModifyGroupNodeDialog);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setVerticalSpacing(2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton_2 = new QRadioButton(ModifyGroupNodeDialog);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout->addWidget(radioButton_2);

        radioButton = new QRadioButton(ModifyGroupNodeDialog);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 9);

        treeView = new QTreeView(ModifyGroupNodeDialog);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 5, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        AddButton = new QPushButton(ModifyGroupNodeDialog);
        AddButton->setObjectName(QString::fromUtf8("AddButton"));
        AddButton->setMaximumSize(QSize(50, 16777215));

        verticalLayout->addWidget(AddButton);

        DelButton = new QPushButton(ModifyGroupNodeDialog);
        DelButton->setObjectName(QString::fromUtf8("DelButton"));
        DelButton->setMaximumSize(QSize(50, 16777215));

        verticalLayout->addWidget(DelButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout->addLayout(verticalLayout, 5, 4, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit = new QLineEdit(ModifyGroupNodeDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        FindButton = new QPushButton(ModifyGroupNodeDialog);
        FindButton->setObjectName(QString::fromUtf8("FindButton"));

        horizontalLayout_2->addWidget(FindButton);


        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

        treeView_2 = new QTreeView(ModifyGroupNodeDialog);
        treeView_2->setObjectName(QString::fromUtf8("treeView_2"));

        gridLayout->addWidget(treeView_2, 5, 8, 1, 1);

        label_2 = new QLabel(ModifyGroupNodeDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 4, 8, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        OkButton = new QPushButton(ModifyGroupNodeDialog);
        OkButton->setObjectName(QString::fromUtf8("OkButton"));

        horizontalLayout_3->addWidget(OkButton);

        CancelButton = new QPushButton(ModifyGroupNodeDialog);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));

        horizontalLayout_3->addWidget(CancelButton);


        gridLayout->addLayout(horizontalLayout_3, 6, 0, 1, 9);


        retranslateUi(ModifyGroupNodeDialog);

        QMetaObject::connectSlotsByName(ModifyGroupNodeDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyGroupNodeDialog)
    {
        ModifyGroupNodeDialog->setWindowTitle(QApplication::translate("ModifyGroupNodeDialog", "\347\274\226\350\276\221\347\224\250\346\210\267\350\207\252\345\256\232\344\271\211\347\273\204", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("ModifyGroupNodeDialog", "\344\272\272\345\221\230\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("ModifyGroupNodeDialog", "\347\273\204\347\273\207\347\273\223\346\236\204", 0, QApplication::UnicodeUTF8));
        AddButton->setText(QApplication::translate("ModifyGroupNodeDialog", ">>", 0, QApplication::UnicodeUTF8));
        DelButton->setText(QApplication::translate("ModifyGroupNodeDialog", "<<", 0, QApplication::UnicodeUTF8));
        FindButton->setText(QApplication::translate("ModifyGroupNodeDialog", "\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ModifyGroupNodeDialog", "\351\200\211\344\270\255\347\232\204\347\224\250\346\210\267\346\210\226\347\273\204\357\274\232", 0, QApplication::UnicodeUTF8));
        OkButton->setText(QApplication::translate("ModifyGroupNodeDialog", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        CancelButton->setText(QApplication::translate("ModifyGroupNodeDialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ModifyGroupNodeDialog: public Ui_ModifyGroupNodeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEFIYGROUPNODE_H
