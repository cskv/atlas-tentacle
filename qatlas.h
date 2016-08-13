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

#include <QtCore>

class QATLAS :public QObject
{
    Q_OBJECT

public:
// Constructors and Destructors
    QATLAS();
    ~QATLAS();

/** @brief struct containing all parameters and measurement values of EZO stamp.
 *
 */
typedef struct {
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
    } AtlasProperties;

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
    QByteArray serial(int baudrate); // switch to UART mode
    QByteArray factoryReset();

// Parsing of Atlas Scientific stamp response bytes
    void parseAtlas(QByteArray atlasdata);
    void parseTentacleMini(QByteArray atlasdata);
    void parseAtlasI2C(QByteArray atlasdata);

// getters
    bool    getLedState() const;
    double  getCurrentpH() const;
    double  getCurrentORP() const;
    double  getCurrentTemp() const;
    int     getCalState() const;
    double  getAcidSlope() const;
    double  getBasicSlope() const;
    QString getProbeType() const;
    QString getVersion() const;  
    QString getRstCode() const;
    double  getVoltage() const;
    qint8   getI2cAddress() const;

    AtlasProperties getProps() const;

// setters
    void setI2cAddress(const qint8 &value);
    void setProps(const AtlasProperties &value);

signals:
    void ledRead(bool state); //class QATLAS moet hiervoor een QOBJECT zijn
    void infoRead();
    void measRead();

private:

    AtlasProperties props;
    bool    ledState = true;
    double  currentpH = 7.0;
    double  currentORP = -999.9;
    double  currentTemp = -273.0;
    int     calState = -1;
    double  acidSlope = 0.0;
    double  basicSlope = 0.0;
    QString probeType = "";
    QString version = "";
    QString rstCode = "";
    double  voltage = 0;
    qint8   i2cAddress = -1;

    QByteArray lastAtlasCmd;
};

#endif // QATLAS_H
