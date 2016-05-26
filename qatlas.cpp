#include "qatlas.h"
#include <QtDebug>

QATLAS::QATLAS(){
}

QATLAS::~QATLAS(){
}

/*
Command     Function
L           Enable / Disable or Query the LEDs (pg.46)
R           Returns a single reading (pg.47)
T           Set or Query the temperature compensation (pg.48)
Cal         Performs calibration (pg.49)
Slope       Queries slope (p.55)
I           Device information (pg.56)
Status      Retrieve status information (pg.57)
I2C         I2C address change (pg.58)
Sleep       Enter low power sleep mode (pg.59)
Serial      Switch back to UART mode (pg.60)



*/

// ATLAS commands
//---------------------------------------------------
QByteArray QATLAS::readLED()
//Atlas function: L?
//Response: 1?L,x with x is 0 (led off) or 1 (led on)
{
    QByteArray cmd;
    cmd = "99:L,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::writeLED(bool state)
//Atlas function: L,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "99:L,1\r" : cmd = "99:L,0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------------
QByteArray QATLAS::readpH()
//Atlas function: R
//Response: 1,x.xxx with x.xxx is the pH value e.g. 7.012
{
    QByteArray cmd;
    cmd = "99:r\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------------
QByteArray QATLAS::readTemp()
//Atlas function: T?
//Response: 1?T,xx.x with xx.x is the temperature e.g. 19.5
{
    QByteArray cmd;
    cmd = "99:t,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::writeTemp()
//Atlas function: T,xx.x
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "99:T,20.0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//----------------------------------------------
QByteArray QATLAS::readCal()
//Atlas function: Cal?
//Response: ?CAL,x with x is 0, 1, 2, 3
{
    QByteArray cmd;
    cmd = "99:CAL,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::doCal(int taskid)
//Atlas function: Cal,taskid (clear, mid, low, high)
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "99:CAL,";
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
QByteArray QATLAS::readSlope()
//Atlas function: SLOPE,?
//Response: 1?SLOPE,xx.x,yyy.y
//with xx.x is acid slope e.g 99.7, yyy.y is basic slope e.g. 100.3
{
    QByteArray cmd;
    cmd = "99:SLOPE,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------
QByteArray QATLAS::readInfo()
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
//--------------------------------------------------
QByteArray QATLAS::readStatus()
//Atlas function: STATUS
//Response: ?STATUS,x,y.yyy
//with x is PSBWU, y.yyy supply voltage Vcc
{
    QByteArray cmd;
    cmd = "99:STATUS\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//-------------------------------------------------
QByteArray QATLAS::changeI2C(char addr)
//Atlas function: I2C,char
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "99:I2C,addr\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------
QByteArray QATLAS::sleep()
//Atlas function: SLEEP
//Response: none
{
    QByteArray cmd;
    cmd = "99:SLEEP\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------
QByteArray QATLAS::serial(int baudrate) // switch to UART mode
//Atlas function: SERIAL, baudrate
//Response: none
{
    QByteArray cmd;
    cmd = "99:SERIAL\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------
QByteArray QATLAS::factoryReset()
//Atlas function: Factory
//Response: issue STATUS query after this command and see if "S" is in the repaly
{
    QByteArray cmd;
    cmd = "99:STATUS\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}
//----------------------------------------------------------------
void QATLAS::parseAtlas(QByteArray atlasdata)
{
    //bool ok;
    QByteArray t;
    qDebug() << atlasdata;
    if ( atlasdata[0] == '1' ) {
        if ( atlasdata.contains("?T,") ) {
            t = atlasdata.mid(4,4);
            currentTemp = t.toDouble();
        } else if ( atlasdata.contains("?L,") ) {
                t = atlasdata.mid(4,1);
                ledState = t.toInt();
        } else if ( atlasdata.contains("?CAL,") ) {
            t = atlasdata.mid(6,1);
            calState = t.toInt();
        }
    }
}

void QATLAS::parseTentacleMini(QByteArray atlasdata)
{
    //bool ok;
    QByteArray t;
    qDebug() << atlasdata;
    if ( atlasdata[0] == '1' ) {
        if ( atlasdata.contains("?T,") ) {
            t = atlasdata.mid(4,4);
            currentTemp = t.toDouble();
        } else if ( atlasdata.contains("?L,") ) {
                t = atlasdata.mid(4,1);
                ledState = t.toInt();
        } else if ( atlasdata.contains("?CAL,") ) {
            t = atlasdata.mid(6,1);
            calState = t.toInt();
        }
    }
}
