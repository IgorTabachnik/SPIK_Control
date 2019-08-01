#ifndef HELP_FUNC_H
#define HELP_FUNC_H

#include <QObject>
#include <QtCore>
#include <QComboBox>

class help_func
{
public:
    help_func();
    float dw2float(uint16_t* ptr);
    void float2dw(float value, uint16_t* arr);
    static QStringList findFilesInDirectory(QString pathToDirectory, QStringList ext);
    static void updateComboBox(QComboBox *configComboBox, QStringList listValues);
};

#endif // HELP_FUNC_H
