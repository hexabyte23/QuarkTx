#include "Model.h"

#include <arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "FlashMem.h"


//////////////////////////////////////////////////////////////

ServoCommand::ServoCommand()
{
  reset();
}

uint16_t ServoCommand::getValue(uint16_t rawInputValue)
{
  return map(rawInputValue, ADC_MIN_VALUE, ADC_MAX_VALUE, isRevert_?maxOutCurse_:minOutCurse_, isRevert_?minOutCurse_:maxOutCurse_) + neutral_;
}

void ServoCommand::reset()
{
  maxOutCurse_ = PPM_MAX_VALUE;
  minOutCurse_ = PPM_MIN_VALUE;
  neutral_ = 0;
  isRevert_ = false; 
}

uint16_t ServoCommand::putToEEPROM(uint16_t addr)
{
  EEPROM.put(addr,maxOutCurse_);
  addr += sizeof(uint16_t);
  EEPROM.put(addr,minOutCurse_);
  addr += sizeof(uint16_t);
  EEPROM.put(addr,neutral_);
  addr += sizeof(int16_t);
  EEPROM.put(addr,isRevert_);
  addr += sizeof(bool);

  return addr;
}

uint16_t ServoCommand::getFromEEPROM(uint16_t addr)
{
//  Serial.print(addr);
  EEPROM.get(addr,maxOutCurse_);
  addr += sizeof(uint16_t);
  EEPROM.get(addr,minOutCurse_);
  addr += sizeof(uint16_t);
  EEPROM.get(addr,neutral_);
  addr += sizeof(int16_t);
  EEPROM.get(addr,isRevert_);
  addr += sizeof(bool);

//  Serial.print(" ");
//  Serial.println(addr);
  return addr;
}

//////////////////////////////////////////////////////////////

uint16_t OutputChannel::putToEEPROM(uint16_t addr)
{
  return servo_.putToEEPROM(addr);
}

uint16_t OutputChannel::getFromEEPROM(uint16_t addr)
{
  return servo_.getFromEEPROM(addr);
}

//////////////////////////////////////////////////////////////

Model::Model()
{
}

bool Model::setup()
{
}

uint16_t Model::getValue(uint8_t channel, uint16_t rawInputValue)
{
  channel_[channel].servo_.getValue(rawInputValue);
}

void Model::setMinValue(uint8_t channel, int value)
{
  channel_[channel].servo_.minOutCurse_ = value;
}

void Model::setMaxValue(uint8_t channel, int value)
{
  channel_[channel].servo_.maxOutCurse_ = value;
}

void Model::setNeutralValue(uint8_t channel, uint16_t value)
{
  channel_[channel].servo_.neutral_ = value;
}

void Model::setRevertValue(uint8_t channel, bool value)
{
  channel_[channel].servo_.isRevert_ = value;
}

void Model::dump()
{
  info(INFO_DUMP_SERVO_HEADER);
  for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    Serial.print(idx);
    Serial.print(" ");
    Serial.print(channel_[idx].servo_.minOutCurse_, DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.maxOutCurse_, DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(channel_[idx].servo_.neutral_, DISPLAY_BASE);
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

uint16_t Model::putToEEPROM(uint16_t addr)
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    addr = channel_[idx].putToEEPROM(addr);
  }

  return addr;
}

uint16_t Model::getFromEEPROM(uint16_t addr)
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    addr = channel_[idx].getFromEEPROM(addr);
  }

  return addr;
}
