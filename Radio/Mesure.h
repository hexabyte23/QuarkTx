/*
Mesure.h - QuarkTx
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

#ifndef MESURE_H
#define MESURE_H

#include <arduino.h>

class Mesure
{
  unsigned long sum_, avg_, min_, max_, count_, p1_, loop_;

  public:

  Mesure();

  void start() {p1_ = micros();}
  void stop() 
  {
    uint32_t delta = micros() - p1_;
    
    count_++;
    sum_ += delta;
    
    avg_ = sum_/count_;
    
    if(min_ > delta)
      min_ = delta;
    if(max_ < delta)
      max_ = delta;
  }

  uint32_t getAverage() {return avg_;}

  void displayStat(unsigned long maxLoop);
  void reset();
};

#endif
