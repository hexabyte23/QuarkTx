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

  void calibration(uint16_t val);
  
  public:

  Sensor();

  uint16_t getMinCalibration() {return calibrMin_;}
  uint16_t getMaxCalibration() {return calibrMax_;}
  
  virtual void dump();

  virtual void setup(uint8_t pin) = 0;
  virtual void calibrate() = 0;
  virtual uint16_t getValue() = 0;
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
  virtual uint16_t getValue();
};

class Switch : public Sensor
{
  void debounce();
  
  public:

  Switch();
  
  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue();
};

class BatteryMeter : public Sensor
{
  public:
  
  BatteryMeter() {}

  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue();
};

class Gyroscope : public Sensor
{
  public:

  Gyroscope() {}
  
  virtual void setup(uint8_t pin);
  virtual void calibrate();
  virtual uint16_t getValue();
};
#endif
