/********************************************************************************
** Form generated from reading UI file 'addressrange.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDRESSRANGE_H
#define UI_ADDRESSRANGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AddressRange
{
public:
    QLabel *labelCom;
    QLineEdit *lineEditMinCom;
    QLineEdit *lineEditMaxCom;
    QLineEdit *lineEditMinDev;
    QLineEdit *lineEditMaxDev;
    QLabel *labelDevice;
    QLabel *labelDashCom;
    QLabel *labelDashDev;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;
    QLabel *labelOut;
    QLabel *labelInfo;
    QLabel *labelInvite;

    void setupUi(QDialog *AddressRange)
    {
        if (AddressRange->objectName().isEmpty())
            AddressRange->setObjectName(QStringLiteral("AddressRange"));
        AddressRange->resize(313, 210);
        labelCom = new QLabel(AddressRange);
        labelCom->setObjectName(QStringLiteral("labelCom"));
        labelCom->setGeometry(QRect(20, 100, 81, 16));
        lineEditMinCom = new QLineEdit(AddressRange);
        lineEditMinCom->setObjectName(QStringLiteral("lineEditMinCom"));
        lineEditMinCom->setGeometry(QRect(120, 100, 51, 20));
        lineEditMaxCom = new QLineEdit(AddressRange);
        lineEditMaxCom->setObjectName(QStringLiteral("lineEditMaxCom"));
        lineEditMaxCom->setGeometry(QRect(210, 100, 51, 20));
        lineEditMinDev = new QLineEdit(AddressRange);
        lineEditMinDev->setObjectName(QStringLiteral("lineEditMinDev"));
        lineEditMinDev->setGeometry(QRect(120, 130, 51, 20));
        lineEditMaxDev = new QLineEdit(AddressRange);
        lineEditMaxDev->setObjectName(QStringLiteral("lineEditMaxDev"));
        lineEditMaxDev->setGeometry(QRect(210, 130, 51, 20));
        labelDevice = new QLabel(AddressRange);
        labelDevice->setObjectName(QStringLiteral("labelDevice"));
        labelDevice->setGeometry(QRect(20, 130, 81, 16));
        labelDashCom = new QLabel(AddressRange);
        labelDashCom->setObjectName(QStringLiteral("labelDashCom"));
        labelDashCom->setGeometry(QRect(190, 100, 21, 16));
        labelDashDev = new QLabel(AddressRange);
        labelDashDev->setObjectName(QStringLiteral("labelDashDev"));
        labelDashDev->setGeometry(QRect(190, 130, 21, 16));
        pushButtonOk = new QPushButton(AddressRange);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(60, 180, 75, 23));
        pushButtonCancel = new QPushButton(AddressRange);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(160, 180, 75, 23));
        labelOut = new QLabel(AddressRange);
        labelOut->setObjectName(QStringLiteral("labelOut"));
        labelOut->setGeometry(QRect(20, 30, 271, 16));
        labelInfo = new QLabel(AddressRange);
        labelInfo->setObjectName(QStringLiteral("labelInfo"));
        labelInfo->setGeometry(QRect(10, 10, 261, 16));
        labelInvite = new QLabel(AddressRange);
        labelInvite->setObjectName(QStringLiteral("labelInvite"));
        labelInvite->setGeometry(QRect(10, 50, 271, 31));

        retranslateUi(AddressRange);

        QMetaObject::connectSlotsByName(AddressRange);
    } // setupUi

    void retranslateUi(QDialog *AddressRange)
    {
        AddressRange->setWindowTitle(QApplication::translate("AddressRange", "Dialog", nullptr));
        labelCom->setText(QApplication::translate("AddressRange", "COM \320\277\320\276\321\200\321\202", nullptr));
        labelDevice->setText(QApplication::translate("AddressRange", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
        labelDashCom->setText(QApplication::translate("AddressRange", "-", nullptr));
        labelDashDev->setText(QApplication::translate("AddressRange", "-", nullptr));
        pushButtonOk->setText(QApplication::translate("AddressRange", "OK", nullptr));
        pushButtonCancel->setText(QApplication::translate("AddressRange", "Cancel", nullptr));
        labelOut->setText(QString());
        labelInfo->setText(QApplication::translate("AddressRange", "\320\221\321\213\320\273\320\270 \320\275\320\260\320\271\320\264\320\265\320\275\321\213 COM \320\277\320\276\321\200\321\202\321\213 \321\201 \320\260\320\264\321\200\320\265\321\201\320\260\320\274\320\270:", nullptr));
        labelInvite->setText(QApplication::translate("AddressRange", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\264\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\277\320\276\320\270\321\201\320\272\320\260, \320\270\321\201\320\277\320\276\320\273\321\214\320\267\321\203\321\217 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216\n"
"\320\276 \320\275\320\260\320\271\320\264\320\265\320\275\320\275\321\213\321\205 \320\277\320\276\321\200\321\202\320\260\321\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddressRange: public Ui_AddressRange {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDRESSRANGE_H
