#ifndef PWR_DEVICE_H
#define PWR_DEVICE_H

#include <QObject>
#include <winsock2.h>
#include <windows.h>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include "visa.h"

#define MAX_REC_SIZE 1024

class PWR_Device : public QObject
{
    Q_OBJECT
public:
	
    PWR_Device(QString pwraddr);
	PWR_Device();

    QString pwraddr;
	QString pwrname;
	typedef enum
	{
		PWS4323,
		DP832A,
		PSU7,
		N5747A
	}Type_pwr_device;
	Type_pwr_device type;

	PWR_Device(QString name, QString pwraddr, Type_pwr_device type);

    bool InstrWrite(QString strContent);//write function
    bool InstrRead(QString *pstrResult);//read from the instrument

	QString MeasVoltage();
	QString PwrOn();
	QString PwrOff();

    static QString AllSetSettings(qreal Voltage, qreal Current, int channel);
	static QString AllPwrOff();
	static QString AllPwrOn();
    static QString SearchPWRDevices();
    
    static QSettings pwr_settings;
    static QString pathSavePwrDevice;
    static QString SavePWRSettings();
    static QString LoadPWRSettings();


	static QString array_type_pwr[5];
    static QMap<QString, PWR_Device*> devices;
	static Type_pwr_device StringToType(QString name);

signals:
    void sendMessage(QString mes);


public slots:

};

#endif // PWR_DEVICE_H
