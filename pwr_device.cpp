#include "pwr_device.h"
#include "mainwindow.h"

QString PWR_Device::array_type_pwr[5]={"PWS4323","DP832A","PSU7","N5747A"};
QString PWR_Device::pathSavePwrDevice = QDir::currentPath()+CONFIG_PwrDevice;
QSettings PWR_Device::pwr_settings(PWR_Device::pathSavePwrDevice,QSettings::IniFormat);
QMap<QString, PWR_Device*> PWR_Device::devices;


PWR_Device::PWR_Device(QString name, QString addr, Type_pwr_device t):pwrname(name),pwraddr(addr),type(t)
{

}

PWR_Device::PWR_Device(QString addr): pwraddr(addr)
{

}

PWR_Device::PWR_Device() 
{

}

QString PWR_Device::AllPwrOff()
{
	QString res;
	foreach(QString key, devices.keys()) {
		res = devices[key]->PwrOff();
		if (res != "0") {
			return res;
		}
	}
	return "0";
}

QString PWR_Device::AllPwrOn() 
{
	QString res;
	foreach(QString key, devices.keys()) {
		res = devices[key]->PwrOn();
		if (res != "0") {
			return res;
		}
	}
	return "0";
}

QString PWR_Device::AllSetSettings(qreal Voltage, qreal Current, int channel)
{
    QString res="0";
    PWR_Device power;
    foreach (QString key, devices.keys()) {
        
		QString cmd;
		QString cmd1, cmd2;
		bool ok, ok1, ok2;
		
		switch (devices[key]->type)
		{
		case PWS4323:
			cmd1 = QString("VOLT %1").arg(Voltage);
			cmd2 = QString("CURR %1").arg(Current);
			ok1 = devices[key]->InstrWrite(cmd1);
			if (!ok1) {
				return "Error: transmit cmd is " + cmd1 + " to device: " + key + " is fail";
			}

			ok2 = devices[key]->InstrWrite(cmd2);
			if (!ok2) {
				return "Error: transmit cmd is " + cmd2 + " to device: " + key + " is fail";
			}
			break;
		case DP832A:
			cmd = QString(":APPL CH%1, %2, %3").arg(channel).arg(Voltage).arg(Current);
			ok = devices[key]->InstrWrite(cmd);
			if (!ok) {
				return "Error: transmit cmd is " + cmd + " to device: " + key + " is fail";
			}
			break;
		case PSU7:
			cmd1 = QString("VOLT %1").arg(Voltage);
			cmd2 = QString("CURR %1").arg(Current);
			ok1 = devices[key]->InstrWrite(cmd1);
			if (!ok1) {
				return "Error: transmit cmd is " + cmd1 + " to device: " + key + " is fail";
			}

			ok2 = devices[key]->InstrWrite(cmd2);
			if (!ok2) {
				return "Error: transmit cmd is " + cmd2 + " to device: " + key + " is fail";
			}
			break;
		case N5747A:
			cmd1 = QString("VOLT %1").arg(Voltage);
			cmd2 = QString("CURR %1").arg(Current);
			ok1 = devices[key]->InstrWrite(cmd1);
			if (!ok1) {
				return "Error: transmit cmd is " + cmd1 + " to device: " + key + " is fail";
			}

			ok2 = devices[key]->InstrWrite(cmd2);
			if (!ok2) {
				return "Error: transmit cmd is " + cmd2 + " to device: " + key + " is fail";
			}
			break;
		default:
			break;
		}
    }
    return res;
}

QString PWR_Device::PwrOn(){
    QString res="0";
	QString cmd;
	switch (this->type)
	{
	case PWS4323:
		cmd = QString("OUTP ON");
		break;
	case DP832A:
		cmd = QString(":OUTP CH%1,ON").arg(1);
		break;
	case PSU7:
		cmd = QString("OUTP ON");
		break;
	case N5747A:
		cmd = QString("OUTP ON");
		break;
	default:
		break;
	}

	bool ok = this->InstrWrite(cmd);
	if (!ok) {
		res = "Error: transmit cmd is " + cmd + " to device: " + pwrname + " is fail";
	}
    return res;
}

QString PWR_Device::PwrOff(){
    QString res="0";
    PWR_Device power;
    
	QString cmd;
	switch (this->type)
	{
	case PWS4323:
		cmd = QString("OUTP OFF");
		break;
	case DP832A:
		cmd = QString(":OUTP CH%1,OFF").arg(1);
		break;
	case PSU7:
		cmd = QString("OUTP OFF");
		break;
	case N5747A:
		cmd = QString("OUTP OFF");
		break;
	default:
		break;
	}

    bool ok = this->InstrWrite(cmd);
    if(!ok){
        res = "Error: transmit cmd is "+cmd+" to device: "+pwrname+" is fail";
    }
    
    return res;
}

QString PWR_Device::MeasVoltage()
{
	QString res;
	PWR_Device power;

	QString cmd;
	switch (this->type)
	{
	case PWS4323:
		cmd = QString("Meas:Volt?");
		break;
	case DP832A:
		cmd = QString(":MEASure:VOLTage:DC? CH%1").arg(1);
		break;
	case PSU7:
		cmd = QString("Meas:Volt?");
		break;
	case N5747A:
		cmd = QString("Meas:Volt?");
		break;
	default:
		break;
	}

	bool ok = this->InstrWrite(cmd);
	if (!ok) {
		return "Error: transmit cmd is " + cmd + " to device: " + pwrname + " is fail";
	}
	ok = this->InstrRead(&res);
	if (!ok) {
		return "Error: read Voltage from " + pwrname;
	}

	return res;
}

QString PWR_Device::SearchPWRDevices()
{
    PWR_Device *power = new PWR_Device();
    ViStatus status;
    ViSession defaultRM;
    ViString expr = ViString("?*");
    ViPFindList findList = new unsigned long;
    ViPUInt32 retcnt = new unsigned long;
    ViChar instrDesc[1000];
    QString strSrc = "";
    QString strInstr = "";
    unsigned long i = 0;
    bool bFind = false;

    QString res = LoadPWRSettings();

    if(res!="0"){
        status = viOpenDefaultRM(&defaultRM);

         if (status < VI_SUCCESS) {
            return "Error: Initializing VISA...exiting No VISA instrument was opened !";
        }

        memset(instrDesc,0,1000);

		Type_pwr_device type;
        // Find resource
        status = viFindRsrc(defaultRM,expr,findList, retcnt, instrDesc);
        for (i = 0;i < (*retcnt);i++)
        {
         // Get instrument name
         strSrc = QString::fromUtf8(instrDesc);
		 power->pwraddr = strSrc;

		 power->InstrWrite("*IDN?");
         Sleep(200);
		 power->InstrRead(&strInstr);

         strInstr.toUpper();

         if(strInstr.size()>0)
         {
			 for (int i = 0; i < sizeof(array_type_pwr)/sizeof(QString); i++) {
				 if (strInstr.contains(array_type_pwr[i])) {
					 switch (i) {
					 case 0:
						 type = PWS4323;
						 break;
					 case 1:
						 type = DP832A;
						 break;
					 case 2:
						 type = PSU7;
						 break;
					 case 3:
						 type = N5747A;
						 break;
					 default:break;
					 }

					 power->pwrname = strInstr;
					 power->type = type;
				 }
			 }
            bFind = true;
			
			devices.insert(strInstr, power);
         }

         //Find next instrument
         status = viFindNext(*findList,instrDesc);
        }

        if (bFind == false)
        {
            return "Error: Didn't find any PWR Supply!";
        } else{
            SavePWRSettings();
        }
    }

    return "0";
}


bool PWR_Device::InstrWrite(QString strContent)
{
    ViSession defaultRM,instr;
    ViStatus status;
    ViUInt32 retCount;
    char* SendBuf = NULL;
    char* SendAddr = NULL;
    //bool bWriteOK = false;
    QString str;
    QByteArray ba1,ba2;

    //Change the adress's data style from QString to char*
    ba1 = pwraddr.toLocal8Bit();
    SendAddr = ba1.data();

    //Change the command's data style from QString to char*
    ba2 = strContent.toLocal8Bit();
    SendBuf = ba2.data();

    //open the VISA instrument
    status = viOpenDefaultRM(&defaultRM);
    if(status<VI_SUCCESS)
    {
        //QMessageBox::information(this,"Fail","No VISA instrument was opened!");
        return false;
    }
    //open the instrument
    status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);

    //write command
    status = viWrite(instr,(unsigned char *)SendBuf, strlen(SendBuf), &retCount);

    //close the instrument
    status = viClose(instr);
    status = viClose(defaultRM);

    emit sendMessage("Console: send commnd: "+strContent+" to pwr supply-> "+pwrname);
    return true;
}

bool PWR_Device::InstrRead(QString *pstrResult)
{
    ViSession defaultRM,instr;
    ViStatus status; ViUInt32 retCount;
    char * SendAddr = NULL;
    unsigned char RecBuf[MAX_REC_SIZE];
    QString str;
    QByteArray ba1,ba2;

    memset(RecBuf,0,MAX_REC_SIZE);

    //Change the adress's data style from QString to char*
    ba1 = pwraddr.toLocal8Bit();
    SendAddr = ba1.data();

    //open the VISA instrument
    status = viOpenDefaultRM(&defaultRM);
    if(status<VI_SUCCESS)
    {
        //QMessageBox::information(this,"Fail","No VISA instrument was opened!");
        return false;
    }

    //open the instrument
    status = viOpen(defaultRM, SendAddr, VI_NULL, VI_NULL, &instr);
	if (status < VI_SUCCESS)
	{
		return false;
	}
    //read from instrument
    status = viRead(instr,RecBuf, MAX_REC_SIZE, &retCount);
	if (status < VI_SUCCESS)
	{
		return false;
	}

    //close the instrument
    status = viClose(instr);
	if (status < VI_SUCCESS)
	{
		return false;
	}
    status = viClose(defaultRM);
	if (status < VI_SUCCESS)
	{
		return false;
	}


    pstrResult->clear();
    pstrResult->append((char*)RecBuf);

    return true;
}


QString PWR_Device::SavePWRSettings()
{
    /*!
    Saves settings of found defices as .ini file
    */

    qInfo() << "Store PWR Devices settings in file ->" + pathSavePwrDevice;

    QFile checkConfig(pathSavePwrDevice);

    checkConfig.remove();

    QTextStream streamInput(stdin);

    // Iterate for every Name Devices
    foreach(QString pwrName, devices.keys())
    {
        pwr_settings.setValue(pwrName, devices[pwrName]->pwraddr);
        pwr_settings.sync();
    }

    return "0";
}

QString PWR_Device::LoadPWRSettings()
{
    // Check that config file is exists
    QFile checkConfig(pathSavePwrDevice);
    if(checkConfig.exists())
    {
        // Clear storage for found devices before processing
        devices.clear();

        // Get COM ports from ini
        QStringList keys = pwr_settings.allKeys();

        qInfo() << "You can use the next found PWR devices" << "\n";
        qInfo() << "----------------------------------";

        foreach (QString key, keys)
        {
			//key ->name device
			
            QString strValue = pwr_settings.value(key).toString();//USB address

			PWR_Device* dev = new PWR_Device(key, strValue, StringToType(key));
            devices.insert(key, dev);
            qInfo()<< key;
        }

        qInfo() << "----------------------------------" << "\n";

        return "0";

    }
    else
    {
        qInfo() << "Not found file: "+pathSavePwrDevice;
        return "Error: Not foun file"+pathSavePwrDevice;
    }
}

PWR_Device::Type_pwr_device PWR_Device::StringToType(QString name)
{
	PWR_Device::Type_pwr_device t;
	for (int i = 0; i < sizeof(array_type_pwr) / sizeof(QString); i++) {
		if (name.contains(array_type_pwr[i])) {
			switch (i) {
			case 0:
				t = PWS4323;
				break;
			case 1:
				t = DP832A;
				break;
			case 2:
				t = PSU7;
				break;
			case 3:
				t = N5747A;
				break;
			default:break;
			}
		}
	}
	return t;

}
