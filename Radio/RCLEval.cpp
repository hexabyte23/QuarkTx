/*
Evaluator.cpp - QuarkTx
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

#include "Tx.h"
#include "RCLEval.h"
#include "SerialLink.h"
#include <EEPROM.h>

//////////////////////////////////////////////////////////////////////////
// 
// Some Misc. funtions
//
//////////////////////////////////////////////////////////////////////////

static char g_dump[MAX_SERIAL_INPUT_BUFFER];
static uint8_t g_idxDump = 0;
static uint8_t g_tab4Dump = 0;
static bool g_hierachyDump = true;

char addChar(char c)
{
   if(g_idxDump < MAX_SERIAL_INPUT_BUFFER)
      g_dump[g_idxDump++] = c;

   return c;
}

uint16_t addChar(uint16_t i)
{
   char buf[10];
   itoa(i, buf, 10);
   for(uint8_t i=0; i < 10; i++)
   {
      if(buf[i] == 0)
         break;

      g_dump[g_idxDump] = buf[i];
      g_idxDump++;
   }
   return i;
}

float addChar(float f)
{
   char buf[10];
   dtostrf(f, 4, 3, buf);
   for(uint8_t i=0; i < 10; i++)
   {
      if(buf[i] == 0)
         break;

      g_dump[g_idxDump] = buf[i];
      g_idxDump++;
   }
   return f;
}

bool addChar(bool b)
{
   b?addChar('T'):addChar('F');
   return b;
}

void tabDump()
{
   for(uint8_t i = 0; i < g_tab4Dump; i++)
      STDOUT << " ";
}

void enterDump()
{
   if(!g_hierachyDump)
      return;

   STDOUT << endl;
   tabDump();
   //STDOUT << "{" << endl;
   g_tab4Dump++;
   tabDump();
}

void leaveDump()
{
   if(!g_hierachyDump)
      return;

   g_tab4Dump--;
   STDOUT << endl;
   tabDump();
   //STDOUT << "}";
   tabDump();
}

//
// a float version of map()
//

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//////////////////////////////////////////////////////////////////////////

uint16_t Variant::convert2Int() const
{
   switch(type_)
   {
      case Variant::tInteger: return iData_;
      case Variant::tFloat: return (uint16_t)fData_;
      case Variant::tBool: return (uint16_t)bData_;
   }

   return 0;
}

float Variant::convert2Float() const
{
   switch(type_)
   {
      case Variant::tInteger: return (float)iData_;
      case Variant::tFloat: return fData_;
      case Variant::tBool: return (float)bData_;
   }

   return 0;
}

bool Variant::convert2Bool() const
{
   switch(type_)
   {
      case Variant::tInteger: return iData_==1;
      case Variant::tFloat: return fData_==1.0;
      case Variant::tBool: return bData_;
   }

   return false;
}

Variant operator < (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.iData_ < r.iData_;
            case Variant::tFloat: return l.iData_ < r.fData_;
            case Variant::tBool: return l.iData_ < r.bData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ < r.iData_;
            case Variant::tFloat: return l.fData_ < r.fData_;
            case Variant::tBool: return l.fData_ < r.bData_;
         }
      }
         break;
      case Variant::tBool:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.bData_ < r.iData_;
            case Variant::tFloat: return l.bData_ < r.fData_;
            case Variant::tBool: return l.bData_ < r.bData_;
         }
      }
         break;
   }

   return false;
}

Variant operator > (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.iData_ > r.iData_;
            case Variant::tFloat: return l.iData_ > r.fData_;
            case Variant::tBool: return l.iData_ > r.bData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ > r.iData_;
            case Variant::tFloat: return l.fData_ > r.fData_;
            case Variant::tBool: return l.fData_ > r.bData_;
         }
      }
         break;
      case Variant::tBool:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.bData_ > r.iData_;
            case Variant::tFloat: return l.bData_ > r.fData_;
            case Variant::tBool: return l.bData_ > r.bData_;
         }
      }
         break;
   }

   return false;
}

Variant operator + (const Variant &l, const Variant &r)
{  
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return (uint16_t)(l.iData_ + r.iData_);
            case Variant::tFloat: return l.iData_ + r.fData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ + r.iData_;
            case Variant::tFloat: return l.fData_ + r.fData_;
         }
      }
         break;
   }

   return (uint16_t)0;
}

Variant operator - (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return (uint16_t)(l.iData_ - r.iData_);
            case Variant::tFloat: return l.iData_ - r.fData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ - r.iData_;
            case Variant::tFloat: return l.fData_ - r.fData_;
         }
      }
         break;
   }

   return (uint16_t)0;
}

Variant operator * (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return (uint16_t)(l.iData_ * r.iData_);
            case Variant::tFloat: return l.iData_ * r.fData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ * r.iData_;
            case Variant::tFloat: return l.fData_ * r.fData_;
         }
      }
         break;
   }

   return (uint16_t)0;
}

Variant operator / (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return (uint16_t)(l.iData_ / r.iData_);
            case Variant::tFloat: return l.iData_ / r.fData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ / r.iData_;
            case Variant::tFloat: return l.fData_ / r.fData_;
         }
      }
         break;
   }

   return (uint16_t)0;
}

Variant operator == (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.iData_ == r.iData_;
            case Variant::tFloat: return l.iData_ == r.fData_;

         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ == r.iData_;
            case Variant::tFloat: return l.fData_ == r.fData_;
         }
      }
         break;
      case Variant::tBool:
      {
         switch (r.type_)
         {
            case Variant::tBool: return l.bData_ == r.bData_;
         }
      }
         break;
   }

   return false;
}

Variant operator != (const Variant &l, const Variant &r)
{
   switch(l.type_)
   {
      case Variant::tInteger:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.iData_ != r.iData_;
            case Variant::tFloat: return l.iData_ != r.fData_;
         }
      }
         break;
      case Variant::tFloat:
      {
         switch (r.type_)
         {
            case Variant::tInteger: return l.fData_ != r.iData_;
            case Variant::tFloat: return l.fData_ != r.fData_;
         }
      }
         break;
      case Variant::tBool:
      {
         switch (r.type_)
         {
            case Variant::tBool: return l.bData_ != r.bData_;
         }
      }
         break;
   }

   return false;
}

Print & operator << (Print &obj, const Variant &arg)
{ 
   switch(arg.type_)
   {
      case Variant::tInteger: obj.print(arg.iData_); return obj;
      case Variant::tFloat: obj.print(arg.fData_); return obj;
      case Variant::tBool: obj.print(arg.bData_); return obj;
   }

   return obj;
}

//////////////////////////////////////////////////////////////////////////

SubExpression::~SubExpression()
{
   if(expr_->couldBeDeleted())
      delete expr_;
}

void SubExpression::setup(Expression *expr)
{
   expr_ = expr;
}

Variant SubExpression::evaluate() const
{
   return expr_->evaluate();
}

void SubExpression::dump() const
{
   enterDump();
   STDOUT << addChar('(');
   expr_->dump();
   STDOUT << addChar(')');
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

void ConstantExp::dump() const
{
   enterDump();
   switch(data_.type_)
   {
      case Variant::tNone: STDOUT << F("e-nt");break; // error tNone type
      case Variant::tInteger: STDOUT << addChar(data_.iData_);break;
      case Variant::tFloat: STDOUT << addChar(data_.fData_);break;
      case Variant::tBool: STDOUT << addChar(data_.bData_);break;
   }
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////
extern Tx tx;     // Horrible, must be cleaned

void SensorInputExp::dump() const
{
   enterDump();
   
   addChar('i');
   addChar((uint16_t)tx.getSensorIndex(sensor_->getPin()));
   
   STDOUT << 'i' << tx.getSensorIndex(sensor_->getPin());
   leaveDump();
}
/*
void SensorInputExp::saveToEEPROM(uint16_t &addr) const
{
  EEPROM.put(addr, 'i');
  addr++;
}

void SensorInputExp::loadFromEEPROM(uint16_t &addr)
{
  char c;
  EEPROM.get(addr, c);
  addr++;
  EEPROM.get(addr, c);
  addr++;
}
*/
//////////////////////////////////////////////////////////////////////////

IfExp::~IfExp() 
{
   if(condition_->couldBeDeleted())
      delete condition_;

   if(succeed_->couldBeDeleted())
      delete succeed_;

   if(fail_->couldBeDeleted())
      delete fail_;
}

void IfExp::setup(const Expression *condition, const Expression *succeed, const Expression *fail)
{
   condition_ = condition;
   succeed_ = succeed;
   fail_ = fail;
}

Variant IfExp::evaluate() const
{
   return (condition_->evaluate().convert2Bool())?succeed_->evaluate():fail_->evaluate();
}

void IfExp::dump() const
{
   enterDump();
   condition_->dump();
   STDOUT << addChar('?');
   succeed_->dump();
   STDOUT << addChar(':');
   fail_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

AddExp::~AddExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void AddExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant AddExp::evaluate() const
{
   return left_->evaluate() + right_->evaluate();
}

void AddExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('+');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

SubExp::~SubExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void SubExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant SubExp::evaluate() const
{
   return left_->evaluate() - right_->evaluate();
}

void SubExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('-');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

MulExp::~MulExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void MulExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant MulExp::evaluate() const
{
   return left_->evaluate() * right_->evaluate();
}

void MulExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('*');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

DivExp::~DivExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void DivExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant DivExp::evaluate() const
{
   return left_->evaluate() / right_->evaluate();
}

void DivExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('/');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

EqualExp::~EqualExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void EqualExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant EqualExp::evaluate() const
{
   return left_->evaluate() == right_->evaluate();
}

void EqualExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('=');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

LowerThanExp::~LowerThanExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void LowerThanExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant LowerThanExp::evaluate() const
{
   return left_->evaluate() < right_->evaluate();
}

void LowerThanExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('<');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

GreaterThanExp::~GreaterThanExp() 
{
   if(left_->couldBeDeleted())
      delete left_;

   if(right_->couldBeDeleted())
      delete right_;
}

void GreaterThanExp::setup(const Expression *left, const Expression *right)
{
   left_ = left;
   right_ = right;
}

Variant GreaterThanExp::evaluate() const
{
   return left_->evaluate() > right_->evaluate();
}

void GreaterThanExp::dump() const
{
   enterDump();
   left_->dump();
   STDOUT << addChar('>');
   right_->dump();
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

LimitExp::~LimitExp() 
{
   if(data_->couldBeDeleted())
      delete data_;

   if(min_->couldBeDeleted())
      delete min_;

   if(max_->couldBeDeleted())
      delete max_;
}

void LimitExp::setup(const Expression *data, const Expression *min, const Expression *max)
{
   data_ = data;
   min_ = min;
   max_ = max;
}

Variant LimitExp::evaluate() const
{
   Variant ret((uint16_t)fmap(data_->evaluate().convert2Float(), ADC_MIN_VALUE, ADC_MAX_VALUE, min_->evaluate().convert2Float(), max_->evaluate().convert2Float()));
   return ret;
}

void LimitExp::dump() const
{
   enterDump();
   data_->dump();
   STDOUT << addChar('[');
   min_->dump();
   STDOUT << addChar(';');
   max_->dump();
   STDOUT << addChar(']');
   leaveDump();
}

//////////////////////////////////////////////////////////////////////////

RCLEval::RCLEval()
{
   memset((void*)expression_, 0, sizeof(expression_));
}

void RCLEval::setup(Sensor **sensorRef, volatile uint16_t *outputValueRef, Model *currentModel)
{
   sensorRef_ = sensorRef;
   outputValueRef_ = outputValueRef;
   currentModel_ = currentModel;

   for(int8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
   {
      inputTab_[idx] = new SensorInputExp;
      inputTab_[idx]->setup(sensorRef_[idx]);
   }
}

/*
 * RCL Grammar in EBNF
 *
 * digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 * integer = digit , { digit };
 * float = digit | "." , { digit | "." };
 *
 */

Expression *RCLEval::parseNumeric(char *&in)
{
   uint8_t type = 0;                 // 0 = integer, 1 = float
   char buf[] = {0,0,0,0,0,0,0};     // 6 digits + \0
   char *p = &buf[0];
   uint8_t len = 0;

   while(1)
   {
      if(*in == 0) break;             // end of text
      if(len == sizeof(buf)-1) break; // end of buf
      if(*in == '.')                  // float ?
         type = 1;                    // this is a float
      else
      {
         if(!isdigit(*in))            // digit ?
            break;                    // no more a digit
      }

      *p = *in;                       // copy car into buf

      p++, in++, len++;
   }

   if((len == 0) || (buf[0] == 0))
      return NULL;

   ConstantExp *expr = new ConstantExp;

   if(type == 0)
      expr->setup((uint16_t)atoi(buf));
   else
      expr->setup((float)atof(buf));

   return expr;
}

Expression *RCLEval::parseOperand(char *&in)
{  
   switch(*in)
   {
      case '(':
      {
         in++; // (
         Expression *expr = parseExp(in);
         if(expr == NULL)
            return NULL;

         in++; // )

         SubExpression *sexpr = new SubExpression;
         sexpr->setup(expr);

         if(*in == '[')
         {
            in++; // [

            Expression *_min = parseOperand(in);
            if(_min == NULL)
               return NULL;
            in++; // ;
            Expression *_max = parseOperand(in);
            if(_max == NULL)
               return NULL;
            in++; // ]

            LimitExp *expr = new LimitExp;
            expr->setup(sexpr, _min, _max );

            in++; // ]

            return expr;
         }

         return sexpr;
      }
         break;
      case 'i': // sensor input
      {
         in++; // i

         if(!isdigit(*in))
         {
            STDOUT << F("e-bp ") << *in << F(" digit expected") << endl;  // Bad parameter
            return NULL;
         }
         uint8_t c = in[0] - '0';
         if(c >= MAX_INPUT_CHANNEL)
         {
            STDOUT << F("e-bp ") << c << " " << MAX_INPUT_CHANNEL-1 << endl;  // Bad parameter
            return NULL;
         }

         in++; // x

         // check optionnal limit operator
         if(*in != '[')
            return inputTab_[c];

         in++; // [

         Expression *_min = parseOperand(in);
         if(_min == NULL)
            return NULL;
         in++; // ;
         Expression *_max = parseOperand(in);
         if(_max == NULL)
            return NULL;
         in++; // ]

         LimitExp *expr = new LimitExp;
         expr->setup(inputTab_[c], _min, _max );
         return expr;
      }
         break;
      case 'T': // true const
      {
         in++; // T

         ConstantExp *expr = new ConstantExp;
         expr->setup(true);
         return expr;
      }
         break;
      case 'F': // false const
      {
         in++; // F

         ConstantExp *expr = new ConstantExp;
         expr->setup(false);
         return expr;
      }
         break;
      default:
      {
         if(isdigit(*in))
            return parseNumeric(in);
      }
   }

   return NULL;
}

Expression *RCLEval::parseExp(char *&in)
{
   Expression *leftExp = parseOperand(in);
   if(leftExp == NULL)
      return NULL;

   if(*in == 0)
      return leftExp;

   char op = *in;
   in++;

   switch(op)
   {
      case '+':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         AddExp *expr = new AddExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
      case '-':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         SubExp *expr = new SubExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
      case '*':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         MulExp *expr = new MulExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
      case '/':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         DivExp *expr = new DivExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
      case '?':
      {
         Expression *succeed = parseOperand(in);
         if(succeed == NULL)
            return NULL;
         in++; // :
         Expression *fail = parseOperand(in);
         if(fail == NULL)
            return NULL;
         IfExp *expr = new IfExp;
         expr->setup(leftExp, succeed, fail);
         return expr;
      }
         break;
      case '>':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         GreaterThanExp *expr = new GreaterThanExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
      case '<':
      {
         Expression *rightExp = parseOperand(in);
         if(rightExp == NULL)
            return NULL;
         LowerThanExp *expr = new LowerThanExp;
         expr->setup(leftExp, rightExp);
         return expr;
      }
         break;
   }

   return leftExp;
}

bool RCLEval::setupRCL(uint8_t chan, const char *expStr)
{
   char buffer[MAX_SERIAL_INPUT_BUFFER];
   strncpy(buffer, expStr, MAX_SERIAL_INPUT_BUFFER);

   if(expression_[chan] != NULL)
      clearRCL(chan);

   char *bufferPtr = &buffer[0];
   expression_[chan] = parseExp(bufferPtr);

   return (expression_[chan] != NULL);
}

void RCLEval::saveToEEPROM(uint16_t &addr) const
{
   g_hierachyDump = false;

   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      g_idxDump = 0;
      memset((void*)g_dump,0,sizeof(g_dump));

      dump(idx);

      EEPROM.put(addr, g_dump);
      addr += MAX_SERIAL_INPUT_BUFFER;
   }

   g_hierachyDump = true;
}

void RCLEval::loadFromEEPROM(uint16_t &addr)
{
   char buffer[MAX_SERIAL_INPUT_BUFFER];

   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      EEPROM.get(addr, buffer);
      addr += MAX_SERIAL_INPUT_BUFFER;

      setupRCL(idx, buffer);
   }
}

void RCLEval::clearRCL(uint8_t chan)
{
   Expression *expr = expression_[chan];
   if(expr == NULL)
      return;

   if(expr->couldBeDeleted())
      delete expr;

   expression_[chan] = NULL;
}

void RCLEval::idle()
{
   // Evaluate expression for all channels

   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      if(expression_[idx] != NULL)
         outputValueRef_[idx] = currentModel_->getValue(idx, expression_[idx]->evaluate().convert2Int());
      else
         outputValueRef_[idx] = PPM_MIN_VALUE;
   }
}

void RCLEval::dump(uint8_t outChannelID) const
{
   g_tab4Dump = 0;

   if(expression_[outChannelID] != NULL)
      expression_[outChannelID]->dump();
}

void RCLEval::reset()
{
   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      clearRCL(idx);
}

