#ifndef DEVICEMODBUS_H
#define DEVICEMODBUS_H

#include <QString>
class DeviceModbus
{
public:

    enum deviceType{
        OP,
        BK
    }devType;

    DeviceModbus();
    DeviceModbus(QString nameDevice, uint8_t address, QString comPortNumber);

    QString comPortNumber	= "";
    QString nameDevice      = "";
    uint8_t address         = 0;
};

#endif // DEVICEMODBUS_H
