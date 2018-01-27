#include "RclMainWindow.h"
#include "ui_RclMainWindow.h"

Q_LOGGING_CATEGORY(QUARKTX_RCL, "quarktx.rcl")

RclMainWindow::RclMainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::RclMainWindow)
{
   ui->setupUi(this);

   for(uint8_t i=0; i < QUARKTX_MAX_ADC_INPUT_CHANNEL; i++)
      sensor_[i] = new Stick;

   for(uint8_t i= QUARKTX_MAX_ADC_INPUT_CHANNEL; i < QUARKTX_MAX_ADC_INPUT_CHANNEL+QUARKTX_MAX_DIG_INPUT_CHANNEL; i++)
      sensor_[i] = new Switch;

   eval_.setup(sensor_, outputValue, &model_, NULL);
   eval_.setupRCL(0, "i0");
   eval_.setupRCL(1, "i1");
   ui->lineEdit->setText("i0");

   sensor_[0]->setup(A0);
   sensor_[0]->setMin(99);
   sensor_[0]->setMax(782);
   sensor_[1]->setup(A1);
   sensor_[1]->setMin(248);
   sensor_[1]->setMax(852);

   //relOrAbs_ = true;

   setupPlot();

   // configure scroll bars:
   // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
   // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
   // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
   // just increase the the minimum/maximum values of the scroll bars as needed.
   if(relOrAbs_)
   {
      ui->horizontalScrollBar->setRange(-80, 80);
      ui->verticalScrollBar->setRange(-80, 80);
   }
   else
   {
      ui->horizontalScrollBar->setRange(-500, 500);
      ui->verticalScrollBar->setRange(-500, 500);
   }

   // create connection between axes and scroll bars:
   connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
   connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
   connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
   connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
   connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onFormulaChanged()));

   // initialize axis range (and scroll bar positions via signals we just connected):
   if(relOrAbs_)
   {
      ui->plot->xAxis->setRange(0, 110, Qt::AlignLeft);
      ui->plot->yAxis->setRange(0, 110, Qt::AlignLeft);
   }
   else
   {
      ui->plot->xAxis->setRange(0, 1200, Qt::AlignLeft);
      ui->plot->yAxis->setRange(0, 1800, Qt::AlignLeft);
   }
}

RclMainWindow::~RclMainWindow()
{
   delete ui;
}

void RclMainWindow::setupPlot()
{
   // The following plot setup is mostly taken from the plot demos:
   ui->plot->addGraph();
   ui->plot->graph()->setPen(QPen(Qt::blue));

   //ui->plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
   ui->plot->addGraph();
   ui->plot->graph()->setPen(QPen(Qt::red));

   QVector<double> x(1024), y0(1024), y1(1024);

   for(int i=0; i < 1024; ++i)
   {
      analogWrite(0, i);
      analogWrite(1, i);

      eval_.loop();

      if(relOrAbs_)
      {
         x[i] = (i-QUARKTX_ADC_MIN_VALUE)/(QUARKTX_ADC_MAX_VALUE-QUARKTX_ADC_MIN_VALUE)*100.0;
         y0[i] = (outputValue[0]-QUARKTX_PPM_MIN_VALUE)/(QUARKTX_PPM_MAX_VALUE-QUARKTX_PPM_MIN_VALUE)*100.0;
         y1[i] = (outputValue[1]-QUARKTX_PPM_MIN_VALUE)/(QUARKTX_PPM_MAX_VALUE-QUARKTX_PPM_MIN_VALUE)*100.0;
      }
      else
      {
         x[i] = i;
         y0[i] = outputValue[0];
         y1[i] = outputValue[1];
      }
   }

   ui->plot->graph(0)->setData(x, y0);
   //ui->plot->graph(1)->setData(x, y1);
   ui->plot->axisRect()->setupFullAxesBox(true);
   ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void RclMainWindow::onFormulaChanged()
{
   QString str = ui->lineEdit->text();
   if(eval_.setupRCL(0, str.toStdString().c_str()) == false)
      qCWarning(QUARKTX_RCL) << "RCL Parsing error";
   setupPlot();
   ui->plot->replot();
}

void RclMainWindow::horzScrollBarChanged(int value)
{
   if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
   {
      ui->plot->xAxis->setRange(value/100.0, ui->plot->xAxis->range().size(), Qt::AlignLeft);
      ui->plot->replot();
   }
}

void RclMainWindow::vertScrollBarChanged(int value)
{
   if (qAbs(ui->plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
   {
      ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignLeft);
      ui->plot->replot();
   }
}

void RclMainWindow::xAxisChanged(QCPRange range)
{
   ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
   ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void RclMainWindow::yAxisChanged(QCPRange range)
{
   ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
   ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

