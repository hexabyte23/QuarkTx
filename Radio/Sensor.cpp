#include "config.h"
#include "Sensor.h"
#include "FlashMem.h"
#include <arduino.h>

Sensor::Sensor()
{
  pin_ = -1;
  calibrMin_ = 0xFFFF;
  calibrMax_ = 0;
  trim_ = 0;
}

void Sensor::calibration(uint16_t val)
{
  if(calibrMin_ > val)
    calibrMin_ = val; 
  if(calibrMax_ < val)
    calibrMax_ = val;
}


void Sensor::dump()
{
  Serial.print(pin_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(trim_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(calibrMin_, DISPLAY_BASE);
  Serial.print("\t");
  Serial.print(calibrMax_, DISPLAY_BASE);
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

uint16_t Stick::getValue()
{
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
  calibration(digitalRead(pin_));
}

uint16_t Switch::getValue()
{
  return map(digitalRead(pin_), calibrMin_, calibrMax_, ADC_MIN_VALUE, ADC_MAX_VALUE);
}


////////////////////////////////////////////////////////

void BatteryMeter::setup(uint8_t pin)
{
  
}

void BatteryMeter::calibrate()
{
  
}

uint16_t BatteryMeter::getValue()
{
  return 0;
}

