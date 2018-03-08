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

#ifndef QATLASUSB_H
#define QATLASUSB_H

#include <QtCore>

class QATLASUSB
{
public:
// Constructors and Destructors
    QATLASUSB();
    ~QATLASUSB();

    /** @brief struct containing all parameters and measurement values of EZO stamp.
     *
     */


    struct AtlasUSBProperties
    {
        bool    ledState = true;      /**< LED on EZO stamp enabled (true)/disabled (false) */
        double  currentpH = 7.0;      /**< pH measurement */
        double  currentORP = -999.9;  /**< ORP measurement */
        double  currentTemp = -273.0; /**< Temperature */
        int     calState = -1;        /**< Calibration state: 0,1,2,3 (uncal, mid, low, high) */
        double  acidSlope = 0.0;      /**< Calibration slope pH < 7 */
        double  basicSlope = 0.0;     /**< Calibration slope pH > 7  */
        QString probeType = "";       /**< pH, ORP, EC or D.O. */
        QString version = "";         /**< firmware version */
        QString rstCode = "";         /**< Reset code */
        double  voltage = 0;          /**< supply voltage EZO stamp */
        qint8   i2cAddress = -1;      /**< 7-bits I2C address (1..127)  */
    } ;

public slots:
// Getters and Setters
    AtlasUSBProperties getUsbProps() const;

    QByteArray readLED();
    QByteArray writeLED(bool state);

    QByteArray readTemp();
    QByteArray writeTemp(double temperature);

    QByteArray readpH();
    QByteArray readCont();
    QByteArray writeCont(bool state);

    QByteArray readCal();
    QByteArray doCal(int taskid);

    QByteArray readSlope();
    QByteArray readInfo();
    QByteArray readStatus();

    QByteArray sleep();
    QByteArray changeSerial(int baudrate); // change baudrate in UART mode
    QByteArray factoryReset();

    QByteArray readName();
    QByteArray writeName();
    QByteArray readResponse();
    QByteArray writeResponse(bool state);

    //void parseAtlas(QByteArray atlasdata);
    //void parseTentacleMini(QByteArray atlasdata);
    //void parseAtlasI2C(QByteArray atlasdata);
    void parseAtlasUSB(QByteArray atlasdata);

private:
    AtlasUSBProperties usbProps;
    //char   i2caddress;
    //QString   probe;
    //QString  version;
    //QString rstCode;
    //double voltage;

    //double currentpH;
    //double currentTemp;

    //bool   ledState;
    //bool   contState;
    //bool   respState;
    //int    calState;

    //double acidSlope;
    //double basicSlope;

    QByteArray lastAtlasUSBCmd;

};

#endif // QATLASUSB_H
