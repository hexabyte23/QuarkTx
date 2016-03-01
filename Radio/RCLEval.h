/*
Evaluator.h - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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

#ifndef RCLEVAL_H
#define RCLEVAL_H

#include <arduino.h>
#include "Sensor.h"
#include "Model.h"

#ifdef QUARKTX_TEENSY
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#endif

class Tx;

struct Variant
{
   enum Type {tNone, tInteger, tFloat, tBool} type_;
   uint16_t iData_;
   float fData_;
   bool bData_;

   Variant() : type_(tNone) {}
   Variant(uint16_t iData) : type_(tInteger), iData_(iData) {}
   Variant(float fData) : type_(tFloat), fData_(fData) {}
   Variant(bool bData) : type_(tBool), bData_(bData) {}

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

   // About evaluate() base function. Return a plenty Variant object instead 
   // of a pointer add lot of stack work for the uP. This code is not very efficient 
   // but quite simple. Must be improved in futur...
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

   static const Tx *tx_;
};

class ConstantExp: public Expression
{
   Variant data_;

public:
   ConstantExp() {}
   void setup(uint16_t data) {data_ = data; }
   void setup(float data) {data_ = data; }
   void setup(bool data) {data_ = data; }
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

class RCLEval
{    
   Sensor **sensorRef_;
   volatile uint16_t *outputValueRef_;
   const Model *currentModel_;
   Expression *expression_[MAX_PPM_OUTPUT_CHANNEL];
   SensorInputExp *inputTab_[MAX_INPUT_CHANNEL];
#ifdef QUARKTX_TEENSY
   lua_State *luaState_;
#endif

   Expression *parseExp(char *&in);
   Expression *parseOperand(char *&in);
   Expression *parseNumeric(char *&in);

public:

   RCLEval();
   void setup(Sensor **sensorRef, volatile uint16_t *outputValueRef, const Model *currentModel, const Tx *tx);
   bool setupRCL(uint8_t chan, const char *expStr);
   void changeModel(const Model *currentModel) {currentModel_ = currentModel;}
   void saveToEEPROM(uint16_t &addr) const;
   void loadFromEEPROM(uint16_t &addr);
   void clearRCL(uint8_t chan);
   void loop();
   void dump(uint8_t outChannelID) const;
   void reset();
};

#endif
