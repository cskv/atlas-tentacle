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

#include "qatlasusb.h"
#include <QtDebug>

QATLASUSB::QATLASUSB()
{
}

QATLASUSB::~QATLASUSB()
{
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
/*!
 * \brief Get the state of the LED on the Atlas Scientific EZO stamp.
 *
 * \return cmd for EZO function L?
 * \code command = readLED(); \endcode
 * EZO response: 1?L,x with x is 0 (LED off) or 1 (LED on)
 */
QByteArray QATLASUSB::readLED()
//Atlas function: L?
//Response: 1?L,x with x is 0 (led off) or 1 (led on)
{
    QByteArray cmd;
    cmd = "L,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeLED(bool state)
//Atlas function: L,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "L,1\r" : cmd = "L,0\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------

QByteArray QATLASUSB::readCont()
//Atlas function: C?
//Response: 1?C,x with x is 0 (cont off) or 1 (cont on)
{
    QByteArray cmd;
    cmd = "C,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeCont(bool state)
//Atlas function: C,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "C,1\r" : cmd = "C,0\r";
    qDebug() << cmd;
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------------
QByteArray QATLASUSB::readTemp()
//Atlas function: T?
//Response: 1?T,xx.x with xx.x is the temperature e.g. 19.5
{
    QByteArray cmd;
    cmd = "T,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeTemp()
//Atlas function: T,xx.x
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "T,20.0\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//----------------------------------------------
QByteArray QATLASUSB::readCal()
//Atlas function: CAL?
//Response: ?CAL,x with x is 0, 1, 2, 3
{
    QByteArray cmd;
    cmd = "CAL,?\r";
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//----------------------------------------------------

QByteArray QATLASUSB::readName()
//Atlas function: NAME
//Response: 1?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd;
    cmd = "NAME,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeName()
//Atlas function: NAME,xxx
//Response: 1 (Success)
{
    QByteArray cmd;
    cmd = "NAME,Dev_1\r";
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
    return cmd;
}

QByteArray QATLASUSB::writeResponse(bool state)
//Atlas function: RESPONSE,state
//Response: 1 (Success)
{
    QByteArray cmd;
    state ? cmd = "RESPONSE,1\r" : cmd = "RESPONSE,0\r";
    lastAtlasUSBCmd = cmd;
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
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//-------------------------------------------------

QByteArray QATLASUSB::sleep()
//Atlas function: SLEEP
//Response: none
{
    QByteArray cmd;
    cmd = "SLEEP\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

//---------------------------------------
QByteArray QATLASUSB::changeSerial(int baudrate) // change baudrate in UART mode
//Atlas function: SERIAL, baudrate
//Response: none
// 1. 300 bps 2. 1200 bps 3. 2400 bps 4. 9600 bps 5. 19200 bps
// 6. 38400 bps 7. 57600 bps 8. 115200 bps
{
    QByteArray cmd;
    cmd = "SERIAL,";
    cmd.append(QByteArray::number(baudrate));
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------
QByteArray QATLASUSB::factoryReset()
//Atlas function: Factory
//Response: issue STATUS query after this command and see if "S" is in the repaly
{
    QByteArray cmd;
    cmd = "Factory\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//--------------------------------------------------

QByteArray QATLASUSB::changeI2C(quint8 newAddr)
//Atlas function: I2C,char
//Response: 1 (Success)
{
    QByteArray cmd = "I2C,";
    cmd.append(QByteArray::number(newAddr));
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}

//----------------------------------------------------------------
void QATLASUSB::parseAtlasUSB(QByteArray atlasdata)
{
    QByteArray t;
    qDebug() << atlasdata;

    if ( atlasdata.contains("?L,") ) {
        t = atlasdata.mid(4,1);
        //usbProps.ledState = t.toInt;
    }
    if ( atlasdata.contains("?T,") ) {
        t = atlasdata.mid(4,4);
        usbProps.currentTemp = t.toDouble();
    }
    if ( atlasdata.contains("?CAL,") ) {
        t = atlasdata.mid(6,1);
        usbProps.calState = t.toInt();
    }
    if ( atlasdata.contains("?SLOPE,") ) {
        t = atlasdata.mid(8,4);
        usbProps.acidSlope = t.toDouble();
        t = atlasdata.mid(13,5);
        usbProps.basicSlope = t.toDouble();
    }
    if ( atlasdata.contains("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        usbProps.rstCode = t;
        t = atlasdata.mid(10,5);
        usbProps.voltage = t.toDouble();
    }
}

QATLASUSB::AtlasUSBProperties QATLASUSB::getUsbProps() const
{
    return usbProps;
}

