#include <QtWidgets>
#include "addressrange.h"
#include "ui_addressrange.h"

AddressRange::AddressRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressRange)
{
    ui->setupUi(this);
    this->setWindowTitle("Настройки диапазона поиска");

    // Disable close button
    setWindowFlags(windowFlags() ^ Qt::WindowCloseButtonHint);

    // Set address range for search by default    
    ui->lineEditMinCom->setText(QString::number(minAddress));
    ui->lineEditMaxCom->setText(QString::number(maxAddress));
    ui->lineEditMinDev->setText(QString::number(minAddress));
    ui->lineEditMaxDev->setText(QString::number(maxAddress));

}

AddressRange::~AddressRange()
{
    delete ui;
}

void AddressRange::on_pushButtonOk_clicked()
{
    /*!
      Gets data from UI and sets new values after push on OK button
     */

    // Get data from UI
    quint16 minCom = ui->lineEditMinCom->text().toInt();
    quint16 maxCom = ui->lineEditMaxCom->text().toInt();
    quint16 minDev = ui->lineEditMinDev->text().toInt();
    quint16 maxDev = ui->lineEditMaxDev->text().toInt();

    // Test input data
    bool checkInput = false;
    if (maxCom > 255 || minCom < 1 ||
        minCom > 255 || maxCom < 1)
        QMessageBox::warning(this, "Warning!", "Некорректный диапазон адресов для COM порта (1-255)");
    else if (maxDev > 255 || minDev < 1 ||
             minDev > 255 || maxDev < 1)
        QMessageBox::warning(this, "Warning!", "Некорректный диапазон адресов для устройств (1-255)");
    else if (maxCom < minCom ||
             maxDev < minDev)
        QMessageBox::warning(this, "Warning!", "Минимальный адрес превышает максимальный!");
    else
        checkInput = true;


    // Transmit it to storage and close the dialog
    if (checkInput)
    {
        // Empty storage
        addrRange.clear();

        // Add new values
        addrRange.append(minCom);
        addrRange.append(maxCom);
        addrRange.append(minDev);
        addrRange.append(maxDev);

        // Close UI
        close();
    }
}

void AddressRange::on_pushButtonCancel_clicked()
{
    /*!
      Process Cancel button
     */

    // Empty storage
    addrRange.clear();

    // Add new values
    addrRange << 0 << 0 << 0 << 0;

    // Set address range
    ui->lineEditMinCom->setText("");
    ui->lineEditMaxCom->setText("");
    ui->lineEditMinDev->setText("");
    ui->lineEditMaxDev->setText("");

    // Close UI
    close();
}

void AddressRange::AddText(QString input)
{
    /*!
      Sets test in UI
     */

    ui->labelOut->setText(input);
}
