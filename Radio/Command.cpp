#include <arduino.h>
#include "Command.h"
#include "Tx.h"
#include "FlashMem.h"

Command::Command()
:mode_(cmTransmit),
startUpdate_(true),
tx_(NULL)
{
  
}

void Command::displayPrompt()
{
  printf(">\n");
}

bool Command::setup(Tx *tx)
{
  tx_ = tx;
  
  printf("Command\t\tOK\n");
  return true;
}

void Command::onNewCommand(const char* cmdStr)
{
  //debug("[d] Command '%s'\n", cmdStr);
  if(cmdStr[0] == 0)
  {
    displayPrompt();
    return; // just ignore
  }
        
  if(cmdStr[1] != ' ' && cmdStr[1] != 0)
  {
    error(ERR_BAD_COMMAND);
    return;
  }
  
  switch(cmdStr[0])
  {
    case 'h': helpCmd();break;
    case 'm': setGetModeCmd(cmdStr+2);break;
    case 'l': changeCurrentModelCmd(cmdStr+2);break;
    case 'p': dumpModelCmd(cmdStr+2);break;
    case 'a': starStoptUpdateCmd();break;
    case 'c': calibrateAnalogicSensorCmd();break;
    default: 
      printf("[e] Command '%s' unknown\n", cmdStr);
      break;
  }
  displayPrompt();
}

void Command::helpCmd()
{
  info(INFO_HELP_USAGE);
}

void Command::setGetModeCmd(const char *modeStr)
{
  if(strlen(modeStr) == 0)
  {
    if(mode_ == cmSetting)
      info(INFO_CURRENT_MODE_TRANSMIT);
    else
      info(INFO_CURRENT_MODE_SETTINGS);
  }
  else
  {
    if(modeStr[0] == 't')
    {
      setMode(cmTransmit);
      info(INFO_SWITCH_MODE_TRANSMIT);
    }
    else if(modeStr[0] == 's')
    {
      setMode(cmSetting);
      info(INFO_SWITCH_MODE_SETTINGS);
    }
    else
      error(ERR_BAD_PARAM_EXP_TS);
  }
}

void Command::changeCurrentModelCmd(const char *idxStr)
{
  //debug("[d] load model %s\n", idxStr);  
  if(strlen(idxStr) != 0)
  {
    tx_->onChangeCurrentModel(atoi(idxStr));
  }
  else
    error(ERR_BAD_PARAM_IDX_EMPTY);
}

void Command::dumpModelCmd(const char *idxStr)
{
  //debug("[d] dump model %s\n", idxStr);  
  if(strlen(idxStr) != 0)
  {
    tx_->onDumpModel(atoi(idxStr));
  }
  else
    error(ERR_BAD_PARAM_IDX_EMPTY);
}

void Command::starStoptUpdateCmd()
{
  if(startUpdate_)
    tx_->onStartUpdateToSerial();
  else
    tx_->onStopUpdateToSerial();

  startUpdate_ = !startUpdate_;
}

void Command::calibrateAnalogicSensorCmd()
{
  tx_->onCalibrateAnalogicSensors();
}

