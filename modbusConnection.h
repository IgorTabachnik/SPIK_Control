#ifndef USINGMODBUS_H
#define USINGMODBUS_H

#define _WINSOCKAPI_
#include <QObject>
#include <QString>
#include <QThread>
#include "modbus.h"

class modbusConnection : public QObject
{
    Q_OBJECT

public:
    modbusConnection(QString com, int baudeRate);
    modbusConnection(QThread *parent, modbus_t * m_modbus = nullptr);
    modbusConnection(QThread *parent = nullptr, modbus_t * m_modbus = nullptr, QString pName = "");

    QString sendModbusRequest(const int slave, const int func, const int addr, int num,  uint16_t* dest );
    bool connect(QString comPortNumber);
    bool connect();
    void freeConnect();
    bool getStatusConnection();
    modbus_t * m_modbus;
    int baudeRate;

    QString comPortNumber;
private:
    QThread *parent;
    QString pName;
    bool status_connection;

};

#endif // USINGMODBUS_H
