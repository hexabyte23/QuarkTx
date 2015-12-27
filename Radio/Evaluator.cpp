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

void Evaluator::setup(Sensor **sensor, uint16_t *outputValue, Model *currentModel)
{
  sensor_ = sensor;
  outputValue_ = outputValue;
  currentModel_ = currentModel;

  for(int8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    inputTab[idx] = new SensorExp;
    inputTab[idx]->setup(sensor_[idx]);
  }

  // hard coded expression
  expression_[0] = inputTab[0]; // o1 = i1
  expression_[1] = inputTab[1]; // o2 = i2
  expression_[2] = inputTab[2]; // o3 = i3
  expression_[3] = inputTab[3]; // o4 = i4
  
  l1_.setup(inputTab[4], 0, 512);
  l2_.setup(inputTab[5], 512, 0);
  a1_.setup(&l1_, &l2_);
  expression_[4] = &a1_;        // o5 = i5[0;512] + i6[512;0]

#ifdef TERRATOP
  expression_[5] = inputTab[6]; // o6 = i7
#endif
}

Expression *Evaluator::parseExp(const char *str)
{
  Expression *ret = NULL;
  int lastInput = -1;
  
  int len = strlen(str);
  for(int idx=0; idx < len; idx++)
  {
    switch(str[idx])
    {
      case 'i': 
        {
          lastInput = atoi(str+1);
        }
        break;
      case '+': break;
      case '-': break;
      case '[': break;
      case '>': break;
      case '<': break;
      case '?': break;
      case '(': break;
    }
  }

  return ret;
}

bool Evaluator::parse(uint8_t outChannelID, const char *str)
{
  expression_[outChannelID] = parseExp(str);
}

void Evaluator::idle()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    if(expression_[idx] != NULL)
      outputValue_[idx] = currentModel_->getValue(idx, expression_[idx]->evaluate());
    else
      outputValue_[idx] = PPM_MIN_VALUE;
  }
}

