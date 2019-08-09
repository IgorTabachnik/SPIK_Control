#pragma once

#include <QWidget>
#include <QCheckBox>
#include "functiontest.h"

#include "ui_PreTestResult.h"
#define Count_of_MKM 8

class PreTestResult : public QWidget
{
	Q_OBJECT

public:
	PreTestResult(QWidget *parent = Q_NULLPTR);
	PreTestResult(QMap<QString, FunctionTest::StateDevice> result);
	~PreTestResult();

	uint8_t user_action;
	QMap<QString, uint8_t> getSelectedMCM();
private slots:
	void on_bAccept_clicked();

	void on_bCancel_clicked();

private:
	Ui::PreTestResult ui;
	QMap<QString, FunctionTest::StateDevice> result;
	QMap<QString, uint8_t> sel;
};
