/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected to an Arduino(TM)
**  with Tentacle (TM) interface shield.
**  Copyright (C) 2016 Paul JM van Kan
**
**  AtlasTerminal is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  AtlasTerminal is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.

**  You should have received a copy of the GNU General Public License
**  along with AtlasTerminal.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************
**           Author: Paul JM van Kan                                     **
**  Website/Contact:                                                     **
**             Date:                                                     **
**          Version:                                                     **
***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QTimer>

#include "atlasdialog.h"
//#include "qatlas.h"
//#include "qatlasusb.h"

#include "ledindicator.h"
#include "ezoframe.h"
#include "plotframe.h"
#include "thirdparty/qcustomplot.h"
#include "about.h"
#include "serialdialog.h"
#include "loggingframe.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    static const int NUMSTAMPS = 2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setupEZOFrames();

    void openSerialPort2();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void writeData(const QByteArray &data);
    void readData();

    void readTentacleI2CData();
    //void readAtlasUSBData();
    //void readRawI2CData();

    void displayAllMeas();

    void on_contCB_clicked(bool checked);

    void on_mainTimer();

    void on_action_Help_Tentacle_triggered();
    void on_actionScreenshot_triggered();
    void on_actionAbout_AtlasTerminal_triggered();
    void on_actionAbout_Qt_triggered();

    void on_btnLogStart_clicked();
    void on_btnLogStop_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    //const int NUMSTAMPS = 4;
    QWidget* EZOTab[NUMSTAMPS];
    EZOFrame* ezof[NUMSTAMPS];
    QLabel* lblEZO[NUMSTAMPS];
    QLabel* lblValue[NUMSTAMPS];

    //LedIndicator* ledStateLed;

    SerialDialog* sd;
    QSerialPort *serial;
    QByteArray lastCmd;

    PlotFrame* pf;
    LoggingFrame* logf;
    QString commentLine;

    QTimer* mainTimer;
    //QTimer* delayTimer;

    AtlasDialog* ad;
    //About* aboutAtlas;

    QCPPlotTitle* plotTitle;
    bool isLogging = false;
};

#endif // MAINWINDOW_H
