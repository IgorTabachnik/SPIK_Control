#ifndef EXCELCONFIG_H
#define EXCELCONFIG_H

#include <QDebug>
#include <QAxObject>
#include <QtWidgets/QTableWidget>
#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QHash>
#include "fpga_defines.h"

/*********************************Tables*********************************************/
//OpenShort test table
#define SHEETOPENSHORT 1
#define BEGCOLOPENSHORT 1
#define BEGROWOPENSHORT 2

//Function test table
#define SHEETFUNCTEST 2
#define BEGCOLFUNCTES 1
#define BEGROWFUNCTES 2

#define BEGCOLFPGAOPT 10
#define BEGROWFPGAOPT 2

#define BEGCOLSELMCM 13
#define BEGROWSELMCM 2

//FPGA cmd list
#define SHEETFPGACMD 3
#define BEGCOLFPGACMD 1
#define BEGROWFPGACMD 2

class ExcelConfig : public QObject
{
    Q_OBJECT
public:
    explicit ExcelConfig(QString path, QTableWidget *tableWidget);
    ExcelConfig(QString path);

    ~ExcelConfig();
    static QStringList ext;

    QString OpenFile(void);
    void ClearAndClose(void);
    QString ExtractOpenShortTestTable();
    QString ExtractFuncTestTable();

    QList<float>* getOpenShortTable(void);
    QList<FUNC_CMD>* getFuncTest(void);
    QList<uint16_t>* getFPGAOptions(void);
    QList<SEL_MCM>* getSELMCM(void);
    bool getStateFuncTestTable();


private:

    QAxObject* excel;
    QAxObject* workbooks;
    QAxObject* workbook;
    QAxObject* sheets;
    QAxObject* sheet;
    int NSheets;
    QAxObject* usedRange;
    QAxObject* rows;
    int NRows;
    QAxObject* columns;
    int NCols;
    QTableWidget *tableWidget;

    QList<float>* openshortTable;
    QList<uint16_t>* fpga_options;
    QList<FUNC_CMD>* func_test;
    QHash<QString, uint16_t> *list_cmd;


    QList<SEL_MCM>* list_sel_mcm;
    void InitCmdListFPGA(void);
    QString InitExcelConfig(int Table);

    QString pathFile;
    bool extract_func_test_table;
    bool extract_openshort_test_table;

signals:
    void SendMessageError(QString msg);

};

extern ExcelConfig* config;

#endif // EXCELCONFIG_H
