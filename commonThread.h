#pragma once
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QMap>
#include <QDir>
#include <QString>
#include <QList>
#include <QSettings>
#include "devicemodbus.h"
#include "thsearchdevices.h"
#include "searchsettings.h"

class commonThread : public QThread
{
    Q_OBJECT
public:    
    commonThread(searchSettings *settings);
    commonThread();
    ~commonThread();

    bool m_timeOut;

    static QMap <QString, QString> absentDevices;

    static QString loadFoundDevices();
    static void showFoundDevices();
    void run() override;
    void Stop();

    bool isRunning();

    static QMap < QString, QList <DeviceModbus> > foundDevices;
    static QString settingsFilePath;
    static QSettings appSettings;
    static bool saveData;

private:
    bool work;

    int m_baudRate, m_timeOutValue;

    QMap < QString, QList <DeviceModbus> *> *tempFoundDevices;
    QMap <QString, QList <quint16> > m_searchSettings;

    QList <ThSearchDevices*> *m_listThreads;
    QList <DeviceModbus> *m_listFoundDevices;

    ThSearchDevices *objSearch;
   
    void controlThreads(QList <ThSearchDevices*> listThreads);

    void showAbsentDevices();
    void saveSettings();

private slots:
    void changeProgressBarValue(int value);

signals:
    void valueChanged(int value);

};
