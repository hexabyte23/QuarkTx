
#include <arduino.h>
#include "Rl.h"

StreamBase *LimitOp::evaluate()
{
  ret_.setValue(map(i1_->getValue(), ADC_MIN_VALUE, ADC_MAX_VALUE, min_, max_));
  return &ret_;
}

////////////////////////////////////////////////

StreamBase *OrOp::evaluate()
{
  ret_.setValue(i1_->getValue()|i2_->getValue());
  return &ret_;
}

////////////////////////////////////////////////

Rl::Rl()
{ 
}

void Rl::setup(Sensor **sensorRef, uint16_t *outputValue, Model *currentModel)
{
//  inputValue_ = inputValue;
  outputValue_ = outputValue;
  currentModel_ = currentModel;
  sensorRef_ = sensorRef;

  i1_.setup(sensorRef[0]);
  i2_.setup(sensorRef[1]);
  i3_.setup(sensorRef[2]);
  i4_.setup(sensorRef[3]);
  i5_.setup(sensorRef[4]);
  i6_.setup(sensorRef[5]);

  // o2=mix(i1, i2, 0.5)|i3[1;0]

  e1_.setup(&i1_);    //  o0=i0
  e2_.setup(&i2_);    //  o1=i1
  e3_.setup(&i3_);    //  o2=i2
  e4_.setup(&i4_);    //  o3=i3

  //  o5=i1[0;512]|i2[512;1023]
  l1_.setup(&i5_);
  l1_.min_=0;
  l1_.max_=512;
  l2_.setup(&i6_);
  l2_.min_=512;
  l2_.max_=1023;
#if 0
  e5_.setup(l1_.evaluate());    //  o4=i5[0;512]
#endif
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
  
