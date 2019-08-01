#include <QElapsedTimer>
#include <QTextStream>
#include "commonThread.h"
#include "mainwindow.h"

QMap < QString, QList <DeviceModbus> > commonThread::foundDevices;
QString commonThread::settingsFilePath = QDir::currentPath() + CONFIG_ModBus;// Store settings as .ini
QSettings commonThread::appSettings(commonThread::settingsFilePath, QSettings::IniFormat);
bool commonThread::saveData = false;

commonThread::commonThread(searchSettings *settings)
    : m_searchSettings(settings->m_searchTuning), m_baudRate(settings->m_baud), m_timeOutValue(settings->m_timeout)
{
    tempFoundDevices = new QMap < QString, QList <DeviceModbus> *>();
    m_timeOut = false;
    saveData = false;
}


commonThread::commonThread()
{
    // Store settings as .ini
    //appSettings = new QSettings(settingsFilePath, QSettings::IniFormat);
}

commonThread::~commonThread()
{
    //delete appSettings;
    //appSettings = nullptr;

    for (int i = 0; i < m_listThreads->size(); i++)
    {
        delete (*m_listThreads)[i];
        (*m_listThreads)[i] = nullptr;
    }

    delete m_listThreads;
    m_listThreads = nullptr;

    delete m_listFoundDevices;
    m_listFoundDevices = nullptr;

    delete objSearch;
    objSearch = nullptr;
}

void commonThread::run()
{
    m_listThreads = new QList <ThSearchDevices*> ();

    // Empty the previous thread
    foreach (QString port, tempFoundDevices->keys())
    {
        delete (*tempFoundDevices)[port];
        (*tempFoundDevices)[port] = nullptr;
    }

    // Iterate for every port
    foreach(QString comPortName, m_searchSettings.keys())
    {
        // Check current com port in required range
        qInfo() << "Processing " + comPortName;

        m_listFoundDevices = new QList <DeviceModbus>();

        // Calculate maximum progress bar value        
        QList <quint16> addrRng = m_searchSettings[comPortName];

        // Initialize thread
        objSearch = new ThSearchDevices(comPortName, addrRng, m_listFoundDevices, m_baudRate);
        connect(objSearch, SIGNAL(valueChanged(int)),
          this, SLOT(changeProgressBarValue(int)));

        // Collect threads
        m_listThreads->append(objSearch);
        (*tempFoundDevices)[comPortName] = m_listFoundDevices;
    }

    // Launch every thread
    foreach(ThSearchDevices *sdev, *m_listThreads)
        sdev->start();


    controlThreads(*m_listThreads);

    foreach(QString comPortName, tempFoundDevices->keys())
    {
        QList <DeviceModbus> listDevices = *((*tempFoundDevices)[comPortName]);
        commonThread::foundDevices[comPortName] = listDevices;

        delete (*tempFoundDevices)[comPortName];
        (*tempFoundDevices)[comPortName] = nullptr;

    }

    showFoundDevices();
    showAbsentDevices();

    if (m_timeOut)
        qInfo() << "Stoped by timeout";

    saveSettings();
    delete tempFoundDevices;
    tempFoundDevices = nullptr;
}

QString commonThread::loadFoundDevices()
{
    // Check that config file is exists
    QFile checkConfig(settingsFilePath);
    if(checkConfig.exists())
    {
        // Clear storage for found devices before processing
        foundDevices.clear();

        // Get COM ports from ini
        QStringList keys = appSettings.allKeys();

        foreach (QString comKey, keys)
        {
            QList <DeviceModbus> *devs = new QList <DeviceModbus>();

            // Get devices names
            QString strValues = appSettings.value(comKey).toString();

            QStringList listValues = strValues.split(",");

            foreach (QString value, listValues)
            {
                if (value.isEmpty())
                {
                    continue;
                }
                else
                {
                    DeviceModbus *device = new DeviceModbus();
                    device->address = value.right(1).toInt();
                    device->comPortNumber = comKey;
                    device->nameDevice = value;
                    if(device->nameDevice.contains("OP")){
                        device->devType = DeviceModbus::OP;
                    }else{
                        device->devType = DeviceModbus::BK;
                    }
                    devs->append(*device);
                    delete device;
                    device = nullptr;
                }
            }
            foundDevices[comKey] = *devs;
            delete devs;
            devs = nullptr;
        }

    qInfo() << "You can use the next found devices" << "\n";
    qInfo() << "----------------------------------";

    showFoundDevices();

    qInfo() << "----------------------------------" << "\n";
    qInfo() << "If you want get new settings, please push \"Search\" button";
    qInfo() << "Attention! Settings above will be cleared!";

    return "0";

    }
    else
    {
        qInfo() << "You don't have saved settings SPiK: Comports and Devices. Please make new search ModBus Devices: BK and  OP";
        return "You don't have saved settings SPiK: Comports and Devices. Please make new search ModBus Devices: BK and  OP";
    }
}

void commonThread::controlThreads(QList <ThSearchDevices*> listThreads)
{
    /*!
    Makes checks threads and stops by timeout
    */
    
    bool isFinishedAll = false;
    QElapsedTimer timer;
    timer.start();

    while ((timer.elapsed() < m_timeOutValue) && !isFinishedAll)
    {
        isFinishedAll = true;

        // Check that current thread is finished
        for (int i = 0; i < listThreads.size();)
        {           
            ThSearchDevices *sdev = listThreads[i];
            isFinishedAll &= sdev->isFinished();
            if (sdev->isFinished())
            {
                listThreads.removeAt(i);
                i = 0;
            }
            else
            {
                i++;
            }
        }
    }

    // Force stop all threads
    if (!isFinishedAll)
    {
        foreach(ThSearchDevices *sdev, listThreads)
        {
            if (!(sdev->isFinished()))
            {
                sdev->Stop();
                m_timeOut = true;
                //Sleep(1000);
                while(!sdev->isStoped)
                {
                    continue;
                }
            }
        }
    }
}

void commonThread::showFoundDevices()
{
    /*!
    Shows devices after new search or from saved settings
    */

    // Iterate for every COM port
    foreach(QString comPortName, foundDevices.keys())
    {
        QList <DeviceModbus> devList = foundDevices[comPortName];

        if (!devList.isEmpty())
        {
            saveData = true;
            qInfo() << comPortName + " has";
            foreach(DeviceModbus outDev, devList)
            {
                QString devType = outDev.nameDevice.left(2);
                qInfo() << "\t" << outDev.nameDevice << "by the address" << outDev.address << "with device type" << devType;
            }
        }
    }
}

void commonThread::showAbsentDevices()
{
    /*!
    Shows absent devices
    */

    // Show addesses that were not found
    foreach(QString value, absentDevices.keys())
    {
        qInfo() << value;
        QStringList tempStat = absentDevices[value].split(";");

        foreach(QString str, tempStat)
        {
            if (!str.isEmpty())
            {
                qInfo() << "\t" << str;
            }
        }
    }
}

void commonThread::saveSettings()
{
    /*!
    Saves settings of found defices as .ini file
    */
    if (saveData)
    {

        qInfo() << "If you want to store found devices for future using, please, press 'Y'";
        qInfo() << "else press any key and press Enter";

        QFile checkConfig(settingsFilePath);

        checkConfig.remove();

        QTextStream streamInput(stdin);
        QString answer = streamInput.readLine();

        if (answer.toUpper() == "Y")
        {
            // Iterate for every COM port
            foreach(QString comPortName, foundDevices.keys())
            {
                QString result = "";
                QList <DeviceModbus> devList = foundDevices[comPortName];

                if (!devList.isEmpty())
                {
                    foreach(DeviceModbus outDev, devList)
                    {
                        result += outDev.nameDevice + ",";
                    }
                    appSettings.setValue(comPortName, result);
                    appSettings.sync();
                }
            }
            qInfo() << "Settings were saved here " + settingsFilePath;
        }
    }
    else
    {
        qInfo() << "Nothing to save";
    }
}

bool commonThread::isRunning()
{
    return work;
}

void commonThread::Stop()
{
    work=false;
}


void commonThread::changeProgressBarValue(int value)
{
    emit valueChanged(value);
}
