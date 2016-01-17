#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QQmlContext>
#include "batterymodel.h"
#include "RadioLink.h"
#include "TxCommand.h"

class MainModel
{
    BatteryModel batteryModel_;
    RadioLink radioLink_;
    TxCommand txCommand_;

public:
    MainModel();
    virtual ~MainModel();

    void init(QQmlContext *context);
};

#endif // MAINMODEL_H
