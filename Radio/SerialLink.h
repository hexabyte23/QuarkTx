#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <arduino.h>
#include "Config.h"
#include "Command.h"

class SerialLink
{
  char serialBuffer_[MAX_SERIAL_INPUT_BUFFER+2];
  int idxBuffer_;
  
  Command *cmd_;

  public:

  SerialLink();

  bool setup(Command *cmd);

  void clearSerialBuffer();
  
  void idle();
};

#endif
