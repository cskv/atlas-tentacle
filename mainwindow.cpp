/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.htm1l and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.htm1l.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ad = new AtlasDialog(this);
    //aboutAtlas = new About(this);

    pH1Frame = new EZOFrame(ui->pH1Tab);
    pH2Frame = new EZOFrame(ui->pH2Tab);
    pH1Frame->tm->setI2cAddress(99);
    pH2Frame->tm->setI2cAddress(98);

    pf = new PlotFrame(ui->centralWidget);
    pf->move(560,20);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    sd = new SerialDialog(this);


    mainTimer = new QTimer(this);
    //mainTimer->start(1000);

    //delayTimer->setSingleShot(true);

    ui->actionQuit->setEnabled(true);

// make all connections //initActionsConnections in Terminal example
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));

// make other connections (see Terminal example)
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readTentacleI2CData()));

    connect(mainTimer, SIGNAL(timeout()),
            this, SLOT(on_mainTimer()));

    setupEZOFrames();

    //settings->setModal(true);
    settings->show();
    connect( settings, SIGNAL(accepted()),
             this, SLOT(openSerialPort()) );
    //sd->setModal(true);
    sd->show();
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
    delete settings;
    delete ui;
}

void MainWindow::on_mainTimer()
{
    pH1Frame->on_btnReadMeas_clicked();
    pH2Frame->on_btnReadMeas_clicked();
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
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
