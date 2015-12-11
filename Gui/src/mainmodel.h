#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QQmlContext>
#include "batterymodel.h"

class MainModel
{
    BatteryModel batteryModel_;

public:
    MainModel();
    virtual ~MainModel();

    void Init(QQmlContext *context);
};

#endif // MAINMODEL_H
