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

typedef struct {
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
