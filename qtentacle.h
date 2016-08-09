/** @file qatlas.h
 * @brief Member and fuction declarations for control of EZO stamps.
 *
 * This header contains the prototypes
 * for control of Atlas Scientific EZO stamps
 * measuring pH, ORP, EC and DO
 *
 * @author Paul JM van Kan (cskv)
 */
#ifndef QTENTACLE_H
#define QTENTACLE_H

//#include <QtCore>
#include <QObject>

class QTentacle : public QObject
{
    Q_OBJECT
public:
// Constructors and Destructors
    explicit QTentacle(QObject *parent = 0);
    ~QTentacle();

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
    } EZOProperties;

// getters
    EZOProperties getEZOProps() const;

// setters
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
    QByteArray serial(int baudrate); // switch to UART mode
    QByteArray factoryReset();

// Parsing of Atlas Scientific stamp response bytes
    void parseTentacleMini(QByteArray atlasdata);

signals:
    void ledRead(bool state); //class QATLAS moet hiervoor een QOBJECT zijn
    void infoRead();
    void measRead();

private slots:
    void prependI2CAddr(QByteArray &anyCmd);

private:
    EZOProperties EZOProps;

    QByteArray lastEZOCmd;
};

#endif // QTENTACLE_H