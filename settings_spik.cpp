#include "settings_spik.h"
#include "ui_settings_spik.h"
#include "mainwindow.h"

Settings_SPiK::Settings_SPiK(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings_SPiK)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings SPiK");

    ui->lineEdit->setText(directory=MainWindow::settings.value(CONFIG_PATH,"").toString());
}

Settings_SPiK::~Settings_SPiK()
{
    delete ui;
}

void Settings_SPiK::on_pushButton_2_clicked()
{
    this->close();
}

void Settings_SPiK::on_pushButton_clicked()
{
    directory = ui->lineEdit->text();
	MainWindow::settings.setValue(CONFIG_PATH,directory);
	MainWindow::settings.sync();

    this->close();
    emit UpdateEvent();
}

void Settings_SPiK::on_toolButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this,"Select the directory containing the configuration files");
    ui->lineEdit->setText(directory);
}

QString Settings_SPiK::getDirConfigPath(void)
{
    return directory;
}
