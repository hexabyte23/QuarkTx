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

   QString line = getNextLine();    // Dump
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
   if(sensorCache_.isEmpty()||refreshSensorCache_)
      fillSensorCache();

   QString line = sensorCache_[sensorID];

   QStringList l = line.split("\t");
   qDebug() << l;

   QVariantMap map;
   map.insert("trim", l[1].toInt());
   map.insert("min", l[2].toInt());
   map.insert("max", l[3].toInt());
   map.insert("sim", l[4].toInt());

   return map;
}

void TxCommand::setSensorData(int sensorID, QVariantMap data)
{

   for()
   {

   }

   refreshSensorCache_ = true;
}
