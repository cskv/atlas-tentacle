#include "qatlasusb.h"
#include <QtDebug>

QATLASUSB::QATLASUSB(){
}

QATLASUSB::~QATLASUSB(){
}

/*
Command     Function
L           Enable / Disable or Query the LEDs (pg.23)
C           Continuous mode (pg.24)
R           Returns a single reading (pg.25)
T           Set or Query the temperature compensation (pg.48)
Cal         Performs calibration (pg.49)
Slope       Queries slope (p.55)
NAME        Device name (p.33)
I           Device information (pg.33)
RESPONSE    Response code (p.34)
Status      Retrieve status information (pg.57)
I2C         I2C address change (pg.58)
Sleep       Enter low power sleep mode (pg.59)
Serial      Switch back to UART mode (pg.60)



*/

// ATLAS commands
//---------------------------------------------------

QByteArray QATLASUSB::readLED()
//Atlas function: L?
//Response: 1?L,x with x is 0 (led off) or 1 (led on)
{
    QByteArray cmd;
    cmd = "L,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeLED(bool state)
//Atlas function: L,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "L,1\r" : cmd = "L,0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------

QByteArray QATLASUSB::readCont()
//Atlas function: C?
//Response: 1?C,x with x is 0 (cont off) or 1 (cont on)
{
    QByteArray cmd;
    cmd = "C,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeCont(bool state)
//Atlas function: C,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "C,1\r" : cmd = "C,0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------------
QByteArray QATLASUSB::readpH()
//Atlas function: R
//Response: 1,x.xxx with x.xxx is the pH value e.g. 7.012
{
    QByteArray cmd;
    cmd = "R\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------------
QByteArray QATLASUSB::readTemp()
//Atlas function: T?
//Response: 1?T,xx.x with xx.x is the temperature e.g. 19.5
{
    QByteArray cmd;
    cmd = "T,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeTemp()
//Atlas function: T,xx.x
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "T,20.0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//----------------------------------------------
QByteArray QATLASUSB::readCal()
//Atlas function: CAL?
//Response: ?CAL,x with x is 0, 1, 2, 3
{
    QByteArray cmd;
    cmd = "CAL,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::doCal(int taskid)
//Atlas function: Cal,taskid (clear, mid, low, high)
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "CAL,";
    switch (taskid) {
            case 0 : cmd += "clear\r";
            case 1 : cmd += "mid,7.00\r";
            case 2 : cmd += "low,4.00\r";
            case 3 : cmd += "high,10.00\r";
    }
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------
QByteArray QATLASUSB::readSlope()
//Atlas function: SLOPE,?
//Response: 1?SLOPE,xx.x,yyy.y
//with xx.x is acid slope e.g 99.7, yyy.y is basic slope e.g. 100.3
{
    QByteArray cmd;
    cmd = "SLOPE,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------

QByteArray QATLASUSB::readName()
//Atlas function: NAME
//Response: 1?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd;
    cmd = "NAME,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeName()
//Atlas function: NAME,xxx
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "NAME,Dev_1\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

//--------------------------------------------------

QByteArray QATLASUSB::readInfo()
//Atlas function: I
//Response: 1?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd;
    cmd = "99:I\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------

QByteArray QATLASUSB::readResponse()
//Atlas function: RESPONSE
//Response: 1?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd;
    cmd = "RESPONSE,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeResponse(bool state)
//Atlas function: RESPONSE,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "RESPONSE,1\r" : cmd = "RESPONSE,0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------
QByteArray QATLASUSB::readStatus()
//Atlas function: STATUS
//Response: ?STATUS,x,y.yyy
//with x is PSBWU, y.yyy supply voltage Vcc
{
    QByteArray cmd;
    cmd = "STATUS\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//-------------------------------------------------

QByteArray QATLASUSB::sleep()
//Atlas function: SLEEP
//Response: none
{
    QByteArray cmd;
    cmd = "SLEEP\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

//---------------------------------------
QByteArray QATLASUSB::serial(int baudrate) // switch to UART mode
//Atlas function: SERIAL, baudrate
//Response: none
{
    QByteArray cmd;
    cmd = "SERIAL,baudrate\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------
QByteArray QATLASUSB::factoryReset()
//Atlas function: Factory
//Response: issue STATUS query after this command and see if "S" is in the repaly
{
    QByteArray cmd;
    cmd = "Factory\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------

QByteArray QATLASUSB::changeI2C(char addr)
//Atlas function: I2C,char
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "99:I2C,addr\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

//----------------------------------------------------------------
void QATLASUSB::parseAtlasUSB(QByteArray atlasdata)
{
    QByteArray t;
    qDebug() << atlasdata;

    if ( atlasdata.contains("?L,") ) {
        t = atlasdata.mid(4,1);
        //ledState = t.toInt;
    }
    if ( atlasdata.contains("?T,") ) {
        t = atlasdata.mid(4,4);
        currentTemp = t.toDouble();
    }
    if ( atlasdata.contains("?CAL,") ) {
        t = atlasdata.mid(6,1);
        calState = t.toInt();
    }
    if ( atlasdata.contains("?SLOPE,") ) {
        t = atlasdata.mid(8,4);
        acidSlope = t.toDouble();
        t = atlasdata.mid(13,5);
        basicSlope = t.toDouble();
    }
    if ( atlasdata.contains("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        rstCode = t;
        t = atlasdata.mid(10,5);
        voltage = t.toDouble();
    }
}

