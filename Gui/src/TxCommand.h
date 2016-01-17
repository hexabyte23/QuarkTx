/*
Command.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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

#ifndef TX_COMMAND_H
#define TX_COMMAND_H

#include <QQmlContext>
#include <QVariantMap>
#include "RadioLink.h"


#define TX_CMD_LOAD_FROM_EEPROM     "a\r"
#define TX_CMD_TOGGLE_CALIBRATE     "c\r"
#define TX_CMD_DUMP_ALL             "d\r"
#define TX_CMD_DUMP_MODEL           "d m\r"
#define TX_CMD_DUMP_EEPROM          "d e\r"
#define TX_CMD_DUMP_SENSOR          "d s\r"
#define TX_CMD_DUMP_RCL             "d l\r"
#define TX_CMD_FREE_MEMORY          "f\r"
#define TX_CMD_HELP                 "h\r"

class TxCommand : public QObject
{
   Q_OBJECT

   RadioLink *radioLinkRef_;
   QStringList sensorCache_;
   bool refreshSensorCache_;

   void fillSensorCache();

public:
   TxCommand();
   virtual ~TxCommand();

   void init(RadioLink *radioLinkRef);
   bool sendCommand(const QString &cmd);
   QString getNextLine();

   // QML
   Q_INVOKABLE QString getFreeMemoryStr();

   Q_INVOKABLE QVariantMap getSensorData(int sensorID);
   Q_INVOKABLE void setSensorData(int sensorID, QVariantMap data);

};

#endif // TX_COMMAND_H
