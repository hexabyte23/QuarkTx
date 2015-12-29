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

struct Variant
{
  enum Type {tNone, tInteger, tFloat, tBool} type_;
  uint16_t iData_;
  float fData_;
  bool bData_;
  
  Variant(uint16_t iData) : iData_(iData), type_(tInteger) {}
  Variant(float fData) : fData_(fData), type_(tFloat) {}
  Variant(bool bData) : bData_(bData), type_(tBool) {}

  friend Variant operator < (const Variant &l, const Variant &r);
  friend Variant operator > (const Variant &l, const Variant &r);
  friend Variant operator + (const Variant &l, const Variant &r);
  friend Variant operator - (const Variant &l, const Variant &r);
  friend Variant operator * (const Variant &l, const Variant &r);
  friend Variant operator == (const Variant &l, const Variant &r);
};

class Expression
{
public:
  virtual Variant evaluate() const = 0;
};

class SensorInputExp: public Expression
{
  const Sensor *sensor_;
  
public:
  void setup(const Sensor *sensor) {sensor_ = sensor; }
  virtual Variant evaluate() const {Variant v (sensor_->getValue()); return v;}
};

class IntegerExp: public Expression
{
  Variant data_;
  
public:
  IntegerExp() : data_((uint16_t)0) {}
  void setup(uint16_t data) {data_.iData_ = data; }
  virtual Variant evaluate() const {return data_;}
};

class FloatExp: public Expression
{
  Variant data_;
  
public:
  FloatExp() : data_((float)0.0) {}
  void setup(float data) {data_.fData_ = data; }
  virtual Variant evaluate() const {return data_;}
};

class BoolExp: public Expression
{
  Variant data_;
  
public:
  BoolExp() : data_(false) {}
  void setup(bool data) {data_.bData_ = data; }
  virtual Variant evaluate() const {return data_;}
};

class AddExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
};

class GreaterExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
};

class LowerExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
};

class IfExp : public Expression
{
  const Expression *test_, *succeed_, *fail_;

public:
  void setup(const Expression *test, const Expression *succeed, const Expression *fail);
  virtual Variant evaluate() const;
};

class SubExp : public Expression
{
  const Expression *left_, *right_;

public:
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
};

class MulExp : public Expression
{
  const Expression *exp1_, *exp2_;

public:
  void setup(const Expression *exp1, const Expression *exp2);
  virtual Variant evaluate() const;
};

// Dual rate and Negative expression
class LimitExp : public Expression
{
  const Expression *expr_, *min_, *max_;

public:
  void setup(const Expression *expr, const Expression *min, const Expression *max);
  virtual Variant evaluate() const;
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
