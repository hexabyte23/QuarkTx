/*
Evaluator.h - QuarkTx
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

  uint16_t convert2Int() const;
  float convert2Float() const;
  bool convert2Bool() const;

  friend Variant operator < (const Variant &l, const Variant &r);
  friend Variant operator > (const Variant &l, const Variant &r);
  friend Variant operator + (const Variant &l, const Variant &r);
  friend Variant operator - (const Variant &l, const Variant &r);
  friend Variant operator * (const Variant &l, const Variant &r);
  friend Variant operator / (const Variant &l, const Variant &r);
  friend Variant operator == (const Variant &l, const Variant &r);
  friend Variant operator != (const Variant &l, const Variant &r);
  friend Print & operator << (Print &obj, const Variant &arg);
};

class Expression
{
public:
  virtual ~Expression() {}
  virtual Variant evaluate() const = 0;
  virtual void dump() const = 0;
  virtual bool couldBeDeleted() const {return true;}
};

class SubExpression : public Expression
{
  Expression *expr_;
  
public:
  virtual ~SubExpression();
  void setup(Expression *expr);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class SensorInputExp: public Expression
{
  const Sensor *sensor_;
  
public:
  void setup(const Sensor *sensor) {sensor_ = sensor; }
  virtual Variant evaluate() const {Variant v (sensor_->getValue()); return v;}
  virtual void dump() const;
  virtual bool couldBeDeleted() const {return false;}
};

class IntegerExp: public Expression
{
  Variant data_;
  
public:
  IntegerExp() : data_((uint16_t)0) {}
  void setup(uint16_t data) {data_.iData_ = data; }
  virtual Variant evaluate() const {return data_;}
  virtual void dump() const;
};

class FloatExp: public Expression
{
  Variant data_;
  
public:
  FloatExp() : data_((float)0.0) {}
  void setup(float data) {data_.fData_ = data; }
  virtual Variant evaluate() const {return data_;}
  virtual void dump() const;
};

class BoolExp: public Expression
{
  Variant data_;
  
public:
  BoolExp() : data_(false) {}
  void setup(bool data) {data_.bData_ = data; }
  virtual Variant evaluate() const {return data_;}
  virtual void dump() const;
};

class AddExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~AddExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class SubExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~SubExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class MulExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~MulExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class DivExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~DivExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class EqualExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~EqualExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class GreaterThanExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~GreaterThanExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class LowerThanExp : public Expression
{
  const Expression *left_, *right_;

public:
  virtual ~LowerThanExp();
  void setup(const Expression *left, const Expression *right);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class IfExp : public Expression
{
  const Expression *condition_, *succeed_, *fail_;

public:
  virtual ~IfExp();
  void setup(const Expression *condition_, const Expression *succeed, const Expression *fail);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

// Dual rate and Negative expression
class LimitExp : public Expression
{
  const Expression *data_, *min_, *max_;

public:
  virtual ~LimitExp();
  void setup(const Expression *data, const Expression *min, const Expression *max);
  virtual Variant evaluate() const;
  virtual void dump() const;
};

class Evaluator
{    
  Sensor **sensorRef_;
  uint16_t *outputValueRef_;
  Model *currentModel_;
  Expression *expression_[MAX_PPM_OUTPUT_CHANNEL];
  SensorInputExp *inputTab[MAX_INPUT_CHANNEL];

  Expression *parseExp(char *&in);
  Expression *parseOperand(char *&in);
  Expression *parseNumeric(char *&in);
  
public:

  Evaluator();
  void setup(Sensor **sensorRef, uint16_t *outputValueRef, Model *currentModel);
  bool setupRCL(uint8_t chan, const char *expStr);
  bool saveToEEPROM(uint8_t chan, const char *expStr);
  bool loadFromEEPROM();
  void clearRCL(uint8_t chan);
  void idle();
  uint16_t evaluate();
  void dump(uint8_t outChannelID);
  void reset();
};

#endif
