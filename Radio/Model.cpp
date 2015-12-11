#include "Model.h"

#include <arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "FlashMem.h"

uint16_t ServoCommande::getValue(uint16_t rawInputValue)
{
  return map(rawInputValue, 0, 1024, isRevert_?maxCurse_:minCurse_, isRevert_?minCurse_:maxCurse_) + trim_;
}

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
  //debug("[d] dump model\n");

  Serial.println("Servos");
  Serial.println("# Min   Max     Trim   Rev");
  for(int idx =0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    Serial.print(idx);
    Serial.print(" ");
    Serial.print(channel_[idx].servo_.minCurse_, HEX);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.maxCurse_, HEX);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.trim_, HEX);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.isRevert_, HEX);
    Serial.println();
  }
}

void Model::saveToEEPROM()
{
  EEPROM.put(0, channel_);
/*
  for(int idx =0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    EEPROM.put(idx  , channel_[idx].servo_.minCurse_);
    EEPROM.put(idx+1, channel_[idx].servo_.maxCurse_);
    EEPROM.put(idx+2, channel_[idx].servo_.isRevert_);
  }
*/
}

void Model::loadFromEEPROM()
{
  EEPROM.get(0, channel_);
/*
  int data;
  for(int idx =0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    EEPROM.get(idx, data);
    channel_[idx].servo_.minCurse_ = data;
    EEPROM.get(idx+1, data);
    channel_[idx].servo_.maxCurse_ = data;
    EEPROM.get(idx+2, data);
    channel_[idx].servo_.minCurse_ = data;
  }
*/
}

