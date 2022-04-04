/********************************************************************************
** Form generated from reading UI file 'MainTab.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTAB_H
#define UI_MAINTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainTab
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QFrame *FrameLayout;
    QGridLayout *gridLayout;
    QToolButton *DoNetReadToolButton;
    QLabel *UserNameLable;
    QToolButton *OffLineMessageToolButton;
    QToolButton *UserStateToolButton;
    QSpacerItem *horizontalSpacer;
    QToolButton *UserImageToolButton;
    QFrame *SearchFrameLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *search_lineEdit;
    QPushButton *search_pushButton;
    QTabWidget *MainTabWidget;
    QWidget *OrganizationTab;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout_5;
    QTreeView *ContactPeopleTreeView;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_6;
    QTreeView *OrganizationTreeView;
    QWidget *AddressesTab;

    void setupUi(QWidget *MainTab)
    {
        if (MainTab->objectName().isEmpty())
            MainTab->setObjectName(QString::fromUtf8("MainTab"));
        MainTab->resize(242, 630);
        MainTab->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(MainTab);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
#ifndef Q_OS_MAC
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
#endif
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        FrameLayout = new QFrame(MainTab);
        FrameLayout->setObjectName(QString::fromUtf8("FrameLayout"));
        FrameLayout->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(FrameLayout);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        DoNetReadToolButton = new QToolButton(FrameLayout);
        DoNetReadToolButton->setObjectName(QString::fromUtf8("DoNetReadToolButton"));
        DoNetReadToolButton->setAutoFillBackground(true);

        gridLayout->addWidget(DoNetReadToolButton, 2, 2, 1, 1);

        UserNameLable = new QLabel(FrameLayout);
        UserNameLable->setObjectName(QString::fromUtf8("UserNameLable"));

        gridLayout->addWidget(UserNameLable, 1, 1, 1, 1);

        OffLineMessageToolButton = new QToolButton(FrameLayout);
        OffLineMessageToolButton->setObjectName(QString::fromUtf8("OffLineMessageToolButton"));

        gridLayout->addWidget(OffLineMessageToolButton, 2, 1, 1, 1);

        UserStateToolButton = new QToolButton(FrameLayout);
        UserStateToolButton->setObjectName(QString::fromUtf8("UserStateToolButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UserStateToolButton->sizePolicy().hasHeightForWidth());
        UserStateToolButton->setSizePolicy(sizePolicy);
        UserStateToolButton->setText(QString::fromUtf8("\345\234\250\347\272\277 \342\226\274"));

        gridLayout->addWidget(UserStateToolButton, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 3, 1, 1);

        UserImageToolButton = new QToolButton(FrameLayout);
        UserImageToolButton->setObjectName(QString::fromUtf8("UserImageToolButton"));
        sizePolicy.setHeightForWidth(UserImageToolButton->sizePolicy().hasHeightForWidth());
        UserImageToolButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(UserImageToolButton, 1, 0, 2, 1);


        verticalLayout_2->addWidget(FrameLayout);

        SearchFrameLayout = new QFrame(MainTab);
        SearchFrameLayout->setObjectName(QString::fromUtf8("SearchFrameLayout"));
        horizontalLayout = new QHBoxLayout(SearchFrameLayout);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 4, 1, 4);
        search_lineEdit = new QLineEdit(SearchFrameLayout);
        search_lineEdit->setObjectName(QString::fromUtf8("search_lineEdit"));

        horizontalLayout->addWidget(search_lineEdit);

        search_pushButton = new QPushButton(SearchFrameLayout);
        search_pushButton->setObjectName(QString::fromUtf8("search_pushButton"));
        search_pushButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(search_pushButton);


        verticalLayout_2->addWidget(SearchFrameLayout);

        MainTabWidget = new QTabWidget(MainTab);
        MainTabWidget->setObjectName(QString::fromUtf8("MainTabWidget"));
        MainTabWidget->setStyleSheet(QString::fromUtf8(""));
        MainTabWidget->setTabPosition(QTabWidget::West);
        MainTabWidget->setTabShape(QTabWidget::Rounded);
        OrganizationTab = new QWidget();
        OrganizationTab->setObjectName(QString::fromUtf8("OrganizationTab"));
        sizePolicy.setHeightForWidth(OrganizationTab->sizePolicy().hasHeightForWidth());
        OrganizationTab->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(OrganizationTab);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, -1, 0);
        toolBox = new QToolBox(OrganizationTab);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setEnabled(true);
        toolBox->setFrameShadow(QFrame::Raised);
        toolBox->setLineWidth(1);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 213, 509));
        verticalLayout_5 = new QVBoxLayout(page);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        ContactPeopleTreeView = new QTreeView(page);
        ContactPeopleTreeView->setObjectName(QString::fromUtf8("ContactPeopleTreeView"));
        ContactPeopleTreeView->setFrameShape(QFrame::NoFrame);

        verticalLayout_5->addWidget(ContactPeopleTreeView);

        toolBox->addItem(page, QString::fromUtf8("\350\201\224\347\263\273\344\272\272"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 213, 509));
        verticalLayout_6 = new QVBoxLayout(page_2);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        OrganizationTreeView = new QTreeView(page_2);
        OrganizationTreeView->setObjectName(QString::fromUtf8("OrganizationTreeView"));

        verticalLayout_6->addWidget(OrganizationTreeView);

        toolBox->addItem(page_2, QString::fromUtf8("\347\273\204\347\273\207\347\273\223\346\236\204"));

        verticalLayout_3->addWidget(toolBox);


        verticalLayout_4->addLayout(verticalLayout_3);

        MainTabWidget->addTab(OrganizationTab, QString());
        AddressesTab = new QWidget();
        AddressesTab->setObjectName(QString::fromUtf8("AddressesTab"));
        MainTabWidget->addTab(AddressesTab, QString());

        verticalLayout_2->addWidget(MainTabWidget);


        verticalLayout->addLayout(verticalLayout_2);


        retranslateUi(MainTab);

        MainTabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(0);
        toolBox->layout()->setSpacing(0);


        QMetaObject::connectSlotsByName(MainTab);
    } // setupUi

    void retranslateUi(QWidget *MainTab)
    {
        MainTab->setWindowTitle(QApplication::translate("MainTab", "Form", 0, QApplication::UnicodeUTF8));
        DoNetReadToolButton->setText(QApplication::translate("MainTab", "(0)", 0, QApplication::UnicodeUTF8));
        UserNameLable->setText(QApplication::translate("MainTab", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        OffLineMessageToolButton->setText(QApplication::translate("MainTab", "(0)", 0, QApplication::UnicodeUTF8));
        UserImageToolButton->setText(QApplication::translate("MainTab", "...", 0, QApplication::UnicodeUTF8));
        search_pushButton->setText(QApplication::translate("MainTab", "\346\220\234", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainTab", "\350\201\224\347\263\273\344\272\272", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainTab", "\347\273\204\347\273\207\347\273\223\346\236\204", 0, QApplication::UnicodeUTF8));
        MainTabWidget->setTabText(MainTabWidget->indexOf(OrganizationTab), QApplication::translate("MainTab", "\347\273\204\347\273\207\347\273\223\346\236\204", 0, QApplication::UnicodeUTF8));
        MainTabWidget->setTabText(MainTabWidget->indexOf(AddressesTab), QApplication::translate("MainTab", "\351\200\232\350\256\257\345\275\225", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainTab: public Ui_MainTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTAB_H
