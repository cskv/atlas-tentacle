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

    connect( tm, SIGNAL(ledChanged(bool)),
             this, SLOT(displayLedState()) );
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
    displayInfo();
}

void EZOFrame::updateMeas()
{
    ui->btnReadMeas->click();
    displayMeas();
}

void EZOFrame::displayLedState()
{
    ui->stateLed->setState( tm->getLedState() );
}

void EZOFrame::displayInfo()
{
    double dval = tm->getAcidSlope();
    if (dval > 0) ui->acidSlopeLabel->setText(QString::number(dval));
    dval = tm->getBasicSlope();
    if (dval > 0) ui->basicSlopeLabel->setText(QString::number(dval));

    ui->probeLabel->setText(tm->getProbeType());
    ui->versionLabel->setText(tm->getVersion());

    ui->rstLabel->setText(tm->getRstCode());
    dval = tm->getVoltage();
    if (dval > 0) ui->voltLabel->setText(QString::number(dval));

    dval = tm->getCurrentTemp();
    if (dval > 0) ui->tempLabel->setText(QString::number(dval , 'f', 1 ));
    int ival = tm->getCalState();
    if (ival > -1) ui->calLabel->setText(QString::number(ival));
}

void EZOFrame::displayMeas()
{
    double dval = tm->getCurrentpH();
    if (dval > -1 && dval < 15) ui->pHLabel->setText(QString::number(dval, 'f', 2 ));
}

void EZOFrame::on_btnGetTemp_clicked()
{
    lastCmd = tm->readTemp();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnReadMeas_clicked()
{
    lastCmd = tm->readpHORP();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnLED_clicked()
{
    lastCmd = tm->readLED();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnSetTemp_clicked()
{
    lastCmd = tm->writeTemp(ui->leTemp->text().toDouble());
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(300, this, SLOT(on_btnGetTemp_clicked()));
}

void EZOFrame::on_btnCal_clicked()
{
    lastCmd = tm->readCal();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnCalClear_clicked()
{
    if (tm->getProbeType() == "pH") lastCmd = tm->dopHCal(0);
    else if (tm->getProbeType() == "ORP") lastCmd = tm->doORPCal(200.0);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(false);
    ui->btnCalLow->setEnabled(false);
}

void EZOFrame::on_btnCalMid_clicked()
{
    lastCmd = tm->dopHCal(1);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(true);
    ui->btnCalLow->setEnabled(true);
}

void EZOFrame::on_btnCalLow_clicked()
{
    lastCmd = tm->dopHCal(2);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnCalHigh_clicked()
{
    lastCmd = tm->dopHCal(3);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnSlope_clicked()
{
    lastCmd = tm->readSlope();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnInfo_clicked()
{
    lastCmd = tm->readInfo();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnStatus_clicked()
{
    lastCmd = tm->readStatus();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_ledCheckBox_clicked(bool checked)
{
    lastCmd = tm->writeLED(checked);
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
    lastCmd = tm->sleep();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}
