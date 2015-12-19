
#include <arduino.h>
#include "Rl.h"

StreamBase *OrOp::evaluate()
{
  i1_->setValue(i1_->getValue()|i2_->getValue());
  return i1_;
}

////////////////////////////////////////////////

Rl::Rl()
{ 
}

void Rl::setup(uint16_t *inputValue, uint16_t *outputValue)
{
  inputValue_=inputValue;
  outputValue_=outputValue;

  i1_.setup(&inputValue_[0]);
  i2_.setup(&inputValue_[1]);
  i3_.setup(&inputValue_[2]);
  i4_.setup(&inputValue_[3]);
  i5_.setup(&inputValue_[4]);
  i6_.setup(&inputValue_[5]);
  e1_.setup(&i1_);
  e2_.setup(&i2_);
  e3_.setup(&i3_);
  e4_.setup(&i4_);
  e5_.setup(&i5_);
  e6_.setup(&i6_);
}

void Rl::parse(const char *def)
{
}

void Rl::idle()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    if(op_[idx] != NULL)
    {
      outputValue_[idx] = i1_.getValue();
      //outputValue_[idx] = op_[idx]->evaluate()->getValue();
      //Serial.print(op_[idx]->evaluate()->getValue());
      //Serial.print(" ");
      //Serial.print(inputValue_[idx]);
      //Serial.print(" ");
    }
  }
  //Serial.println(" ");
}
  
