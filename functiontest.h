#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include <QObject>
#include <QMap>

#include "excelconfig.h"
#include "devicemodbus.h"
#include "modbusConnection.h"
#include "help_func.h"
#include "commonThread.h"
#include "modbusregs.h"

#define Expected_Count_Of_MKM 8
#define DelayRequest 100

class FunctionTest
{
public:
    typedef enum
    {
        FPGA_OFF,
        FPGA_INIT_PROGRESS,
        FPGA_INIT_DONE,
        FPGA_FUNCTION_TEST_IN_PROGRESS,
        FPGA_FUNCTION_TEST_RECHECK,
        FPGA_FUNCTION_TEST_DONE,
        Critical_Error_Power

    }STATE_OF_FUNCTIONS_TEST;

    typedef struct
    {
        uint16_t current_inst;
        STATE_OF_FUNCTIONS_TEST state_ft;
        uint16_t results[Expected_Count_Of_MKM];
    }StateDevice;

    explicit FunctionTest(modbusConnection* connect_=nullptr, const QList<DeviceModbus>* list_devs_=nullptr, ExcelConfig *configEX_=nullptr);
    ~FunctionTest();

    //Communications with devices
    QString SendExcelConfigToDevice(DeviceModbus device);
    QString SendExcelConfigToAllDevices();
    QString UpdateStateFuncTest();

    //Control func test
    QString StartFunctionTest();
    QString StopFunctionsTest();

    //Output results
    void OutputFuncTestResults();
    int GetPercentCompleted();

    QString CheckStatusDevices(STATE_OF_FUNCTIONS_TEST original_state);

    const QList<DeviceModbus>* GetListDev();
    modbusConnection* getModBusConnection();

    QString trStateEnum(STATE_OF_FUNCTIONS_TEST s);
         
private:
    modbusConnection* connect;
    const QList<DeviceModbus> *list_dev;
    ExcelConfig *excel;
    DeviceModbus devOP;
    QMap<QString, StateDevice> stateDevs;
};

#endif // FUNCTIONTEST_H
