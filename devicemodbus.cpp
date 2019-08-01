#include "devicemodbus.h"

DeviceModbus::DeviceModbus()
{

}

DeviceModbus::DeviceModbus(QString name, uint8_t addr, QString comPort) : nameDevice(name), address(addr), comPortNumber(comPort)
{
    if(name.contains("OP")){
        devType=OP;
    }
    if(name.contains("BK")){
        devType=BK;
    }

}
