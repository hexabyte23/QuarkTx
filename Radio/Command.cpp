/*
Command.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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

void Command::setup(Tx *tx)
{
   tx_ = tx;
}

void Command::onNewCommand(const char* cmdStr)
{
   if(cmdStr[0] == 0)
      return; // just ignore, should raise only in \n\r termination char case

   if(cmdStr[1] != ' ' && cmdStr[1] != 0)
   {
      STDOUT << F("e-bcf") << endl;    // bad command format
      return;
   }

   switch(cmdStr[0])
   {
      case 'a': loadFromEEPROMCmd();break;
      case 'b': rebootCmd();break;
      case 'c': toggleCalibrateAnalogicSensorCmd();break;
      case 'd': dumpCmd(cmdStr+2);break;
      case 'f': getFreeMemoryCmd();break;
      case 'h': helpCmd();break;
      case 'i': toggleDisplayInputUpdateCmd(cmdStr+2);break;
      case 'l': changeCurrentModelCmd(cmdStr+2);break;
      case 'm': toggleTxModeCmd();break;
      case 'n': displayVersionCmd();break;
      case 'o': toggleDisplayOutputUpdateCmd(cmdStr+2);break;
      case 'r': resetCmd(cmdStr+2);break;
      case 's': setCmd(cmdStr+2);break;
      case 't': btATCmd(cmdStr+2);break;
      case 'u': getUUIDCmd();break;
      case 'v': saveToEEPROMCmd();break;
      default:
         STDOUT << F("e-cu ") << cmdStr[0] << endl;   // Command unknown
         break;
   }
}

void Command::helpCmd()
{
   STDOUT << F(
                "help command\n"
                "a: Load from EEPROM\n"
                "c: Toggle sensors calibration\n"
#ifdef QUARKTX_TEENSY
                "d [m]|[e]|[s]|[l]|[d]: Dump Model, EEPROM, Sensor, RCL or Directory\n"
#else
                "d [m]|[e]|[s]|[l]: Dump Model, EEPROM, Sensor or RCL\n"
#endif
                "f: Get free memory\n"
                "h: Help\n"
                "i [freq]: Toggle input sensor update\n"
                "l (0..1): Load cur. model\n"
                "m: Toggle Tx mode (transmit or debug)\n"
                "n: Display version\n"
                "o [freq]: Toggle output PPM update\n"
                "r [m]|[s]|[l]: Clean model, sensor or RCL\n"
                "s a chan val: Set cur. model servo max\n"
                "s i chan val: Set cur. model servo min\n"
                "s l chan rclStr: Set RCL code to PPM chan\n"
                "s n chan val: Set cur. model servo neutral\n"
                "s r chan val: Set cur. model servo revert\n"
                "s t sensorID val: Set sensor trim\n"
                "s u sensorID val: Set simulate value\n"
                "s v sensorID val: Set sensor Min\n"
                "s w sensorID val: Set sensor Max\n"
                "t cmd: Bluetooth module AT cmd\n"
                "u: getUUID\n"
                "v: Save to EEPROM\n"
                );
}

void Command::toggleTxModeCmd()
{
   tx_->onToggleTxMode();
}

void Command::rebootCmd()
{
  
}

void Command::btATCmd(const char* param)
{
  delay(1000);
  STDOUT << "AT" << param;
  delay(1000);
  STDOUT << endl;
}

void Command::getUUIDCmd()
{
#ifdef QUARKTX_TEENSY
  MacAddress ma;
  ma.setup();
  
  STDOUT << _HEX(ma[0]) << ":"
         << _HEX(ma[1]) << ":"
         << _HEX(ma[2]) << ":"
         << _HEX(ma[3]) << ":"
         << _HEX(ma[4]) << ":"
         << _HEX(ma[5]) << endl;
#else
  STDOUT << F("e-ni") << endl;  // not implemented
#endif
}

void Command::changeCurrentModelCmd(const char *idxStr)
{  
   if(strlen(idxStr) != 0)
      tx_->onChangeCurrentModel(atoi(idxStr));
   else
      STDOUT << F("e-bpim") << endl;    // bad parameter: index is missing
}

void Command::dumpCmd(const char* param)
{
   STDOUT << F("Dump") << endl;
   tx_->onDump(param);
}

void Command::toggleDisplayInputUpdateCmd(const char* param)
{
   int period = -1;
   if(param)
      period = atoi(param);
   tx_->onToggleDisplayInputUpdate(period);
}

void Command::toggleDisplayOutputUpdateCmd(const char* param)
{
   int period = -1;
   if(param)
      period = atoi(param);
   tx_->onToggleDisplayOutputUpdate(period);
}

void Command::toggleCalibrateAnalogicSensorCmd()
{
   tx_->onToggleCalibrateSensor();
}

void Command::loadFromEEPROMCmd()
{
   STDOUT << F("Load from EEPROM ");
   if(tx_->onLoadFromEEPROM())
      STDOUT << F("succeed") << endl;
   else
      STDOUT << F("failed") << endl;
}

void Command::saveToEEPROMCmd()
{
   tx_->onSaveToEEPROM();
   STDOUT << F("Saved to EEPROM") << endl;
}

uint8_t getChannel(const char *str)
{
   uint8_t channel = str[0] - '0';
   if(channel > QUARKTX_MAX_PPM_OUTPUT_CHANNEL-1)
   {
      STDOUT << F("e-bp ") << channel << (" ") << QUARKTX_MAX_PPM_OUTPUT_CHANNEL-1 << endl;  // Bad parameter
      return -1;
   }

   return channel;
}

uint8_t getSensorID(const char *str)
{
   uint8_t sensorID = str[0] - '0';
   if(sensorID > QUARKTX_MAX_INPUT_CHANNEL-1)
   {
      STDOUT << F("e-bp ") << sensorID << (" ") << QUARKTX_MAX_INPUT_CHANNEL-1 << endl;  // Bad parameter
      return -1;
   }

   return sensorID;
}

void Command::setCmd(const char* param)
{
   Model *currentModel = tx_->getCurrentModel();

   switch(param[0])
   {
      case 'a':   // a chan val: Set cur. model servo max
      {
         uint8_t channel = getChannel(param+2);
         int val = atoi(param+4);
         currentModel->setMaxValue(channel, val);
      }
         break;
      case 'i':   // i chan val: Set cur. model servo min
      {
         uint8_t channel = getChannel(param+2);
         int val = atoi(param+4);
         currentModel->setMinValue(channel, val);
      }
         break;
      case 'l':   // l chan rclStr: Set RCL code to PPM chan
      {
         uint8_t channel = getChannel(param+2);
         tx_->onSetRCL(channel, param+4);
      }
         break;
      case 'n':   // n chan val: Set cur. model servo neutral
      {
         uint8_t channel = getChannel(param+2);
         int val = atoi(param+4);
         currentModel->setNeutralValue(channel, val);
      }
         break;
      case 'r':   // r chan val: Set cur. model servo revert
      {
         uint8_t channel = getChannel(param+2);
         int val = atoi(param+4);
         currentModel->setRevertValue(channel , val);
      }
         break;
      case 't':   // t sensorID val: Set sensor trim
      {
         int val = atoi(param+4);
         if(param[2] == 'b')
         {
           tx_->onSetTrimBattery(val);
         }
         else
         {
           uint8_t sensorID = getSensorID(param+2);
           tx_->onSetTrimSensorValue(sensorID, val);
         }
      }
         break;
      case 'v':   // u sensorID val: Set sensor Min value
      {
         uint8_t sensorID = getSensorID(param+2);
         int val = atoi(param+4);
         tx_->onSetMinSensorValue(sensorID, val);
      }
         break;
      case 'w':   // u sensorID val: Set sensor Max value
      {
         uint8_t sensorID = getSensorID(param+2);
         int val = atoi(param+4);
         tx_->onSetMaxSensorValue(sensorID, val);
      }
         break;
      default:
         STDOUT << F("e-bpim") << endl;    // bad parameter: index is missing
         break;
   }
}

void Command::resetCmd(const char* param)
{
   tx_->onSoftwareReset(param);
   STDOUT << F("Ok") << endl;
}

void Command::getFreeMemoryCmd()
{
   STDOUT << freeMemory() << endl;
}

void Command::displayVersionCmd()
{
   STDOUT << F(QUARKTX_VERSION) << endl;
}

