#include "TxCommand.h"

TxCommand::TxCommand()
   : radioLinkRef_(NULL)
{

}

TxCommand::~TxCommand()
{

}

void TxCommand::init(RadioLink *radioLinkRef)
{
   radioLinkRef_ = radioLinkRef;

   //sendCommand(TX_CMD_HELP);
   //sendCommand(TX_CMD_DUMP_SENSOR);
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

QString TxCommand::getFreeMemoryStr()
{
   QString ret = "NA";

   if(radioLinkRef_ == NULL)
   {
      qWarning() << "radioLinkRef is NULL";
      return "";
   }

   if(sendCommand(TX_CMD_FREE_MEMORY))
      ret = radioLinkRef_->getNextLine();

   return ret;
}
