/*
Sensor.cpp - QuarkTx
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
   STDOUT << pin_ << "\t" << trim_ << "\t" << calibrMin_ << "\t" << calibrMax_ << "\t" << simuVal_;
}

void Sensor::reset()
{
   pin_ = -1;
   calibrMin_ = 0xFFFF;
   calibrMax_ = 0;
   trim_ = 0;
   simuVal_ = 0;
   isSimu_ = false;
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
   if(isSimu_)
      return simuVal_;
   else
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
   if(isSimu_)
      return simuVal_;
   else
      return map(digitalRead(pin_), calibrMin_, calibrMax_, ADC_MIN_VALUE, ADC_MAX_VALUE);
}


////////////////////////////////////////////////////////

void BatteryMeter::setup(uint8_t pin)
{
   pin_ = pin;
   pinMode(pin_, INPUT_PULLUP);
}

void BatteryMeter::calibrate()
{

}

uint16_t BatteryMeter::getValue() const
{
   if(isSimu_)
      return simuVal_;
   else
      return 0;
}

