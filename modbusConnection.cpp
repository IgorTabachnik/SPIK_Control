#include "modbusConnection.h"
//#include <QtWidgets>
modbusConnection::modbusConnection(QString com, int bR):comPortNumber(com), baudeRate(bR)
{
    status_connection=false;
}

modbusConnection::modbusConnection(QThread *parent, modbus_t * m_modbus)
    : parent(parent), m_modbus(m_modbus)
{
    status_connection=false;
}

modbusConnection::modbusConnection(QThread *parent, modbus_t * m_modbus, QString port)
    : parent(parent), m_modbus(m_modbus), pName(port)
{
    status_connection=false;
}

QString modbusConnection::sendModbusRequest(const int slave, const int func, const int addr, int num,  uint16_t* dest )
{

    if( m_modbus == NULL )
    {
        return "-1";
    }

    int ret = -1;
    bool writeAccess = false;
    modbus_set_slave( m_modbus, slave);
    const int value = dest[0];
    uint8_t* dest8 = (uint8_t*)dest;

    switch( func )
    {
        case MODBUS_FC_READ_COILS:
            ret = modbus_read_bits( m_modbus, addr, num, dest8 );
            break;
        case MODBUS_FC_READ_DISCRETE_INPUTS:
            ret = modbus_read_bits( m_modbus, addr, num, dest8 );
            break;
        case MODBUS_FC_READ_HOLDING_REGISTERS:
            ret = modbus_read_registers( m_modbus, addr, num, dest );

            break;
        case MODBUS_FC_READ_INPUT_REGISTERS:
            ret = modbus_read_input_registers( m_modbus, addr, num, dest );

            break;
        case MODBUS_FC_WRITE_SINGLE_COIL:
            ret = modbus_read_bits( m_modbus, addr, num, dest8 );
            break;
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
            ret = modbus_write_register( m_modbus, addr, value);
            writeAccess = true;
            num = 1;
            break;

        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        {
            ret = modbus_write_bits( m_modbus, addr, num, dest8 );
            writeAccess = true;

            break;
        }
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        {
            ret = modbus_write_registers( m_modbus, addr, num, dest );

            writeAccess = true;
            break;
        }

        default:
            break;
    }

    if( ret == num  )
    {
            return "0";
    }
    else
    {
        if( ret < 0 )
        {
            if((errno == WSAETIMEDOUT) || (errno == EIO))
            {
                return "I/O error: did not receive any data from slave.";
            }
            else
            {
                return QString("Slave threw exception \"%1\" or "
                               "function not implemented.").arg(modbus_strerror(errno));

            }
        }
        else
        {
          return "ModBusError:Number of registers returned does not match number of registers requested!";
        }
    }
}

bool modbusConnection::connect(QString comPortNumber)
{
    /*!
      Checks status connection for com port
    */
    this->comPortNumber = comPortNumber;

    // Request to com port
    m_modbus = modbus_new_rtu(comPortNumber.toLatin1().constData(),
                              baudeRate,                        //baude rate
                              'N',                              //Parity
                              8,                                //Data bits
                              1);

    int statusMB = modbus_connect(m_modbus);

    // Check returned value and set status based on it
    if(statusMB == 0)
        this->status_connection = true;

    return this->status_connection;
}

bool modbusConnection::connect()
{
    /*!
      Checks status connection for com port
    */
    this->comPortNumber = comPortNumber;

    // Request to com port
    m_modbus = modbus_new_rtu(comPortNumber.toLatin1().constData(),
                              baudeRate,                        //baude rate
                              'N',                              //Parity
                              8,                                //Data bits
                              1);

    int statusMB = modbus_connect(m_modbus);

    // Check returned value and set status based on it
    if(statusMB == 0)
        this->status_connection = true;

    return this->status_connection;
}

void modbusConnection::freeConnect()
{
    if(m_modbus)
    {
        modbus_close(m_modbus);
        modbus_free(m_modbus);
        m_modbus=NULL;
    }
    status_connection = false;
}

bool modbusConnection::getStatusConnection()
{
    return status_connection;
}
