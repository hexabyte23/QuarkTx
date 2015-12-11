#include "Model.h"

#include <arduino.h>
#include "config.h"
#include "FlashMem.h"


//////////////////////////////////////////////////////////////

ServoCommande::ServoCommande()
{
  reset();
}

uint16_t ServoCommande::getValue(uint16_t rawInputValue)
{
  return map(rawInputValue, 0, 1023, isRevert_?maxCurse_:minCurse_, isRevert_?minCurse_:maxCurse_) + trim_;
}

void ServoCommande::reset()
{
  maxCurse_ = PPM_MAX_VALUE;
  minCurse_ = PPM_MIN_VALUE;
  trim_ = 0;
  isRevert_ = false; 
}

//////////////////////////////////////////////////////////////

Model::Model()
{
}

bool Model::setup()
{
}

uint16_t Model::getOutputValue(uint8_t channel, uint16_t rawInputValue)
{
  channel_[channel].servo_.getValue(rawInputValue);
}

void Model::setMinValue(uint8_t channel, int value)
{
  channel_[channel].servo_.minCurse_ = value;
}

void Model::setMaxValue(uint8_t channel, int value)
{
  channel_[channel].servo_.maxCurse_ = value;
}

void Model::setTrimValue(uint8_t channel, uint16_t value)
{
  channel_[channel].servo_.trim_ = value;
}

void Model::setRevertValue(uint8_t channel, bool value)
{
  channel_[channel].servo_.isRevert_ = value;
}

void Model::dump()
{
  Serial.println("Servo");
  Serial.println("# Min   Max     Trim   Rev");
  for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    Serial.print(idx);
    Serial.print(" ");
    Serial.print(channel_[idx].servo_.minCurse_, DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.maxCurse_, DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.trim_, DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.isRevert_, DISPLAY_BASE);
    Serial.println();
  }
}

void Model::reset()
{
  for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    channel_[idx].servo_.reset();
}

