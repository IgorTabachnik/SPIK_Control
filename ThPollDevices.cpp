#include "ThPollDevices.h"


ThPollDevices::ThPollDevices(FunctionTest* functest):test(functest)
{
    work=true;
    cStop=false;
    percent = 0;
}

ThPollDevices::~ThPollDevices()
{

}

bool ThPollDevices::isRunning()
{
    return work;
}

void ThPollDevices::Stop()
{
    work=false;
}



void ThPollDevices::run()
{
    //Send Excel table to All Devices(BK)
    error = test->SendExcelConfigToAllDevices();//to all devices on comport
    /*If one of the devices failed to transfer the table, then the thread is terminated.*/
    if(error!="0"){
        work=false;
    }

    if(work){
        error = test->StartFunctionTest();
        if(error!="0"){
            work=false;
        }

        error = test->UpdateStateFuncTest();
        if(error!="0"){
            work=false;
        }

        while(1)
        {
            if(work){
                error = test->UpdateStateFuncTest();
                percent = test->GetPercentCompleted();//get exceptions
				Sleep(100);
            }else{
                if(cStop){
					error = test->StopFunctionsTest();
                    break;
                }
                Sleep(100);
            }
        }
    }

}

FunctionTest* ThPollDevices::getFuncTest(void){
    return test;
}

int ThPollDevices::getPercentCompleted(void)
{
    return percent;
}
