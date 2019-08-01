#define _WINSOCKAPI_

#ifndef THSEARCHDEVICES_H
#define THSEARCHDEVICES_H

#include <QObject>
#include <QThread>
#include <QTextStream>
#include "addressrange.h"
#include "devicemodbus.h"
#include "modbusConnection.h"

class ThSearchDevices : public QThread
{
    Q_OBJECT

public:
    ThSearchDevices(QString port, QList <quint16> addrRng, QList <DeviceModbus> *modbusDevices, int baudRate);    
    ~ThSearchDevices();
    
    void run() override;
    void Stop();
    bool isWorking();
    bool isStoped;

    int address;
    QList <DeviceModbus> *m_mbDevs;
    static int progressValue;

signals:
    void valueChanged(int value);

private:
    modbusConnection *objModBus;
    modbus_t *mb;

    bool work;
    bool statusConnection;    

    int m_BaudRate;

    QString m_comPortName;

    QList <quint16> m_addrRange;
};

#endif // THSEARCHDEVICES_H
