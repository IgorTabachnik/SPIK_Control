#include "settings_spik.h"
#include "ui_settings_spik.h"
#include "mainwindow.h"

Settings_SPiK::Settings_SPiK(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings_SPiK)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings SPiK");

    ui->lineEdit->setText(dirConfig = MainWindow::settings.value(CONFIG_PATH,"").toString());
	ui->lineEdit_2->setText(dirResult = MainWindow::settings.value(RESULT_PATH, "").toString());
}

Settings_SPiK::~Settings_SPiK()
{
    delete ui;
}

void Settings_SPiK::on_pushButton_clicked()
{
	dirConfig = ui->lineEdit->text();
	MainWindow::settings.setValue(CONFIG_PATH,dirConfig);
	MainWindow::settings.setValue(RESULT_PATH, dirResult);
	MainWindow::settings.sync();

    this->close();
    emit UpdateEvent();
}

void Settings_SPiK::on_pushButton_2_clicked()
{
	this->close();
}

void Settings_SPiK::on_toolButton_clicked()
{
    dirConfig = QFileDialog::getExistingDirectory(this,"Select the directory containing the configuration files");
    ui->lineEdit->setText(dirConfig);
}

void Settings_SPiK::on_toolButton_2_clicked()
{
	dirResult = QFileDialog::getExistingDirectory(this, "Select the directory for Result files");
	ui->lineEdit_2->setText(dirResult);
}

QString Settings_SPiK::getDirConfigPath(void)
{
	return dirConfig;
}

QString Settings_SPiK::getDirResultPath(void)
{
	return dirResult;
}