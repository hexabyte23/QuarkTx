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

void IfExp::setup(const Expression *test, const Expression *succeed, const Expression *fail)
{
  test_ = test;
  succeed_ = succeed;
  fail_ = fail;
}

uint16_t IfExp::evaluate() const
{
  return test_->evaluate()?succeed_->evaluate():fail_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void LowerExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

uint16_t LowerExp::evaluate() const
{
  return left_->evaluate() < right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void GreaterExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

uint16_t GreaterExp::evaluate() const
{
  return left_->evaluate() > right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void AddExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

uint16_t AddExp::evaluate() const
{
  return left_->evaluate() + right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

void SubExp::setup(const Expression *left, const Expression *right)
{
  left_ = left;
  right_ = right;
}

uint16_t SubExp::evaluate() const
{
  return left_->evaluate() + right_->evaluate();
}

//////////////////////////////////////////////////////////////////////////

//void LimitExp::setup(const Expression *expr, int min, int max)
void LimitExp::setup(const Expression *expr, const Expression *min, const Expression *max)
{
  expr_ = expr;
  min_ = min;
  max_ = max;
}

uint16_t LimitExp::evaluate() const
{
  return map(expr_->evaluate(), ADC_MIN_VALUE, ADC_MAX_VALUE, min_->evaluate(), max_->evaluate());
}

//////////////////////////////////////////////////////////////////////////

void MixExp::setup(const Expression *fromChannel, float rate)
{
  fromChannel_ = fromChannel;
  rate_ = rate;
}

uint16_t MixExp::evaluate() const
{
  return fromChannel_->evaluate() * rate_;
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

int getNextNumeric(char *&text)
{
  char buf[] = {0,0,0,0,0}; // 4 digits + \0
  char *p = &buf[0];
  int i = 0;
  int ret;
  //STDOUT << "gnn(" << text << ")" << endl;
  
  while(1)
  {
    if(*text == 0) break;
    if(*text - '0' > 9) break; // read until digit
    if(i == sizeof(buf)-1) break;
    
    *p = *text;
    //STDOUT << *p;
    p++;
    text++;
    i++;
  }
  
  ret = atoi(buf);
  
  if(*text != 0)
    text++;
  
  //STDOUT << "exit gnn " << ret << " '" << *text << "'(" << _HEX(*text) << ")" << endl;

  return ret;
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

            SubExp *expr = new SubExp;
            expr->setup(leftExp, rightExp);
            leftExp = expr;

            //STDOUT << "[d] op - " << (int)&leftExp << " " << (int)&rightExp << endl;
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
          Expression *_max = parseExp(ps);          

          LimitExp *expr = new LimitExp;
          expr->setup(leftExp, _min, _max );
          leftExp = expr;
          
          //STDOUT << "[d] op [" << _min << " " << _max << " next car='" << *ps << "'" << endl;
        }
        break;
      case '>': break;
      case '<': break;
      case '?': break;
      case '(': break;
      default:
      {
        // check if numeric
        if(ps[0] - '0' <= 9)
        {
          //STDOUT << "Numeric" << endl;

          int constant = getNextNumeric(ps);
          ConstantInputExp *expr = new ConstantInputExp;
          expr->setup(constant);
          return expr;
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
      outputValueRef_[idx] = currentModel_->getValue(idx, expression_[idx]->evaluate());
    else
      outputValueRef_[idx] = PPM_MIN_VALUE;
  }
}

