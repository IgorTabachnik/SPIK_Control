#include "functiontest.h"

FunctionTest::FunctionTest(modbusConnection* connect_, const QList<DeviceModbus> *list_devs_, ExcelConfig *configEX) : connect(connect_), excel(configEX)
{
    this->list_dev = list_devs_;

    for(int k=0; k<list_dev->size();k++)
    {
        DeviceModbus dev = list_dev->at(k);
        if(dev.devType!=DeviceModbus::BK){
            devOP = dev;
            break;
        }
    }

    for(int k=0; k<list_dev->size();k++)
    {
        DeviceModbus dev = list_dev->at(k);
        if(dev.devType!=DeviceModbus::BK){
            continue;
        }
        QString name = devOP.nameDevice + ": " + dev.nameDevice;
        StateDevice state;
        stateDevs.insert(name, state);
    }
}

FunctionTest::~FunctionTest()
{
    if(excel!=NULL)
    delete excel;
}

QString FunctionTest::SendExcelConfigToAllDevices()
{
    uint16_t* array = new uint16_t[MODBUS_MAX_WRITE_REGISTERS];
    uint16_t* array2 = new uint16_t[MODBUS_MAX_WRITE_REGISTERS];
    uint16_t size = 0;
    help_func *help = new help_func();
    uint32_t delayMB = DelayRequest;

    if(!excel->getStateFuncTestTable()){
        delete array;
        delete array2;
        delete help;
        return "Error: Extract Func Test Table";
    }

    QList<FUNC_CMD>* list1 = excel->getFuncTest();
    QList<SEL_MCM> *list2 = excel->getSELMCM();
    QList<uint16_t> *list3 = excel->getFPGAOptions();

    uint16_t j=0;
    FUNC_CMD cmd;
    if(list1->size()<=MODBUS_MAX_WRITE_REGISTERS)
    {
        for(uint16_t i=0;i<list1->size();i++)
        {
            cmd = list1->at(i);
            array[j++]=cmd.cmd;
            help->float2dw(cmd.Vcc, &array[j]);j+=2;
            help->float2dw(cmd.VccQ, &array[j]);j+=2;
            help->float2dw(cmd.Temp, &array[j]);j+=2;
            array[j++]=cmd.pattern;
        }
    }
    size = j+1;

    if(list3->size()<=MODBUS_MAX_WRITE_REGISTERS){
        for(uint16_t i=0;i<list3->size();i++){
            array2[i] = list3->at(i);
        }
    }

    QString result;
    for(int k=0;k<list_dev->size();k++)
    {
        DeviceModbus devMB = list_dev->at(k);

        if(devMB.devType!=DeviceModbus::BK){
            continue;
        }
        //Send Func test
        size = j+1;
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegFuncTestTable, size, array);
        if(result!="0"){
            return "Error: Don't transmit Func test to Device:"+devMB.nameDevice;
        }
        Sleep(delayMB);

        size = list1->size();
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_SINGLE_REGISTER, RegSizeFuncTable,1, &size);
        if(result!="0"){
            return "Error: Don't transmit size Func test to Device:"+devMB.nameDevice;
        }
        Sleep(delayMB);

        uint16_t comand_update = 1;
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_SINGLE_REGISTER,RegUpdateFuncTable,1, &comand_update);
        if(result!="0"){
            return "Error: Don't transmit command update to Device:"+devMB.nameDevice;
        }

        Sleep(delayMB);

        //Send MCM select
        SEL_MCM sel;
        sel.BK = devMB.nameDevice;
        sel.OP = devOP.nameDevice;
        foreach (SEL_MCM var, *list2) {
			//check SEL_MCM is 0!
            QString temp = sel.OP+sel.BK;
            if(temp.compare(var.OP+var.BK)==0){
                result = connect->sendModbusRequest(devMB.address, MODBUS_FC_WRITE_SINGLE_REGISTER, RegSelectMKM, 1, &(var.sel_mcm));
                if(result!="0"){
                    return "Error: Don't transmit sell MCM to Device:"+devMB.nameDevice;
                }
                Sleep(delayMB);
                break;
            }
        }

        //Send FPGA options
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegFPGAOptStruct, list3->size(), array2);
        if(result!="0"){
            return "Error: Don't transmit FPGA options to Device:"+devMB.nameDevice;
        }
        Sleep(delayMB);


        //Comand for update
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegUpdateFPGAOpt, 1, &comand_update);
        if(result!="0"){
            return "Error: Don't transmit Comand for update to Device:"+devMB.nameDevice;
        }
        Sleep(delayMB);

        //Init Func test
        result = connect->sendModbusRequest(devMB.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS, RegInitFuncTest, 1, &comand_update);
        if(result!="0"){
            return "Error: Don't transmit Comand for init Function Test to Device:"+devMB.nameDevice;
        }
        Sleep(delayMB);

    }

    delete help;
    delete array;
    delete array2;
    return "0";
}

QString FunctionTest::SendExcelConfigToDevice(DeviceModbus device)
{
    uint16_t* array = new uint16_t[MODBUS_MAX_WRITE_REGISTERS];
    uint16_t* array2 = new uint16_t[MODBUS_MAX_WRITE_REGISTERS];
    uint16_t size = 0;
    help_func *help = new help_func();
    uint32_t delayMB = DelayRequest;

    if(device.devType!=DeviceModbus::BK){
        return "Error: device is OP, required device BK";
    }

    if(!excel->getStateFuncTestTable()){
        delete array;
        delete array2;
        delete help;
        return "Error: Extract Func Test Table";
    }

    QList<FUNC_CMD>* list1 = excel->getFuncTest();
    QList<SEL_MCM> *list2 = excel->getSELMCM();
    QList<uint16_t> *list3 = excel->getFPGAOptions();

    uint16_t j=0;
    FUNC_CMD cmd;
    if(list1->size()<=MODBUS_MAX_WRITE_REGISTERS)
    {
        for(uint16_t i=0;i<list1->size();i++)
        {
            cmd = list1->at(i);
            array[j++]=cmd.cmd;
            help->float2dw(cmd.Vcc, &array[j]);j+=2;
            help->float2dw(cmd.VccQ, &array[j]);j+=2;
            help->float2dw(cmd.Temp, &array[j]);j+=2;
            array[j++]=cmd.pattern;
        }
    }
    size = j+1;

    if(list3->size()<=MODBUS_MAX_WRITE_REGISTERS){
        for(uint16_t i=0;i<list3->size();i++){
            array2[i] = list3->at(i);
        }
    }

    QString result;


    //Send Func test
    size = j+1;
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegFuncTestTable, size, array);
    if(result!="0"){
        return "Error: Don't transmit Func test to Device:"+device.nameDevice;
    }
    Sleep(delayMB);

    size = list1->size();
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_SINGLE_REGISTER, RegSizeFuncTable,1, &size);
    if(result!="0"){
        return "Error: Don't transmit size Func test to Device:"+device.nameDevice;
    }
    Sleep(delayMB);

    uint16_t comand_update = 1;
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_SINGLE_REGISTER,RegUpdateFuncTable,1, &comand_update);
    if(result!="0"){
        return "Error: Don't transmit command update to Device:"+device.nameDevice;
    }

    Sleep(delayMB);

    //Send MCM select 
    SEL_MCM sel;
    sel.BK = device.nameDevice;
    sel.OP = devOP.nameDevice;
    foreach (SEL_MCM var, *list2) {
        QString temp = sel.OP+sel.BK;
        if(temp.compare(var.OP+var.BK)==0){
            result = connect->sendModbusRequest(device.address, MODBUS_FC_WRITE_SINGLE_REGISTER, RegSelectMKM, 1, &(var.sel_mcm));
            if(result!="0"){
                return "Error: Don't transmit sell MCM to Device:"+device.nameDevice;
            }
            Sleep(delayMB);
            break;
        }
    }

    //Send FPGA options
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegFPGAOptStruct, list3->size(), array2);
    if(result!="0"){
        return "Error: Don't transmit FPGA options to Device:"+device.nameDevice;
    }
    Sleep(delayMB);


    //Comand for update
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegUpdateFPGAOpt, 1, &comand_update);
    if(result!="0"){
        return "Error: Don't transmit Comand for update to Device:"+device.nameDevice;
    }
    Sleep(delayMB);

    //Init Func test
	comand_update = 1;
    result = connect->sendModbusRequest(device.address,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,RegInitFuncTest, 1, &comand_update);
    if(result!="0"){
        return "Error: Don't transmit Comand for init Function Test to Device:"+device.nameDevice;
    }
    Sleep(delayMB);


	//NEED TO CHECK ALL sending Parameters !


    delete help;
    delete array;
    delete array2;
    return "0";
}

QString FunctionTest::StartFunctionTest()
{
    QString err = 0;
    uint16_t command = 1;
    uint32_t delay = DelayRequest;

    for(uint16_t i=0;i<list_dev->size();i++)
    {
        DeviceModbus dev = list_dev->at(i);
        if(dev.devType==DeviceModbus::BK)
        {
            err = connect->sendModbusRequest(dev.address,MODBUS_FC_WRITE_SINGLE_REGISTER, RegStartFuncTest, 1, &command);
            if(err!="0"){
                return err;
            }
            Sleep(delay);
        }
    }

    return err;
}

QString FunctionTest::StopFunctionsTest()
{
	QString err = 0;
	uint16_t command = 1;
	uint32_t delay = DelayRequest;

	for (uint16_t i = 0; i < list_dev->size(); i++)
	{
		DeviceModbus dev = list_dev->at(i);
		if (dev.devType == DeviceModbus::BK)
		{

			err = connect->sendModbusRequest(dev.address, MODBUS_FC_WRITE_SINGLE_REGISTER, RegFinishFuncTest, 1, &command);

			if (err != "0") {
				for (int j = 0; j < CountCommunicationsAttemps; j++) {
					err = connect->sendModbusRequest(dev.address, MODBUS_FC_WRITE_SINGLE_REGISTER, RegFinishFuncTest, 1, &command);
					Sleep(delay);
					if (err == "0") {
						break;
					}
				}
				if (err == "0") {
					continue;
				}
				return QString("Finished test command not transferred to device ->" + devOP.nameDevice + dev.nameDevice);
			}

			Sleep(delay);
		}
	}

	return err;
}


QString FunctionTest::UpdateStateFuncTest()
{
    QString err;
    uint32_t delayMB = DelayRequest;

    StateDevice state;
    QString name;
    uint16_t buffer[Expected_Count_Of_MKM+3];

    for(int k=0; k<list_dev->size();k++)
    {
        DeviceModbus dev = list_dev->at(k);
        if(dev.devType!=DeviceModbus::BK){
            continue;
        }
        unsigned int s = sizeof(state.results);
        memset(&state.results, 0, s);

        name = devOP.nameDevice + ": " + dev.nameDevice;
        err = connect->sendModbusRequest(dev.address, MODBUS_FC_READ_INPUT_REGISTERS, RegCountMKM, Expected_Count_Of_MKM+3, buffer);
        Sleep(delayMB);

        state.state_ft = (STATE_OF_FUNCTIONS_TEST)buffer[1];
        state.current_inst = buffer[2];
        for(int j=3;j<Expected_Count_Of_MKM+3;j++)
        {
            state.results[j-3]=buffer[j];
        }

        QMap<QString, StateDevice>::const_iterator i = stateDevs.find(name);
        if( (i != stateDevs.end()) && (i.key() != "End") )
        {
            //Found the device
            stateDevs[name] = state;
        }else{
            //No found results for current device;
            stateDevs.insert(name, state);
        }
    }

    return err;
}

int FunctionTest::GetPercentCompleted()
{
    int percent = 0;
    QMapIterator<QString, StateDevice> i(stateDevs);
    uint16_t size_ft_table = excel->getFuncTest()->size();
    
	if(size_ft_table==0){
        return 100;
    }

    while( i.hasNext() )
    {
        i.next();
        StateDevice st = i.value();
		if ((st.state_ft == FunctionTest::FPGA_FUNCTION_TEST_DONE)) {
			percent += 100;
		}
		else{
			percent += (st.current_inst * 100) / size_ft_table;
		}
    }

	percent /= stateDevs.size();

    return percent;
}

void FunctionTest::OutputFuncTestResults()
{
    QMap<QString, StateDevice>::const_iterator i = stateDevs.constBegin();
    while(i!=stateDevs.constEnd())
    {
        if(i.key()!="End"){
            StateDevice s = i.value();

            qInfo()<<i.key();//name dev
            QString buf;
            switch(s.state_ft)
            {
            case 0: buf = "FPGA OFF";
                break;
            case 1:

                break;
            default:
                break;
            }
            qInfo()<<"State Func test="<<buf;

            qInfo()<<"Current index instruction done="<<(s.current_inst+1)<<"("<<excel->getFuncTest()->size()<<")";

            qInfo()<<"Results of test:";

            for(uint16_t j=0; j<Expected_Count_Of_MKM; j++) {
                qInfo()<<"MKM"<<j+1<<":"<<QString("%1").arg(s.results[j], 2, 16)<<" ";
            }

            qInfo()<<"\n";
        }
        ++i;
    }
}

QString FunctionTest::CheckStatusDevices(STATE_OF_FUNCTIONS_TEST original_state){
    QString err="0";
    QMapIterator<QString, StateDevice> i(stateDevs);

    while( i.hasNext() )
    {
        i.next();
        StateDevice st = i.value();
        if(st.state_ft!=original_state){
            err.append( "Error: Device-> "+ i.key() + " in state: " + trStateEnum(st.state_ft) + "\n");
        }
    }

    return err;
}

QString FunctionTest::trStateEnum(STATE_OF_FUNCTIONS_TEST s)
{
    QString res="Unknow";
    switch (s) {
    case FPGA_OFF:
        res = "FPGA_OFF";
        break;
    case FPGA_INIT_PROGRESS:
        res = "FPGA_INIT_PROGRESS";
        break;
    case FPGA_INIT_DONE:
        res = "FPGA_INIT_DONE";
        break;
    case FPGA_FUNCTION_TEST_IN_PROGRESS:
        res = "FPGA_FUNCTION_TEST_IN_PROGRESS";
        break;
    case FPGA_FUNCTION_TEST_RECHECK:
        res = "FPGA_FUNCTION_TEST_RECHECK";
        break;
    case FPGA_FUNCTION_TEST_DONE:
        res = "FPGA_FUNCTION_TEST_DONE";
        break;
    case Critical_Error_Power:
        res = "Critical_Error_Power";
        break;

    default:
        break;
    }
    return res;

}

const QList<DeviceModbus>* FunctionTest::GetListDev()
{
    return list_dev;
}

modbusConnection* FunctionTest::getModBusConnection()
{
    return connect;
}

QMap<QString, FunctionTest::StateDevice> FunctionTest::getResultFunctionTest()
{
	return stateDevs;
}

