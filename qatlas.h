#ifndef QKERN_H
#define QKERN_H

#include <QtCore>

class QKERN
{
public:
// Constructors and Destructors
    QKERN();
    ~QKERN();

// Getters and Setters
    double getWeight() {
        return kernWeight;
    }

//Functions:
    QByteArray getW();
    QByteArray getS();
    QByteArray getT();

    void parseKern(QByteArray kerndata);

    private:
        int kernWeight;
        QByteArray kernUnit;
        bool kernStable;
        bool kernError;

        QByteArray lastKernCmd;
};

#endif // QKERN_H
