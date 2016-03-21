#include "qatlas.h"
#include <QtDebug>

QATLAS::QATLAS(){
}

QATLAS::~QATLAS(){
}

// ATLAS commands
QByteArray QATLAS::readpH()
//Function: pH value for the weight
//is sent via the serial interface
{
    QByteArray cmd;
    cmd = "99:r\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::readTemp()
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    cmd = "99:t,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::readLED()
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    cmd = "99:L,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::readCal()
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    cmd = "99:CAL,?\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::writeLED(bool state)
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    state ? cmd = "99:L,1\r" : cmd = "99:L,0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::writeTemp()
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    cmd = "99:T,20.0\r";
    qDebug() << cmd;
    lastAtlasCmd = cmd;
    return cmd;
}

QByteArray QATLAS::doCal(int taskid)
//Function: No data are sent,
//the balance carries out the tare function.
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
