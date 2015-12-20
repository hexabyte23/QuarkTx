
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

void Rl::setup(uint16_t *inputValue, uint16_t *outputValue, Model *currentModel)
{
  inputValue_=inputValue;
  outputValue_=outputValue;
  currentModel_ = currentModel;

  i1_.setup(&inputValue_[0]);
  i2_.setup(&inputValue_[1]);
  i3_.setup(&inputValue_[2]);
  i4_.setup(&inputValue_[3]);
  i5_.setup(&inputValue_[4]);
  i6_.setup(&inputValue_[5]);

  
  //  o1=i1|i2
  //  o2=mix(i1, i2, 0.5)|i3[1;0]
  
  e1_.setup(&i1_);    //  o0=i0
  e2_.setup(&i2_);    //  o1=i1
  e3_.setup(&i3_);    //  o2=i2
  e4_.setup(&i4_);    //  o3=i3
  e5_.setup(&i5_);    //  o4=i4
  e6_.setup(&i6_);    //  o5=i5
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
      outputValue_[idx] = currentModel_->getValue(idx, op_[idx]->evaluate()->getValue());
    }
  }
}
  
