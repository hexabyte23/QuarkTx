#ifndef BATTERYMODEL_H
#define BATTERYMODEL_H

#include <QObject>
#include <QString>
#include <QTime>

class BatteryModel : public QObject
{
    Q_OBJECT

    QTime remainingTime_;
    int remainingLevel_;

public:
    BatteryModel();
    virtual ~BatteryModel();

    Q_INVOKABLE QString getRemainingTime() const;
    Q_INVOKABLE int getRemainingLevel() const;
};

#endif // BATTERYMODEL_H

