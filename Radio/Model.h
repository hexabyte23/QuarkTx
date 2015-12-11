#ifndef MODEL_H
#define MODEL_H

#include <inttypes.h>
#include "config.h"

struct Mixer
{
  int fromChannel_;
  int toChannel_;
  int rate_;
  
  int getValue(uint8_t channelNum) {return 0;}
};

struct ServoCommande
{
  int maxCurse_;    // in microsecs
  int minCurse_;    // in microsecs
  uint16_t trim_;   // in microsecs
  bool isRevert_;

  ServoCommande(): maxCurse_(PPM_MAX_VALUE), minCurse_(PPM_MIN_VALUE), trim_(0), isRevert_(false) {}

  uint16_t getValue(uint16_t rawInputValue);
};

struct OutputChannel
{
  ServoCommande servo_;
};

class Model
{
  //Mixer mixer_[MAX_MIXER];
  OutputChannel channel_[MAX_PPM_OUTPUT_CHANNEL];

  public:

  Model();

  bool setup();

  uint16_t getOutputValue(uint8_t channel, uint16_t rawInputValue);
  void setMinValue(uint8_t channel, int value); 
  void setMaxValue(uint8_t channel, int value);
  void setTrimValue(uint8_t channel, uint16_t value); 
  void setRevertValue(uint8_t channel, bool value);
  
  void dump();
  
  void saveToEEPROM();
  void loadFromEEPROM();
};

#endif
