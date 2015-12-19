
#include <arduino.h>
#include "Rl.h"

StreamBase *OrOp::evaluate()
{
  i1_->setValue(i1_->getValue()|i2_->getValue());
  return i1_;
}

////////////////////////////////////////////////

Rl::Rl()
:
i1_(&inputValue_[0]),
i2_(&inputValue_[1]),
i3_(&inputValue_[2]),
i4_(&inputValue_[3]),
e1_(&i1_),
e2_(&i2_),
e3_(&i3_),
e4_(&i4_)
{ 
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
      outputValue_[idx] = op_[idx]->evaluate()->getValue();
    }
  }
}
  
