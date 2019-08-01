#ifndef THPOLLDEVICES_H
#define THPOLLDEVICES_H

#include <QThread>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QList>

#include "functiontest.h"

class ThPollDevices : public QThread
{
    Q_OBJECT
public:
    explicit ThPollDevices(FunctionTest* functest=nullptr);
    ~ThPollDevices();

    void run() override;
    bool isRunning();
    void Stop();

    int getPercentCompleted(void);
    FunctionTest* getFuncTest(void);
    QString error;
    bool cStop;
    bool work;

	int percent;

private:
    FunctionTest* test;
    
signals:
    void resultReady(const QString &s);
    void sendProgressStatus(const int procent);
    void sendMessage(const QString s);
};

#endif // THPOLLDEVICES_H
