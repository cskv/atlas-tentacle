/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.htm1l and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.htm1l.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QTimer>

#include "atlasdialog.h"
//#include "qatlas.h"
#include "qatlasusb.h"

#include "ledindicator.h"
#include "ezoframe.h"
#include "plotframe.h"
#include "qcustomplot.h"
#include "about.h"
#include "serialdialog.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void openSerialPort2();

    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void writeData(const QByteArray &data);
    void readData();

    void readTentacleI2CData();
    //void readAtlasUSBData();

    void on_action_Help_Tentacle_triggered();

    void displayAllMeas();

    //void readRawI2CData();

    void on_contCB_clicked(bool checked);


    void on_actionScreenshot_triggered();
    void setupEZOFrames();

    void on_mainTimer();

    void on_actionAbout_AtlasTerminal_triggered();

    void on_actionAbout_Qt_triggered();

private:
    Ui::MainWindow *ui;
    LedIndicator* ledStateLed;

    SettingsDialog *settings;
    SerialDialog* sd;
    QSerialPort *serial;
    QByteArray lastCmd;

    EZOFrame* pH1Frame;
    EZOFrame* pH2Frame;

    PlotFrame* pf;

    QTimer* mainTimer;
    //QTimer* delayTimer;

    AtlasDialog* ad;
    //About* aboutAtlas;

    QCPPlotTitle* plotTitle;
};

#endif // MAINWINDOW_H
