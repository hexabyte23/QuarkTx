#ifndef MODEL_H
#define MODEL_H

#include <inttypes.h>
#include "config.h"

struct Mixer
{
  int fromChannel_;
  int toChannel_;
  float rate_;
};

struct ServoCommand
{
  uint16_t maxOutCurse_;    // in microsecs
  uint16_t minOutCurse_;    // in microsecs
  int16_t neutral_;         // in microsecs
  bool isRevert_;

  ServoCommand();

  uint16_t getValue(uint16_t rawInputValue);
  void reset();
  uint16_t putToEEPROM(uint16_t addr);
  uint16_t getFromEEPROM(uint16_t addr);
};

struct OutputChannel
{
  ServoCommand servo_;

  uint16_t putToEEPROM(uint16_t addr);
  uint16_t getFromEEPROM(uint16_t addr);
};

class Model
{
  //Mixer mixer_[MAX_MIXER];
  OutputChannel channel_[MAX_PPM_OUTPUT_CHANNEL];

  public:

  Model();

  bool setup();
  void reset();
  void dump();
  
  uint16_t getValue(uint8_t channel, uint16_t rawInputValue);
  void setMinValue(uint8_t channel, int value); 
  void setMaxValue(uint8_t channel, int value);
  void setNeutralValue(uint8_t channel, uint16_t value); 
  void setRevertValue(uint8_t channel, bool value);
  uint16_t putToEEPROM(uint16_t addr);
  uint16_t getFromEEPROM(uint16_t addr);
};

#endif
