#define _WINSOCKAPI_

#include <QTextStream>
#include <QtWidgets>
#include "thsearchdevices.h"
#include "devicemodbus.h"
//#include "dmodbussettings.h"
#include "commonThread.h"


QMap <QString, QString> commonThread::absentDevices;
int ThSearchDevices::progressValue;

ThSearchDevices::ThSearchDevices(QString cPort, QList <quint16> addrRng, QList <DeviceModbus> *mDevices, int baudRate)
    : m_comPortName(cPort), m_addrRange(addrRng), m_mbDevs(mDevices), m_BaudRate(baudRate)
{
    isStoped = false;
    //mb = NULL;
}

ThSearchDevices::~ThSearchDevices()
{
    delete objModBus;
}


void ThSearchDevices::run()
{
    work = true;

    // Get range of addresses for com ports and devices
    quint16 addrDevMin = m_addrRange[0];
    quint16 addrDevMax = m_addrRange[1];

    // Initialize variable to get status of connection
    statusConnection = false;

    DeviceModbus *device = new DeviceModbus();

    objModBus = new modbusConnection(this, mb, m_comPortName);
    objModBus->baudeRate = m_BaudRate;


    // add disconnect objModBus//
    statusConnection = objModBus->connect(m_comPortName);
    mb               = objModBus->m_modbus;

    if (statusConnection)
    {
        // Store com port number
        device->comPortNumber = m_comPortName;

        QString absentDev = "";

        // Iterate in range of allowed addresses
        address = addrDevMin;
        while (work)
        {           

            if (address > addrDevMax)
            {
                work = false;
                break;
            }

            // Pause
            Sleep(100);

            // Device name
            QString nameDev = "";

            // Storage for output modbus data
            uint16_t regDevice[2];
            memset(regDevice, 0, sizeof(regDevice));

            // Request to Modbus
            objModBus->sendModbusRequest(address, MODBUS_FC_READ_INPUT_REGISTERS, 0, 2, regDevice);
            uint8_t* ptr = (uint8_t*)regDevice;

            // Get device name
            nameDev += (ptr[0]);
            nameDev += (ptr[1]);

            // Get device type
            if ( (nameDev != "OP") && (nameDev != "BK") )
            {
                absentDev += "within address " + QString::number(address) + " is not found" + ";";
                //commonThread::absentDevices[m_comPortName] = absentDev;//generate exceptions
            }
            else
            {
                if (nameDev == "OP")
                    device->devType = DeviceModbus::OP;
                else if (nameDev == "BK")
                    device->devType = DeviceModbus::BK;

                // Define name of device as concatenation of type and address
                nameDev+=QString::number(address);
                device->nameDevice = nameDev;

                // Write device address
                device->address = address;

                // Store all data in output list
                m_mbDevs->append(*device);
            }
            address++;

            ++progressValue;
            emit valueChanged(progressValue);
        }

        // Close and free the connection
        modbus_close(mb);
        modbus_free(mb);
        isStoped = true;
    }
    else
    {
        qInfo() << "Cannot create connection with" << m_comPortName;
        progressValue += addrDevMax - addrDevMin + 1;
        emit valueChanged(progressValue);

        work        = false;
        isStoped    = true;
    }
	
	delete device;
}

void ThSearchDevices::Stop()
{
    work=false;
}

bool ThSearchDevices::isWorking()
{
    return work;
}
