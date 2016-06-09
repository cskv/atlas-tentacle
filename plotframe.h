#ifndef PLOTFRAME_H
#define PLOTFRAME_H

#include <QFrame>
#include "qcustomplot.h"

namespace Ui {
class PlotFrame;
}

class PlotFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlotFrame(QWidget *parent = 0);
    ~PlotFrame();

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
