#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include "settings_spik.h"
#include "thcontrolfunctest.h"
#include "dmodbussettings.h"
#include "help_func.h"
#include "pwr_device.h"

/*Defining*/
#define ORGANIZATION_NAME "GS NANOTECH"
#define ORGANIZATION_DOMAIN "http://www.gsnanotech.ru/"
#define APPLICATION_NAME "SPiK"
#define SETTINGS_TRAY "settings/tray"
#define CONFIG_PATH "settings/config_path"
#define CONFIG_ModBus "/ModBusDevices.ini"
#define CONFIG_PwrDevice "/PWRDevices.ini"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QSettings settings;

private slots:
	void UpdateProgressStatus(int percent);

    void ReceiveMessage(QString mes);

    void UpdateSettingsSPiK(void);

    void on_bStartTest_clicked();

    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    ThControlFuncTest* th_control_func_test;
    DModBusSettings* d_mb_settings;
    Settings_SPiK* set_spik;
    ExcelConfig* excel;
    PWR_Device* pwr;
    bool start_test;

    QString pathDirConfig;
};

#endif // MAINWINDOW_H
