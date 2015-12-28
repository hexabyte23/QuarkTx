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

void LimitExp::setup(const Expression *expr, int min, int max)
{
  expr_ = expr;
  min_ = min;
  max_ = max;
}

uint16_t LimitExp::evaluate() const
{
  return map(expr_->evaluate(), ADC_MIN_VALUE, ADC_MAX_VALUE, min_, max_);
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
    inputTab[idx] = new SensorExp;
    inputTab[idx]->setup(sensorRef_[idx]);
  }
}

int getNextNumeric(char* text, char delim, int &value)
{
  char buf[4];
  int i = 0;
  
  while(text[i] != 0 && text[i] != delim && i < 4)
  {
    buf[i] = text[i];
    i++;
  }
  
  buf[i] = 0;
  value = atoi(buf);

  return i+1;
}

Expression *Evaluator::parseExp(const char *str, int &len)
{
  char *ps = (char*)str;
  Expression *leftExp = NULL;
  //STDOUT << "[d] str=" << str << " len=" << strlen(str) << endl;
  
  while(*ps != 0)
  {
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
          len += 2;
          //STDOUT << "[d] i=" << c << " next car='" << *ps << "'" << endl;
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
            len++;

            int rightLen = 0;
            Expression *rightExp = parseExp(ps, rightLen);

            AddExp *expr = new AddExp;
            expr->setup(leftExp, rightExp);
            leftExp = expr;
            ps += rightLen;
            len += rightLen;
            //STDOUT << "[d] + " << (int)&leftExp << " " << (int)&rightExp << endl;
        }
        break;
      case '-': break;
      case '[':
        {
          // check if previous expression is not NULL (expected ixx)
          if(leftExp == NULL)
          {
            error(ERR_LEFT_OP_EMPTY, '[');
            return NULL;
          }
          
          ps++;
          len++;
          int _min, _max;

          int lenNum = getNextNumeric(ps, ';', _min);
          ps += lenNum;
          len += lenNum;
          lenNum = getNextNumeric(ps, ']', _max);;
          ps += lenNum;
          len += lenNum;

          LimitExp *expr = new LimitExp;
          expr->setup(leftExp, _min, _max );
          leftExp = expr;
          //STDOUT << "[d] [" << _min << " " << _max << " next car='" << *ps << "'" << endl;
        }
        break;
      case '>': break;
      case '<': break;
      case '?': break;
      case '(': break;
    }
  }

  return leftExp;
}

bool Evaluator::setupOutputChannel(uint8_t outChannelID, const char *str)
{
  int i = 0;
  expression_[outChannelID] = parseExp(str, i);
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

