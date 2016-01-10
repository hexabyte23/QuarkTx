#ifndef RADIOLINK_H
#define RADIOLINK_H

#include <QObject>
#include <QString>
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
#include <QSerialPort>
#include <QSerialPortInfo>
#endif
#include "global.h"

class RadioLink : public QObject
{
    Q_OBJECT

    QSerialPortInfo portInfo_;

public:
    RadioLink();
    virtual ~RadioLink();

    void setup();

    QSerialPortInfo getFirstAvailableConnexion() {return portInfo_;}
    Q_INVOKABLE QString getNextLine() const;
};

#endif // RADIOLINK_H

