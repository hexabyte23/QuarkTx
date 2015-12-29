/*
Evaluator.cpp - QuarkTx
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

#include "Evaluator.h"
#include "FlashMem.h"
#include "SerialLink.h"
#include "MemoryFree.h"


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

//////////////////////////////////////////////////////////////////////////

void IfExp::setup(const Expression *test, const Expression *succeed, const Expression *fail)
{
  test_ = test;
  succeed_ = succeed;
  fail_ = fail;
}

Variant IfExp::evaluate() const
{
  return (test_->evaluate().bData_)?succeed_->evaluate():fail_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void LowerExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

Variant LowerExp::evaluate() const
{
  return left_->evaluate() < right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void GreaterExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

Variant GreaterExp::evaluate() const
{
  return left_->evaluate() > right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void AddExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

Variant AddExp::evaluate() const
{
  return left_->evaluate() + right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void SubExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

Variant SubExp::evaluate() const
{
  return left_->evaluate() - right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void LimitExp::setup(const Expression *expr, const Expression *min, const Expression *max)
{
  expr_ = expr;
  min_ = min;
  max_ = max;
}

Variant LimitExp::evaluate() const
{
  Variant ret((uint16_t)map(expr_->evaluate().iData_, ADC_MIN_VALUE, ADC_MAX_VALUE, min_->evaluate().iData_, max_->evaluate().iData_));
  return ret;
}

//////////////////////////////////////////////////////////////////////////

void MulExp::setup(const Expression *exp1, const Expression *exp2)
{
  exp1_ = exp1;
  exp2_ = exp2;
}

Variant MulExp::evaluate() const
{
  return exp1_->evaluate() * exp2_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

Evaluator::Evaluator()
{
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

int getNextNumeric(char *&text, int &iVal, float &fVal)
{
  int type = 0; // 0 = interger, 1 = float
  char buf[] = {0,0,0,0,0,0,0}; // 6 digits + \0
  char *p = &buf[0];
  int i = 0;
  int ret;
  //STDOUT << "gnn(" << text << ")" << endl;
  
  while(1)
  {
    if(*text == 0) break;
    if(*text == '.')
      type = 1;
    else if(*text - '0' > 9)
        break; // read until digit
    if(i == sizeof(buf)-1) break;
    
    *p = *text;
    //STDOUT << *p;
    p++;
    text++;
    i++;
  }

  if(type == 0)
    iVal = atoi(buf);
  else
    fVal = atof(buf);  
  
  if(*text != 0)
    text++;
  
  //STDOUT << "exit gnn " << ret << " '" << *text << "'(" << _HEX(*text) << ")" << endl;

  return type;
}

Expression *Evaluator::parseExp(char *&ps)
{
  Expression *leftExp = NULL;
  //STDOUT << "[d] parseExp(str=" << ps << ", len=" << strlen(ps) << ")" << endl;
  
  while(*ps != 0)
  {
    //STDOUT << "enter loop ps='" << *ps << "'" << endl;
    
    switch(ps[0])
    {
      case 'i': 
        {
          int c = ps[1] - '0';
          if(c > MAX_INPUT_CHANNEL)
          {
            error(ERR_BAD_PARAM_IDX_HIGH, c, MAX_INPUT_CHANNEL);
            return NULL;
          }
          leftExp = inputTab[c];
          ps += 2;
          
          //STDOUT << "[d] op i=" << c << " next car='" << *ps << "'" << endl;
        }
        break;
      case '+':
        {
            // check if previous expression is not NULL
            if(leftExp == NULL)
            {
              error(ERR_LEFT_OP_EMPTY, '+');
              return NULL;
            }
            ps++;

            Expression *rightExp = parseExp(ps);
            if(rightExp == NULL)
              return NULL;
              
            AddExp *expr = new AddExp;
            expr->setup(leftExp, rightExp);
            leftExp = expr;

            //STDOUT << "[d] op + " << (int)&leftExp << " " << (int)&rightExp << endl;
        }
        break;
      case '-': 
        {
            // check if previous expression is not NULL
            if(leftExp == NULL)
            {
              error(ERR_LEFT_OP_EMPTY, '-');
              return NULL;
            }
            ps++;

            Expression *rightExp = parseExp(ps);
            if(rightExp == NULL)
              return NULL;
              
            SubExp *expr = new SubExp;
            expr->setup(leftExp, rightExp);
            leftExp = expr;

            //STDOUT << "[d] op - " << (int)&leftExp << " " << (int)&rightExp << endl;
        }
        break;
      case '*': 
        {
            // check if previous expression is not NULL
            if(leftExp == NULL)
            {
              error(ERR_LEFT_OP_EMPTY, '*');
              return NULL;
            }
            ps++;

            Expression *rightExp = parseExp(ps);
            if(rightExp == NULL)
              return NULL;

            MulExp *expr = new MulExp;
            expr->setup(leftExp, rightExp);
            leftExp = expr;

            //STDOUT << "[d] op * " << (int)&leftExp << " " << (int)&rightExp << endl;
        }
        break;
      case '[':
        {
          // check if previous expression is not NULL (expected ixx)
          if(leftExp == NULL)
          {
            error(ERR_LEFT_OP_EMPTY, '[');
            return NULL;
          }
          
          ps++;

          Expression *_min = parseExp(ps);
          if(_min == NULL)
            return NULL;
          Expression *_max = parseExp(ps);          
          if(_max == NULL)
            return NULL;
            
          LimitExp *expr = new LimitExp;
          expr->setup(leftExp, _min, _max );
          leftExp = expr;
          
          //STDOUT << "[d] op [" << _min << " " << _max << " next car='" << *ps << "'" << endl;
        }
        break;
      case 'T':
        {
          ps++;
          // True constant
          BoolExp *expr = new BoolExp;
          expr->setup(true);
          return expr;
        }
        break;
      case 'F':
        {
          ps++;
          // False constant
          BoolExp *expr = new BoolExp;
          expr->setup(false);
          return expr;
        }
        break;
        break;
      case '?': break;
      case '(': break;
      default:
      {
        // check if numeric
        if(ps[0] - '0' <= 9)
        {
          //STDOUT << "Numeric" << endl;

          int iData;
          float fData;
          int type = getNextNumeric(ps, iData, fData);
          //STDOUT << type << " " << iData << " " << fData << endl;
          
          if(type == 0)
          {
            IntegerExp *expr = new IntegerExp;
            expr->setup(iData);
            return expr;
          }
          else
          {
            FloatExp *expr = new FloatExp;
            expr->setup(fData);
            return expr;
          }
        }
      }
      break;
    }
  }

  //STDOUT << "exit parseExp()" << endl;
  return leftExp;
}

bool Evaluator::setupOutputChannel(uint8_t outChannelID, const char *str)
{
  char buff[100];
  char *buf = &buff[0];
  strcpy(buf, str);
  expression_[outChannelID] = parseExp(buf);

  //STDOUT << freeMemory() << endl;
}

void Evaluator::idle()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    if(expression_[idx] != NULL)
      outputValueRef_[idx] = currentModel_->getValue(idx, expression_[idx]->evaluate().iData_);
    else
      outputValueRef_[idx] = PPM_MIN_VALUE;
  }
}

