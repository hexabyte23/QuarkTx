/*
Evaluator.cpp - QuarkTx
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

#include "Evaluator.h"
#include "FlashMem.h"
#include "SerialLink.h"

//////////////////////////////////////////////////////////////////////////
// 
// Some Misc funtions
//
//////////////////////////////////////////////////////////////////////////

static uint8_t g_tab4Dump = 0;

void tabDump()
{
  for(uint8_t i = 0; i < g_tab4Dump; i++)
    STDOUT << " ";
}

void enterDump()
{
  STDOUT << endl;
  tabDump();
  STDOUT << "{" << endl;
  g_tab4Dump++;
  tabDump();
}

void leaveDump()
{
  g_tab4Dump--;
  STDOUT << endl;
  tabDump();
  STDOUT << "}";
  tabDump();
}

//
// a float version of map
//
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//////////////////////////////////////////////////////////////////////////

uint16_t Variant::convertInt() const
{
  switch(type_)
  {
    case Variant::tInteger: return iData_;
    case Variant::tFloat: return (uint16_t)fData_;
    case Variant::tBool: return (uint16_t)bData_;
  }

  return 0;
}

float Variant::convertFloat() const
{
  switch(type_)
  {
    case Variant::tInteger: return (float)iData_;
    case Variant::tFloat: return fData_;
    case Variant::tBool: return (float)bData_;
  }

  return 0;
}

bool Variant::convertBool() const
{
  switch(type_)
  {
    case Variant::tInteger: return iData_==1;
    case Variant::tFloat: return fData_==1.0;
    case Variant::tBool: return bData_;
  }

  return 0;
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
          case Variant::tInteger: return l.iData_ + r.iData_;
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
          case Variant::tInteger: return l.iData_ - r.iData_;
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
          case Variant::tInteger: return l.iData_ * r.iData_;
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
          case Variant::tInteger: return l.iData_ / r.iData_;
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
}

//////////////////////////////////////////////////////////////////////////

void IntegerExp::dump() const
{
  enterDump();
  STDOUT << "i " << data_.iData_;
  leaveDump();
}

void FloatExp::dump() const
{
  enterDump();
  STDOUT << "f " << data_.fData_;
  leaveDump();
}

void BoolExp::dump() const
{
  enterDump();
  STDOUT << "b " << data_.bData_;
  leaveDump();
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
  STDOUT << "( ";
  expr_->dump();
  leaveDump();
}

//////////////////////////////////////////////////////////////////////////

void SensorInputExp::dump() const
{
  enterDump();
  STDOUT << "si " << sensor_->getPin();
  leaveDump();
}

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
  return (condition_->evaluate().convertBool())?succeed_->evaluate():fail_->evaluate();
}

void IfExp::dump() const
{
  enterDump();
  STDOUT << "if";
  condition_->dump();
  succeed_->dump();
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
  STDOUT << "+";
  left_->dump();
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
  STDOUT << "-";
  left_->dump();
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
  STDOUT << "*";
  left_->dump();
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
  STDOUT << "/";
  left_->dump();
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
  STDOUT << "=";
  left_->dump();
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
  STDOUT << "<";
  left_->dump();
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
  STDOUT << ">";
  left_->dump();
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
  Variant ret((uint16_t)fmap(data_->evaluate().convertFloat(), ADC_MIN_VALUE, ADC_MAX_VALUE, min_->evaluate().convertFloat(), max_->evaluate().convertFloat()));
  return ret;
}

void LimitExp::dump() const
{
  enterDump();
  STDOUT << "[]";
  data_->dump();
  min_->dump();
  max_->dump();
  leaveDump();
}

//////////////////////////////////////////////////////////////////////////

Evaluator::Evaluator()
{
  memset((void*)expression_,0,sizeof(expression_));
}

void Evaluator::setup(Sensor **sensorRef, uint16_t *outputValueRef, Model *currentModel)
{
  sensorRef_ = sensorRef;
  outputValueRef_ = outputValueRef;
  currentModel_ = currentModel;

  for(int8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    inputTab[idx] = new SensorInputExp;
    inputTab[idx]->setup(sensorRef_[idx]);
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

Expression *Evaluator::parseNumeric(char *&in)
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
      type = 1;                     // this is a float
    else
    { 
      if(!isdigit(*in))             // digit ?
          break;                    // no more a digit
    }
    
    *p = *in;                       // copy car into buf
    //STDOUT << *p;
    
    p++;
    in++;
    len++;
  }
    
  if(type == 0)
  {
    //STDOUT << "int " << atoi(buf) << " next='" << *in << "'" << endl;
    IntegerExp *expr = new IntegerExp;
    expr->setup(atoi(buf));
    return expr;
  }
  else
  {
    //STDOUT << "float " << atof(buf) << endl;
    FloatExp *expr = new FloatExp;
    expr->setup(atof(buf));
    return expr;
  }

  return NULL;
}

Expression *Evaluator::parseOperand(char *&in)
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
      return sexpr;
    }
    break;
    case 'i': // sensor input
    {
      int c = in[1] - '0';
      if(c >= MAX_INPUT_CHANNEL)
      {
        error(ERR_BAD_PARAM_IDX_HIGH, c, MAX_INPUT_CHANNEL-1);
        return NULL;
      }
      in += 2;

      // STDOUT << "i" << c << " '" << *in << "'" << endl;

      if(*in != '[')
        return inputTab[c];
      
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
      expr->setup(inputTab[c], _min, _max );
      return expr;
    }
    break;
    case 'T': // true const
    {
      in++; // T

      BoolExp *expr = new BoolExp;
      expr->setup(true);
      return expr;
    }
    break;
    case 'F': // false const
    {
      in++; // F

      BoolExp *expr = new BoolExp;
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

Expression *Evaluator::parseExp(char *&in)
{
  //STDOUT << "in='" << *in << "'" << endl;
  
  Expression *leftExp = parseOperand(in);

  //STDOUT << "next='" << *in << "'" << endl; 
  
  if(leftExp == NULL)
    return NULL;
  if(*in == 0)
    return leftExp;

  char op = *in;
  in++;

  //STDOUT << "op " << op << endl;

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

bool Evaluator::setupOutputChannel(uint8_t chan, const char *str)
{
  char buff[100];
  char *buf = &buff[0];
  strncpy(buf, str, sizeof(buff));
  
  if(expression_[chan] != NULL)
      clearOuputChannel(chan);
      
  expression_[chan] = parseExp(buf);

  return (expression_[chan] != NULL);
}

void Evaluator::clearOuputChannel(uint8_t chan)
{
  Expression *expr = expression_[chan];
  if(expr == NULL)
    return;

  if(expr->couldBeDeleted())
    delete expr;

  expression_[chan] = NULL;
}

void Evaluator::idle()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    if(expression_[idx] != NULL)
      outputValueRef_[idx] = currentModel_->getValue(idx, expression_[idx]->evaluate().convertInt());
    else
      outputValueRef_[idx] = PPM_MIN_VALUE;
  }
}

void Evaluator::dump(uint8_t outChannelID)
{
  g_tab4Dump = 0;
  
  if(expression_[outChannelID] != NULL)
    expression_[outChannelID]->dump();
}

void Evaluator::reset()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    clearOuputChannel(idx);
}

