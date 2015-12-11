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
  void dumpModelCmd(const char *modelIdx);
  void toggleCalibrateAnalogicSensorCmd();
  void loadModelsFromEEPROMCmd();
  void saveModelsToEEPROMCmd();
  void setModelCmd(const char* param);
};

#endif
