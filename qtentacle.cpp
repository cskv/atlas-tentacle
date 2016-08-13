#include "qtentacle.h"
#include <QtDebug>

QTentacle::QTentacle(QObject *parent) : QObject(parent)
{

}
 QTentacle::~QTentacle()
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
  * \brief Prepend the EZO I2C address to a Tentacle command.
  *
  * Example:
  * \code prependI2CAddr(QByteArray &anyCmd) \endcode
  */
 void QTentacle::prependI2CAddr(QByteArray &anyCmd)         // pH, ORP
 {
     anyCmd.prepend(":");
     anyCmd.prepend( QByteArray::number(EZOProps.i2cAddress) );
 }
 //---------------------------------------------------
 /*!
  * \brief Get the current state of the LED on the Atlas Scientific stamp.
  *
  * Example:
  * \code readLED(); \endcode
  * Atlas function: L?
  * Response: 1?L,x with x is 0 (LED off) or 1 (LED on)
  */
 QByteArray QTentacle::readLED()         // pH, ORP
 {
     QByteArray cmd = "L,?\r";
     prependI2CAddr(cmd);
     lastEZOCmd = cmd;
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
QByteArray QTentacle::writeLED(bool state)        // pH, ORP
//Atlas function: L,state
{
    QByteArray cmd;
    state ? cmd = "L,1\r" : cmd = "L,0\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::readpHORP()
{
    QByteArray cmd = "R\r";         // Capital R to comply with manual ( changed in .ino)
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::readTemp()
{
    QByteArray cmd = "T,?\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::writeTemp(double temperature)
{
    QByteArray cmd = "T,";
    cmd.append(QByteArray::number(temperature, 'f', 2));
    cmd.append("\r");
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::readCal()
//Atlas function: Cal?
//Response: ?CAL,x with x is 0, 1, 2, 3
{
    QByteArray cmd = "Cal,?\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::dopHCal(int taskid)
//Atlas function: Cal,taskid (clear, mid, low, high)
{
    QByteArray cmd = "Cal,";
    switch (taskid) {
            case 0 : cmd += "clear\r"; break;
            case 1 : cmd += "mid,7.00\r"; break;
            case 2 : cmd += "low,4.00\r"; break;
            case 3 : cmd += "high,10.00\r"; break;
    }
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::doORPCal(double orpRef)
//Atlas function: Cal,taskid (clear, mid, low, high)
{
    QByteArray cmd = "Cal,";
    cmd.append(QByteArray::number(orpRef,'f',1));
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
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
QByteArray QTentacle::readSlope()
{
    QByteArray cmd = "SLOPE,?\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/**
 * @brief Query the type and firmware version of the EZO stamp.
 *
 * @return
 */
QByteArray QTentacle::readInfo()
//Atlas function: I
//Response: ?I,pH,x.x
//with x.x is firmware version number e.g 1.0
{
    QByteArray cmd = "I\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}

//--------------------------------------------------
/**
 * @brief Query the status of the ATLAS Scientific stamp.
 *
 * @retval QByteArray with command to send to Tentacle Mini
 */
QByteArray QTentacle::readStatus()
//Atlas function: STATUS
//Response: ?STATUS,x,y.yyy
//with x is PSBWU, y.yyy supply voltage Vcc
{
    QByteArray cmd = "STATUS\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}
//-------------------------------------------------
/**
 * @brief QTentacle::changeI2C
 * @param newAddr
 * @return
 */
QByteArray QTentacle::changeI2C(qint8 newAddr)
//Atlas function: I2C,char
{
    QByteArray cmd = "I2C,";
    cmd.append(QByteArray::number(newAddr));
    cmd.append("\r");
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/*!
 * \brief QTentacle::sleep
 * \return
 */
QByteArray QTentacle::sleep()
//Atlas function: SLEEP
//Response: none
{
    QByteArray cmd = "SLEEP\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}
//---------------------------------------
/**
 * @brief QTentacle::serial
 * @param baudrate
 * @return
*/
QByteArray QTentacle::serial(int baudrate) // switch to UART mode
//Atlas function: SERIAL, baudrate
//Response: none
{
    QByteArray cmd = "SERIAL,";
    //cmd.append(QByteArray::number(baudrate));
    cmd.append(QByteArray::number(9600));
    cmd.append("\r");
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}
//---------------------------------------
/**
 * @brief QTentacle::factoryReset
 * @return
 */
QByteArray QTentacle::factoryReset()
//Atlas function: Factory
//Response: issue STATUS query after this command and see if "S" is in the reply
{
    QByteArray cmd = "STATUS\r";
    prependI2CAddr(cmd);
    lastEZOCmd = cmd;
    return cmd;
}

//----------------------------------------------------------------
/**
 * @brief Parse the response of the EZO stamp connected via a Tentacle Mini interface board.
 *
 * @param atlasdata
 */
void QTentacle::parseTentacleMini(QByteArray atlasdata)
{
    QByteArray t;
    //qDebug() << atlasdata;
    if ( atlasdata.startsWith("?L,") ) {
        t = atlasdata.mid(3,1);
        EZOProps.ledState = (t.toInt() == 1);
        EZOProps.ledState = (t.toInt() != 0);
        emit ledRead(EZOProps.ledState);
    } else if ( atlasdata.startsWith("?T,") ) {
        t = atlasdata.mid(3,5);
        EZOProps.currentTemp = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?CAL,") ) {
        t = atlasdata.mid(5,1);
        EZOProps.calState = t.toInt();
        emit infoRead();
    } else if ( atlasdata.startsWith("?SLOPE,") ) {
        t = atlasdata.mid(7,4);
        EZOProps.acidSlope = t.toDouble();
        t = atlasdata.mid(12,6);
        EZOProps.basicSlope = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?I,") ) {
        t = atlasdata.mid(3,2);
        if (t.contains("pH")) {
            EZOProps.probeType = QString(t);     // EZO "pH" stamp
            t = atlasdata.mid(6,4);
            EZOProps.version = QString(t);
        } else {
            t = atlasdata.mid(3,3);    // EZO "ORP" stamp
            EZOProps.probeType = QString(t);
            t = atlasdata.mid(7,4);
            EZOProps.version = QString(t);
        }
        emit infoRead();
    } else if ( atlasdata.startsWith("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        EZOProps.rstCode = t;
        t = atlasdata.mid(10,5);
        EZOProps.voltage = t.toDouble();
        emit infoRead();
    } else {
        t = atlasdata.mid(0,7);     // pH: 6 bytes ORP: 7 bytes max
        EZOProps.currentpH = t.toDouble();
        if ( EZOProps.currentpH > 0 && EZOProps.currentpH < 14 ) emit measRead();
    }
}

// Getters and Setters
QTentacle::EZOProperties QTentacle::getEZOProps() const
{
    return EZOProps;
}

void QTentacle::setI2cAddress(const qint8 &value)
{
    EZOProps.i2cAddress = value;
}





