/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected to an Arduino(TM)
**  with Tentacle (TM) interface shield.
**  Copyright (C) 2016 Paul JM van Kan
**
**  AtlasTerminal is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  AtlasTerminal is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.

**  You should have received a copy of the GNU General Public License
**  along with AtlasTerminal.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************
**           Author: Paul JM van Kan                                     **
**  Website/Contact:                                                     **
**             Date:                                                     **
**          Version:                                                     **
***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ad = new AtlasDialog(this);
    //aboutAtlas = new About(this);

    pf = new PlotFrame(ui->centralWidget);
    pf->move(560,20);

    serial = new QSerialPort(this);
    sd = new SerialDialog(this);

    mainTimer = new QTimer(this);
    //mainTimer->start(1000);
    //delayTimer->setSingleShot(true);

    ui->actionQuit->setEnabled(true);

// make all connections //initActionsConnections in Terminal example
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort2()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), sd, SLOT(show()));

// make other connections (see Terminal example)
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readTentacleI2CData()));

    connect(mainTimer, SIGNAL(timeout()),
            this, SLOT(on_mainTimer()));

    pH1Frame = new EZOFrame(ui->pH1Tab);
    pH2Frame = new EZOFrame(ui->pH2Tab);
    pH1Frame->tm->setI2cAddress(99);
    pH2Frame->tm->setI2cAddress(98);

    setupEZOFrames();

    sd->setModal(true);
    sd->show();
    //connect( sd, SIGNAL(accepted()),
             //this, SLOT(openSerialPort2()) );

}

void MainWindow::setupEZOFrames()
{
    connect( pH1Frame, SIGNAL(cmdAvailable(QByteArray)),
             this, SLOT(writeData(QByteArray)) );
    connect( pH2Frame, SIGNAL(cmdAvailable(QByteArray)),
             this, SLOT(writeData(QByteArray)) );

    connect( pH1Frame->tm, SIGNAL(measRead()),
             this, SLOT(displayAllMeas()) );
    connect( pH2Frame->tm, SIGNAL(measRead()),
             this, SLOT(displayAllMeas()) );
}

MainWindow::~MainWindow()
{
    //delete sd;
    delete ui;
}

void MainWindow::on_mainTimer()
{
    pH1Frame->on_btnReadMeas_clicked();
    pH2Frame->on_btnReadMeas_clicked();
}

void MainWindow::openSerialPort2()
{
    qDebug() << "entry";
    SerialDialog::PortParameters p = sd->getCp();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}


void MainWindow::writeData(const QByteArray &data)
{
    //qDebug() << data;
    serial->write(data);
}

void MainWindow::displayAllMeas()
{
    double dval[2] = {7.0, 7.0};
    dval[0] = pH1Frame->tm->getCurrentpH();
    if (dval[0] > -1 && dval[0] < 15) {
        ui->pH1Label->setText(QString::number(dval[0], 'f', 2 ));
    }
    dval[1] = pH2Frame->tm->getCurrentpH();
    if (dval[1] > -1 && dval[1] < 15) {
        ui->pH2Label->setText(QString::number(dval[1], 'f', 2 ));
    }
    pf->realtimeTentacleSlot(dval[0], dval[1]);
}

void MainWindow::readData()
{
    //QByteArray tentacledata = serial->readAll();
    //tm1->parseAtlas(tentacledata);

    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        //tm1->parseAtlas(serialdata.trimmed());
    }
}

void MainWindow::readTentacleI2CData()
{
    int address = 0;
    //QByteArray adr;
    int colonpos = 0;
    while(serial->canReadLine()) {
        QByteArray tentacledata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        QByteArray reply = tentacledata.trimmed();
        if ( !reply.isEmpty() ) {
            if (reply[0] == '<') ui->statusBar->showMessage(QString(reply),1500);
            else {
                colonpos = reply.indexOf(':');
                if (colonpos > 0 ) {
                    address = reply.left(colonpos).toInt();
                    //adr = strtok(reply.data(), ":");
                    reply = reply.mid(colonpos+1, -1);
                    if ( !reply.isEmpty() ) {
                        if (address == pH1Frame->tm->getI2cAddress())
                          pH1Frame->tm->parseTentacleMini(reply);
                        if (address == pH2Frame->tm->getI2cAddress())
                          pH2Frame->tm->parseTentacleMini(reply);
                    }
                }
            }
            //qDebug() << address << colonpos << reply << "(" << tentacledata << ")";
            qDebug() << tentacledata;
        }   // if not empty
    }       // while canReadLine
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::on_action_Help_Tentacle_triggered()
{
    ad->show();
}

void MainWindow::on_contCB_clicked(bool checked)
{
    if (checked) mainTimer->start(2000);
    else mainTimer->stop();
}



/*
void MainWindow::readRawI2CData()
{
    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        QByteArray reply = serialdata.trimmed();
        if (reply[0] == (char)0x01) {
            ui->statusBar->showMessage("Success");
            tm1->parseAtlasI2C(reply);
        }
        else if (reply[0] == (char)0x02) ui->statusBar->showMessage("Request Failed");
        else if (reply.at(0) == (char)0xFE) ui->statusBar->showMessage("Data Pending");
        else if (reply.at(0) == (char)0xFF) ui->statusBar->showMessage("No Data");
    }
}
*/

/*
void MainWindow::readAtlasUSBData()
{
    //QByteArray tentacledata = serial->readAll();
    //tm1->parseAtlas(tentacledata);

    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        QByteArray response = serialdata.trimmed();
        if ( response.contains("OK") ) ui->statusBar->showMessage("Success");
        else if ( response.contains("*ER") ) ui->statusBar->showMessage("Request Unknown");
        else if ( response.contains("*OV") ) ui->statusBar->showMessage("Over Voltage");
        else if ( response.contains("*UV") ) ui->statusBar->showMessage("Under Voltage");
        else if ( response.contains("*RS") ) ui->statusBar->showMessage("Device Reset");
        else if ( response.contains("*RE") ) ui->statusBar->showMessage("Boot up Completed");
        else if ( response.contains("*SL") ) ui->statusBar->showMessage("Device Asleep");
        else if ( response.contains("*WA") ) ui->statusBar->showMessage("Device Woken Up");
        else tm1->parseAtlasI2C(response);
    }
}
*/

void MainWindow::on_actionScreenshot_triggered()
{
    ui->centralWidget->grab().save("image.png");
}

void MainWindow::on_actionAbout_AtlasTerminal_triggered()
{
    About aboutAtlas;
    aboutAtlas.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}
