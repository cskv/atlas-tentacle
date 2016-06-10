#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SerialDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SerialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialDialog(QWidget *parent = 0);
    ~SerialDialog();

    struct PortParameters {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    PortParameters pp();

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

    Ui::SerialDialog *ui;
    PortParameters cp;
    QIntValidator *intValidator;
};

#endif // SERIALDIALOG_H
