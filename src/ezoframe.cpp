/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected to an Arduino(TM)
**  with Tentacle (TM) interface shield.
**  Copyright (C) 2016-2018 Paul JM van Kan
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
//disable EZO functions not implemented for I2C
    ui->btnGetName->setEnabled(false);
    ui->btnSetName->setEnabled(false);
    ui->btnSerial->setEnabled(false);
    ui->btnFactReset->setEnabled(false);
    ui->respCB->setEnabled(false);
    ui->contCB->setEnabled(false);

    stampTimer = new QTimer;
    connect(stampTimer, SIGNAL(timeout()),
            this, SLOT(on_btnReadMeas_clicked()));

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

void EZOFrame::displayInfo()
{
    QAtlas::EZOProperties pr = stamp->getEZOProps();

    double dval = pr.acidSlope;
    if (dval > 0) ui->acidSlopeLabel->setText(QString::number(dval));
    dval = pr.basicSlope;
    if (dval > 0) ui->basicSlopeLabel->setText(QString::number(dval));

    //ui->leName->setText(pr.name);
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

void EZOFrame::on_btnSleep_clicked()
{
    lastCmd = stamp->sleep();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

/**
 * @brief EZOFrame::on_cbAuto_clicked
 * @param checked
 *
 * start stampTimer
 * timeout SLOT sends "R\n" command to stamp
 * with regular intervals ( >= 1000 ms due to conversion time
 * this SLOT works in both SERIAL and I2C mode
 * note: I2C protocol is synchronous
 */
void EZOFrame::on_cbAuto_clicked(bool checked)
{
    if (checked) stampTimer->start(1000);
    else stampTimer->stop();
}

/**
 * @brief EZOFrame::on_btnI2CAddr_clicked
 *
 * in I2C mode
 * this SLOT changes the I2C address of the stamp
 * if working with a Tentacle (Mini) shield for Arduino
 * - this application with its serial port object must be closed
 * - the I2C address must be MANUALLY changed in the Arduino sketch
 * - the sketch must be compiled and uploaded to the Arduino
 * - this application must be restarted
 *
 * if working with a Tentacle T3 shield for RPi
 *                 or EZO Carrier board (non USB)
 * - this SLOT can be useful for changing the I2C address programmatically
 */
void EZOFrame::on_btnI2CAddr_clicked()
{
    lastCmd = stamp->changeI2C(ui->leI2CAddress->text().toInt());
    emit cmdAvailable(lastCmd);
}

//-------------------------------------------------------------------
// I2C only commands
//-------------------------------------------------------------------
/**
 * @brief EZOFrame::on_btnSerial_clicked
 *
 * changes baudrate for stamp in SERIAL mode
 * changes control from I2C to SERIAL mode in I2C mode
 */
void EZOFrame::on_btnSerial_clicked()
{
    lastCmd = stamp->changeSerial(ui->leBaud->text().toInt());
    emit cmdAvailable(lastCmd);
}
