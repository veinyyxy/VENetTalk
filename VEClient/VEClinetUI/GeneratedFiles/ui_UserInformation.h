/********************************************************************************
** Form generated from reading UI file 'UserInformation.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFORMATION_H
#define UI_USERINFORMATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_UserInfoDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *NamelineEdit;
    QGroupBox *groupBox;
    QLabel *label_2;
    QComboBox *HeaderImageComboBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *MobilelineEdit;
    QLineEdit *PhonelineEdit;
    QLineEdit *EMaillineEdit;
    QLineEdit *AddresslineEdit;
    QLineEdit *QQlineEdit;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(342, 223);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(170, 190, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 100, 36, 16));
        NamelineEdit = new QLineEdit(Dialog);
        NamelineEdit->setObjectName(QString::fromUtf8("NamelineEdit"));
        NamelineEdit->setGeometry(QRect(60, 100, 111, 20));
        NamelineEdit->setMaxLength(22);
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 321, 71));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 71, 31));
        HeaderImageComboBox = new QComboBox(groupBox);
        HeaderImageComboBox->setObjectName(QString::fromUtf8("HeaderImageComboBox"));
        HeaderImageComboBox->setGeometry(QRect(90, 10, 69, 51));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(175, 100, 41, 20));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 130, 41, 16));
        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(180, 130, 36, 16));
        label_7 = new QLabel(Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 160, 36, 16));
        label_8 = new QLabel(Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(190, 160, 21, 16));
        MobilelineEdit = new QLineEdit(Dialog);
        MobilelineEdit->setObjectName(QString::fromUtf8("MobilelineEdit"));
        MobilelineEdit->setGeometry(QRect(220, 100, 111, 20));
        MobilelineEdit->setMaxLength(18);
        PhonelineEdit = new QLineEdit(Dialog);
        PhonelineEdit->setObjectName(QString::fromUtf8("PhonelineEdit"));
        PhonelineEdit->setGeometry(QRect(60, 130, 111, 20));
        PhonelineEdit->setMaxLength(14);
        EMaillineEdit = new QLineEdit(Dialog);
        EMaillineEdit->setObjectName(QString::fromUtf8("EMaillineEdit"));
        EMaillineEdit->setGeometry(QRect(220, 130, 111, 20));
        EMaillineEdit->setMaxLength(99);
        AddresslineEdit = new QLineEdit(Dialog);
        AddresslineEdit->setObjectName(QString::fromUtf8("AddresslineEdit"));
        AddresslineEdit->setGeometry(QRect(60, 160, 111, 20));
        AddresslineEdit->setMaxLength(500);
        QQlineEdit = new QLineEdit(Dialog);
        QQlineEdit->setObjectName(QString::fromUtf8("QQlineEdit"));
        QQlineEdit->setGeometry(QRect(220, 160, 111, 20));
        QQlineEdit->setMaxLength(17);

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "\344\270\252\344\272\272\350\265\204\346\226\231\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "\345\247\223\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Dialog", "\345\244\264\345\203\217", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "\347\263\273\347\273\237\345\244\264\345\203\217", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "\346\211\213\346\234\272\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "\345\233\272\350\257\235\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog", "\351\202\256\347\256\261\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Dialog", "\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Dialog", "QQ\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UserInfoDialog: public Ui_UserInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFORMATION_H
