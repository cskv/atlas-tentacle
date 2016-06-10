#include "plotframe.h"
#include "ui_plotframe.h"

PlotFrame::PlotFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlotFrame)
{
    ui->setupUi(this);
    dataTimer = new QTimer(this);
    setupPlot3();
}

PlotFrame::~PlotFrame()
{
    delete ui;
}

void PlotFrame::setupPlot()
{
// generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot->plotLayout()->insertRow(0); // inserts an empty row above the default axis rect
    ui->customPlot->plotLayout()->
            addElement(0, 0, plotTitle = new QCPPlotTitle(ui->customPlot, "pH during experiment"));
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsImpulse);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("time");
    ui->customPlot->yAxis->setLabel("pH");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(0, 14);
    ui->customPlot->replot();
}

void PlotFrame::setupPlot2()
{
// set locale to English, so we get english month names:
    ui->customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
// seconds of current time, we'll use it as starting point in time for data:
    double now = QDateTime::currentDateTime().toTime_t();
    srand(8); // set the random seed, so we always get the same random data
// create multiple graphs:
    for (int gi=0; gi<2; ++gi) {
        ui->customPlot->addGraph();
        QPen pen;
        pen.setColor(QColor(0, 0, 255, 200));
        pen.setColor(QColor(Qt::red));
        ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph()->setPen(pen);
       // ui->customPlot->graph()->setBrush(QBrush(QColor(255/4.0*gi,160,50,150)));
// generate random walk data:
        QVector<double> time(250), value(250);
        for (int i=0; i<250; ++i) {
            time[i] = now + 60*i;
            value[i] = 7.00 + (rand()/(double)RAND_MAX-0.5);
        }
        ui->customPlot->graph()->setData(time, value);
    }
    ui->customPlot->graph(0)->setName("pH1");
    ui->customPlot->graph(1)->setName("pH2");
// configure bottom axis to show date and time instead of number:
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm");
// set a more compact font size for bottom and left axis tick labels:
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
// set a fixed tick-step to one tick per month:
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(600); // one minute in seconds
    ui->customPlot->xAxis->setSubTickCount(2);
// set axis labels:
    ui->customPlot->xAxis->setLabel("time");
    ui->customPlot->yAxis->setLabel("pH");
// make top and right axes visible but without ticks and labels:
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);
// set axis ranges to show all data:
    ui->customPlot->xAxis->setRange(now, now+3600);
    ui->customPlot->yAxis->setRange(0, 14);
// show legend:
    ui->customPlot->legend->setVisible(true);
}

void PlotFrame::setupPlot3()
{
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    //ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    //ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));

    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

// configure x-axis
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    //ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->xAxis->setTickStep(30);

// configure y-axis
    ui->customPlot->yAxis->setRange(0, 14);

// complete box around plot and
// make left and bottom axes transfer their ranges to right and top axes:
    ui->customPlot->axisRect()->setupFullAxesBox();
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
// (dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
// connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeTentacleSlot()));
// dataTimer->start(2000); // Interval 0 means to refresh as fast as possible
}

void PlotFrame::realtimeDataSlot()
{
// calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) {// at most add point every 10 ms
        double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
        double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
// add data to lines:
        ui->customPlot->graph(0)->addData(key, value0);
        ui->customPlot->graph(1)->addData(key, value1);
// set data of dots:
        ui->customPlot->graph(2)->clearData();
        ui->customPlot->graph(2)->addData(key, value0);
        ui->customPlot->graph(3)->clearData();
        ui->customPlot->graph(3)->addData(key, value1);
// remove data of lines that's outside visible range:
        ui->customPlot->graph(0)->removeDataBefore(key-8);
        ui->customPlot->graph(1)->removeDataBefore(key-8);
// rescale value (vertical) axis to fit the current data:
        ui->customPlot->graph(0)->rescaleValueAxis();
        ui->customPlot->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
// make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->customPlot->replot();

// calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) {       // average fps over 2 seconds
       // ui->statusBar->showMessage(
       // QString("%1 FPS, Total Data points: %2")
        //.arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
       // .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
        //, 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void PlotFrame::realtimeTentacleSlot(const double value0, double value1)
{
// calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    //double value0 = 7.000; //pH1Frame->tm->getpHORP();
    //double value1 = 6.95; //pH2Frame->tm->getpHORP();

// add data to lines:
        ui->customPlot->graph(0)->addData(key, value0);
        ui->customPlot->graph(1)->addData(key, value1);

// set data of dots:
        ui->customPlot->graph(2)->clearData();
        ui->customPlot->graph(2)->addData(key, value0);
        ui->customPlot->graph(3)->clearData();
        ui->customPlot->graph(3)->addData(key, value1);

// remove data of lines that's outside visible range:
        ui->customPlot->graph(0)->removeDataBefore(key-120);
        ui->customPlot->graph(1)->removeDataBefore(key-120);

// rescale value (vertical) axis to fit the current data:
        //ui->customPlot->graph(0)->rescaleValueAxis();
        //ui->customPlot->graph(1)->rescaleValueAxis(true);
// make key axis range scroll with the data (at a constant range size of 60):
    ui->customPlot->xAxis->setRange(key+2, 120, Qt::AlignRight);
    ui->customPlot->replot();
}
