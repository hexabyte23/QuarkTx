/*
Model.h - QuarkTx
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

#ifndef MODEL_H
#define MODEL_H

#include <inttypes.h>
#include "config.h"

struct ServoCommand
{
   uint16_t maxOutCurse_;    // in microsec
   uint16_t minOutCurse_;    // in microsec
   int16_t neutral_;         // in microsec
   bool isRevert_;

   ServoCommand();

   uint16_t getValue(uint16_t rawInputValue);
   void reset();
   void saveToEEPROM(uint16_t &addr) const;
   void loadFromEEPROM(uint16_t &addr);
};

struct OutputChannel
{
   ServoCommand servo_;      // next step, put many servo per channel

   void saveToEEPROM(uint16_t &addr) const;
   void loadFromEEPROM(uint16_t &addr);
};

class Model
{
   OutputChannel channel_[MAX_PPM_OUTPUT_CHANNEL];

public:

   Model();

   bool setup();
   void reset();
   void dump();

   uint16_t getValue(uint8_t channel, uint16_t rawInputValue);
   void setMinValue(uint8_t channel, int value);
   void setMaxValue(uint8_t channel, int value);
   void setNeutralValue(uint8_t channel, uint16_t value);
   void setRevertValue(uint8_t channel, bool value);
   void saveToEEPROM(uint16_t &addr) const;
   void loadFromEEPROM(uint16_t &addr);
};

#endif
