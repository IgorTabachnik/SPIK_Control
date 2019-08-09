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
	Lot::bStart = false;
	ui->bStartTest->setEnabled(false);
	ui->bShowResult->setEnabled(false);
	startTimer(100);
	state_test = 0;
}

void MainWindow::on_bShowResult_clicked() 
{
	testResult = new PreTestResult(th_control_func_test->getStatesFunctionTests());
	testResult->show();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
	if (state_test == 2) {
		if (th_control_func_test->isFinished()) {
			lot->SaveData(th_control_func_test->getResultFunctionTest());
			testResult = new PreTestResult(th_control_func_test->getStatesFunctionTests());
			testResult->show();

			ui->bStartTest->setText("Finish Test");
			ui->bStartTest->setEnabled(true);
			ui->bShowResult->setEnabled(true);

			state_test = 3;
		}
	}

	if (state_test == 4) {
		if (th_control_func_test != nullptr) {
			if (th_control_func_test->isFinished()) {
				Sleep(2000);
				PWR_Device::AllPwrOff();
				ui->bShowResult->setEnabled(false);
				ui->bStartTest->setEnabled(true);
				state_test = 0;
			}
		}
	}

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
		if (settings.value(CONFIG_PATH, "").toString() == "") {
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
		
        r = PWR_Device::AllSetSettings(32, 6, 1);
		qInfo() << "Set Voltage <-32 Current <- 8 channel 1; Error is "+r;
		
        r = PWR_Device::AllPwrOn();//After bug debug for access memory
		qInfo() << "Power ON; Error is " + r;

		qInfo() << " ----Measure Voltage----";
		for each (QString key in PWR_Device::devices.keys())
		{
			r = PWR_Device::devices[key]->MeasVoltage();
			if (r.contains("Error")) {
				QMessageBox::critical(this, "Error Power", r);
				return;
			}
			else {
				qInfo() << "PWR Device ->" + key + "Voltage --->" + r;
			}
		}

        QString pathConfig = settings.value(CONFIG_PATH,"").toString() + "/" + ui->ComBoxConfigFile->currentText();
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

		excel->ClearAndClose();

        th_control_func_test = new ThControlFuncTest(excel);
        connect(th_control_func_test, SIGNAL(SendMesToMain(QString)),this,SLOT(ReceiveMessage(QString)));
		connect(th_control_func_test, SIGNAL(sendProgressTest(int)), this, SLOT(UpdateProgressStatus(int)));
        th_control_func_test->start();
        start_test=true;
		ui->bStartTest->setText("Abort Test");
		ui->progressBar->setValue(0);
		state_test = 1;

    }

	else
	{
		th_control_func_test->work = false;
		ui->bStartTest->setText("Start Test");
		start_test = false;


		if (state_test==1) {
			ui->bStartTest->setEnabled(false);
			qInfo() << "----------------------";
			qInfo() << "Function test aborted!";
			qInfo() << "----------------------";
		}else if(state_test == 3){
			qInfo() << "----------------------";
			qInfo() << "Function test finished!";
			qInfo() << "----------------------";
		}

		state_test = 4;//PWR off
    }

}

void MainWindow::on_bStartLot_clicked() {
	QString nameLot = ui->lineEdit->text();
	if (nameLot.isEmpty()) {
		QMessageBox::critical(this, "Error", "The name of the lot is empty!, please enter the name of lot");
		return;
	}
	QString err;
	QString path = settings.value(RESULT_PATH, "").toString() + "/" + nameLot + ".csv";

	if (Lot::bStart) {
		bool ok1; bool ok2;
		int in = ui->IN_QTY->text().toInt(&ok1, 10);
		int out = ui->OUT_QTY->text().toInt(&ok2, 10);
		if (ok1 && ok2) {
			err = lot->EndLot(in, out);
		}
		else
		{
			err = lot->EndLot();
		}

		if (err != "0") {
			QMessageBox::critical(this, "Error End lot", err);
			return;
		}
		ui->bStartLot->setText("Start Lot");

		QString r = PWR_Device::AllPwrOff();//After bug debug for access memory
		qInfo() << "Power ON; Error is " + r;

		ui->bStartTest->setEnabled(false);
		ui->lineEdit->clear();
	}
	else {
		lot = new Lot(path);
		err = lot->StartLot();

		if (err != "0") {
			QMessageBox::critical(this, "Error End lot", err);
			return;
		}
		ui->bStartLot->setText("End Lot");

		ui->bStartTest->setEnabled(true);
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
	if (percent == 100) {
		if (state_test == 1) {
			state_test = 2;
		}
		
	}
}
