/********************************************************************************
** Form generated from reading UI file 'PreTestResult.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRETESTRESULT_H
#define UI_PRETESTRESULT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreTestResult
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *bCancel;
    QPushButton *bAccept;

    void setupUi(QWidget *PreTestResult)
    {
        if (PreTestResult->objectName().isEmpty())
            PreTestResult->setObjectName(QStringLiteral("PreTestResult"));
        PreTestResult->resize(985, 310);
        gridLayout = new QGridLayout(PreTestResult);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableWidget = new QTableWidget(PreTestResult);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 2);

        bCancel = new QPushButton(PreTestResult);
        bCancel->setObjectName(QStringLiteral("bCancel"));
        QFont font;
        font.setPointSize(14);
        bCancel->setFont(font);

        gridLayout->addWidget(bCancel, 1, 1, 1, 1);

        bAccept = new QPushButton(PreTestResult);
        bAccept->setObjectName(QStringLiteral("bAccept"));
        bAccept->setFont(font);

        gridLayout->addWidget(bAccept, 1, 0, 1, 1);


        retranslateUi(PreTestResult);

        QMetaObject::connectSlotsByName(PreTestResult);
    } // setupUi

    void retranslateUi(QWidget *PreTestResult)
    {
        PreTestResult->setWindowTitle(QApplication::translate("PreTestResult", "PreTestResult", nullptr));
        bCancel->setText(QApplication::translate("PreTestResult", "Cancel", nullptr));
        bAccept->setText(QApplication::translate("PreTestResult", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PreTestResult: public Ui_PreTestResult {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRETESTRESULT_H
