/*
Command.cpp - QuarkTx
Copyright (c) 2015-2018 Thierry & Betrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
   QString transportStatusStr, cnxStatusStr_, txVersionStr_;

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

