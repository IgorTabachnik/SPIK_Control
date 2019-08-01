/********************************************************************************
** Form generated from reading UI file 'settings_spik.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_SPIK_H
#define UI_SETTINGS_SPIK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings_SPiK
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QWidget *Settings_SPiK)
    {
        if (Settings_SPiK->objectName().isEmpty())
            Settings_SPiK->setObjectName(QStringLiteral("Settings_SPiK"));
        Settings_SPiK->resize(631, 303);
        gridLayout_4 = new QGridLayout(Settings_SPiK);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox = new QGroupBox(Settings_SPiK);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QFont font;
        font.setPointSize(14);
        groupBox->setFont(font);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        toolButton = new QToolButton(groupBox);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        gridLayout->addWidget(toolButton, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 2);

        pushButton = new QPushButton(Settings_SPiK);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setPointSize(14);
        pushButton->setFont(font1);

        gridLayout_4->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(Settings_SPiK);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(true);
        pushButton_2->setFont(font1);

        gridLayout_4->addWidget(pushButton_2, 2, 1, 1, 1);

        groupBox_2 = new QGroupBox(Settings_SPiK);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setFont(font);
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 2);


        retranslateUi(Settings_SPiK);

        QMetaObject::connectSlotsByName(Settings_SPiK);
    } // setupUi

    void retranslateUi(QWidget *Settings_SPiK)
    {
        Settings_SPiK->setWindowTitle(QApplication::translate("Settings_SPiK", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("Settings_SPiK", "Config files", nullptr));
        label->setText(QApplication::translate("Settings_SPiK", "Directory to configurations files", nullptr));
        toolButton->setText(QApplication::translate("Settings_SPiK", "...", nullptr));
        pushButton->setText(QApplication::translate("Settings_SPiK", "Accept", nullptr));
        pushButton_2->setText(QApplication::translate("Settings_SPiK", "Exit", nullptr));
        groupBox_2->setTitle(QApplication::translate("Settings_SPiK", "Power Supply", nullptr));
        label_2->setText(QApplication::translate("Settings_SPiK", "PWR name1", nullptr));
        label_3->setText(QApplication::translate("Settings_SPiK", "PWR name2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Settings_SPiK: public Ui_Settings_SPiK {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_SPIK_H
