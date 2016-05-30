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
}

EZOFrame::~EZOFrame()
{
    delete ui;
}

void EZOFrame::updateAll()
{
    //ui->btnLED->click();
    //ui->btnSlope->click();
    //ui->btnInfo->click();
    //ui->btnStatus->click();

    if ( ui->cbAuto->isChecked() ) {
        ui->btnpH->click();
    }
    //ui->btnGetTemp->click();
    //ui->btnCal->click();
    displayAll();
}

void EZOFrame::displayAll()
{
    ui->stateLed->setState( tm1->getLedState() );

    double dval = tm1->getAcidSlope();
    if (dval > 0) ui->acidSlopeLabel->setText(QString::number(dval));
    dval = tm1->getBasicSlope();
    if (dval > 0) ui->basicSlopeLabel->setText(QString::number(dval));

    ui->probeLabel->setText(tm1->getProbeType());
    ui->versionLabel->setText(tm1->getVersion());

    ui->rstLabel->setText(tm1->getRstCode());
    dval = tm1->getVoltage();
    if (dval > 0) ui->voltLabel->setText(QString::number(dval));

    if ( ui->cbAuto->isChecked() ) {
        dval = tm1->getCurrentpH();
        if (dval > -1 && dval < 15) ui->pHLabel->setText(QString::number(dval, 'f', 2 ));
    }

    dval = tm1->getCurrentTemp();
    if (dval > 0) ui->tempLabel->setText(QString::number(dval , 'f', 1 ));
    int ival = tm1->getCalState();
    if (ival > -1) ui->calLabel->setText(QString::number(ival));
}

void EZOFrame::on_btnGetTemp_clicked()
{
    lastCmd = tm1->readTemp();
    //serial->write(lastCmd);
}

void EZOFrame::on_btnpH_clicked()
{
    lastCmd = tm1->readpH();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnLED_clicked()
{
    lastCmd = tm1->readLED();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnSetTemp_clicked()
{
    lastCmd = tm1->writeTemp();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(300, this, SLOT(on_btnGetTemp_clicked()));
}

void EZOFrame::on_btnCal_clicked()
{
    lastCmd = tm1->readCal();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnCalClear_clicked()
{
    lastCmd = tm1->doCal(0);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(false);
    ui->btnCalLow->setEnabled(false);
}

void EZOFrame::on_btnCalMid_clicked()
{
    lastCmd = tm1->doCal(1);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(true);
    ui->btnCalLow->setEnabled(true);
}

void EZOFrame::on_btnCalLow_clicked()
{
    lastCmd = tm1->doCal(2);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnCalHigh_clicked()
{
    lastCmd = tm1->doCal(3);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnSlope_clicked()
{
    lastCmd = tm1->readSlope();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnInfo_clicked()
{
    lastCmd = tm1->readInfo();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnStatus_clicked()
{
    lastCmd = tm1->readStatus();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_ledCheckBox_clicked(bool checked)
{
    lastCmd = tm1->writeLED(checked);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(300, this, SLOT(on_btnLED_clicked()));
}

void EZOFrame::on_contCB_clicked(bool checked)
{
    if (checked) mainTimer->start(1000);
    else mainTimer->stop();
}

void EZOFrame::on_btnSleep_clicked()
{
    lastCmd = tm1->sleep();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}
