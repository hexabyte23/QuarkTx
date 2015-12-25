/*
SerialLink.cpp - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

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
#include <SoftwareSerial.h>
#include "FlashMem.h"
#include "SerialLink.h"


int serialWrite(char c, FILE *f)
{
  Serial.write(c);
  return 0;
}

SerialLink::SerialLink()
: cmd_(NULL)
{
  clearSerialBuffer();
}

bool SerialLink::setup(Command *cmd)
{
  cmd_ = cmd;
  
  Serial.begin(SERIAL_SPEED);

  // reroute printf() output to serial
  stdout = stderr = fdevopen(serialWrite, NULL);

  info(INFO_BOOTING_MESSAGE, VERSION);
    
  info(INFO_SERIAL);

  // Setup BT module
 #ifdef BLUETOOTH
  pinMode(BT_RX_PIN, INPUT);  
  pinMode(BT_TX_PIN, OUTPUT);
  BTSerial_ = new SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
  BTSerial_->begin(SERIAL_SPEED);
  info(INFO_BT_READY);
#endif

  return true;
}

void SerialLink::clearSerialBuffer() 
{
  idxBuffer_ = 0;
  memset((void*)idxBuffer_,0,sizeof(idxBuffer_));
}

void SerialLink::displayPrompt()
{
  printf(">\n");
}

void SerialLink::idle()
{
  if(Serial.available() <= 0)
    return;
    
  while (Serial.available())
  {
    char c = (char)Serial.read();
    //debug("[d] '%d'",c);
    if (c == '\n')
    {
      serialBuffer_[idxBuffer_] = 0;
      serialBuffer_[idxBuffer_+1] = 0;
      
      if(cmd_ != NULL)
      {
        //debug(" %d Sent\n", idxBuffer_);
        Serial.println(serialBuffer_);
        cmd_->onNewCommand(serialBuffer_);
        idxBuffer_ = 0;
        displayPrompt();
      }
    }
    else
    {
      //debug(" %d %c\n", idxBuffer_, c);
      serialBuffer_[idxBuffer_] = c;
      
      if(idxBuffer_ < MAX_SERIAL_INPUT_BUFFER)
        idxBuffer_++;
      else
      {
        error(ERR_STRING_TOO_LONG, serialBuffer_);
        idxBuffer_ = 0;
        break;
      }
    }
  }
}

