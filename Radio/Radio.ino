/*
Radio.ino - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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

#include "Tx.h"

Tx tx;

void setup()
{
   if(tx.setup())
      STDOUT << F("Ready\n>") << endl;
   else
      STDOUT << F("Failed\n>") << endl;
}

#if __MK20DX256__
void pit1_isr()
#else
ISR(TIMER1_COMPA_vect)
{
   tx.onIsrTimerChange();
}
#endif

void loop()
{   
   tx.idle();
}

