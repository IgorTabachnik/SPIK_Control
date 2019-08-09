#pragma once

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QDate>

class Lot : public QObject
{
	Q_OBJECT

public:
	Lot(QObject *parent, QString pFile);
	Lot(QString pFile);
	~Lot();
	static QString start;
	static QString end;
	static QString title;
	QString StartLot();
	QString EndLot();
	QString EndLot(int in, int out);

	QString SaveData(QStringList data);
	static bool bStart;

private:
	QString pathFile;
	
};
