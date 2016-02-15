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
   return map(analogRead(pin_)+trim_, calibrMin_, calibrMax_, ADC_MIN_VALUE, ADC_MAX_VALUE);
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
   return map(digitalRead(pin_), calibrMin_, calibrMax_, ADC_MIN_VALUE, ADC_MAX_VALUE);
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
}

void BatteryMeter::reset()
{
   calibrMin_ = 0;
   calibrMax_ = 1023;
   trim_ = 1075;

   currentHistoIdx_ = 0;
   oldestHistoIdx_ = 1;
   memset((void*)histoLevel_, 0, sizeof(histoLevel_));
   levelSum_ = 0.0;
}

uint16_t BatteryMeter::getValue() const
{
   return analogRead(pin_);
}

float BatteryMeter::getValueInVolt() const
{
   return getValue()/(float)ADC_MAX_VALUE*VREF*trim_/1000.0*(BATTERY_R1+BATTERY_R2)/BATTERY_R2;
}

float BatteryMeter::getAverageValueInVolt()
{
  //  remove oldest value
  levelSum_ -= histoLevel_[oldestHistoIdx_];

  // add a new one
  levelSum_ += histoLevel_[currentHistoIdx_] = getValueInVolt();

  // update indexes
  currentHistoIdx_++;
  currentHistoIdx_ %= BATTERY_HISTO_BUFFER_SIZE;

  oldestHistoIdx_++;
  oldestHistoIdx_  %= BATTERY_HISTO_BUFFER_SIZE;

  // compute the average
  return levelSum_/BATTERY_HISTO_BUFFER_SIZE;
}

bool BatteryMeter::checkLevelTooLow()
{
  if(updateRate_ > BATTERY_RATE_UPDATE)
  {
    updateRate_ = 0;
    if(getAverageValueInVolt() < BATTERY_1S_REF_VOLT*BATTERY_LIPO_TYPE*BATTERY_RAISE_ALARM_LEVEL)
      return true;
  }
  else
    updateRate_++;
  
  return false;
}

