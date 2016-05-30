#ifndef QATLAS_H
#define QATLAS_H

#include <QtCore>

class QATLAS
{
public:
// Constructors and Destructors
    QATLAS();
    ~QATLAS();

//Functions:
    QByteArray readLED();
    QByteArray writeLED(bool state);

    QByteArray readpH();
    QByteArray readTemp();
    QByteArray writeTemp();

    QByteArray readCal();
    QByteArray doCal(int taskid);

    QByteArray readSlope();
    QByteArray readInfo();
    QByteArray readStatus();

    QByteArray changeI2C(char addr);
    QByteArray sleep();
    QByteArray serial(int baudrate); // switch to UART mode
    QByteArray factoryReset();

    void parseAtlas(QByteArray atlasdata);
    void parseTentacleMini(QByteArray atlasdata);
    void parseAtlasI2C(QByteArray atlasdata);

    double getCurrentpH() const;
    double getCurrentTemp() const ;
    bool getLedState() const;
    int getCalState() const;
    double getAcidSlope() const;
    double getBasicSlope() const;
    QString getProbeType() const;
    QString getVersion() const;
    QString getRstCode() const;
    double getVoltage() const;

//signals:
    //ledChanged(bool ledState); class QATLAS moet hiervoor een QOBJECT zijn

    qint8 getI2cAddress() const;
    void setI2cAddress(const qint8 &value);

private:
    qint8   i2cAddress = -1;
    QString probeType = "";
    QString  version = "";
    QString rstCode = "";
    double voltage = 0;

    double currentpH = -7.0;
    double currentTemp = -273.0;
    bool   ledState = true;
    int    calState = -1;
    double acidSlope = 0.0;
    double basicSlope = 0.0;

    QByteArray lastAtlasCmd;
};

#endif // QATLAS_H
