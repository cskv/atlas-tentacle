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

#include "ezoframe.h"
#include "ui_ezoframe.h"

EZOFrame::EZOFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EZOFrame)
{
    ui->setupUi(this);

    ui->stateLed->setOnColor(Qt::blue);
    ui->stateLed->setOffColor(Qt::gray);
    ui->stateLed->setState(true);

    stampTimer = new QTimer;

    connect(stampTimer, SIGNAL(timeout()), this, SLOT(on_btnReadMeas_clicked()));

    connect( stamp, SIGNAL(ledRead(bool)),
             this, SLOT(displayLedState()) );
    connect( stamp, SIGNAL(infoRead()),
             this, SLOT(displayInfo()) );
    connect( stamp, SIGNAL(measRead()),
             this, SLOT(displayMeas()) );
}

EZOFrame::~EZOFrame()
{
    delete ui;
}

void EZOFrame::updateInfo()
{
    ui->btnSlope->click();
    ui->btnInfo->click();
    ui->btnStatus->click();
    ui->btnGetTemp->click();
    ui->btnCal->click();
}

void EZOFrame::displayLedState()
{
    ui->stateLed->setState( stamp->getEZOProps().ledState );
}

void EZOFrame::displayInfo()
{
    QAtlas::EZOProperties pr = stamp->getEZOProps();

    double dval = pr.acidSlope;
    if (dval > 0) ui->acidSlopeLabel->setText(QString::number(dval));
    dval = pr.basicSlope;
    if (dval > 0) ui->basicSlopeLabel->setText(QString::number(dval));


    ui->probeLabel->setText(pr.probeType);
    ui->versionLabel->setText(pr.version);

    ui->rstLabel->setText(pr.rstCode);
    dval = pr.voltage;
    if (dval > 0) ui->voltLabel->setText(QString::number(dval));

    dval = pr.currentTemp;
    if (dval > 0) ui->leTemp->setText(QString::number(dval , 'f', 1 ));
    int ival = pr.calState;
    if (ival > -1) ui->calLabel->setText(QString::number(ival));

    ui->leI2CAddress->setText(QString::number(stamp->getI2cAddress()));
}

void EZOFrame::displayMeas()
{
    QAtlas::EZOProperties pr = stamp->getEZOProps();
    double dval = 0;
    QString pt = pr.probeType;

    if ( !ui->EZOLabel->text().startsWith(pt) ) {
        ui->EZOLabel->setText(pt);
        if (pt == "pH"){
            ui->EZOLabel->setStyleSheet("QLabel {color : red;}");
        } else if (pt == "ORP"){
            ui->EZOLabel->setStyleSheet("QLabel {color : blue;}");
        }
    }

    if (pt == "pH") {
        dval = pr.currentpH;
        if (dval > 0 && dval < 14) ui->valueLabel->setText(QString::number(dval, 'f', 2 ));
    } else if (pt == "ORP") {
        dval = pr.currentORP;
        if (dval > -1021 && dval < 1021) ui->valueLabel->setText(QString::number(dval, 'f', 1 ) + " mV");
    }
}
    //if (dval > -1 && dval < 15) ui->pHLabel->setText(QString::number(dval, 'f', 2 ));

void EZOFrame::on_btnGetTemp_clicked()
{
    lastCmd = stamp->readTemp();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnReadMeas_clicked()
{
    lastCmd = stamp->readpHORP();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnLED_clicked()
{
    lastCmd = stamp->readLED();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnSetTemp_clicked()
{
    lastCmd = stamp->writeTemp(ui->leTemp->text().toDouble());
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(400, this, SLOT(on_btnGetTemp_clicked()));
}

void EZOFrame::on_btnCal_clicked()
{
    lastCmd = stamp->readCal();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnCalClear_clicked()
{
    QAtlas::EZOProperties pr = stamp->getEZOProps();
    if (pr.probeType == "pH") lastCmd = stamp->dopHCal(0);
    else if (pr.probeType == "ORP") lastCmd = stamp->doORPCal(200.0);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(false);
    ui->btnCalLow->setEnabled(false);
}

void EZOFrame::on_btnCalMid_clicked()
{
    lastCmd = stamp->dopHCal(1);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(true);
    ui->btnCalLow->setEnabled(true);
}

void EZOFrame::on_btnCalLow_clicked()
{
    lastCmd = stamp->dopHCal(2);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnCalHigh_clicked()
{
    lastCmd = stamp->dopHCal(3);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnSlope_clicked()
{
    lastCmd = stamp->readSlope();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnInfo_clicked()
{
    lastCmd = stamp->readInfo();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnStatus_clicked()
{
    lastCmd = stamp->readStatus();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_ledCheckBox_clicked(bool checked)
{
    lastCmd = stamp->writeLED(checked);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(300, this, SLOT(on_btnLED_clicked()));
}

void EZOFrame::on_contCB_clicked(bool checked)
{
    if (checked) stampTimer->start(1000);
    else stampTimer->stop();
}

void EZOFrame::on_btnSleep_clicked()
{
    lastCmd = stamp->sleep();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_cbAuto_clicked(bool checked)
{
    if (checked) stampTimer->start(1000);
    else stampTimer->stop();
}

void EZOFrame::on_btnI2CAddr_clicked()
{
    lastCmd = stamp->changeI2C(ui->leI2CAddress->text().toInt());
    emit cmdAvailable(lastCmd);
}
