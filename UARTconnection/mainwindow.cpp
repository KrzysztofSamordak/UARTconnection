#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
}


void MainWindow::on_searchButton_clicked()
{
    addToLogs("Szukam urzadzen:");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++)
    {
        addToLogs(devices.at(i).portName() + devices.at(i).description() + " " + devices.at(i).manufacturer()
                  +" SN: " + devices.at(i).serialNumber());
        if ((ui->comboBoxDevices->findText(devices.at(i).portName()
                  + " " + devices.at(i).description())) == -1)  //check if comboBoxDevices do not contains
                                                                //such a records (prevent duplicating)
        {
            ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
        }
    }
}

void MainWindow::addToLogs(QString message)
{
    QString dateAndTime =  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(dateAndTime + "\t" + message);
}

void MainWindow::on_connectButton_clicked()
{
    if(ui->comboBoxDevices->count() == 0)
    {
        this->addToLogs("Nie wykryto żadnych urządzeń!");
        return;
    }

    QString portName = ui->comboBoxDevices->currentText().split("\t").first();
    this->device->setPortName(portName);

    // Open and configure port
    if(!device->isOpen())
    {
        if(device->open(QSerialPort::ReadWrite))
        {
            this->device->setBaudRate(QSerialPort::Baud9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);

            this->addToLogs("Otwarto port szeregowy.");

            // CONNECT:
            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
        } else
        {
            this->addToLogs("Otwarcie porty szeregowego się nie powiodło!");
        }
    }else
    {
        this->addToLogs("Port już jest otwarty!");
        return;
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    if(this->device->isOpen()) {
    this->device->close();
    this->addToLogs("Zamknięto połączenie.");
    } else
    {
        this->addToLogs("Port nie jest otwarty!");
        return;
    }
}

void MainWindow::readFromPort()
{
    while(this->device->canReadLine())
    {
        QString line = this->device->readLine();
        //qDebug() << line;

        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);
        //qDebug() << line.left(pos);

        this->addToLogs(line.left(pos));
    }
}

void MainWindow::sendMessageToDevice(QString message)
{
    if(this->device->isOpen() && this->device->isWritable()) {
    this->addToLogs("Wysyłam informacje do urządzenia: " + message);
    this->device->write(message.toStdString().c_str());
    } else
    {
        this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty!");
    }

}

void MainWindow::on_turnOnLedButton_clicked()
{
    this->sendMessageToDevice("1");
}


void MainWindow::on_TurnOffLedButton_clicked()
{
    this->sendMessageToDevice("0");
}


void MainWindow::on_ClearButton_clicked()
{
    ui->textEditLogs->clear();
}

