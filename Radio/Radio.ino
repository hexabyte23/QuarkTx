
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

ISR(TIMER1_COMPA_vect)
{
  tx.onIrqTimerChange();
}

void loop()
{
  tx.idle();
}

