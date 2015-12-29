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

class SensorInputExp: public Expression
{
  const Sensor *sensor_;
  
public:
  void setup(const Sensor *sensor) {sensor_ = sensor; }
  virtual uint16_t evaluate() const {return sensor_->getValue();}
};

class ConstantInputExp: public Expression
{
  uint16_t data_;
  
public:
  void setup(uint16_t data) {data_ = data; }
  virtual uint16_t evaluate() const {return data_;}
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
  const Expression *expr_, *min_, *max_;

public:
  void setup(const Expression *expr, const Expression *min, const Expression *max);
  virtual uint16_t evaluate() const;
};

class Evaluator
{    
  Sensor **sensorRef_;
  uint16_t *outputValueRef_;
  Model *currentModel_;
  Expression *expression_[MAX_PPM_OUTPUT_CHANNEL];
  SensorInputExp *inputTab[MAX_INPUT_CHANNEL];

  Expression *parseExp(char *&str);
  
public:

  Evaluator();
  void setup(Sensor **sensorRef, uint16_t *outputValueRef, Model *currentModel);
  bool setupOutputChannel(uint8_t outChannelID, const char *expStr);
  void idle();
  uint16_t evaluate();
};

#endif
