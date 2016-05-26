#ifndef QATLAS_H
#define QATLAS_H

#include <QtCore>

class QATLAS
{
public:
// Constructors and Destructors
    QATLAS();
    ~QATLAS();

// Getters and Setters
    double getpH() {
        return currentpH;
    }

    double getTemp() {
        return currentTemp;
    }

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

private:
    char   i2caddress;
    QString   probe;
    QString  version;
    QString rstCode;
    double voltage;

    double currentpH;
    double currentTemp;
    bool   ledState;
    int    calState;
    double acidSlope;
    double basicSlope;

    QByteArray lastAtlasCmd;
};

#endif // QATLAS_H
