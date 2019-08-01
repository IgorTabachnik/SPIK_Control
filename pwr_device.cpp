#include "pwr_device.h"
#include "mainwindow.h"

QString PWR_Device::pwr_array[5]={"PWS4323","DP832A","PSU7","N5747A"};
QString PWR_Device::pathSavePwrDevice = QDir::currentPath()+CONFIG_PwrDevice;
QSettings PWR_Device::pwr_settings(PWR_Device::pathSavePwrDevice,QSettings::IniFormat);
QMap<QString, QString> PWR_Device::devices;

PWR_Device::PWR_Device()
{

}

QString PWR_Device::AllInstrWrite(QString instr)
{
    QString res="0";
    PWR_Device power;
    foreach (QString key, devices.keys()) {
        QString pwr_addr = devices[key];
        bool ok = power.InstrWrite(pwr_addr,instr);
        if(!ok){
            res = "Error: transmit cmd is "+instr+" to device: "+key+" is fail";
        }
    }
    return res;
}

QString PWR_Device::AllSetSettings(float Voltage, float Current, int channel)
{
    QString res="0";
    PWR_Device power;
    foreach (QString key, devices.keys()) {
        QString pwr_addr = devices[key];
        QString cmd = QString(":APPL CH%1, %2, %3").arg(channel).arg(Voltage).arg(Current);
        bool ok = power.InstrWrite(pwr_addr,cmd);
        if(!ok){
            res = "Error: transmit cmd is "+cmd+" to device: "+key+" is fail";
        }
    }
    return res;
}

QString PWR_Device::AllPwrOn(int channel){
    QString res="0";
    PWR_Device power;
    foreach (QString key, devices.keys()) {
        QString pwr_addr = devices[key];
        QString cmd = QString(":OUTP CH%1,ON").arg(channel);
        bool ok = power.InstrWrite(pwr_addr,cmd);
        if(!ok){
            res = "Error: transmit cmd is "+cmd+" to device: "+key+" is fail";
        }
    }

    return res;
}

QString PWR_Device::AllPwrOff(int channel){
    QString res="0";
    PWR_Device power;
    foreach (QString key, devices.keys()) {
        QString pwr_addr = devices[key];
        QString cmd = QString(":OUTP CH%1,OFF").arg(channel);
        bool ok = power.InstrWrite(pwr_addr,cmd);
        if(!ok){
            res = "Error: transmit cmd is "+cmd+" to device: "+key+" is fail";
        }
    }
    return res;
}

QString PWR_Device::SearchPWRDevices()
{
    PWR_Device power;
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

        // Find resource
        status = viFindRsrc(defaultRM,expr,findList, retcnt, instrDesc);
        for (i = 0;i < (*retcnt);i++)
        {
         // Get instrument name
         strSrc = QString::fromUtf8(instrDesc);
         power.InstrWrite(strSrc,"*IDN?");
         Sleep(200);
         power.InstrRead(strSrc,&strInstr);

         strInstr.toUpper();

         if(strInstr.size()>0)
         {
            bFind = true;
            devices.insert(strInstr, strSrc);
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


bool PWR_Device::InstrWrite(QString strAddr, QString strContent)
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
    ba1 = strAddr.toLocal8Bit();
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

    emit sendMessage("Console: send commnd: "+strContent+" to pwr supply-> "+strAddr);
    return true;
}

bool PWR_Device::InstrRead(QString strAddr, QString *pstrResult)
{
    ViSession defaultRM,instr;
    ViStatus status; ViUInt32 retCount;
    char * SendAddr = NULL;
    unsigned char RecBuf[MAX_REC_SIZE];
    bool bReadOK = false;
    QString str;
    QByteArray ba1,ba2;

    memset(RecBuf,0,MAX_REC_SIZE);

    //Change the adress's data style from QString to char*
    ba1 = strAddr.toLocal8Bit();
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

    //read from instrument
    status = viRead(instr,RecBuf, MAX_REC_SIZE, &retCount);

    //close the instrument
    status = viClose(instr);
    status = viClose(defaultRM);


    pstrResult->clear();
    pstrResult->append((char*)RecBuf);

    return bReadOK;
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
        QString result = "";
        QString pwrAddr = devices[pwrName];

        pwr_settings.setValue(pwrName, pwrAddr);
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
            // Get devices names
            QString strValue = pwr_settings.value(key).toString();
            devices.insert(key, strValue);
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
