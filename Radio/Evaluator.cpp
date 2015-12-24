
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

  in1_.setup(sensor_[0]);
  in2_.setup(sensor_[1]);
  in3_.setup(sensor_[2]);
  in4_.setup(sensor_[3]);
  in5_.setup(sensor_[4]);
  in6_.setup(sensor_[5]);

  // o5 = i5[0;512] + i6[512;0]
  l1_.setup(&in5_, 0, 512);
  l2_.setup(&in6_, 512, 0);
  a1_.setup(&l1_, &l2_);
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

