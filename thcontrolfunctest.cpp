#include "thcontrolfunctest.h"

ThControlFuncTest::ThControlFuncTest(ExcelConfig* e)
{
    this->excel = e;
    list_th = new QList<ThPollDevices*>();
    work = true;
	done = false;
}

ThControlFuncTest::~ThControlFuncTest()
{
    delete list_th;
}

void ThControlFuncTest::run(){
	
    QString r = commonThread::loadFoundDevices();
    if(r != "0")
	{
        emit SendMesToMain("Search Devices GUI");
        return;
    }

    bool empty_list_devices = true;
    QMapIterator<QString, QList<DeviceModbus>> i(commonThread::foundDevices);
    list_con = new QList<modbusConnection*>();
    while(i.hasNext()){
        i.next();
        if(i.value().length()>0){
            modbusConnection* con = new modbusConnection(i.key(), BaudeRateDef);
            bool success = con->connect();
            if(success){
                list_con->append(con);
                empty_list_devices = false;
                FunctionTest *f = new FunctionTest(con, &(i.value()), excel);
                ThPollDevices* poll_devs = new ThPollDevices(f);
                connect(poll_devs, SIGNAL(sendMessage(QString)), this, SLOT(TransitMessage(QString)));
                list_th->append(poll_devs);

            }else{
                emit SendMesToMain("Console: Can not to connect COM port -> "+i.key());
            }
        }
    }

    if(empty_list_devices){
        emit SendMesToMain("GUI Error: No Devices for test, please find Devices or check connections with COM port");
        return;
    }

    //start all func test on all devices
    for(int l=0; l<list_th->size(); l++){
       list_th->at(l)->start();
    }

    //control all func test
    int j = 0;
    int percent = 0;
    while(work){
		done = false;
        if(j<list_th->length()){
            ThPollDevices *th = list_th->at(j);

            //need check that the thread is alive
            if(th->work){
                percent = th->getPercentCompleted();
            }else{
                emit SendMesToMain(th->error);
                th->cStop=true;

                if(list_th->length()==1){
                    work = false;
                    emit SendMesToMain("GUI: Error: the last tread poll devices is terminated, the function test is stoped!");
                }
                list_th->removeAt(j);
            }

			emit sendProgressTest(percent);
			if (percent == 100) {
				work = false;
			}
			
			Sleep(100);
			
            //emit SendMesToMain("GUI Progress:" + QString("%1").arg(percent));//Throw BAD ALLOC!!!
            j++;
        }else{
            j=0;
            percent=0;
        }
    }

    //Stop all Thread Poll Devices
    for(int l=0; l<list_th->size(); l++){
       list_th->at(l)->Stop();
	   list_th->at(l)->cStop = true;
	   list_th->at(l)->wait(WaitStopProcess);
    }

    //Disconnect
    for(int l=0; l<list_con->size(); l++){
       list_con->at(l)->freeConnect();
    }

    delete list_con;
	done = true;
}

QStringList ThControlFuncTest::getResultFunctionTest()
{
	QStringList data;
	for (int i = 0; i < list_th->size(); i++) {
		ThPollDevices* th = list_th->at(i);
		FunctionTest* f = th->getFuncTest();
		QMap<QString, FunctionTest::StateDevice> res = f->getResultFunctionTest();

		QString str;
		for each (QString key in res.keys())
		{
			str = key;
			FunctionTest::StateDevice state = res[key];

			//Making results of one device
			for (int j = 0; j < Expected_Count_Of_MKM; j++) {
				if (((1 << j) & 255) == (1 << j)) {
					str.append(QString(";%1").arg(state.results[j], 2, 16));
				}
				else {
					str.append(QString(";NU"));
				}
			}
			data.append(str);
		}
	}
	return data;
}

QMap<QString, FunctionTest::StateDevice> ThControlFuncTest::getStatesFunctionTests()
{
	QMap<QString, FunctionTest::StateDevice> Res;

	for (int i = 0; i < list_th->size(); i++) {
		ThPollDevices* th = list_th->at(i);
		FunctionTest* f = th->getFuncTest();
		QMap<QString, FunctionTest::StateDevice> iRes = f->getResultFunctionTest();

		QString str;
		for each (QString key in iRes.keys())
		{
			FunctionTest::StateDevice state = iRes[key];
			Res.insert(key, state);
		}
	}

	return Res;
}

void ThControlFuncTest::TransitMessage(QString mes){
    emit SendMesToMain(mes);
}

