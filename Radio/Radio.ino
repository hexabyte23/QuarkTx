
#include "Tx.h"

Tx tx;

void setup()
{
  if(tx.setup() == true)
    printf("Ready.\n>\n");
  else
    printf("Failed.\n");
}

#ifdef GET_ADC_BY_IRQ
ISR(ADC_vect)
{
  tx.onIrqAdcChange();
}
#endif

void loop()
{
  tx.idle();
}

