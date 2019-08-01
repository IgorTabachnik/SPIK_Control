#include "mainwindow.h"
#include "ui_mainwindow.h"

/* При создании главного окна производим установку начальных параметров
 * из настроек, сохранённых в операционной системе(а точнее в регистрах regedit)
 * */
QSettings MainWindow::settings(ORGANIZATION_NAME, APPLICATION_NAME);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("SPiK Control");
    th_control_func_test = nullptr;
    excel = nullptr;

    d_mb_settings = new DModBusSettings(nullptr);
    help_func::updateComboBox( ui->ComBoxConfigFile, help_func::findFilesInDirectory(settings.value(CONFIG_PATH,"").toString(), ExcelConfig::ext) );

    pwr = new PWR_Device();
    connect( pwr, SIGNAL(sendMessage(QString)), this, SLOT(ReceiveMessage(QString)) );
    start_test = false;

}

MainWindow::~MainWindow()
{
    delete ui;

    if(excel!=nullptr)
    delete excel;

    if(th_control_func_test!=nullptr)
    delete th_control_func_test;
}

void MainWindow::on_bStartTest_clicked()
{
    if(!start_test)
	{
		//append choose name file
		pathDirConfig = settings.value(CONFIG_PATH, "").toString();
		if (pathDirConfig == "") {
			QMessageBox::critical(this, "Error", " Path to config files not set ");
			set_spik = new Settings_SPiK();
			set_spik->show();
			return;
		}

        QString r = PWR_Device::SearchPWRDevices();
        if(r!="0")
		{
            QMessageBox::critical(this,"Error", r);
            return;
        }
		
        r = PWR_Device::AllSetSettings(32, 2, 1);
		qInfo() << "Set Voltage <-32 Current <- 2 channel 1; Error is "+r;
		
        r = PWR_Device::AllPwrOn(1);//After bug debug for access memory
		qInfo() << "Power ON; Error is " + r;



        QString pathConfig = pathDirConfig + "/"+ ui->ComBoxConfigFile->currentText();
        if(ui->ComBoxConfigFile->currentText().isEmpty())
		{
            QMessageBox::critical(this,"Error","Сonfig file name is emty!");
            return;
        }

        QString res;
        excel = new ExcelConfig(pathConfig);
        connect(excel,SIGNAL(SendMessageError(QString)),this,SLOT(ReceiveMessage(QString)));

        res = excel->OpenFile();
		if (res != "0") 
		{
			QMessageBox::critical(this, "Error", res);
			return;
		}

		res = excel->ExtractFuncTestTable();
		if (res != "0") 
		{
			QMessageBox::critical(this, "Error", res);
			return;
		}

        th_control_func_test = new ThControlFuncTest(excel);
        connect(th_control_func_test, SIGNAL(SendMesToMain(QString)),this,SLOT(ReceiveMessage(QString)));
		connect(th_control_func_test, SIGNAL(sendProgressTest(int)), this, SLOT(UpdateProgressStatus(int)));
        th_control_func_test->start();
        start_test=true;
		ui->bStartTest->setText("Abort Test");

    }
	else
	{
        ui->bStartTest->setText("Start Test");
        qInfo()<<"------------------------------------";
        qInfo()<<"Function test aborted by user!";
        th_control_func_test->work = false;
        start_test=false;
        Sleep(500);
		qInfo() << "Power OFF";
        PWR_Device::AllPwrOff(1);
        //pwr->InstrWrite(pwr->pwraddr,":OUTP CH1,OFF");
    }

}

void MainWindow::ReceiveMessage(QString mes)
{
    if(mes.contains("Error")){
        QMessageBox::critical(this,"Error", mes);
    }

    if(mes.contains("GUI"))
	{
        if(mes.contains("Search Device"))
		{
            d_mb_settings->exec();
            //дать возможность заново производить поиск устроств а не по случаю отсутствия файла настроек.
        }
    }

    if(mes.contains("Console"))
	{
		qInfo() << "---------------------------------" << "\n";
        qInfo()<<mes<<"\n";
		qInfo() << "---------------------------------" << "\n";
    }

}
void MainWindow::UpdateSettingsSPiK(void){
	qInfo() << "Update List of ConfigFiles! ";
	QStringList files = help_func::findFilesInDirectory(set_spik->getDirConfigPath(), ExcelConfig::ext);
	for each (QString var in files)
	{
		qInfo() << var;
	}
    help_func::updateComboBox(ui->ComBoxConfigFile, files);
}

void MainWindow::on_actionSettings_triggered()
{
    set_spik = new Settings_SPiK();
    connect( set_spik, SIGNAL( UpdateEvent(void) ), this, SLOT( UpdateSettingsSPiK(void) ) );
    set_spik->show();
}

void MainWindow::UpdateProgressStatus(int percent)
{
	ui->progressBar->setValue(percent);
}
