
#include <arduino.h>
#include "Rl.h"

OrOp::OrOp(StreamBase *i1, StreamBase *i2)
:OperatorBase(i1, i2)
{
}

StreamBase* OrOp::evaluate()
{
//  i1_->getValue()|i2_->getValue();
}

////////////////////////////////////////////////

Rl::Rl()
:i1_(&inputValue_[0])
{
  
}

void Rl::parse(const char *def)
{
  
}

void Rl::evaluate()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    if(op_[idx] != NULL)
    {
      outputValue_[idx] = op_[idx]->evaluate()->getValue();
    }
  }
}
  
