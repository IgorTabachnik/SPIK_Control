#ifndef PWR_DEVICE_H
#define PWR_DEVICE_H

#include <QObject>
#include <winsock2.h>
#include <windows.h>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include "visa.h"

#define MAX_REC_SIZE 1024

class PWR_Device : public QObject
{
    Q_OBJECT
public:
    PWR_Device();
    QString pwraddr;

    bool ConnectToPWR();
    bool InstrWrite(QString strAddr, QString strContent);//write function
    bool InstrRead(QString strAddr, QString *pstrResult);//read from the instrument

    static QString AllSetSettings(float Voltage, float Current, int channel);
    static QString AllPwrOn(int channel);
    static QString AllPwrOff(int channel);
    static QString AllInstrWrite(QString instr);

    static QString SearchPWRDevices();

    static QString pwr_array[5];
    static QSettings pwr_settings;
    static QString pathSavePwrDevice;
    static QString SavePWRSettings();
    static QString LoadPWRSettings();

    static QMap<QString, QString> devices;

signals:
    void sendMessage(QString mes);


public slots:



};

#endif // PWR_DEVICE_H
