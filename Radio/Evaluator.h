/*
Evaluator.h - QuarkTx
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

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <arduino.h>
#include "Sensor.h"
#include "Model.h"

class Expression
{
public:
  virtual uint16_t evaluate() const = 0;
};

class SensorExp: public Expression
{
  const Sensor *sensor_;
  
public:
  void setup(const Sensor *sensor) {sensor_ = sensor; }
  virtual uint16_t evaluate() const {sensor_->getValue();}
};

class AddExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual uint16_t evaluate() const;
};

class GreaterExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual uint16_t evaluate() const;
};

class LowerExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual uint16_t evaluate() const;
};

class IfExp : public Expression
{
  const Expression *test_, *succeed_, *fail_;

public:
  void setup(const Expression *test, const Expression *succeed, const Expression *fail);
  virtual uint16_t evaluate() const;
};

class SubExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual uint16_t evaluate() const;
};

class MixExp : public Expression
{
  const Expression *fromChannel_;
  float rate_;

public:
  void setup(const Expression *fromChannel, float rate);
  virtual uint16_t evaluate() const;
};

// Dual rate and Negative expression
class LimitExp : public Expression
{
  const Expression *expr_;
  int min_;
  int max_;

public:
  void setup(const Expression *expr, int min, int max);
  virtual uint16_t evaluate() const;
};

class Evaluator
{
  SensorExp in1_;
  SensorExp in2_;
  SensorExp in3_;
  SensorExp in4_;
  SensorExp in5_;
  SensorExp in6_;
    
  LimitExp l1_;
  LimitExp l2_;

  AddExp a1_;
    
  Sensor **sensor_;
  uint16_t *outputValue_;
  Model *currentModel_;
  Expression *expression_[MAX_PPM_OUTPUT_CHANNEL] = {&in1_, &in2_, &in3_, &in4_, &a1_};

  Expression *parseExp(const char *str);
  
public:

  Evaluator();
  void setup(Sensor **sensor, uint16_t *outputValue, Model *currentModel);
  void idle();
  bool parse(uint8_t outChannelID, const char *expStr);
  uint16_t evaluate();
};

#endif
