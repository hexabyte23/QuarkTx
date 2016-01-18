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

#include "TxCommand.h"

TxCommand::TxCommand()
   :
     radioLinkRef_(NULL),
     refreshSensorCache_(true)
{

}

TxCommand::~TxCommand()
{

}

void TxCommand::init(RadioLink *radioLinkRef)
{
   radioLinkRef_ = radioLinkRef;
}

bool TxCommand::sendCommand(const QString &cmd)
{
   if(radioLinkRef_ == NULL)
   {
      qWarning() << "radioLinkRef is NULL";
      return false;
   }
   return radioLinkRef_->sendCommand(cmd);
}

QString TxCommand::getNextLine()
{
   QString ret = "NA";

   if(radioLinkRef_ == NULL)
   {
      qWarning() << "radioLinkRef is NULL";
      return "";
   }

   ret = radioLinkRef_->getNextLine();

   return ret;
}

QString TxCommand::getFreeMemoryStr()
{
   QString ret = "NA";

   if(sendCommand(TX_CMD_FREE_MEMORY))
      ret = getNextLine();

   return ret;
}

void TxCommand::fillSensorCache()
{
   sendCommand("d s\r");

   sensorCache_.clear();
   qDebug() << "FillSensorCache";

   QString line;
   while(!line.startsWith("Dump"))
      line= getNextLine();          // Dump
   line = getNextLine();            // Sensors(xx)
   line = getNextLine();            // #pin #trim

   sensorCache_.push_back(getNextLine()); // #0
   sensorCache_.push_back(getNextLine()); // #1
   sensorCache_.push_back(getNextLine()); // #2
   sensorCache_.push_back(getNextLine()); // #3
   sensorCache_.push_back(getNextLine()); // #4
   sensorCache_.push_back(getNextLine()); // #5
   sensorCache_.push_back(getNextLine()); // #6

   refreshSensorCache_ = false;
}

QVariantMap TxCommand::getSensorData(int sensorID)
{
   if(sensorCache_.isEmpty() || refreshSensorCache_)
      fillSensorCache();

   QString line = sensorCache_[sensorID];

   QStringList l = line.split("\t");
   qDebug() << l;

   QVariantMap map;

   if(l.size() < 4)
   {
      qWarning() << "Bad sensor cache format";
      return map;
   }
   map.insert("trim", l[1].toInt());
   map.insert("min", l[2].toInt());
   map.insert("max", l[3].toInt());
   map.insert("sim", l[4].toInt());

   return map;
}

void TxCommand::setSensorData(int sensorID, QVariantMap data)
{
   if(data.contains("trim"))
      sendCommand(QString("s t %1 %2\r").arg(sensorID).arg(data["trim"].toInt()));

   if(data.contains("min"))
      sendCommand(QString("s i %1 %2\r").arg(sensorID).arg(data["min"].toInt()));

   if(data.contains("max"))
      sendCommand(QString("s a %1 %2\r").arg(sensorID).arg(data["max"].toInt()));

   refreshSensorCache_ = true;
}
