#include "dmodbussettings.h"
#include "ui_dmodbussettings.h"


DModBusSettings::DModBusSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DModBusSettings)
{
    ui->setupUi(this);
    this->setWindowTitle("Настройки RTU ModBus");

    // Initialize string with names of ports. It needs to out to console
    portsInfo = "";

    // Set time of search by default
    ui->lineSearch->setText("60");

    portAddresses		= new QList <quint8>();
    allPorts			= new QStringList();

    // Get COM ports that were found	
	QList <QextPortInfo> *ports = new QList <QextPortInfo>();
    QString err = UpdateListComPorts(ports);

    if(err=="0"){
        settings = new searchSettings();

        m_listThreads = new QList <commonThread*>();

        addrComMax = 1;
        addrComMin = 255;

        // Iterate for every port
        foreach(QextPortInfo port, *ports)
        {
            QString comPortNumber = port.portName;

            // Skip any not COM port. LPT, for example
            if (!(comPortNumber.contains("COM")) ||
              (comPortNumber.contains("com")) ||
               (comPortNumber.contains("Com")))
                continue;

            // Collect addresses of ports
            quint16 portAddr = extractComPortAddress(comPortNumber);
            *portAddresses << portAddr;

            // Collect COM ports into another storage as names
            *allPorts << comPortNumber;
            portsInfo += comPortNumber + "    ";

            // Define max and min values of com ports
            if (portAddr > addrComMax)
                addrComMax = portAddr;

            if (portAddr < addrComMin)
                addrComMin = portAddr;
        }
    }else{
        QMessageBox::warning(this, "Warning!", err);

        // Disable Search button if no ports
        ui->SearchDevices->setEnabled(false);
    }

	delete ports;
	ports = nullptr;
}

DModBusSettings::~DModBusSettings()
{
	delete ui;

	delete portAddresses;
	portAddresses = nullptr;

	delete allPorts;
	allPorts = nullptr;

    for (int i = 0; i < m_listThreads->size(); i++)
    {
        delete (*m_listThreads)[i];
        (*m_listThreads)[i] = nullptr;
    }

    delete m_listThreads;
    m_listThreads = nullptr;

    delete objThread;
    objThread = nullptr;

    delete settings;
    settings = nullptr;

}

void DModBusSettings::getPortAddresses(QString answer)
{
    /*!
        Allows choose settings for COM ports
    */

    // Create input stream
    QTextStream streamInput(stdin);

    // Create variable to make some checks
    bool readData = false;

    // Choose settings if need range of ports
    if (answer.toUpper() == "Y")
    {
        // Initialize input parameters
        addrComMinUser = 0;
        addrComMaxUser = 0;

        // Set filter for COM ports        
        while(!readData)
        {
            qInfo() << "Input min address of COM port";
            QString minCom = streamInput.readLine();
            addrComMinUser = minCom.toInt(&readData);

            qInfo() << "Input max address of COM port";
            QString maxCom = streamInput.readLine();
            addrComMaxUser = maxCom.toInt(&readData);

            if (addrComMinUser < addrComMin || addrComMinUser > addrComMax)
            {
                qInfo() << "Input COM port MIN address is not in range";
                readData = false;
            }

            else if (addrComMaxUser > addrComMax || addrComMaxUser < addrComMin)
            {
                qInfo() << "Input COM port MAX address is not in range";
                readData = false;
            }

            else if ( (addrComMaxUser == addrComMinUser) && (!(*portAddresses).contains(addrComMaxUser)) )
            {
                qInfo() << "Your COM port address is not in list of allowed. Please try again";
                readData = false;
            }

            else if (addrComMaxUser < addrComMinUser)
            {
                qInfo() << "Warning! MAX address more than MIN address!";
                readData = false;
            }

            else
                readData = true;
        }

        // Collect ports in range of max and min values
        foreach (QString comPortName, *allPorts)
        {
            quint8 comNum = extractComPortAddress(comPortName);
            if (!(comNum > addrComMaxUser || comNum < addrComMinUser))
            {
                QList<quint16> initialAddresses = {0, 0};
                settings->m_searchTuning.insert(comPortName, initialAddresses);
            }
        }
    }

    // Choose settings if you need list of ports
    else if (answer.toUpper() == "N")
    {
        while(!readData)
        {
            qInfo() << "Type your values";

            // Get addresses of COM ports
            QString portValues = streamInput.readLine();

            // Convert input string into list
            QStringList tempPortStorage = portValues.split(",");

            // Collect ports
            foreach (QString comPortName, *allPorts)
            {
                quint8 comNum = extractComPortAddress(comPortName);
                foreach (QString value, tempPortStorage)
                {
                    if (comNum == value.toInt())
                    {
                        QList<quint16> initialAddresses = {0, 0};
                        settings->m_searchTuning.insert(comPortName, initialAddresses);
                    }
                }
            }

            readData = settings->m_searchTuning.isEmpty() ? false : true;
        }
    }
}

void DModBusSettings::getInputData()
{
    /*!
        Gets input data from console input
     */

    QTextStream streamInput(stdin);

    // Set filter for COM ports
    bool readData = false;
    while(!readData)
    {
        // Clear console
        system("CLS");

        qInfo() << "The next COM ports were found: " + portsInfo;
        qInfo() << "Please define settings for COM ports";
        qInfo() << "Y - you will use range of addresses of ports. You can type values between" << addrComMin << "and" << addrComMax;
        qInfo() << "N - you will write COM port addresses using comma separator (1,2 for example)";
        qInfo() << "Type and push ENTER";

        QString searchModePort  = streamInput.readLine();
        
        if ( ( searchModePort.toUpper() == "Y") || (searchModePort.toUpper() == "N") )
        {
            getPortAddresses(searchModePort);
            readData = true;
            
        }             
    }
    
    QString outInfo = "";
    foreach (QString portValue, settings->m_searchTuning.keys())
        outInfo += portValue + "    ";
    

    readData = false;
    while(!readData)
    {
        qInfo() << "You chose the next ports " + outInfo << "\n\n";

        // Set filter for devices
        qInfo() << "Please define settings for devices";
        qInfo() << "Y - you will use common addresses of devices for every COM port";
        qInfo() << "\t" << "It means if you have COM1, COM2 etc search for every port will be make with same range of addresses";
        qInfo() << "N - you will use separated addresses of devices for every COM port";
        qInfo() << "\t" << "For example, if you have COM1 and COM2, you can use range of addresses between 1 and 10 for the first and 50-55 for the second";
        qInfo() << "Type and push ENTER";

        QString searchModeDevice    = streamInput.readLine();
        if ( (searchModeDevice.toUpper() == "Y") || (searchModeDevice.toUpper() == "N") )
        {
            readData = true;
            getDeviceAddresses(searchModeDevice);
        }
    }
}

void DModBusSettings::getDeviceAddresses(QString answer)
{
    /*!
      Allows choose settings for range of addresses of devices
    */

    // Create input stream
    QTextStream streamInput(stdin);

    bool readData = false;

    // Use this branch if need use common settings for every port
    if (answer.toUpper() == "Y")
    {
        // Set filter for devices
        while(!readData)
        {
            qInfo() << "Input min value for device";
            QString minDev = streamInput.readLine();
            addrDevMinUser = minDev.toInt(&readData);

            qInfo() << "Input max value for device";
            QString maxDev = streamInput.readLine();
            addrDevMaxUser = maxDev.toInt(&readData);

            if (addrDevMinUser > addrDevMaxUser)
            {
                qInfo() << "Warning! MAX address of device is more than MIN address!";
                readData = false;
            }

            else if (addrDevMinUser > 255 || addrDevMinUser < 1)
            {
                qInfo() << "Input MIN device address is not in range";
                readData = false;
            }

            else if (addrDevMaxUser > 255 || addrDevMaxUser < 1)
            {
                qInfo() << "Input MAX device address is not in range";
                readData = false;
            }

            else
                readData = true;
        }

        foreach (QString comPortName, settings->m_searchTuning.keys())
        {
            QList<quint16> addresses = {addrDevMinUser, addrDevMaxUser};
            maxProgressValue += addrDevMaxUser - addrDevMinUser + 1;
            settings->m_searchTuning[comPortName] = addresses;
        }
    }

    // Use this branch if need use separated settings for every port
    else if (answer.toUpper() == "N")
    {
        foreach (QString comPortName, settings->m_searchTuning.keys())
        {            
            // Set filter for devices
            readData = false;
            while(!readData)
            {
                qInfo() << "Input min value for device" << comPortName;
                QString minDev = streamInput.readLine();
                addrDevMinUser = minDev.toInt(&readData);

                qInfo() << "Input max value for device" << comPortName;
                QString maxDev = streamInput.readLine();
                addrDevMaxUser = maxDev.toInt(&readData);

                if (addrDevMinUser > addrDevMaxUser)
                {
                    qInfo() << "Warning! MAX address of device is more than MIN address!";
                    readData = false;
                }

                else if (addrDevMinUser > 255 || addrDevMinUser < 1)
                {
                    qInfo() << "Input MIN device address is not in range";
                    readData = false;
                }

                else if (addrDevMaxUser > 255 || addrDevMaxUser < 1)
                {
                    qInfo() << "Input MAX device address is not in range";
                    readData = false;
                }

                else
                    readData = true;
            }

            QList<quint16> addresses = {addrDevMinUser, addrDevMaxUser};
            maxProgressValue += addrDevMaxUser - addrDevMinUser + 1;
            settings->m_searchTuning[comPortName] = addresses;
        }
    }
}

void DModBusSettings::on_LoadDevices_clicked()
{
    /*!
        Loads settings of devices from saved .ini file if it exists
    */
    LoadDevices_FromFile();
}

QString DModBusSettings::LoadDevices_FromFile(void)
{
    // Clear console
    system("CLS");
    objThread = new commonThread();
    m_listThreads->append(objThread);
    return objThread->loadFoundDevices();
}

void DModBusSettings::on_SearchDevices_clicked()
{
    /*!
     Starts up search of devices using multithread
    */

    // Clear progress bar
    maxProgressValue = 0;
    ui->progressSearch->setValue(maxProgressValue);
    
    // Empty previous threads
    settings->m_searchTuning.clear();

    // Get timeout from UI and convert this value to milliseconds
    m_timeOutValue = ui->lineSearch->text().toInt();
    m_timeOutValue *= 1000;
    settings->m_timeout = m_timeOutValue;

    // Initialize baud rate from UI
    m_Baudrate = ui->comboBaudeRate->currentText().toInt();
    settings->m_baud = m_Baudrate;

    QMessageBox::warning(this, "Warning!", "Для ввода настроек поиска необходимо\nвоспользоваться командной строкой");
    
    // Initialize input parameters
    getInputData();

    // Clear console
    system("CLS");
    ThSearchDevices::progressValue = 0;
    commonThread::absentDevices.clear();

    // Set max value in progress bar
    ui->progressSearch->setMaximum(maxProgressValue);

    objThread = new commonThread(settings);
    m_listThreads->append(objThread);
    connect(objThread, SIGNAL(valueChanged(int)), this, SLOT(updateProgressBar(int)) );
    objThread->start();
    
}

void DModBusSettings::updateProgressBar(int value)
{
    ui->progressSearch->setValue(value);
    if (objThread->m_timeOut)
        ui->progressSearch->setValue(maxProgressValue);
}

quint16 DModBusSettings::extractComPortAddress(QString comPortName)
{
    /*!
      Extracts digit value from COM port address
     */

    // Get com port digit address
    QString tempNumCom  = "";
    for (int i = 0; i < comPortName.size(); i++ )
        if (comPortName[i].isDigit())
            tempNumCom += comPortName[i];

    // Check current com port in required range
    quint16 comNum = tempNumCom.toInt();

    return comNum;
}

int DModBusSettings::getBaudRate()
{
    return m_Baudrate;
}

QString DModBusSettings::UpdateListComPorts(QList <QextPortInfo> *list)
{
    QString err="0";
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if(ports.isEmpty())
    {
        return "No ComPort found";
    }
    //ports.append({"\\\\.\\COM1","\\\\.\\COM1","\\\\.\\COM1","\\\\.\\COM1",0,1});
    ports.append({"COM2","\\\\.\\COM2","\\\\.\\COM2","\\\\.\\COM2",0,1});
    ports.append({"COM3","\\\\.\\COM3","\\\\.\\COM3","\\\\.\\COM3",0,1});
    ports.append({"COM5","\\\\.\\COM5","\\\\.\\COM5","\\\\.\\COM5",0,1});
    ports.append({"COM4","\\\\.\\COM4","\\\\.\\COM4","\\\\.\\COM4",0,1});

    list->clear();
    foreach( QextPortInfo port, ports)
    {
        list->append(port);
    }
    return err;
}
