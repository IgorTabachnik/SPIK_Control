#include "thcontrolfunctest.h"

ThControlFuncTest::ThControlFuncTest(ExcelConfig* e)
{
    this->excel = e;
    list_th = new QList<ThPollDevices*>();
    work = true;
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
        emit SendMesToMain("GUI Error: No Devices for test, please find Devices or check connections with COM port\n");
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
    }

    Sleep(500);

    //Disconnect
    for(int l=0; l<list_con->size(); l++){
       list_con->at(l)->freeConnect();
    }

    delete list_con;
}

void ThControlFuncTest::TransitMessage(QString mes){
    emit SendMesToMain(mes);
}

