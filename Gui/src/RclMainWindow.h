/*
Command.cpp - QuarkTx
Copyright (c) 2015-2018 Thierry & Betrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef RCLMAINWINDOW_H
#define RCLMAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include "src/qcustomplot.h"
#include "../../Radio/RCLEval.h"
//#include "../../Radio/Tx.h"

namespace Ui {
class RclMainWindow;
}

class RclMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit RclMainWindow(QWidget *parent = 0);
   ~RclMainWindow();

   void setupPlot();

private slots:
   void horzScrollBarChanged(int value);
   void vertScrollBarChanged(int value);
   void xAxisChanged(QCPRange range);
   void yAxisChanged(QCPRange range);
   void onFormulaChanged();

private:
   Ui::RclMainWindow *ui;
   Sensor *sensor_[10];
   volatile uint16_t outputValue[10];
   Model model_;
   //Tx tx_;
   RCLEval eval_;
   bool relOrAbs_;
};

#endif // RCLMAINWINDOW_H
