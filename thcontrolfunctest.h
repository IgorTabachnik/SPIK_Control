#ifndef THCONTROLFUNCTEST_H
#define THCONTROLFUNCTEST_H

#include <QThread>
#include "ThPollDevices.h"
#include "functiontest.h"
#include "commonThread.h"
#include "dmodbussettings.h"
#define WaitStopProcess 10000

#define BaudeRateDef 115200
class ThControlFuncTest : public QThread
{
    Q_OBJECT
private:
    QList<ThPollDevices*>* list_th;
    QList<modbusConnection*> *list_con;
    ExcelConfig* excel;

public:
    explicit ThControlFuncTest(ExcelConfig* excel);
    ~ThControlFuncTest();
    void run() override;

	bool work; bool done;
	QStringList getResultFunctionTest();
	QMap<QString, FunctionTest::StateDevice> getStatesFunctionTests();
signals:
    void SendMesToMain(QString mes);
	void sendProgressTest(int percent);

public slots:
    void TransitMessage(QString mes);

};

#endif // THCONTROLFUNCTEST_H
