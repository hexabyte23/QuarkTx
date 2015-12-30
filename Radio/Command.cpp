/*
Command.cpp - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

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

#include <arduino.h>
#include "SerialLink.h"
#include "Command.h"
#include "Tx.h"
#include "FlashMem.h"
#include "MemoryFree.h"

Command::Command()
:tx_(NULL)
{
}

bool Command::setup(Tx *tx)
{
  tx_ = tx;
  
  info(INFO_COMMAND);
  return true;
}

void Command::onNewCommand(const char* cmdStr)
{
  if(cmdStr[0] == 0)
    return; // just ignore
        
  if(cmdStr[1] != ' ' && cmdStr[1] != 0)
  {
    error(ERR_BAD_COMMAND);
    return;
  }
  
  switch(cmdStr[0])
  {
    case 'a': loadModelsFromEEPROMCmd();break;
    case 'c': toggleCalibrateAnalogicSensorCmd();break;
    case 'd': dumpCmd(cmdStr+2);break;
    case 'f': getFreeMemoryCmd();break;        
    case 'h': helpCmd();break;
    case 'i': toggleDisplayInputUpdateCmd();break;
    case 'l': changeCurrentModelCmd(cmdStr+2);break;
    case 'm': toggleTransmitModeCmd();break;
    case 'o': toggleDisplayOutputUpdateCmd();break;
    case 'r': resetCmd();break;
    case 's': setModelCmd(cmdStr+2);break;
    case 'u': setSimulateSensorValueCmd(cmdStr+2);break;
    case 'v': saveModelsToEEPROMCmd();break;
    case 'w': toggleSimulation();break;

    default: 
      error(ERR_COMMAND_UNKNOWN, cmdStr);
      break;
  }
}

void Command::helpCmd()
{
  info(INFO_HELP_USAGE1);
  info(INFO_HELP_USAGE2);
  info(INFO_HELP_USAGE3);
}

void Command::toggleTransmitModeCmd()
{
   tx_->onToggleMode();
}

void Command::changeCurrentModelCmd(const char *idxStr)
{  
  if(strlen(idxStr) != 0)
    tx_->onChangeCurrentModel(atoi(idxStr));
  else
    error(ERR_BAD_PARAM_IDX_EMPTY);
}

void Command::dumpCmd(const char* param)
{
  info(INFO_DUMP_CMD);
  tx_->onDump(param);
}

void Command::toggleDisplayInputUpdateCmd()
{
  tx_->onToggleDisplayInputUpdate();
}

void Command::toggleDisplayOutputUpdateCmd()
{
  tx_->onToggleDisplayOutputUpdate();
}

void Command::toggleCalibrateAnalogicSensorCmd()
{
  tx_->onToggleCalibrateSensor();
}

void Command::loadModelsFromEEPROMCmd()
{
 tx_->onLoadFromEEPROM();
 info(INFO_LOAD_FROM_EEPROM);
}

void Command::saveModelsToEEPROMCmd()
{
 tx_->onSaveToEEPROM();
 info(INFO_SAVE_TO_EEPROM);
}

void Command::setModelCmd(const char* param)
{
  uint8_t c = atoi(param+2);
  if(c > MAX_PPM_OUTPUT_CHANNEL-1)
  {
    error(ERR_BAD_PARAM_IDX_HIGH, c, MAX_PPM_OUTPUT_CHANNEL-1);
    return;
  }
  int v = atoi(param+4);
  
  switch(param[0])
  {
    case 'a':
      tx_->getCurrentModel()->setMaxValue(c , v);
      info(INFO_SET_MAX_CHANNEL, c, v);
      break;
    case 'c':
      tx_->onRCL(c, param+4);
      break;
    case 'i':
      tx_->getCurrentModel()->setMinValue(c, v);
      info(INFO_SET_MIN_CHANNEL, c, v);
      break;
    case 'n':
      tx_->getCurrentModel()->setNeutralValue(c , v);
      info(INFO_SET_NEUTRAL_CHANNEL, c, v);
      break;
    case 'r':
      tx_->getCurrentModel()->setRevertValue(c , v);
      info(INFO_SET_REVERT_CHANNEL, c, v);
      break;
    default:
      error(ERR_BAD_PARAM_IDX_EMPTY);
      break;
  }
}

void Command::resetCmd()
{
  tx_->onReset();
  info(INFO_RESET_CMD);
}

void Command::toggleSimulation()
{
  tx_->onToggleSimulation();
}

void Command::setSimulateSensorValueCmd(const char* param)
{
  uint8_t c = atoi(param);
  if(c > MAX_INPUT_CHANNEL-1)
  {
    error(ERR_BAD_PARAM_IDX_HIGH, c, MAX_INPUT_CHANNEL-1);
    return;
  }
  
  uint16_t v = atoi(param+2);
  tx_->onSetSimulateSensorValue(c, v);
}

void Command::getFreeMemoryCmd()
{
    STDOUT << freeMemory() << endl;
}

