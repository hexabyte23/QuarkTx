#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QQmlContext>
#include "batterymodel.h"
#include "RadioLink.h"

class MainModel
{
    BatteryModel batteryModel_;
    RadioLink radioLink_;

public:
    MainModel();
    virtual ~MainModel();

    void init(QQmlContext *context);
};

#endif // MAINMODEL_H
