#include "batterymodel.h"

BatteryModel::BatteryModel()
    : remainingTime_(8,0),
      remainingLevel_(50)
{
}

BatteryModel::~BatteryModel()
{
}

QString BatteryModel::getRemainingTime() const
{
    return remainingTime_.toString("hh:mm");
}

int BatteryModel::getRemainingLevel() const
{
    return remainingLevel_;
}
