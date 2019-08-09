#include "PreTestResult.h"

PreTestResult::PreTestResult(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PreTestResult::PreTestResult(QMap<QString, FunctionTest::StateDevice> res) : QWidget(nullptr), result(res)
{
	ui.setupUi(this);
	user_action = 0;
	this->setWindowTitle("Test Result");
	ui.tableWidget->setRowCount(result.keys().length());
	ui.tableWidget->setColumnCount(Count_of_MKM);//Expected 8 MKM

	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "MKM1" << "MKM2" << "MKM3" << "MKM4" << "MKM5" << "MKM6" << "MKM7" << "MKM8");
	QColor* green = new QColor(133, 187, 101);
	QColor* red = new QColor(255, 100, 0);
	int i = 0;
	QMapIterator<QString, FunctionTest::StateDevice>iter(result);
	while (iter.hasNext()) {
		iter.next();
		sel.insert(iter.key(), 0);
		FunctionTest::StateDevice state = iter.value();
		for (int j = 0; j < Count_of_MKM; j++) {
			QTableWidgetItem* itm = new QTableWidgetItem();
			itm->data(Qt::CheckStateRole);
			if (state.results[j] == 0) {
				itm->setCheckState(Qt::Checked);
				itm->setText("0x00");
				itm->setBackground(*green);
			}
			else {
				itm->setCheckState(Qt::Unchecked);
				itm->setText(QString("0x%1").arg(state.results[j],2,16));
				itm->setBackground(*red);
			}
			ui.tableWidget->setItem(i, j, itm);
		}
		i++;
	}
	ui.tableWidget->setVerticalHeaderLabels(sel.keys());
}

PreTestResult::~PreTestResult()
{
}

void PreTestResult::on_bAccept_clicked()
{
	sel.clear();
	QMap<QString, uint8_t>::const_iterator iter;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
		QTableWidgetItem* ItmDevice = ui.tableWidget->takeVerticalHeaderItem(i);
		QString nameDev = ItmDevice->text();
		uint8_t sel_mcm = 0;
		for (int j = 0; j < ui.tableWidget->columnCount(); j++) {
			QTableWidgetItem* itm = ui.tableWidget->item(i, j);
			Qt::CheckState st = itm->checkState();
			if (st) {
				sel_mcm |= (1 << j);
			}
		}
		sel.insert(nameDev, sel_mcm);
	}

	this->close();
	user_action = 1;
}

void PreTestResult::on_bCancel_clicked()
{
	this->close();
	user_action = 2;
}

QMap<QString, uint8_t> PreTestResult::getSelectedMCM() {
	return sel;
}
