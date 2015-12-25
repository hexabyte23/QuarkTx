/*
Command.h - QuarkTx
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

#ifndef COMMAND_H
#define COMMAND_H

#include "config.h"

// forward decl
class Tx;

class Command
{
  Tx *tx_;
  
  public:
 
  Command();

  bool setup(Tx *tx);

  // signals
  void onNewCommand(const char* cmdStr);

  // Commands
  void helpCmd();
  void toggleTransmitModeCmd();
  void toggleDisplayInputUpdateCmd();
  void toggleDisplayOutputUpdateCmd();
  void changeCurrentModelCmd(const char* modelIdx);
  void dumpCmd(const char* param);
  void toggleCalibrateAnalogicSensorCmd();
  void loadModelsFromEEPROMCmd();
  void saveModelsToEEPROMCmd();
  void setModelCmd(const char* param);
  void toggleSimulation();
  void setSimulateSensorValueCmd(const char* param);
  void resetCmd();
};

#endif
