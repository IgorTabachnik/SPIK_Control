#ifndef DMODBUSSETTINGS_H
#define DMODBUSSETTINGS_H

#include <QMap>
#include <QDialog>
#include <QList>
#include <QString>
#include <QMessageBox>
#include <QTextStream>
#include <QMessageBox>
#include <QProgressBar>

#include "commonThread.h"
#include "searchsettings.h"
//#include "settingsmbdevice.h"
#include "modbusConnection.h"
#include "qextserialenumerator.h"
#include "addressrange.h"

namespace Ui {
class DModBusSettings;
}

class DModBusSettings : public QDialog
{
    Q_OBJECT

public:

    explicit DModBusSettings(QWidget *parent = 0);
    ~DModBusSettings();

    void releaseSerialModbus(modbus_t* mb);
    Ui::DModBusSettings *ui;

    QString LoadDevices_FromFile(void);
    int getBaudRate();    

private slots:
    void on_SearchDevices_clicked();
    void on_LoadDevices_clicked();
    void updateProgressBar(int value);

private:

    bool checkInput;

    int maxProgressValue, m_Baudrate, m_timeOutValue;

    quint16 addrComMinUser, addrComMaxUser, addrDevMinUser, addrDevMaxUser, addrComMin, addrComMax;
   
    searchSettings *settings;

    commonThread *objThread;

    QList <commonThread*> *m_listThreads;
    QList <quint8> *portAddresses;
    QStringList *allPorts;

    QString portsInfo, settingsFilePath;

    void getInputData(); // Fills searchSettings structure
    void getPortAddresses(QString settings);
    void getDeviceAddresses(QString settings);
    bool getConnection(QString comPortNumber);
    quint16 extractComPortAddress(QString comPortName);	 
    QString UpdateListComPorts(QList <QextPortInfo> *ports);

signals:
    void sendMessage(const QString s);

};

#endif // DMODBUSSETTINGS_H
