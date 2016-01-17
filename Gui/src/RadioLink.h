#ifndef RADIOLINK_H
#define RADIOLINK_H

#include <QObject>
#include <QString>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
#include <QSerialPort>
#include <QSerialPortInfo>
#endif
#include "global.h"

class RadioLink : public QObject
{
   Q_OBJECT

   QByteArray input_, output_;
   QString cnxStatusStr_;
   QString txVersion_;

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
   QSerialPort *serialPort_;
   QSerialPortInfo serialPortInfo_;
#endif
   QBluetoothLocalDevice blueToothlocalDevice_;

   bool searchForSerialLink();
   void closeSerialLink();
   bool searchForBlueToothLink();
   void closeBlueToothLink();

public:
   RadioLink();
   virtual ~RadioLink();

   void init();

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
   QSerialPortInfo getFirstAvailableSerialConnexionInfo() {return serialPortInfo_;}
#endif

   void writeData(const QByteArray &data);
   const QByteArray readData();
   QString getNextLine();
   bool sendCommand(const QString &cmd);

   // QML
   Q_INVOKABLE bool findTxAndConnect();
   Q_INVOKABLE QString getConnexionStatus() {return cnxStatusStr_;}
   Q_INVOKABLE QString getTxVersion() {return txVersion_;}

private slots:
   void serialPortReadyRead();
   void serialPortHandleError(QSerialPort::SerialPortError error);
};

#endif // RADIOLINK_H

