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

#ifndef PLOTFRAME_H
#define PLOTFRAME_H

#include <QFrame>
#include "thirdparty/qcustomplot.h"
#include "ezoframe.h"

namespace Ui {
class PlotFrame;
}

class PlotFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlotFrame(QWidget *parent = 0);
    ~PlotFrame();

public slots:
    void realtimeTentacleSlot(double value0, double value1);

private slots:
    void setupPlot();
    void setupPlot2();
    void setupPlot3();
    void realtimeDataSlot();


private:
    Ui::PlotFrame *ui;

    QCPPlotTitle* plotTitle;
    QTimer* dataTimer;
};

#endif // PLOTFRAME_H
