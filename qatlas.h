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

private:
    char   i2caddress;
    double currentpH;
    double currentTemp;
    bool   ledState;
    int    calState;

    QByteArray lastAtlasCmd;
};

#endif // QATLAS_H
