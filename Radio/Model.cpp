/*
Model.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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

#include <arduino.h>
#include <EEPROM.h>
#include "Model.h"
#include "SerialLink.h"

//////////////////////////////////////////////////////////////

ServoCommand::ServoCommand()
{
   reset();
}

uint16_t ServoCommand::getValue(uint16_t rawInputValue)
{
   uint16_t ret = map(rawInputValue, ADC_MIN_VALUE, ADC_MAX_VALUE, isRevert_?maxOutCurse_:minOutCurse_, isRevert_?minOutCurse_:maxOutCurse_) + neutral_;

   // apply PPM bound limitations
   if(ret > PPM_MAX_VALUE)
      ret = PPM_MAX_VALUE;
   if(ret < PPM_MIN_VALUE)
      ret = PPM_MIN_VALUE;

   return ret;
}

void ServoCommand::reset()
{
   maxOutCurse_ = PPM_MAX_VALUE;
   minOutCurse_ = PPM_MIN_VALUE;
   neutral_ = 0;
   isRevert_ = false;
}

uint16_t ServoCommand::saveToEEPROM(uint16_t addr) const
{
   EEPROM.put(addr, maxOutCurse_);
   addr += sizeof(uint16_t);
   EEPROM.put(addr, minOutCurse_);
   addr += sizeof(uint16_t);
   EEPROM.put(addr, neutral_);
   addr += sizeof(int16_t);
   EEPROM.put(addr, isRevert_);
   addr += sizeof(bool);

   return addr;
}

uint16_t ServoCommand::loadFromEEPROM(uint16_t addr)
{
   EEPROM.get(addr, maxOutCurse_);
   addr += sizeof(uint16_t);
   EEPROM.get(addr, minOutCurse_);
   addr += sizeof(uint16_t);
   EEPROM.get(addr, neutral_);
   addr += sizeof(int16_t);
   EEPROM.get(addr, isRevert_);
   addr += sizeof(bool);

   return addr;
}

//////////////////////////////////////////////////////////////

uint16_t OutputChannel::saveToEEPROM(uint16_t addr) const
{
   return servo_.saveToEEPROM(addr);
}

uint16_t OutputChannel::loadFromEEPROM(uint16_t addr)
{
   return servo_.loadFromEEPROM(addr);
}

//////////////////////////////////////////////////////////////

Model::Model()
{
}

bool Model::setup()
{
   return true;
}

uint16_t Model::getValue(uint8_t channel, uint16_t rawInputValue)
{
   return channel_[channel].servo_.getValue(rawInputValue);
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
   STDOUT << F("Servo\n# Min   Max   Neutral   Rev") << endl;

   for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      STDOUT << idx << " " << channel_[idx].servo_.minOutCurse_ << "\t" <<
                channel_[idx].servo_.maxOutCurse_ << "\t" <<
                channel_[idx].servo_.neutral_ << "\t" <<
                channel_[idx].servo_.isRevert_ << endl;
   }
}

void Model::reset()
{
   for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      channel_[idx].servo_.reset();
}

uint16_t Model::saveToEEPROM(uint16_t addr) const
{
   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      addr = channel_[idx].saveToEEPROM(addr);

   return addr;
}

uint16_t Model::loadFromEEPROM(uint16_t addr)
{
   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      addr = channel_[idx].loadFromEEPROM(addr);

   return addr;
}

