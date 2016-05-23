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
    QByteArray readpH();
    QByteArray readTemp();
    QByteArray readLED();
    QByteArray readCal();
    QByteArray writeLED(bool state);
    QByteArray writeTemp();
    QByteArray doCal(int taskid);

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
