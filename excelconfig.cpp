#include "excelconfig.h"

QStringList ExcelConfig::ext = {"*.xlsm", "*.xlsx", "*.xls"};

ExcelConfig::ExcelConfig(QString path, QTableWidget *tableWidget)
{
    this->pathFile = path;
    this->tableWidget = tableWidget;
    this->excel=NULL;
    this->sheet=NULL;
    this->sheets=NULL;
    this->workbook=NULL;
    this->workbooks=NULL;
    openshortTable = new QList<float>();
    fpga_options = new QList<uint16_t>();
    func_test = new QList<FUNC_CMD>();
    list_cmd = new QHash<QString, uint16_t>();
    list_sel_mcm = new QList<SEL_MCM>();

    this->extract_func_test_table = false;
    this->extract_openshort_test_table = false;
}

ExcelConfig::ExcelConfig(QString path)
{
    this->pathFile = path;
    this->tableWidget = nullptr;
    this->excel=NULL;
    this->sheet=NULL;
    this->sheets=NULL;
    this->workbook=NULL;
    this->workbooks=NULL;
    openshortTable = new QList<float>();
    fpga_options = new QList<uint16_t>();
    func_test = new QList<FUNC_CMD>();
    list_cmd = new QHash<QString, uint16_t>();
    list_sel_mcm = new QList<SEL_MCM>();

    this->extract_func_test_table = false;
    this->extract_openshort_test_table = false;
}

ExcelConfig::~ExcelConfig()
{
    delete openshortTable;
    delete fpga_options;
    delete func_test;
    delete list_cmd;
    delete list_sel_mcm;
    ClearAndClose();
}

QString ExcelConfig::OpenFile()
{
    QFile test(pathFile);
    if(test.exists()){
        if(excel!=NULL){
            ClearAndClose();
        }

        if(pathFile.isEmpty()){
           emit SendMessageError("Error: "+QString("Path the ConfigFile is empty "));
           return "Error: path to config is empty";
        }

        this->excel = new QAxObject("Excel.Application",0);
        if(excel==NULL){
            emit SendMessageError("Error: Could not open file Excel is NULL");
            return "Error: Excel is NULL";
        }
        this->workbooks = excel->querySubObject("Workbooks");
        if(workbooks==NULL){
            emit SendMessageError("Error: Could not open file workbooks is NULL");
            return "Error: Could not open file workbooks is NULL";
        }
        this->workbook = workbooks->querySubObject("Open(const QString&)", pathFile);//Error segm fault
        if(workbook==NULL){
            emit SendMessageError("Error: Could not open file workbook is NULL");
            return "Error:Could not open file workbook is NULL";
        }
    }else{
        return "Config file is ->"+pathFile+" dont exist";
    }


    return "0";
}

void ExcelConfig::ClearAndClose(void)
{
    if(sheet!=NULL){
        this->sheet->clear();
        delete sheet;
        this->sheet = NULL;
        delete sheets;
        this->sheets=NULL;
        this->workbook->clear();
        delete workbook;
        this->workbook = NULL;
        this->workbooks->dynamicCall("Close()");
        this->workbooks->clear();
        delete workbooks;
        this->workbooks =NULL;
        this->excel->dynamicCall("Quit()");
        delete excel;
        this->excel=NULL;
    }
}

QString ExcelConfig::ExtractOpenShortTestTable()
{
    if(!extract_openshort_test_table){
        openshortTable->clear();

        QString res = InitExcelConfig(SHEETOPENSHORT);
        if(res!="0"){
            return res;
        }

        if(tableWidget!=nullptr){
            this->tableWidget->setRowCount(NRows);//Segment fault
            this->tableWidget->setColumnCount(NCols);
        }

        for ( int row = BEGROWOPENSHORT; row < NRows; row++ ){
            for ( int column = BEGCOLOPENSHORT; column < NCols; column++ ){
                QAxObject* cell = sheet->querySubObject("Cells(int,int)", row + 1, column + 1);
                QVariant value = cell->property("Value");
                bool ok=false;
                float L = value.toFloat(&ok);
                if(ok){
                    openshortTable->append(L);

                }else{
                    emit SendMessageError("Error;"+QString("Could not conver \" %1 \" to float").arg(value.toString()));
                    return "Error: "+QString("Could not conver \" %1 \" to float").arg(value.toString());
                }
                if(tableWidget!=nullptr){
                    QTableWidgetItem* item = new QTableWidgetItem(value.toString());//Построение графической таблицы
                    this->tableWidget->setItem(row, column, item);
                }
            }
        }
    }
    extract_openshort_test_table=true;
    return "0";
}

QString ExcelConfig::ExtractFuncTestTable()
{
    if(!extract_func_test_table){
        fpga_options->clear();
        func_test->clear();
        list_sel_mcm->clear();

        InitCmdListFPGA();
        InitExcelConfig(SHEETFUNCTEST);

        if(tableWidget!=nullptr){
            this->tableWidget->setRowCount(NRows);
            this->tableWidget->setColumnCount(NCols);
        }


        for ( int row = BEGROWFUNCTES; row < NRows; row++ ){

            FUNC_CMD cmd;
            QAxObject* cell1 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFUNCTES+1);
            QAxObject* cell2 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFUNCTES+2);
            QAxObject* cell3 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFUNCTES+3);
            QAxObject* cell4 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFUNCTES+4);
            QAxObject* cell5 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFUNCTES+5);
            QVariant value1 = cell1->property("Value");
            QVariant value2 = cell2->property("Value");
            QVariant value3 = cell3->property("Value");
            QVariant value4 = cell4->property("Value");
            QVariant value5 = cell5->property("Value");

            QAxObject* cell6 = sheet->querySubObject("Cells(int,int)", row, BEGCOLFPGAOPT);
            QVariant value6 = cell6->property("Value");

            QAxObject* cell7 = sheet->querySubObject("Cells(int,int)", row, BEGCOLSELMCM);
            QAxObject* cell8 = sheet->querySubObject("Cells(int,int)", row, BEGCOLSELMCM+1);
            QAxObject* cell9 = sheet->querySubObject("Cells(int,int)", row, BEGCOLSELMCM+2);
            QVariant value7 = cell7->property("Value");
            QVariant value8 = cell8->property("Value");
            QVariant value9 = cell9->property("Value");

            if(value1.isValid()){

                cmd.name = value1.toString();
                cmd.cmd = list_cmd->value(cmd.name);
                cmd.Vcc = value2.toFloat();
                cmd.VccQ = value3.toFloat();
                cmd.Temp = value4.toFloat();
                if(value5.isValid()){
                    bool ok;
                    QString t1 = value5.toString();
                    uint16_t t2 = t1.toUInt(&ok,16);

                    if(ok)
                    cmd.pattern = t2;
                }else{
                    cmd.pattern=0;
                }
                func_test->append(cmd);
            }

            if(value6.isValid())
            {
                fpga_options->append(value6.toInt());
            }

            if(value7.isValid())
            {
                SEL_MCM sel_mcm;
                sel_mcm.OP = value7.toString();
                sel_mcm.BK = value8.toString();
                sel_mcm.sel_mcm = value9.toInt();
                list_sel_mcm->append(sel_mcm);
            }

            if(!value1.isValid() && !value6.isValid() && !value7.isValid())
            {
                break;
            }
            if(tableWidget!=nullptr){
                QTableWidgetItem* item = new QTableWidgetItem(value1.toString());//Построение графической таблицы
                this->tableWidget->setItem(row, BEGCOLFUNCTES, item);
            }
        }
    }

    if(func_test->length()==0){
        emit SendMessageError("Error: Table Func Test is empty");
        return "Error: Table Func Test is empty";
    }
    extract_func_test_table = true;

    return "0";
}

void ExcelConfig::InitCmdListFPGA(void)
{
    list_cmd->clear();
    InitExcelConfig(SHEETFPGACMD);

    for(int i=2;i<=NRows;i++)
    {
        QAxObject* cell_1 = sheet->querySubObject("Cells(int,int)", i, 1);
        QAxObject* cell_2 = sheet->querySubObject("Cells(int,int)", i, 2);
        QVariant v1 = cell_1->property("Value");
        QVariant v2 = cell_2->property("Value");

        if(v1.isValid()&&v2.isValid()){
            QString name = v1.toString();
            uint16_t dec = v2.toInt();
            list_cmd->insert(name,dec);
        }
    }
}

QString ExcelConfig::InitExcelConfig(int Table)
{
    if(workbook==NULL){
        return "Error: Workbook is NULL";
    }

    this->sheets = workbook->querySubObject("Worksheets");//SegMent faul if workbook is NULL
    if(sheets==NULL){
        return "Error: Sheets is NULL";
    }

    this->sheet = sheets->querySubObject("Item(int)", Table);
    this->NSheets = sheets->property("Count").toInt();

    this->usedRange = sheet->querySubObject("UsedRange");
    this->rows = usedRange->querySubObject("Rows");
    this->NRows = rows->property("Count").toInt();

    this->columns = usedRange->querySubObject("Columns");
    this->NCols = columns->property("Count").toInt();

    if(NRows<0){
        return "Error: Nrows < 0 in file "+pathFile;
    }

    if(NCols<0){
        return "Error: NCols < 0 in file "+pathFile;
    }

    return "0";
}

QList<float>* ExcelConfig::getOpenShortTable(void)
{
    return openshortTable;
}

QList<FUNC_CMD>* ExcelConfig::getFuncTest(void)
{
    return func_test;
}

QList<uint16_t>* ExcelConfig::getFPGAOptions(void)
{
    return  fpga_options;
}

QList<SEL_MCM>* ExcelConfig::getSELMCM(void)
{
    return list_sel_mcm;
}

bool ExcelConfig::getStateFuncTestTable(){
    return extract_func_test_table;
}



