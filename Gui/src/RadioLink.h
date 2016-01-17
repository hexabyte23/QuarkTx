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

#define TX_CMD_LOAD_FROM_EEPROM     "a\r"
#define TX_CMD_TOGGLE_CALIBRATE     "c\r"
#define TX_CMD_DUMP_ALL             "d\r"
#define TX_CMD_DUMP_MODEL           "d m\r"
#define TX_CMD_DUMP_EEPROM          "d e\r"
#define TX_CMD_DUMP_SENSOR          "d s\r"
#define TX_CMD_DUMP_RCL             "d l\r"
#define TX_CMD_HELP                 "h\r"


class RadioLink : public QObject
{
   Q_OBJECT

   QByteArray input_, output_;
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
   QSerialPortInfo getFirstAvailableConnexion() {return serialPortInfo_;}
#endif

   void writeData(const QByteArray &data);
   const QByteArray readData();

   // QML
   Q_INVOKABLE bool findTxAndConnect();
   Q_INVOKABLE QString getNextLine();
   Q_INVOKABLE bool sendCommand(const QString &cmd);
   Q_INVOKABLE QString getTxVersion() {return txVersion_;}

private slots:
   void serialPortReadyRead();
   void serialPortHandleError(QSerialPort::SerialPortError error);
};

#endif // RADIOLINK_H

