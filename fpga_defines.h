#ifndef FPGA_DEFINES_H
#define FPGA_DEFINES_H
#include <QtCore>

typedef struct
{
    uint16_t cmd;
    QString name;
    float Vcc;
    float VccQ;
    float Temp;
    uint16_t pattern;
}FUNC_CMD;

typedef struct
{
    QString OP;
    QString BK;
    uint16_t sel_mcm;
}SEL_MCM;

#endif // FPGA_DEFINES_H
