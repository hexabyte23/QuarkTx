/*
Sensor.h - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

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

#ifndef SENSOR_H
#define SENSOR_H

#include <inttypes.h>

class Sensor
{
  protected:
  
  uint8_t pin_;
  uint16_t calibrMin_;
  uint16_t calibrMax_;
  int trim_;
  uint16_t simuVal_;
  bool isSimu_;

  void calibration(uint16_t val);
  
  public:

  Sensor();

  uint16_t getMinCalibration() const {return calibrMin_;}
  uint16_t getMaxCalibration() const {return calibrMax_;}
  void setSimulateValue(uint16_t simuVal) {simuVal_ = simuVal;}
  void setSimulation(bool isSimu) {isSimu_ = isSimu;}
  
  virtual void dump() const;

  virtual void setup(uint8_t pin) = 0;
  virtual void calibrate() = 0;
  virtual uint16_t getValue() const = 0;
  virtual uint16_t putToEEPROM(uint16_t addr);
  virtual uint16_t getFromEEPROM(uint16_t addr);
  virtual void reset();
};

class Stick : public Sensor
{
  public:

  Stick();

  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue() const;
};

class Switch : public Sensor
{
  void debounce();
  
  public:

  Switch();
  
  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue() const;
};

class BatteryMeter : public Sensor
{
  public:
  
  BatteryMeter() {}

  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue() const;
};

class Gyroscope : public Sensor
{
  public:

  Gyroscope() {}
  
  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue() const;
};

#endif