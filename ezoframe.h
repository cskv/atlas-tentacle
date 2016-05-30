#ifndef EZOFRAME_H
#define EZOFRAME_H

#include <QFrame>

#include <QTimer>

#include "atlasdialog.h"
#include "qatlas.h"
#include "qatlasusb.h"
#include "ledindicator.h"

namespace Ui {
class EZOFrame;
}

class EZOFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EZOFrame(QWidget *parent = 0);
    ~EZOFrame();

    QByteArray lastCmd;
    QATLAS* tm = new QATLAS();  // wel even aanmaken !

signals:
    void cmdAvailable(QByteArray newCommand);

private slots:
    void on_btnGetTemp_clicked();
    void on_btnpH_clicked();
    void on_btnSetTemp_clicked();
    void on_btnCal_clicked();
    void on_btnCalClear_clicked();
    void on_btnCalMid_clicked();
    void on_btnCalLow_clicked();
    void on_btnCalHigh_clicked();

    void updateAll();
    void displayAll();
    void on_btnSlope_clicked();
    void on_btnInfo_clicked();
    void on_btnStatus_clicked();
    void on_ledCheckBox_clicked(bool checked);
    void on_btnLED_clicked();
    void on_contCB_clicked(bool checked);
    void on_btnSleep_clicked();

private:
    Ui::EZOFrame *ui;
    LedIndicator* ledStateLed;


    //QATLAS* tm2 = new QATLAS(101);
    //QTimer* mainTimer;
};

#endif // EZOFRAME_H
