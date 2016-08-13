#include "qatlas.h"
#include <QtDebug>

QATLAS::QATLAS()
{
}

QATLAS::~QATLAS()
{
}

/*
EZO pH
Command     Function
L           Enable / Disable or Query the LEDs (pg.46)
R           Returns a single reading (pg.47)
T           Set or Query the temperature compensation (pg.48)  diff
Cal         Performs calibration (pg.49)
SLOPE       Queries slope (p.55)
I           Device information (pg.56)
STATUS      Retrieve status information (pg.57)
I2C         I2C address change (pg.58)
SLEEP       Enter low power sleep mode (pg.59)
SERIAL      Switch back to UART mode (pg.60)
Factory     Factory reset (p.47)

EZO ORP
L           Enable / Disable or Query the LEDs (pg.39)
R           Returns a single reading (pg.40)
Cal         Performs calibration (pg.41)
I           Device information (pg.42)
Status      Retrieve status information (pg.43)
I2C         I2C address change (pg.44)
Sleep       Enter low power sleep mode (pg.45)
Serial      Switch back to UART mode (pg.46)
Factory     Factory reset (p.47)
PLOCK       Protocol lock (p.49)
*/

// ATLAS commands
//---------------------------------------------------
/*!
 * \brief Get the current state of the LED on the Atlas Scientific stamp.
 *
 * Example:
 * \code readLED(); \endcode
 * Atlas function: L?
 * Response: 1?L,x with x is 0 (LED off) or 1 (LED on)
 */
QByteArray QATLAS::readLED()         // pH, ORP
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":L,?\r");
    lastAtlasCmd = cmd;
    return cmd;
}
/*!
 * \brief Set the state of the LED on the Atlas Scientific stamp.
 *
 * \param state
 * \retval cmd
 *
 * Example:
 * @code writeLED(bool state); @endcode
 * Atlas function: L,state
 * Response: 1
 */
QByteArray QATLAS::writeLED(bool state)        // pH, ORP
//Atlas function: L,state
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    state ? cmd.append(":L,1\r") : cmd.append(":L,0\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------------
/**
 * @brief Get the current pH, ORP, EC or D.O. value from the Atlas Scientific stamp
 *
 * @code readpHORP(); @endcode
 * Example:
 * Atlas function: R
 * Response: xx.xxx with the pH or ORP value
 */
QByteArray QATLAS::readpHORP()
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":R\r");         // Capital R to comply with manual ( changed in .ino)
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------------
/**
 * @brief Get the current temperature from the Atlas Scientific stamp
 *
 * Example:
 * @code readTemp(); @endcode
 * Atlas function: T?
 * Response: ?T,xx.xx with xx.xx is the temperature e.g. 25.00
 */
QByteArray QATLAS::readTemp()
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":T,?\r");
    lastAtlasCmd = cmd;
    return cmd;
}
/**
 * @brief Set the temperature on the Atlas Scientific stamp
 *
 * Example:
 * @code writeTemp(); @endcode
 * Atlas function: T,xx.xx?
 * Response: 1
 */
QByteArray QATLAS::writeTemp(double temperature)
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":T,");
    cmd.append(QByteArray::number(temperature, 'f', 2));
    cmd.append("\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//----------------------------------------------
/**
 * @brief Get the Calibration state from the Atlas Scientific stamp
 *
 * Example:
 * @code readCal(); @endcode
 * Atlas function: Cal?
 * Response: ?Cal,x with x is 0, 1, 2, 3
 */
QByteArray QATLAS::readCal()
//Atlas function: Cal?
//Response: ?CAL,x with x is 0, 1, 2, 3
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":Cal,?\r");
    lastAtlasCmd = cmd;
    return cmd;
}
/**
 * @brief Perform a pH calibration of the Atlas Scientific stamp
 *
 * Example:
 * @code dopHCal(int taskid); @endcode
 * Atlas function: Cal,0 (clear), 1 (pH7), 2 (pH4) or 3 (pH10)
 * Response: 1
 */
QByteArray QATLAS::dopHCal(int taskid)
//Atlas function: Cal,taskid (clear, mid, low, high)
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":Cal,");
    switch (taskid) {
            case 0 : cmd += "clear\r"; break;
            case 1 : cmd += "mid,7.00\r"; break;
            case 2 : cmd += "low,4.00\r"; break;
            case 3 : cmd += "high,10.00\r"; break;
    }
    lastAtlasCmd = cmd;
    return cmd;
}
/**
 * @brief Perform a ORP (mV) calibration of the Atlas Scientific stamp
 *
 * Example:
 * @code doORPCal(double orpRef); @endcode
 * Atlas function: Cal,xxx.x with xxx.x in mV
 * Response: 1
 */
QByteArray QATLAS::doORPCal(double orpRef)
//Atlas function: Cal,taskid (clear, mid, low, high)
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":Cal,");
    cmd.append(QByteArray::number(orpRef,'f',1));
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------------------
/**
 * @brief Get the pH calibration slopes after executing Cal,123 commands
 *
 * Example:
 * @code readSlope(); @endcode
 * Atlas function: SLOPE,?
 * Response: ?SLOPE,xx.x,yyy.y
 * with xx.x is acid slope e.g 99.7, yyy.y is basic slope e.g. 100.3
 */
QByteArray QATLAS::readSlope()
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":SLOPE,?\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/**
 * @brief Query the type and firmware version of the EZO stamp.
 *
 * @return
 */
QByteArray QATLAS::readInfo()
//Atlas function: I
//Response: ?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":I\r");
    lastAtlasCmd = cmd;
    return cmd;
}

//--------------------------------------------------
/**
 * @brief Query the status of the ATLAS Scientific stamp.
 *
 * @retval QByteArray with command to send to Tentacle Mini
 */
QByteArray QATLAS::readStatus()
//Atlas function: STATUS
//Response: ?STATUS,x,y.yyy
//with x is PSBWU, y.yyy supply voltage Vcc
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":STATUS\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//-------------------------------------------------
/**
 * @brief QATLAS::changeI2C
 * @param newAddr
 * @return
 */
QByteArray QATLAS::changeI2C(qint8 newAddr)
//Atlas function: I2C,char
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":I2C,");
    cmd.append(QByteArray::number(newAddr));
    cmd.append("\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/*!
 * \brief QATLAS::sleep
 * \return
 */
QByteArray QATLAS::sleep()
//Atlas function: SLEEP
//Response: none
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":SLEEP\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------
/**
 * @brief QATLAS::serial
 * @param baudrate
 * @return
*/
QByteArray QATLAS::serial(int baudrate) // switch to UART mode
//Atlas function: SERIAL, baudrate
//Response: none
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":SERIAL,");
    //cmd.append(QByteArray::number(baudrate));
    cmd.append(QByteArray::number(9600));
    cmd.append("\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//---------------------------------------
/**
 * @brief QATLAS::factoryReset
 * @return
 */
QByteArray QATLAS::factoryReset()
//Atlas function: Factory
//Response: issue STATUS query after this command and see if "S" is in the reply
{
    QByteArray cmd = QByteArray::number(i2cAddress);
    cmd.append(":STATUS\r");
    lastAtlasCmd = cmd;
    return cmd;
}
//----------------------------------------------------------------
/**
 * @brief Parse the response of the EZO stamp
 *
 * @param atlasdata
 * @retval void
 */
void QATLAS::parseAtlasI2C(QByteArray atlasdata)
{
    QByteArray t;
    //qDebug() << atlasdata;

    if ( atlasdata.contains("?L,") ) {
        t = atlasdata.mid(3,1);
        ledState = (t.toInt() == 1);
        ledState = (t.toInt() != 0);
        //emit ledChanged(ledState);
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
    if ( atlasdata.contains("?I,") ) {
        t = atlasdata.mid(3,3);
        t =t.trimmed();
        probeType = QString(t);
        t = atlasdata.mid(5,4);
        version = QString(t);
    }
    if ( atlasdata.contains("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        rstCode = t;
        t = atlasdata.mid(10,5);
        voltage = t.toDouble();
    }
}

/**
 * @brief Parse the response of the EZO stamp connected via a Tentacle Mini interface board.
 *
 * @param atlasdata
 */
void QATLAS::parseTentacleMini(QByteArray atlasdata)
{
    QByteArray t;
    //qDebug() << atlasdata;
    if ( atlasdata.startsWith("?L,") ) {
        t = atlasdata.mid(3,1);
        ledState = (t.toInt() == 1);
        ledState = (t.toInt() != 0);
        emit ledRead(ledState);
    } else if ( atlasdata.startsWith("?T,") ) {
        t = atlasdata.mid(3,5);
        currentTemp = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?CAL,") ) {
        t = atlasdata.mid(5,1);
        calState = t.toInt();
        emit infoRead();
    } else if ( atlasdata.startsWith("?SLOPE,") ) {
        t = atlasdata.mid(7,4);
        acidSlope = t.toDouble();
        t = atlasdata.mid(12,6);
        basicSlope = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?I,") ) {
        t = atlasdata.mid(3,2);
        if (t.contains("pH")) {
            probeType = QString(t);     // EZO "pH" stamp
            t = atlasdata.mid(6,4);
            version = QString(t);
        } else {
            t = atlasdata.mid(3,3);    // EZO "ORP" stamp
            probeType = QString(t);
            t = atlasdata.mid(7,4);
            version = QString(t);
        }
        emit infoRead();
    } else if ( atlasdata.startsWith("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        rstCode = t;
        t = atlasdata.mid(10,5);
        voltage = t.toDouble();
        emit infoRead();
    } else {
        t = atlasdata.mid(0,7);     // pH: 6 bytes ORP: 7 bytes max
        currentpH = t.toDouble();
        if ( currentpH > 0 && currentpH < 14 )emit measRead();
    }
}

// Getters and Setters
bool QATLAS::getLedState() const
{
    return ledState;
}

double QATLAS::getCurrentpH() const
{
    return currentpH;
}

double QATLAS::getCurrentORP() const
{
    return currentORP;
}

double QATLAS::getCurrentTemp() const
{
    return currentTemp;
}

int QATLAS::getCalState() const
{
    return calState;
}

double QATLAS::getAcidSlope() const
{
    return acidSlope;
}

double QATLAS::getBasicSlope() const
{
    return basicSlope;
}

QString QATLAS::getProbeType() const
{
    return probeType;
}

QString QATLAS::getVersion() const
{
    return version;
}

QString QATLAS::getRstCode() const
{
    return rstCode;
}

double QATLAS::getVoltage() const
{
    return voltage;
}

qint8 QATLAS::getI2cAddress() const
{
    return i2cAddress;
}

void QATLAS::setI2cAddress(const qint8 &value)
{
    i2cAddress = value;
}

QATLAS::AtlasProperties QATLAS::getProps() const
{
    return props;
}

void QATLAS::setProps(const AtlasProperties &value)
{
    props = value;
}

