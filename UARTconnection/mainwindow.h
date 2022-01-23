#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchButton_clicked();

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void readFromPort();

    void on_turnOnLedButton_clicked();

    void on_TurnOffLedButton_clicked();

    void on_ClearButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *device;
    void addToLogs(QString message);
    void sendMessageToDevice(QString message);
};
#endif // MAINWINDOW_H
