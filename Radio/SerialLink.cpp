/*
SerialLink.cpp - QuarkTx
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

#include <arduino.h>
#include "SerialLink.h"
#include "Command.h"

Stream *SerialLink::currentStream_ = NULL;

SerialLink::SerialLink()
   : cmdRef_(NULL)
{
   reset();
}

bool SerialLink::setup(Command *cmd)
{
   cmdRef_ = cmd;
   primaryActive_ = true;

   /*
    * We first initiate primary serial
    */
    
   currentStream_ = &QUARKTX_PRIM_SERIAL;
#ifdef QUARKTX_TEENSY
   ((usb_serial_class*)currentStream_)->begin(QUARKTX_PRIM_SERIAL_SPEED);
#else
   ((HardwareSerial*)currentStream_)->begin(QUARKTX_PRIM_SERIAL_SPEED);
#endif

#ifdef QUARKTX_ALT_SERIAL

  /*
   * We try to connect to alternate serial until primary serial is not active. 
   * Alternate serial is generally bluetooth dedicated
   * when Tx running in standalone mode
   */

   unsigned long d = millis();
   bool timeout = false;

   while(true)
   {
      if(*((usb_serial_class*)currentStream_) == true) // only teensy have alternate serial
        break;

      if((millis()-d) > 2000)
      {
         timeout = true;
         break;
      }
   }
   
   if(timeout)
   {
      currentStream_ = &QUARKTX_ALT_SERIAL;
      ((HardwareSerial*)currentStream_)->begin(QUARKTX_ALT_SERIAL_SPEED);
      primaryActive_ = false;
   }

#endif
   
#ifdef QUARKTX_TEENSY
   // To fix STDOUT writing Teensy problem just after Serial.begin()
   delay(300);
#endif

  return true;
}

void SerialLink::reset() 
{
   idxBuffer_ = 0;
   memset((void*)serialBuffer_, 0, sizeof(serialBuffer_));
}

void SerialLink::displayPrompt()
{
   STDOUT << ">" << endl;
}

void SerialLink::loop()
{  
   if(currentStream_->available() <= 0)
      return;

   while(currentStream_->available())
   {
      char c = (char)currentStream_->read();

      // Accept both termination
      if((c == '\r') || (c == '\n'))
      {
         serialBuffer_[idxBuffer_] = 0;
         serialBuffer_[idxBuffer_+1] = 0;

         if(cmdRef_ != NULL)
         {
            cmdRef_->onNewCommand(serialBuffer_);
            idxBuffer_ = 0;
            displayPrompt();
         }
      }
      else
      {
         serialBuffer_[idxBuffer_] = c;

         if(idxBuffer_ < QUARKTX_MAX_SERIAL_BUFFER)
            idxBuffer_++;
         else
         {
            STDOUT << F("e-cstl ") << QUARKTX_MAX_SERIAL_BUFFER << endl;    // Command string too long
            idxBuffer_ = 0;
            return;
         }
      }
   }
}

