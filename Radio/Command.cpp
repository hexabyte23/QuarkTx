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

#include <arduino.h>
#include "SerialLink.h"
#include "Command.h"
#include "Tx.h"
#include "MemoryFree.h"

Command::Command()
:tx_(NULL)
{
}

bool Command::setup(Tx *tx)
{
  tx_ = tx;

  STDOUT << F("Command\t\tOK") << endl;
  return true;
}

void Command::onNewCommand(const char* cmdStr)
{
  if(cmdStr[0] == 0)
    return; // just ignore
        
  if(cmdStr[1] != ' ' && cmdStr[1] != 0)
  {
    STDOUT << F("e-bcf") << endl;    // bad command format
    return;
  }
  
  switch(cmdStr[0])
  {
    case 'a': loadFromEEPROMCmd();break;
    case 'c': toggleCalibrateAnalogicSensorCmd();break;
    case 'd': dumpCmd(cmdStr+2);break;
    case 'f': getFreeMemoryCmd();break;        
    case 'h': helpCmd();break;
    case 'i': toggleDisplayInputUpdateCmd();break;
    case 'l': changeCurrentModelCmd(cmdStr+2);break;
    case 'm': toggleTxModeCmd();break;
    case 'n': displayVersionCmd();break;
    case 'o': toggleDisplayOutputUpdateCmd();break;
    case 'r': resetCmd(cmdStr+2);break;
    case 's': setCmd(cmdStr+2);break;
    case 'v': saveToEEPROMCmd();break;
    case 'w': toggleSimulation();break;

    default:
      STDOUT << F("e-cu ") << cmdStr[0] << endl;   // Command unknown
      break;
  }
}

void Command::helpCmd()
{
  STDOUT << F(
         "help command :\n"
         "a: Load from EEPROM\n"
         "c: Toggle sensors calibration\n" 
         "d [m]|[e]|[s]|[l]: Dump model, EEPROM, sensor or RCL\n"
         "f: Get free memory\n"
         "h: Help\n"
         "i: Toggle input sensor update\n"
         "l [0..1]: Load cur. model\n"
         "m: Toggle Tx mode (transmit or debug)\n"
         "n: Display version\n"
         "o: Toggle output PPM update\n"
         "r [m]|[s]|[l]: Clean model, sensor or RCL\n"
         "s a chan val: Set cur. model servo max\n"
         "s i chan val: Set cur. model servo min\n"
         "s l chan rclStr: Set RCL code to PPM chan\n"
         "s n chan val: Set cur. model servo neutral\n"
         "s r chan val: Set cur. model servo revert\n"
         "s t sensorID val: Set sensor trim\n"
         "s u sensorID val: Set simulate value\n"
         "v: Save to EEPROM\n"
         "w: Toggle simulation mode\n"
         );
}

void Command::toggleTxModeCmd()
{
   tx_->onToggleTxMode();
}

void Command::changeCurrentModelCmd(const char *idxStr)
{  
  if(strlen(idxStr) != 0)
    tx_->onChangeCurrentModel(atoi(idxStr));
  else
    STDOUT << F("e-bpie") << endl;    // bad parameter: index empty
}

void Command::dumpCmd(const char* param)
{
  STDOUT << F("Dump") << endl;
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

void Command::loadFromEEPROMCmd()
{
  tx_->onLoadFromEEPROM();
  STDOUT << F("Load from EEPROM") << endl;
}

void Command::saveToEEPROMCmd()
{
  tx_->onSaveToEEPROM();
  STDOUT << F("Saved to EEPROM") << endl;
}

uint8_t getChannel(const char *str)
{
  uint8_t channel = atoi(str+2);
  if(channel > MAX_PPM_OUTPUT_CHANNEL-1)
  {
    STDOUT << F("e-bp ") << channel << (" ") << MAX_PPM_OUTPUT_CHANNEL-1 << endl;  // Bad parameter
    return -1;
  }

  return channel;
}

uint8_t getSensorID(const char *str)
{
  uint8_t sensorID = atoi(str+2);
  if(sensorID > MAX_INPUT_CHANNEL-1)
  {
    STDOUT << F("e-bp ") << sensorID << (" ") << MAX_INPUT_CHANNEL-1 << endl;  // Bad parameter
    return -1;
  }

  return sensorID;
}

void Command::setCmd(const char* param)
{  
  int val = atoi(param+4);
  
  switch(param[0])
  {
    case 'a':
      tx_->getCurrentModel()->setMaxValue(getChannel(param) , val);
      //STDOUT << F("Set Max channel ") << getChannel(param) << F(" value ") << val << endl;
      
      break;
    case 'i':
      tx_->getCurrentModel()->setMinValue(getChannel(param), val);
      //STDOUT << F("Set Min channel ") << getChannel(param) << F(" value ") << val << endl;
      break;
    case 'l':
      tx_->onSetRCL(getChannel(param), param+4);
      break;
    case 'n':
      tx_->getCurrentModel()->setNeutralValue(getChannel(param), val);
      //STDOUT << F("Set Neutral channel ") << getChannel(param) << F(" value ") << val << endl;
      break;
    case 'r':
      tx_->getCurrentModel()->setRevertValue(getChannel(param) , val);
      //STDOUT << F("Set Revert channel ") << getChannel(param) << F(" value ") << val << endl;
      break;
    case 't':
      tx_->onSetTrimSensorValue(getSensorID(param), val);
      break;
    case 'u': 
      tx_->onSetSimulateSensorValue(getSensorID(param), val);
      break;
    default:
      STDOUT << F("e-bpie") << endl;    // bad parameter: index empty
      break;
  }
}

void Command::resetCmd(const char* param)
{
  tx_->onSoftwareReset(param);
  STDOUT << F("Ok") << endl;
}

void Command::toggleSimulation()
{
  tx_->onToggleSimulation();
}

void Command::getFreeMemoryCmd()
{
    STDOUT << freeMemory() << endl;
}

void Command::displayVersionCmd()
{
  STDOUT << F(QUARKTX_VERSION) << endl;
}

