#include "Lot.h"
QString Lot::start = "Start >; hh;mm;ss; ;Day;Month;Year";
QString Lot::end = "End >; hh;mm;ss; ;Day;Month;Year";
QString Lot::title = "Device; MKM1; MKM2; MKM3; MKM4; MKM5; MKM6; MKM7; MKM8";
bool Lot::bStart = false;

Lot::Lot(QObject *parent, QString pFile)
	: QObject(parent), pathFile(pFile)
{
	bStart = false;
}

Lot::Lot(QString pFile)
	:pathFile(pFile)
{
	bStart = false;
}

Lot::~Lot()
{
}

QString Lot::SaveData(QStringList data)
{
	QString err = "0";
	bool flag;
	QFile f(pathFile);
	if (f.exists()) {
		flag = f.open(QIODevice::WriteOnly | QIODevice::Append);
		if (!flag) {
			return "Could not open file for write-> " + pathFile;
		}
	}
	else {
		flag = f.open(QIODevice::WriteOnly | QIODevice::Text);
		if (!flag) {
			return "Could not open file for write-> " + pathFile;
		}
	}

	QTextStream in(&f);
	for each (QString var in data)
	{
		in << var <<"\n";
	}
	f.close();

	return err;
}


QString Lot::StartLot()
{
	QTime currTime = QTime::currentTime();
	QDate currDate = QDate::currentDate();
	QStringList data;
	data.append(start);
	data.append(";"+currTime.toString("hh;mm;ss")+"; ;"+currDate.toString("dd;MM;yy"));
	data.append(title);
	QString err = SaveData(data);
	if (err != "0") {
		return err;
	}
	bStart = true;
	return "0";
}


QString Lot::EndLot(int in, int out) {
	QTime currTime = QTime::currentTime();
	QDate currDate = QDate::currentDate();
	QStringList data;
	data.append(end);
	data.append(";" + currTime.toString("hh;mm;ss") + "; ;" + currDate.toString("dd;MM;yy"));
	data.append(QString("IN=;%1; ;OUT=;%2").arg(in).arg(out));
	QString err = SaveData(data);
	if (err != "0") {
		return err;
	}
	bStart = false;
	return "0";
}

QString Lot::EndLot()
{
	QTime currTime = QTime::currentTime();
	QDate currDate = QDate::currentDate();
	QStringList data;
	data.append(end);
	data.append(";" + currTime.toString("hh;mm;ss") + "; ;" + currDate.toString("dd;MM;yy"));
	QString err = SaveData(data);
	if (err != "0") {
		return err;
	}
	bStart = false;
	return "0";
}
