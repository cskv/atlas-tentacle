/** @file qatlas.h
 * @brief Member and fuction declarations for control of EZO stamps.
 *
 * This header contains the prototypes
 * for control of Atlas Scientific EZO stamps
 * measuring pH, ORP, EC and DO
 *
 * @author Paul JM van Kan (cskv)
 */
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

#ifndef QATLAS_H
#define QATLAS_H

#include <QObject>

class QAtlas :public QObject
{
    Q_OBJECT

public:
// Constructors and Destructors
    explicit QAtlas(QObject *parent = 0);
    ~QAtlas();

/** @brief struct containing all parameters and measurement values of EZO stamp.
 *
 */
struct EZOProperties {
    bool    ledState = true;      /**< LED on EZO stamp enabled (true)/disabled (false) */
    double  currentpH = 7.0;      /**< pH measurement */
    double  currentORP = -999.9;  /**< ORP measurement */
    double  currentEC = -1.0;     /**< EC measurement */
    double  currentTemp = -273.0; /**< Temperature */
    int     calState = -1;        /**< Calibration state: 0,1,2,3 (uncal, mid, low, high) */
    double  acidSlope = 0.0;      /**< Calibration slope pH < 7 */
    double  basicSlope = 0.0;     /**< Calibration slope pH > 7  */
    QString name = "Stamp";       /**< string to give device a name */
    QString probeType = "";       /**< pH, ORP, EC or D.O. */
    QString version = "";         /**< firmware version */
    QString rstCode = "";         /**< Reset code */
    double  voltage = 0;          /**< supply voltage EZO stamp */
    qint8   i2cAddress = -1;      /**< 7-bits I2C address (1..127)  */
    int     baud = 9600;          /**< baudrate of virtual serial port to EZO stamp */
    bool    isConnectedAsSerial = true;          /**< serial or I2C */
    };

// getters
    EZOProperties getEZOProps() const;
    qint8         getI2cAddress() const;

// setters
    void setEZOProps(const EZOProperties &value);
    void setI2cAddress(const qint8 &value);

public slots:
    // Atlas Scientific commands
    QByteArray readLED();
    QByteArray writeLED(bool state);

    QByteArray readpHORP();

    QByteArray readTemp();
    QByteArray writeTemp(double temperature);

    QByteArray readCal();
    QByteArray dopHCal(int taskid);
    QByteArray doORPCal(double orpRef);

    QByteArray readSlope();
    QByteArray readInfo();
    QByteArray readStatus();

    QByteArray changeI2C(qint8 newAddr);
    QByteArray sleep();
    QByteArray changeSerial(int baudrate); // switch to UART mode
    QByteArray factoryReset();

// Parsing of Atlas Scientific stamp response bytes
    //void parseAtlas(QByteArray atlasdata);
    void parseTentacleMini(QByteArray atlasdata);
    void parseAtlasI2C(QByteArray atlasdata);

signals:
    void ledRead(bool state);
    void infoRead();          //class QATLAS moet hiervoor een QOBJECT zijn
    void measRead();

private:
    EZOProperties props;
    QByteArray lastEZOCmd;

    void prependI2CAddr(QByteArray &anyCmd);
};

#endif // QATLAS_H
