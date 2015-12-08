#ifndef COMMAND_H
#define COMMAND_H

#include "config.h"

// forward decl
class Tx;

class Command
{
  Tx *tx_;
  bool startUpdate_;

  void displayPrompt();
  
  public:

  // 2 Modes
  enum Mode {cmTransmit, cmSetting} mode_;
  
  Command();

  bool setup(Tx *tx);
  void setMode(Mode mode) {mode_ = mode;}

  // signals
  void onNewCommand(const char* cmdStr);

  // Commands
  void helpCmd();
  void setGetModeCmd(const char *modeStr);
  void changeCurrentModelCmd(const char *modelIdx);
  void dumpModelCmd(const char *modelIdx);
  void starStoptUpdateCmd();
  void calibrateAnalogicSensorCmd();
};

#endif
