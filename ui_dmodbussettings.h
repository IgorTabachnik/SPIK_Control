/********************************************************************************
** Form generated from reading UI file 'dmodbussettings.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DMODBUSSETTINGS_H
#define UI_DMODBUSSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DModBusSettings
{
public:
    QProgressBar *progressSearch;
    QPushButton *SearchDevices;
    QComboBox *comboBaudeRate;
    QLabel *labelSearch;
    QLabel *labelRate;
    QLineEdit *lineSearch;
    QPushButton *LoadDevices;
    QLabel *label;

    void setupUi(QDialog *DModBusSettings)
    {
        if (DModBusSettings->objectName().isEmpty())
            DModBusSettings->setObjectName(QStringLiteral("DModBusSettings"));
        DModBusSettings->resize(292, 194);
        progressSearch = new QProgressBar(DModBusSettings);
        progressSearch->setObjectName(QStringLiteral("progressSearch"));
        progressSearch->setGeometry(QRect(10, 40, 271, 21));
        progressSearch->setValue(0);
        SearchDevices = new QPushButton(DModBusSettings);
        SearchDevices->setObjectName(QStringLiteral("SearchDevices"));
        SearchDevices->setGeometry(QRect(10, 0, 131, 31));
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(14);
        SearchDevices->setFont(font);
        comboBaudeRate = new QComboBox(DModBusSettings);
        comboBaudeRate->addItem(QString());
        comboBaudeRate->addItem(QString());
        comboBaudeRate->addItem(QString());
        comboBaudeRate->addItem(QString());
        comboBaudeRate->setObjectName(QStringLiteral("comboBaudeRate"));
        comboBaudeRate->setGeometry(QRect(210, 80, 69, 22));
        comboBaudeRate->setMaxVisibleItems(10);
        labelSearch = new QLabel(DModBusSettings);
        labelSearch->setObjectName(QStringLiteral("labelSearch"));
        labelSearch->setGeometry(QRect(20, 110, 90, 39));
        labelRate = new QLabel(DModBusSettings);
        labelRate->setObjectName(QStringLiteral("labelRate"));
        labelRate->setGeometry(QRect(20, 80, 130, 16));
        lineSearch = new QLineEdit(DModBusSettings);
        lineSearch->setObjectName(QStringLiteral("lineSearch"));
        lineSearch->setGeometry(QRect(210, 120, 71, 20));
        LoadDevices = new QPushButton(DModBusSettings);
        LoadDevices->setObjectName(QStringLiteral("LoadDevices"));
        LoadDevices->setGeometry(QRect(150, 0, 131, 31));
        LoadDevices->setFont(font);
        label = new QLabel(DModBusSettings);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 160, 291, 31));

        retranslateUi(DModBusSettings);

        QMetaObject::connectSlotsByName(DModBusSettings);
    } // setupUi

    void retranslateUi(QDialog *DModBusSettings)
    {
        DModBusSettings->setWindowTitle(QApplication::translate("DModBusSettings", "Dialog", nullptr));
        SearchDevices->setText(QApplication::translate("DModBusSettings", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        comboBaudeRate->setItemText(0, QApplication::translate("DModBusSettings", "9600", nullptr));
        comboBaudeRate->setItemText(1, QApplication::translate("DModBusSettings", "19200", nullptr));
        comboBaudeRate->setItemText(2, QApplication::translate("DModBusSettings", "57600", nullptr));
        comboBaudeRate->setItemText(3, QApplication::translate("DModBusSettings", "115200", nullptr));

        comboBaudeRate->setCurrentText(QApplication::translate("DModBusSettings", "115200", nullptr));
        labelSearch->setText(QApplication::translate("DModBusSettings", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217\n"
"\320\277\320\276\320\270\321\201\320\272\320\260 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\n"
"(\321\201\320\265\320\272\321\203\320\275\320\264\321\213)", nullptr));
        labelRate->setText(QApplication::translate("DModBusSettings", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214 \320\277\320\265\321\200\320\265\320\264\320\260\321\207\320\270", nullptr));
        LoadDevices->setText(QApplication::translate("DModBusSettings", "\320\227\320\260\320\263\321\200\321\203\320\267\320\272\320\260", nullptr));
        label->setText(QApplication::translate("DModBusSettings", "\"\320\237\320\276\320\270\321\201\320\272\" \320\262\321\213\320\277\320\276\320\273\320\275\321\217\320\265\321\202 \320\277\320\276\320\270\321\201\320\272 \320\275\320\276\320\262\321\213\321\205 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\n"
"\"\320\227\320\260\320\263\321\200\321\203\320\267\320\272\320\260\" \320\270\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202 \321\201\320\276\321\205\321\200\320\260\320\275\321\221\320\275\320\275\321\213\320\271 \321\201\320\277\320\270\321\201\320\276\320\272 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DModBusSettings: public Ui_DModBusSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DMODBUSSETTINGS_H
