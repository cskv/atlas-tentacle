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

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int n = 0; n < NUMSTAMPS; n++) {
        lblEZO[n] = new QLabel(ui->mainTab);
        //ui->mainTab->layout()->addWidget(lblEZO[n]);
        lblEZO[n]->setGeometry(20, 20+76*n, 130, 56);
        //lblEZO[n]->setFont(QFont::Bold);
        lblEZO[n]->setText("pH");
        lblEZO[n]->setStyleSheet("color: red;"
                                 "font: bold 36pt");
    }

    for (int n = 0; n < NUMSTAMPS; n++) {
        lblValue[n] = new QLabel(ui->mainTab);
        //ui->mainTab->layout()->addWidget(lblEZO[n]);
        lblValue[n]->setGeometry(150, 20+76*n, 360, 56);
        lblValue[n]->setText("7.00");
        lblValue[n]->setStyleSheet("background: black; "
                                   "color: orange;"
                                   "font: bold 36pt ""Arial"" ");
        lblValue[n]->setAlignment(Qt::AlignCenter);
    }

    for (int n = 0; n < NUMSTAMPS; n++) {
        EZOTab[n] = new QWidget();
        ui->tabWidget->insertTab(n+1, EZOTab[n], QString("EZO %1").arg(n));
    }

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

    for (int n = 0; n < NUMSTAMPS; n++) {
        ezof[n] = new EZOFrame(this->EZOTab[n]);
        ezof[n]->stamp->setI2cAddress(16+n);
    }

    setupEZOFrames();

    logf = new LoggingFrame(ui->logTab);
    //logf->setLogDir("C:/Data");
    //lf->setLogFile(qs.value("LogFile", "SolTraQ_").toString());

    sd->setModal(true);
    sd->show();
    //connect( sd, SIGNAL(accepted()),
             //this, SLOT(openSerialPort2()) );

}

MainWindow::~MainWindow()
{
    //delete sd;
    delete ui;
}
//*********************************************************************

void MainWindow::setupEZOFrames()
{
    connect( ezof[0], SIGNAL(cmdAvailable(QByteArray)),
             this, SLOT(writeData(QByteArray)) );
    connect( ezof[1], SIGNAL(cmdAvailable(QByteArray)),
             this, SLOT(writeData(QByteArray)) );

    connect( ezof[0]->stamp, SIGNAL(measRead()),
             this, SLOT(displayAllMeas()) );
    connect( ezof[1]->stamp, SIGNAL(measRead()),
             this, SLOT(displayAllMeas()) );
}

void MainWindow::on_mainTimer()
{
    ezof[0]->on_btnReadMeas_clicked();
    ezof[1]->on_btnReadMeas_clicked();
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
    QAtlas::EZOProperties pr[2];
    double dval[2] = {0.0, 0.0};
    QString pt[2] = {"pH", "pH"};

    for (int n = 0; n < 2; n++) {
        pr[n] = ezof[n]->stamp->getEZOProps();
        pt[n] = pr[n].probeType;
    }

    //for (int n = 0; n < 2; n++) {

    if ( !this->lblEZO[0]->text().startsWith(pt[0]) ) {
        this->lblEZO[0]->setText(pt[0]);
        if (pt[0] == "pH"){
            this->lblEZO[0]->setStyleSheet("QLabel {color : red;}");
            ui->tabWidget->setTabIcon(1, *(new QIcon(":/new/images/images/ph-circuit-large.jpg")));
        } else if (pt[0] == "ORP"){
            this->lblEZO[0]->setStyleSheet("QLabel {color : blue;}");
            ui->tabWidget->setTabIcon(1, *(new QIcon(":/new/images/images/orp-circuit-large.jpg")));
        } else if (pt[0] == "EC"){
            this->lblEZO[0]->setStyleSheet("QLabel {color : green;}");
            ui->tabWidget->setTabIcon(1, *(new QIcon(":/new/images/images/ec-circuit-large.jpg")));
        }
    }
/*
    if (pt == "pH") {
        dval = pr.currentpH;
        if (dval > 0 && dval < 14) ui->lblValue1->setText(QString::number(dval[0], 'f', 2 ));
    } else if (pt == "ORP") {
        dval = pr.currentORP;
        if (dval > -1021 && dval < 1021) ui->lblValue1->setText(QString::number(dval[0], 'f', 1 ) + " mV");
    }
*/
    dval[0] = pr[0].currentpH;
    if (dval[0] > 0 && dval[0] < 14) {
        this->lblValue[0]->setText(QString::number(dval[0], 'f', 2 ));
    }
    dval[1] = pr[1].currentpH;
    if (dval[1] > 0 && dval[1] < 14) {
        this->lblValue[1]->setText(QString::number(dval[1], 'f', 2 ));
    }
    pf->realtimeTentacleSlot(dval[0], dval[1]);

    if (isLogging) {
        //QString line;

        QDateTime dt = QDateTime::currentDateTime();
        int32_t unixTime = dt.toSecsSinceEpoch();
        QString dateStr = dt.toString("yyyy-MM-dd");
        QString timeStr = dt.toString("hh:mm:ss");

        //line.sprintf("%10i, %s, %s, %5.2lf, %5.2lf",
        //             unixTime, dateStr, timeStr, dval[0], dval[1]);



        QString line = QString("%1, %2, %3, %4, %5")
                .arg(unixTime)
                .arg(dateStr)
                .arg(timeStr)
                .arg(dval[0])
                .arg(dval[1]);

        logf->write(line);
        if (!commentLine.isEmpty()) {
            logf-> write(commentLine);
            commentLine.clear();
        }
    }
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
                        if (address == ezof[0]->stamp->getI2cAddress())
                          ezof[0]->stamp->parseTentacleMini(reply);
                        if (address == ezof[1]->stamp->getI2cAddress())
                          ezof[1]->stamp->parseTentacleMini(reply);
                    }
                }
            }
            //qDebug() << address << colonpos << reply << "(" << tentacledata << ")";
            qDebug() << "Read: " << tentacledata;
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

void MainWindow::on_btnLogStart_clicked()
{
    QDateTime datetime(QDateTime::currentDateTime());
    QString dtString = datetime.toString("yyyyMMdd_hhmmss");
    logf->setLogFile("C:/Data/Atlas_" + dtString + ".log");

    logf->on_btnStart_clicked();
    isLogging = true;

    ui->btnLogStart->setEnabled(false);
    ui->btnLogStop->setEnabled(true);
}

void MainWindow::on_btnLogStop_clicked()
{
    isLogging = false;
    logf->on_btnStop_clicked();

    ui->btnLogStart->setEnabled(true);
    ui->btnLogStop->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{
    commentLine = ui->leComment->text();
    commentLine.prepend("# ");
}

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



