#include <arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "Sensor.h"
#include "FlashMem.h"

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

uint16_t Sensor::putToEEPROM(uint16_t addr)
{
  EEPROM.put(addr,pin_);
  addr += sizeof(uint8_t);
  EEPROM.put(addr,calibrMin_);
  addr += sizeof(uint16_t);
  EEPROM.put(addr,calibrMax_);
  addr += sizeof(int16_t);
  EEPROM.put(addr,trim_);
  addr += sizeof(int);
  
  return addr;
}

uint16_t Sensor::getFromEEPROM(uint16_t addr)
{
  EEPROM.get(addr,pin_);
  addr += sizeof(uint8_t);
  EEPROM.get(addr,calibrMin_);
  addr += sizeof(uint16_t);
  EEPROM.get(addr,calibrMax_);
  addr += sizeof(int16_t);
  EEPROM.get(addr,trim_);
  addr += sizeof(int);
  
  return addr;
}
  
void Sensor::dump() const
{
  Serial.print(pin_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(trim_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(calibrMin_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(calibrMax_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(simuVal_, DISPLAY_BASE);
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
    return map(analogRead(pin_), calibrMin_, calibrMax_, ADC_MIN_VALUE, ADC_MAX_VALUE);
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
  //Serial.print(pin_);
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

