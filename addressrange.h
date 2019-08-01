#ifndef ADDRESSRANGE_H
#define ADDRESSRANGE_H

#include <QDialog>
#include <QList>

namespace Ui {
class AddressRange;
}

class AddressRange : public QDialog
{
    Q_OBJECT

public:
    explicit AddressRange(QWidget *parent = 0);
    QList<quint16> addrRange;
    ~AddressRange();
    void AddText(QString input);

public slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::AddressRange *ui;
    void setAddrRange(quint8 minCom, quint8 maxCom, quint8 minDev, quint8 maxDev);

    // Set addresses range by default
    const quint8 minAddress = 1;
    const quint8 maxAddress = 255;
};

#endif // ADDRESSRANGE_H
