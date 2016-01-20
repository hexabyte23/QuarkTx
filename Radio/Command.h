/*
Command.h - QuarkTx
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

#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>
#include "config.h"

// forward decl
class Tx;

class Command
{
   Tx *tx_;

   // internal commands
   void helpCmd();
   void toggleTxModeCmd();
   void displayVersionCmd();
   void toggleDisplayInputUpdateCmd(const char* param);
   void toggleDisplayOutputUpdateCmd(const char* param);
   void changeCurrentModelCmd(const char* modelIdx);
   void dumpCmd(const char* param);
   void toggleCalibrateAnalogicSensorCmd();
   void loadFromEEPROMCmd();
   void saveToEEPROMCmd();
   void setCmd(const char* param);
   void toggleSimulation();
   void resetCmd(const char* param);
   void getFreeMemoryCmd();
   void setRCLCmd(const char* param);

public:

   Command();

   bool setup(Tx *tx);

   // signals
   void onNewCommand(const char* cmdStr);
};

#endif
