

#include "Tx.h"

Tx tx;

void setup()
{
  if(tx.setup() == true)
    printf("Ready.\n>\n");
  else
    printf("Failed.\n");
}

ISR(TIMER1_COMPA_vect)
{
  tx.onIrqTimerChange();
}

void loop()
{
  tx.idle();
}

