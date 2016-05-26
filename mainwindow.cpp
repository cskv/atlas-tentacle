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
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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
    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    tm = new QATLAS;
    ad = new AtlasDialog(this);

    ui->actionQuit->setEnabled(true);

// make all connections //initActionsConnections in Terminal example
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));

// make other connections (see Terminal example)
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(serial, SIGNAL(bytesWritten(qint64)), this, SLOT(start2()));
//! [2]
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

//! [4]
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
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}
//! [5]

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    //QByteArray tentacledata = serial->readAll();
    //tm->parseAtlas(tentacledata);

    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        //tm->parseAtlas(serialdata.trimmed());
    }

    //QByteArray serialdata = serial->readLine(10);
    //if (serialdata.contains("\r")) {
        //heiend->parseHEI(serialdata);
    //}
}
void MainWindow::readRawAtlasI2CData()
{
    //QByteArray tentacledata = serial->readAll();
    //tm->parseAtlas(tentacledata);

    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        QByteArray reply = serialdata.trimmed();
        if (reply[0] == 0x01) {
            ui->statusBar->showMessage("Success");
            tm->parseAtlasI2C(reply);
        }
        else if (reply[0] == 0x02) ui->statusBar->showMessage("Request Failed");
        else if (reply[0] == 0xFE) ui->statusBar->showMessage("Data Pending");
        else if (reply[0] == 0xFF) ui->statusBar->showMessage("No Data");
    }
}

//! [7]

//! [8]

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::on_action_Help_Kern_triggered()
{
    ad->show();
}

void MainWindow::on_btnGetTemp_clicked()
{
    lastCmd = tm->readTemp();
    serial->write(lastCmd);
}


void MainWindow::on_btnpH_clicked()
{
    lastCmd = tm->readpH();
    serial->write(lastCmd);
}

void MainWindow::on_btnLED_clicked()
{
    lastCmd = tm->readLED();
    serial->write(lastCmd);
}

void MainWindow::on_btnSetLED_toggled(bool checked)
{
    ui->btnSetLED->setText(checked ? "LED OFF" : "LED ON");

    if (checked)
    {
        lastCmd = tm->writeLED(false);
        serial->write(lastCmd);
    }
    else
    {
        lastCmd = tm->writeLED(true);
        serial->write(lastCmd);
    }
}

void MainWindow::on_btnSetTemp_clicked()
{
    lastCmd = tm->writeTemp();
    serial->write(lastCmd);
}

void MainWindow::on_btnCal_clicked()
{
    lastCmd = tm->readCal();
    serial->write(lastCmd);
}

void MainWindow::on_btnCalClear_clicked()
{
    lastCmd = tm->doCal(0);
    serial->write(lastCmd);
}

void MainWindow::on_btnCalMid_clicked()
{
    lastCmd = tm->doCal(1);
    serial->write(lastCmd);
}

void MainWindow::on_btnCalLow_clicked()
{
    lastCmd = tm->doCal(2);
    serial->write(lastCmd);
}

void MainWindow::on_btnCalHigh_clicked()
{
    lastCmd = tm->doCal(3);
    serial->write(lastCmd);
}
