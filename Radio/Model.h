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
  uint16_t maxOutCurse_;    // in microsecs
  uint16_t minOutCurse_;    // in microsecs
  int16_t trim_;            // in microsecs
  bool isRevert_;

  ServoCommande();

  uint16_t getValue(uint16_t minCalib, uint16_t maxCalib, uint16_t rawInputValue);
  void reset();
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
  void reset();
  void dump();
  
  uint16_t getValue(uint8_t channel, uint16_t minCalib, uint16_t maxCalib, uint16_t rawInputValue);
  void setMinValue(uint8_t channel, int value); 
  void setMaxValue(uint8_t channel, int value);
  void setTrimValue(uint8_t channel, uint16_t value); 
  void setRevertValue(uint8_t channel, bool value);
};

#endif
