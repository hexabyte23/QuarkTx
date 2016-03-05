/*
SerialLink.h - QuarkTx
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

#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <stream.h>
#include "config.h"
#include "Streaming.h"

class Command;

class SerialLink
{
   char serialBuffer_[QUARKTX_MAX_SERIAL_BUFFER+2];
   uint8_t idxBuffer_;
   Command *cmdRef_;

   void reset();

public:
#ifdef QUARKTX_TEENSY
  static usb_serial_class *currentStream_;
#else
   static HardwareSerial *currentStream_;
#endif

   SerialLink();
   bool setup(Command *cmd);
   void loop();
   void displayPrompt();
};

#ifdef QUARKTX_TEENSY
class MacAddress
{
    uint8_t mac_[6];

    void setup(uint8_t word, uint8_t loc) 
    {
        // "Kinetis Peripheral Module Quick Reference" page 85 and
        // "K20 Sub-Family Reference Manual" page 548.
        cli();
        FTFL_FCCOB0 = 0x41;             // READONCE command
        FTFL_FCCOB1 = word;             // read the given word of read once area
                                        // -- this is one half of the mac addr.
        FTFL_FSTAT = FTFL_FSTAT_CCIF;   // Launch command

        // Wait for command completion
        while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

        // Skip FTFL_FCCOB4 always 0
        mac_[loc] =   FTFL_FCCOB5;       // collect only the top three bytes (big endian)
        mac_[loc+1] = FTFL_FCCOB6;       
        mac_[loc+2] = FTFL_FCCOB7;       
        sei();
    }
    
public:

    MacAddress() {}

    uint8_t operator[](int index) const {return mac_[index];}
    uint8_t& operator[](int index) {return mac_[index];}

    void setup()
    {
        setup(0x0E, 0);
        setup(0x0F, 3);
    }
};

#endif

#endif
