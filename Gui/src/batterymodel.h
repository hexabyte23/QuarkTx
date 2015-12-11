#ifndef BATTERYDATA_H
#define BATTERYDATA_H

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

#endif // BATTERYDATA_H

