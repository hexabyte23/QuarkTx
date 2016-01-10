/*
SerialLink.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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
#include <stream.h>
#include "SerialLink.h"
#include "Streaming.h"
#ifdef BLUETOOTH
#include <SoftwareSerial.h>
#endif

Stream *SerialLink::currentStream_ = NULL;

int serialWrite(char c, FILE *f)
{
  return SerialLink::currentStream_->write(c);
}

SerialLink::SerialLink()
: cmd_(NULL)
{
  clearSerialBuffer();
}

bool SerialLink::setup(Command *cmd)
{
  cmd_ = cmd;

#ifdef BLUETOOTH
  pinMode(BT_RX_PIN, INPUT);  
  pinMode(BT_TX_PIN, OUTPUT);
  currentStream_ = new SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
  ((SoftwareSerial*)currentStream_)->begin(SERIAL_SPEED);
#else
  currentStream_ = &Serial;
  Serial.begin(SERIAL_SPEED);
#endif

  // reroute printf() output to currentStream_
  stdout = stderr = fdevopen(serialWrite, NULL);

  //info(INFO_BOOTING_MESSAGE, QUARKTX_VERSION);
  STDOUT << F("Quark Tx v") << F(QUARKTX_VERSION) << F("\nBooting...") << endl;
  STDOUT << F("Serial\t\tOK") << endl;
#ifdef BLUETOOTH
  STDOUT << F("Bluetooth\tOK") << endl;
#endif

  return true;
}

void SerialLink::clearSerialBuffer() 
{
  idxBuffer_ = 0;
  memset((void*)serialBuffer_,0,sizeof(serialBuffer_));
}

void SerialLink::displayPrompt()
{
  Serial << ">" << endl;
}

void SerialLink::idle()
{
  if(currentStream_->available() <= 0)
    return;
    
  while (currentStream_->available())
  {
    char c = (char)currentStream_->read();
    if (c == '\n')
    {
      serialBuffer_[idxBuffer_] = 0;
      serialBuffer_[idxBuffer_+1] = 0;
      
      if(cmd_ != NULL)
      {
        //currentStream_->println(serialBuffer_);
        cmd_->onNewCommand(serialBuffer_);
        idxBuffer_ = 0;
        displayPrompt();
      }
    }
    else
    {
      serialBuffer_[idxBuffer_] = c;
      
      if(idxBuffer_ < MAX_SERIAL_INPUT_BUFFER)
        idxBuffer_++;
      else
      {
        STDOUT << F("e-cstl ") << MAX_SERIAL_INPUT_BUFFER << endl;    // Command string '%s' too long
        idxBuffer_ = 0;
        return;
      }
    }
  }
}

