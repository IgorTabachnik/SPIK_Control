#include "help_func.h"

help_func::help_func()
{

}

float help_func::dw2float(uint16_t* ptr)
{
    float res=0;
    uint8_t* p = (uint8_t*)&res;

    p[0] = ptr[0]&0x00FF;
    p[1] = (ptr[0]>>8)&0x00FF;
    p[2] = ptr[1]&0x00FF;
    p[3] = (ptr[1]>>8)&0x00FF;

    return res;
}

void help_func::float2dw(float value, uint16_t* arr)
{
    uint16_t* p = (uint16_t*)&value;

    arr[0] = p[0];
    arr[1] = p[1];
}

QStringList help_func::findFilesInDirectory(QString pathToDirectory, QStringList ext)
{
    /*!
        Filters files in a directory based on file extension
    */

    // Storage for filtered files
    QStringList temporaryList = {""};

    // Path to directory
    QDir folder(pathToDirectory);

    // Filter files in folder using predefined extensions
    folder.setNameFilters(ext);
    temporaryList = folder.entryList();

    return temporaryList;
}

void help_func::updateComboBox(QComboBox *configComboBox, QStringList listValues)
{
    /*!
        Updates elements from filtered list. Informs about list of elements
    */

    // Remove any elements in the box
    configComboBox->clear();

    // Show all values from predefined list
    for (qint8 i = 0; i < listValues.size(); i++)
        configComboBox->addItem(listValues[i]);
}

void FlushBuffer(uint16_t *Arr, int size){

    for(int i=0;i<size;i++){
        Arr[i]=0;
    }
}
