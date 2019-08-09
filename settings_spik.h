#ifndef SETTINGS_SPIK_H
#define SETTINGS_SPIK_H

#include <QWidget>
#include <QFileDialog>
#include <QAction>

namespace Ui {
class Settings_SPiK;
}

class Settings_SPiK : public QWidget
{
    Q_OBJECT

public:
    explicit Settings_SPiK(QWidget *parent = 0);
    ~Settings_SPiK();
    QString getDirConfigPath(void);

	QString getDirResultPath(void);

private slots:

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

    void on_toolButton_clicked();

	void on_toolButton_2_clicked();

private:
    Ui::Settings_SPiK *ui;
    QString dirConfig;
	QString dirResult;

signals:
    void UpdateEvent(void);

};

#endif // SETTINGS_SPIK_H
