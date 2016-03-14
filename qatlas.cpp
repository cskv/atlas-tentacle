#include "qatlas.h"
#include <QtDebug>

QKERN::QKERN()
{

}

QKERN::~QKERN()
{

}

// SET commands
QByteArray QKERN::getW()
//Function: Weighing value for the weight (stable or unstable)
//is sent via the RS232 interface
{
    QByteArray cmd;
    cmd = "w";
    qDebug() << cmd;
    lastKernCmd = cmd;
    return cmd;
}

QByteArray QKERN::getS()
//Function: Stable weighing value for the weight
//is sent via the RS232 interface
{
    QByteArray cmd;
    cmd = "s";
    qDebug() << cmd;
    lastKernCmd = cmd;
    return cmd;
}

QByteArray QKERN::getT()
//Function: No data are sent,
//the balance carries out the tare function.
{
    QByteArray cmd;
    cmd = "t";
    qDebug() << cmd;
    lastKernCmd = cmd;
    return cmd;
}

void QKERN::parseKern(QByteArray kerndata)
{
    //bool ok;
    QByteArray wt;
    qDebug() << kerndata;
    if (kerndata.contains("Error")) {
        kernError = true;
    } else if (kerndata.contains("    ")) {
        wt = kerndata.mid(1,11);
        kernWeight = wt.toDouble();
        kernUnit = "   ";
        kernStable = false;
    } else {
        wt = kerndata.mid(1,11);
        kernWeight = wt.toDouble();
        kernUnit = kerndata.mid(13,3);
        kernStable = true;
    }
}
