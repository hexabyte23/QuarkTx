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
   return radioLinkRef_->sendCommand(cmd);
}
