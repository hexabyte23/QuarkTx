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
   bool isTxConnected_;
   QString transportStatusStr,cnxStatusStr_, txVersionStr_;

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

   bool writeData(const QByteArray &data);
   const QByteArray readData();
   QString getNextLine();
   bool sendCommand(const QString &cmd);

   // QML
   Q_INVOKABLE bool findTxAndConnect();
   Q_INVOKABLE void txDisconnect();
   Q_INVOKABLE bool isTxConnected() {return isTxConnected_;}
   Q_INVOKABLE QString getTransportStatusStr() {return transportStatusStr;}
   Q_INVOKABLE QString getConnexionStatusStr() {return cnxStatusStr_;}
   Q_INVOKABLE QString getTxVersionStr() {return txVersionStr_;}

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
private slots:
   void serialPortReadyRead();
   void serialPortHandleError(QSerialPort::SerialPortError error);
#endif

};

#endif // RADIOLINK_H

