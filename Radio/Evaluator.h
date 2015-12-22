#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <arduino.h>
#include "Sensor.h"
#include "Model.h"

class Expression
{
public:
  virtual uint16_t evaluate() = 0;
};

class SensorExp: public Expression
{
  Sensor *sensor_;
  
public:
  void setup(Sensor *sensor) {sensor_ = sensor; }
  virtual uint16_t evaluate() {sensor_->getValue();}
};

class AddExp : public Expression
{
  Expression *left_, *right_;

public:
  void setup(Expression *left, Expression *right);
  virtual uint16_t evaluate();
};

class SubExp : public Expression
{
  Expression *left_, *right_;

public:
  void setup(Expression *left, Expression *right);
  virtual uint16_t evaluate();
};

class MixExp : public Expression
{
  Expression *fromChannel_;
  float rate_;

public:
  void setup(Expression *fromChannel, float rate);
  virtual uint16_t evaluate();
};

class LimitExp : public Expression
{
  Expression *expr_;
  int min_;
  int max_;

public:
  void setup(Expression *expr, int min, int max);
  virtual uint16_t evaluate();
};

class Evaluator
{
  SensorExp in1_;
  SensorExp in2_;
  SensorExp in3_;
  SensorExp in4_;
  SensorExp in5_;
  SensorExp in6_;
  SensorExp in7_;
    
  LimitExp l1_;
  LimitExp l2_;

  AddExp a1_;
    
  Sensor **sensor_;
  uint16_t *outputValue_;
  Model *currentModel_;

  Expression *expression_[MAX_PPM_OUTPUT_CHANNEL] = {&in1_, &in2_, &in3_, &in4_, &a1_};
  
public:

  Evaluator();
  void setup(Sensor **sensor, uint16_t *outputValue, Model *currentModel);
  void idle();
  bool parse(uint8_t outChannelID, const char *string);
  uint16_t evaluate();
};

#endif
