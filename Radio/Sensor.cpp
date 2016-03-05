/*
Sensor.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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
#include "SerialLink.h"
#include "Sensor.h"


Sensor::Sensor()
{
   reset();
}

void Sensor::calibration(uint16_t val)
{
   if(calibrMin_ > val)
      calibrMin_ = val;
   if(calibrMax_ < val)
      calibrMax_ = val;
}

void Sensor::saveToEEPROM(uint16_t &addr) const
{
   EEPROM.put(addr,pin_);
   addr += sizeof(uint8_t);
   EEPROM.put(addr,calibrMin_);
   addr += sizeof(uint16_t);
   EEPROM.put(addr,calibrMax_);
   addr += sizeof(int16_t);
   EEPROM.put(addr,trim_);
   addr += sizeof(int);
}

void Sensor::loadFromEEPROM(uint16_t &addr)
{
   EEPROM.get(addr,pin_);
   addr += sizeof(uint8_t);
   EEPROM.get(addr,calibrMin_);
   addr += sizeof(uint16_t);
   EEPROM.get(addr,calibrMax_);
   addr += sizeof(int16_t);
   EEPROM.get(addr,trim_);
   addr += sizeof(int);
}

void Sensor::dump() const
{
#ifndef QUARKTX_TEST
   STDOUT << pin_ << "\t" << trim_ << "\t" << calibrMin_ << "\t" << calibrMax_;
#endif
}

void Sensor::reset()
{
   pin_ = -1;
   calibrMin_ = 0xFFFF;
   calibrMax_ = 0;
   trim_ = 0;
}

////////////////////////////////////////////////////////

Stick::Stick()
{
}

void Stick::setup(uint8_t pin)
{
   pin_ = pin;
   pinMode(pin_, INPUT);
}

void Stick::calibrate()
{
   calibration(analogRead(pin_));
}

uint16_t Stick::getValue() const
{
   return map(analogRead(pin_)+trim_, calibrMin_, calibrMax_, QUARKTX_ADC_MIN_VALUE, QUARKTX_ADC_MAX_VALUE);
}

////////////////////////////////////////////////////////

Switch::Switch()
{
}

void Switch::debounce()
{

}

void Switch::setup(uint8_t pin)
{
   pin_ = pin;
   pinMode(pin_, INPUT_PULLUP);
}

void Switch::calibrate()
{
   calibration(digitalRead(pin_));
}

uint16_t Switch::getValue() const
{
   return map(digitalRead(pin_), calibrMin_, calibrMax_, QUARKTX_ADC_MIN_VALUE, QUARKTX_ADC_MAX_VALUE);
}

////////////////////////////////////////////////////////

BatteryMeter::BatteryMeter()
{
  reset();
}
  
void BatteryMeter::setup(uint8_t pin)
{
   pin_ = pin;
   pinMode(pin_, INPUT);

   levelSum_ = QUARKTX_BATTERY_ALARM_LEVEL*BATTERY_HISTO_BUFFER_SIZE;
}

void BatteryMeter::reset()
{
   calibrMin_ = 0;
   calibrMax_ = QUARKTX_ADC_MAX_VALUE;
   trim_ = 0;
   lastValue_ = 0.0;

   currentHistoIdx_ = 0;
   oldestHistoIdx_ = 1;
   memset((void*)histoLevel_, 0, sizeof(histoLevel_));
   levelSum_ = 0.0;
}

uint16_t BatteryMeter::getValue() const
{
   return map(analogRead(pin_)+trim_, calibrMin_, calibrMax_, QUARKTX_ADC_MIN_VALUE, QUARKTX_ADC_MAX_VALUE);
}

float BatteryMeter::getValueInVolt() const
{
   return getValue()/(float)QUARKTX_ADC_MAX_VALUE*QUARKTX_VREF*(QUARKTX_BATTERY_R1 + QUARKTX_BATTERY_R2)/QUARKTX_BATTERY_R2;
}

float BatteryMeter::computeAverageValueInVolt()
{
  // Remove oldest value
  levelSum_ -= histoLevel_[oldestHistoIdx_];

  // Add a new one
  levelSum_ += histoLevel_[currentHistoIdx_] = getValueInVolt();

  // Update indexes
  currentHistoIdx_++;
  currentHistoIdx_ %= BATTERY_HISTO_BUFFER_SIZE;

  oldestHistoIdx_++;
  oldestHistoIdx_  %= BATTERY_HISTO_BUFFER_SIZE;

  // Compute the average
  return levelSum_/BATTERY_HISTO_BUFFER_SIZE;
}

bool BatteryMeter::isLevelTooLow()
{  
  lastValue_ = computeAverageValueInVolt();
    
  if(lastValue_ < QUARKTX_BATTERY_ALARM_LEVEL)
  {
     STDOUT << "e-btl " << lastValue_ << endl;

     return true;
  }

  return false;
}

